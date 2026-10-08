#include "btb/full_game_audio.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

using namespace btb::full_game;

int main() {
    AudioEffectPlanner planner;
    std::istringstream original_like_catalog(
        "MP_BOB_01.wav 510\r\n"
        "MP_BOB_02.wav\t511\r\n"
        "MP_WEN_01.wav 523\n"
        "MP_PIC_01.wav 536\n");
    auto parsed = planner.load_catalog(original_like_catalog);
    assert(parsed.success);
    assert(parsed.entries == 4);
    assert(planner.catalog().find(510));
    assert(planner.catalog().find(510)->filename == "MP_BOB_01.wav");
    assert(planner.catalog().find(509) == nullptr);

    std::istringstream duplicate("test.wav 510\nother.wav 510\n");
    const auto bad = planner.load_catalog(duplicate);
    assert(!bad.success);
    assert(bad.error_line == 2);
    // Reject damaged replacements without losing the installed catalog.
    assert(planner.catalog().size() == 4);
    assert(planner.catalog().find(510)->filename == "MP_BOB_01.wav");

    // Original managed Play acquisition plays a newly loaded buffer twice.
    std::vector<Audio> first{{
        AudioOperation::ManagedSoundId, {}, 510, 50, 2
    }};
    const auto initial = planner.plan(first, {});
    assert(initial.invalid_requests == 0);
    assert(initial.managed_results.size() == 1);
    assert(initial.managed_results[0].status == 1);
    assert(initial.operations.size() == 3);
    assert(initial.operations[0].kind == SoundEffectKind::LoadManagedWav);
    assert(initial.operations[0].filename ==
           "Data/sound/MP_BOB_01.wav");
    assert(initial.operations[0].slot == 0);
    assert(initial.operations[1].kind == SoundEffectKind::PlayManagedSlot);
    assert(initial.operations[2].kind == SoundEffectKind::PlayManagedSlot);

    ManagedSoundObservation playing{};
    playing.playing[0] = true;
    const auto duplicate_active = planner.plan(first, playing);
    assert(duplicate_active.managed_results[0].status == 2);
    assert(duplicate_active.operations.empty());

    // An incoming class-1 voice preempts an existing class-2 voice.
    std::vector<Audio> second{{
        AudioOperation::ManagedSoundId, {}, 511, 50, 1
    }};
    const auto preempt = planner.plan(second, playing);
    assert(preempt.managed_results[0].preempted_count == 1);
    assert(preempt.operations.size() == 4);
    assert(preempt.operations[0].kind ==
           SoundEffectKind::StopRewindManagedSlot);
    assert(preempt.operations[0].slot == 0);
    assert(preempt.operations[1].kind == SoundEffectKind::LoadManagedWav);
    assert(preempt.operations[1].slot == 1);

    ManagedSoundObservation exclusive{};
    exclusive.playing[1] = true;
    const auto refused = planner.plan(first, exclusive);
    assert(refused.managed_results.size() == 1);
    assert(refused.managed_results[0].status == 0);
    assert(refused.managed_results[0].rejected_by_exclusive_blocker);
    assert(refused.operations.empty());

    // StopAllManagedSounds retains buffer slots for later replay.
    const auto stop = planner.plan({{
        AudioOperation::StopManagedSounds
    }}, exclusive);
    assert(stop.operations.size() == 2);
    assert(stop.operations[0].kind ==
           SoundEffectKind::StopRewindManagedSlot);
    assert(stop.operations[1].kind ==
           SoundEffectKind::StopRewindManagedSlot);
    const auto replay = planner.plan(first, {});
    assert(replay.managed_results[0].status == 1);
    assert(replay.operations.size() == 1);
    assert(replay.operations[0].kind == SoundEffectKind::PlayManagedSlot);

    const auto missing = planner.plan({{
        AudioOperation::ManagedSoundId, {}, 599, 90, 1
    }}, {});
    assert(missing.operations.empty());
    assert(missing.missing_catalog_sound_ids.size() == 1);
    assert(missing.missing_catalog_sound_ids[0] == 599);

    const auto direct = planner.plan({
        {AudioOperation::StartBackingTrack, "Data/SubGameOpen/bobmt.wav"},
        {AudioOperation::PlaySampleFile, "Data/SubGameOpen/roley1_5.wav"},
        {AudioOperation::StopBackingTrack},
    }, {});
    assert(direct.operations.size() == 3);
    assert(direct.operations[0].kind == SoundEffectKind::StartBackingWav);
    assert(direct.operations[1].kind == SoundEffectKind::PlayDirectSample);
    assert(direct.operations[2].kind == SoundEffectKind::StopBackingWav);

    planner.reset();
    assert(planner.manager().mapped_slot(510) == -1);
}
