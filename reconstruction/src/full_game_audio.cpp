#include "btb/full_game_audio.hpp"

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <utility>

namespace btb::full_game {

CatalogLoadResult SoundCatalog::read(std::istream& stream) {
    std::array<CatalogEntry, sound::kSoundCatalogCount> candidate{};
    std::size_t count = 0;
    std::size_t line_number = 0;
    std::string line;

    while (std::getline(stream, line)) {
        ++line_number;
        std::istringstream row(line);
        std::string filename;
        if (!(row >> filename)) {
            continue;
        }
        int id = -1;
        std::string extra;
        if (!(row >> id) || (row >> extra) ||
            id < 0 || id >= static_cast<int>(sound::kSoundCatalogCount) ||
            filename.empty() ||
            filename.size() >= sound::kFilenameRecordBytes) {
            return {
                false, count, line_number,
                "invalid retail binklist filename, numeric sound ID or extra field"
            };
        }
        const auto index = static_cast<std::size_t>(id);
        if (candidate[index].present) {
            return {false, count, line_number, "duplicate retail sound ID"};
        }
        candidate[index] = {true, std::move(filename)};
        ++count;
    }

    if (stream.bad() || count == 0) {
        return {false, count, line_number,
                "sound catalog read failure or empty binklist"};
    }

    // Commit atomically so a malformed update cannot erase the previous
    // complete original catalog.
    entries_ = std::move(candidate);
    count_ = count;
    return {true,count,0,{}};
}

const CatalogEntry* SoundCatalog::find(int sound_id) const noexcept {
    if (sound_id < 0 ||
        sound_id >= static_cast<int>(sound::kSoundCatalogCount)) {
        return nullptr;
    }
    const auto& entry = entries_[static_cast<std::size_t>(sound_id)];
    return entry.present ? &entry : nullptr;
}

void AudioEffectPlanner::populate_retail_filename_records() noexcept {
    for (std::size_t i = 0; i < sound::kSoundCatalogCount; ++i) {
        const auto* entry = catalog_.find(static_cast<int>(i));
        if (!entry) {
            continue;
        }
        auto& fixed = manager_.filename_by_sound_id[i].bytes;
        // The retail object owns a 20-byte filename record. Catalog::read
        // guarantees at most 19 ASCII filename bytes, leaving a NUL terminator.
        std::copy(entry->filename.begin(), entry->filename.end(),
                  fixed.begin());
    }
}

void AudioEffectPlanner::reset() noexcept {
    manager_ = {};
    sound::initialize_retail_metadata(manager_);
    // Reset transient playback slots, not the installed WAV catalog.
    populate_retail_filename_records();
}

CatalogLoadResult AudioEffectPlanner::load_catalog(std::istream& input) {
    auto result = catalog_.read(input);
    if (result.success) {
        reset();
    }
    return result;
}

std::string AudioEffectPlanner::sound_file(int sound_id) const {
    const auto* record = catalog_.find(sound_id);
    if (!record) {
        return {};
    }
    return "Data/sound/" + record->filename;
}

AudioFramePlan AudioEffectPlanner::plan(
    const std::vector<Audio>& requests,
    ManagedSoundObservation observed) {

    AudioFramePlan frame;
    for (const auto& request : requests) {
        switch (request.operation) {
        case AudioOperation::StartBackingTrack:
            if (request.source_asset.empty()) {
                ++frame.invalid_requests;
                break;
            }
            frame.operations.push_back({
                SoundEffectKind::StartBackingWav, -1, -1,
                request.source_asset
            });
            break;

        case AudioOperation::StopBackingTrack:
            frame.operations.push_back({
                SoundEffectKind::StopBackingWav
            });
            break;

        case AudioOperation::PlaySampleFile:
            if (request.source_asset.empty()) {
                ++frame.invalid_requests;
                break;
            }
            frame.operations.push_back({
                SoundEffectKind::PlayDirectSample, -1, -1,
                request.source_asset
            });
            break;

        case AudioOperation::StopManagedSounds:
            // 0x00402C90 walks all 80 available groups; a stopped slot
            // stays in the manager cache, ready for replay.
            for (std::size_t i = 0; i < sound::kManagedSlotCount; ++i) {
                if (!manager_.buffer_group_ptr32[i]) {
                    continue;
                }
                static_cast<void>(sound::stop_slot_metadata(manager_, i));
                observed.playing[i] = false;
                frame.operations.push_back({
                    SoundEffectKind::StopRewindManagedSlot,
                    static_cast<int>(i)
                });
            }
            break;

        case AudioOperation::ManagedSoundId: {
            const int id = request.sound_id;
            const auto filename = sound_file(id);
            if (filename.empty()) {
                frame.missing_catalog_sound_ids.push_back(id);
                continue;
            }
            sound::ManagedPlayEnvironment environment{};
            for (std::size_t i = 0; i < sound::kManagedSlotCount; ++i) {
                environment.group_exists[i] =
                    manager_.buffer_group_ptr32[i] != 0;
            }
            environment.group_is_playing = observed.playing;
            environment.acquired_group_load_succeeds =
                observed.next_managed_load_succeeds;

            const auto played = sound::play_managed_sound(
                manager_, environment, id,
                request.priority, request.arbitration_class);
            frame.managed_results.push_back(played);

            // Native PreparePlayAgainTransition marks the new 573 voice's
            // managed buffer slot +0xD98=1, so an input pulse can stop it.
            // Keep this out of the normal hover/feedback voice policy.
            if (request.input_interruptible && played.status != 0 &&
                played.slot >= 0 &&
                played.slot < static_cast<int>(sound::kManagedSlotCount)) {
                manager_.input_interruptible[
                    static_cast<std::size_t>(played.slot)] = 1;
            }

            for (std::size_t i = 0; i < played.preempted_count; ++i) {
                frame.operations.push_back({
                    SoundEffectKind::StopRewindManagedSlot,
                    played.preempted_slots[i]
                });
            }

            if (played.released_slot) {
                frame.operations.push_back({
                    SoundEffectKind::ReleaseManagedSlot,
                    *played.released_slot
                });
            }

            if (played.load_attempted && played.slot >= 0) {
                frame.operations.push_back({
                    SoundEffectKind::LoadManagedWav,
                    played.slot, id, filename
                });
            }

            // The retail helper calls CSound::Play twice on new groups,
            // once when acquiring and once in PlayManagedSoundById.
            for (int i = 0; i < played.low_level_play_calls; ++i) {
                frame.operations.push_back({
                    SoundEffectKind::PlayManagedSlot, played.slot, id
                });
            }
            observed.playing = environment.group_is_playing;
            break;
        }
        }
    }
    return frame;
}

AudioFramePlan AudioEffectPlanner::reap(
    const ManagedSoundObservation& observed,
    bool primary_input_pulse,
    bool secondary_input_pulse) {

    AudioFramePlan frame;
    const auto reaped = sound::reap_finished_slots(
        manager_, observed.playing,
        primary_input_pulse,
        secondary_input_pulse);

    for (std::size_t i = 0; i < reaped.stopped_count; ++i) {
        frame.operations.push_back({
            SoundEffectKind::StopRewindManagedSlot,
            reaped.stopped_slots[i]
        });
    }
    return frame;
}

} // namespace btb::full_game
