#include "btb/spud_skate_runtime.hpp"

#include <cassert>

using namespace btb::spud_skate;

namespace {

TimingData make_timing() {
    TimingData data;
    data.stunt_count = 8;
    data.windows = {{
        {85, 97, 107, 116},
        {134, 146, 156, 165},
        {222, 234, 244, 253},
        {290, 302, 312, 322},
        {376, 388, 399, 409},
        {461, 473, 483, 492},
        {543, 555, 566, 575},
        {631, 643, 653, 661},
    }};
    data.return_to_bad_frames =
        {134, 202, 271, 360, 426, 531, 592, 679};
    data.loop_start_frame = 44;
    data.loop_end_frame = 660;
    return data;
}

SoundData make_sounds() {
    SoundData sounds;
    sounds.trigger_frames =
        {124, 180, 260, 330, 415, 500, 580, 665};
    return sounds;
}

void score_good_stunt(
    RuntimeState& state,
    const TimingData& timing,
    const SoundData& sounds,
    std::size_t stunt) {

    const auto& window = timing.windows[stunt];
    auto step = update_synchronized_run(
        state, timing, sounds, {window.good_start, true});
    assert(step.accepted_stunt == stunt);
    assert(state.quality == StuntQuality::Good);
    assert(state.active_stunt == static_cast<int>(stunt));

    if (stunt < 7) {
        step = update_synchronized_run(
            state, timing, sounds, {sounds.trigger_frames[stunt], false});
        assert(step.sound_request);
        assert(step.sound_request->stunt == stunt);
        assert(step.sound_request->quality == StuntQuality::Good);
        assert(!step.sound_request->first_pass_final_stunt_special);
        assert(step.score_delta == 3);
    }
}

} // namespace

int main() {
    const auto timing = make_timing();
    const auto sounds = make_sounds();

    static_assert(kRetailPlaybackPasses == 2);
    static_assert(kRetailScoringOpportunities == 15);
    static_assert(kRetailMaximumScore == 45);
    static_assert(kMaximumScore == 45);

    static_assert(
        sound_trigger_stunt_at_frame(
            TimingData{
                1,
                {{{85,97,107,116}}},
                {},
                44,
                660},
            SoundData{{}, {124}},
            124) == 0);

    // Quality selection is exactly 1/2/3 from the three successful timing
    // subranges; Bad remains 0 when no press is accepted.
    RuntimeState quality_state;
    auto step = update_synchronized_run(
        quality_state, timing, sounds, {85, true});
    assert(step.accepted_stunt == 0);
    assert(quality_state.quality == StuntQuality::Normal);
    assert(step.selected_bink_stream == 1);
    assert(step.quality_overlay_index == 1);

    RuntimeState okay_state;
    step = update_synchronized_run(
        okay_state, timing, sounds, {97, true});
    assert(okay_state.quality == StuntQuality::Okay);
    assert(step.selected_bink_stream == 2);

    RuntimeState good_state;
    step = update_synchronized_run(
        good_state, timing, sounds, {107, true});
    assert(good_state.quality == StuntQuality::Good);
    assert(step.selected_bink_stream == 3);

    // The configured trigger scores the currently selected quality directly.
    step = update_synchronized_run(
        good_state, timing, sounds, {124, false});
    assert(step.sound_request);
    assert(step.sound_request->stunt == 0);
    assert(step.sound_request->quality == StuntQuality::Good);
    assert(step.score_delta == 3);
    assert(good_state.score == 3);

    // Return-to-Bad occurs after the sound-trigger/score check.
    step = update_synchronized_run(
        good_state, timing, sounds, {134, false});
    assert(step.returned_stunt == 0);
    assert(good_state.quality == StuntQuality::Bad);
    assert(good_state.active_stunt == -1);
    assert(step.selected_bink_stream == 0);
    assert(!step.quality_overlay_index);

    // A press is ignored while another stunt remains active.
    RuntimeState locked_stunt;
    step = update_synchronized_run(
        locked_stunt, timing, sounds, {107, true});
    assert(step.accepted_stunt == 0);
    step = update_synchronized_run(
        locked_stunt, timing, sounds, {156, true});
    assert(!step.accepted_stunt);
    assert(locked_stunt.active_stunt == 0);

    // First pass: seven ordinary trigger-frame scores plus the special final
    // score at frame 660 because stunt 7's configured trigger is 665.
    RuntimeState perfect;
    for (std::size_t stunt = 0; stunt < 7; ++stunt) {
        score_good_stunt(perfect, timing, sounds, stunt);
        step = update_synchronized_run(
            perfect,
            timing,
            sounds,
            {timing.return_to_bad_frames[stunt], false});
        assert(step.returned_stunt == stunt);
    }

    score_good_stunt(perfect, timing, sounds, 7);
    assert(perfect.score == 21);

    step = update_synchronized_run(
        perfect, timing, sounds, {660, false});
    assert(step.loop_marker_seen);
    assert(perfect.loop_marker_hits == 1);
    assert(step.first_pass_final_stunt_scored);
    assert(step.sound_request);
    assert(step.sound_request->stunt == 7);
    assert(step.sound_request->quality == StuntQuality::Good);
    assert(step.sound_request->first_pass_final_stunt_special);
    assert(step.score_delta == 3);
    assert(perfect.score == 24);
    assert(!step.start_end_movie);
    assert(perfect.phase == PlaybackPhase::SynchronizedRun);

    // Playback continues beyond 660 on pass one; frame 679 clears stunt 7
    // before the Binks eventually wrap back to frame 44.
    step = update_synchronized_run(
        perfect, timing, sounds, {679, false});
    assert(step.returned_stunt == 7);
    assert(perfect.quality == StuntQuality::Bad);
    assert(perfect.active_stunt == -1);

    // Second pass scores only stunts 0..6.
    for (std::size_t stunt = 0; stunt < 7; ++stunt) {
        score_good_stunt(perfect, timing, sounds, stunt);
        step = update_synchronized_run(
            perfect,
            timing,
            sounds,
            {timing.return_to_bad_frames[stunt], false});
        assert(step.returned_stunt == stunt);
    }
    assert(perfect.score == 45);

    // The second observation of frame 660 enters end.bik immediately. There
    // is no second special score for stunt 7.
    step = update_synchronized_run(
        perfect, timing, sounds, {660, false});
    assert(step.loop_marker_seen);
    assert(perfect.loop_marker_hits == 2);
    assert(step.start_end_movie);
    assert(!step.first_pass_final_stunt_scored);
    assert(step.score_delta == 0);
    assert(perfect.score == 45);
    assert(perfect.phase == PlaybackPhase::EndMovie);

    // Exact result-voice fallthrough behavior.
    constexpr auto low = result_presentation(9);
    static_assert(low.tier == ResultTier::Low);
    static_assert(low.sound_pool_count == 2);
    static_assert(low.sound_pools[0].first_id == 773);
    static_assert(low.sound_pools[0].count == 2);
    static_assert(low.sound_pools[1].first_id == 775);
    static_assert(low.sound_pools[1].count == 3);

    constexpr auto medium = result_presentation(10);
    static_assert(medium.tier == ResultTier::Medium);
    static_assert(medium.sound_pool_count == 1);
    static_assert(medium.sound_pools[0].first_id == 775);
    static_assert(medium.sound_pools[0].count == 3);

    constexpr auto high = result_presentation(45);
    static_assert(high.tier == ResultTier::High);
    static_assert(high.sound_pool_count == 1);
    static_assert(high.sound_pools[0].first_id == 778);
    static_assert(high.sound_pools[0].count == 2);
}
