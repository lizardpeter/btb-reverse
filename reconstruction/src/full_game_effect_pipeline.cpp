#include "btb/full_game_effect_pipeline.hpp"

#include <utility>

namespace btb::full_game {

PreparedGameFrame GameEffectPipeline::prepare(
    GameFrame frame,
    ManagedSoundObservation observed,
    bool primary_input_pulse,
    bool secondary_input_pulse) {

    // Native RunActiveGameFrame reaps completed managed buffers ahead of
    // RunMainGameFlow. Preserve that ordering on the output bus as well.
    auto reaped = audio_.reap(
        observed, primary_input_pulse, secondary_input_pulse);

    // The host's observation describes the instant before reaping. If a slot
    // was stopped by an input pulse, later effects in this SAME frame must
    // not continue treating it as an exclusive, currently playing blocker.
    for (const auto& stop : reaped.operations) {
        if (stop.kind == SoundEffectKind::StopRewindManagedSlot &&
            stop.slot >= 0 &&
            stop.slot < static_cast<int>(sound::kManagedSlotCount)) {
            observed.playing[static_cast<std::size_t>(stop.slot)] = false;
        }
    }

    auto requested = audio_.plan(frame.effects.audio, observed);
    AudioFramePlan combined;
    combined.operations = std::move(reaped.operations);
    combined.operations.insert(
        combined.operations.end(),
        std::make_move_iterator(requested.operations.begin()),
        std::make_move_iterator(requested.operations.end()));
    combined.managed_results = std::move(requested.managed_results);
    combined.missing_catalog_sound_ids =
        std::move(requested.missing_catalog_sound_ids);
    combined.invalid_requests = requested.invalid_requests;

    auto resources = resolve_game_frame_assets(
        resolver_, frame.effects.draws, combined);

    PreparedGameFrame prepared;
    prepared.game = std::move(frame);
    prepared.missing_catalog_ids =
        !combined.missing_catalog_sound_ids.empty();
    prepared.audio_policy = std::move(combined);
    prepared.resources = std::move(resources);
    return prepared;
}

} // namespace btb::full_game
