#include "btb/spud_skate_runtime.hpp"

namespace btb::spud_skate {
namespace {

[[nodiscard]] constexpr std::optional<std::size_t> accepted_stunt_for_press(
    const TimingData& timing,
    std::int32_t frame) noexcept {

    for (std::size_t i = 0;
         i < static_cast<std::size_t>(timing.stunt_count) && i < kStuntCount;
         ++i) {
        if (timing.windows[i].contains(frame)) {
            return i;
        }
    }
    return std::nullopt;
}

} // namespace

PlaybackFrameStep update_synchronized_run(
    RuntimeState& state,
    const TimingData& timing,
    const SoundData& sounds,
    const PlaybackFrameInput& input) noexcept {

    PlaybackFrameStep step;
    step.frame = input.frame;
    step.quality_before = state.quality;
    step.active_stunt_before = state.active_stunt;

    if (state.phase != PlaybackPhase::SynchronizedRun) {
        step.quality_after = state.quality;
        step.active_stunt_after = state.active_stunt;
        step.score_after = state.score;
        step.selected_bink_stream =
            static_cast<std::int32_t>(state.quality);
        if (state.quality != StuntQuality::Bad) {
            step.quality_overlay_index =
                static_cast<std::int32_t>(state.quality);
        }
        return step;
    }

    // 1. Retail first checks the per-stunt sound-trigger frames. The selected
    // quality determines both the sound row and the number of points awarded.
    if (const auto trigger =
            sound_trigger_stunt_at_frame(timing, sounds, input.frame)) {
        step.sound_request = StuntSoundRequest{
            *trigger,
            state.quality,
            false,
        };
        step.score_delta += score_for_quality(state.quality);
        state.score += score_for_quality(state.quality);
    }

    // 2. Then the active stunt is returned to Bad at its configured return
    // frame. This happens after its normal sound/score opportunity.
    if (state.active_stunt >= 0 &&
        state.active_stunt <
            static_cast<std::int32_t>(timing.stunt_count)) {
        const auto active = static_cast<std::size_t>(state.active_stunt);
        if (timing.return_to_bad_frames[active] == input.frame) {
            step.returned_stunt = active;
            state.quality = StuntQuality::Bad;
            state.active_stunt = -1;
        }
    }

    // 3. New stunt input is accepted only while no stunt is currently active.
    // The window thresholds map exactly to quality values 1, 2, and 3.
    if (input.stunt_press && state.active_stunt == -1) {
        if (const auto stunt =
                accepted_stunt_for_press(timing, input.frame)) {
            state.active_stunt = static_cast<std::int32_t>(*stunt);
            state.quality =
                timing.windows[*stunt].quality_for_press(input.frame);
            step.accepted_stunt = *stunt;
        }
    }

    // 4. The pass marker is checked after input handling. The first encounter
    // manually performs the last stunt's sound/score because its configured
    // trigger is frame 665, after the marker at 660. The second encounter
    // transitions immediately to end.bik and does NOT score stunt 7 again.
    if (input.frame == timing.loop_end_frame) {
        step.loop_marker_seen = true;
        ++state.loop_marker_hits;

        if (state.loop_marker_hits >= kRetailPlaybackPasses) {
            state.phase = PlaybackPhase::EndMovie;
            step.start_end_movie = true;
        } else {
            constexpr auto last_stunt = kStuntCount - 1;
            step.sound_request = StuntSoundRequest{
                last_stunt,
                state.quality,
                true,
            };
            step.first_pass_final_stunt_scored = true;
            const auto points = score_for_quality(state.quality);
            step.score_delta += points;
            state.score += points;
        }
    }

    step.quality_after = state.quality;
    step.active_stunt_after = state.active_stunt;
    step.score_after = state.score;
    step.selected_bink_stream =
        static_cast<std::int32_t>(state.quality);

    if (state.quality != StuntQuality::Bad) {
        step.quality_overlay_index =
            static_cast<std::int32_t>(state.quality);
    }

    return step;
}

} // namespace btb::spud_skate
