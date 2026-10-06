#include "btb/bink_system.hpp"

#include <cassert>
#include <string_view>

using namespace btb::bink;

int main() {
    static_assert(kSharedPlaybackSurface.descriptor_size == 0x7C);
    static_assert(kSharedPlaybackSurface.descriptor_flags == 0x7);
    static_assert(kSharedPlaybackSurface.width == 640);
    static_assert(kSharedPlaybackSurface.height == 480);
    static_assert(kSharedPlaybackSurface.surface_caps == 0x40);

    static_assert(kInitializePlan.create_offscreen_surface);
    static_assert(kInitializePlan.bind_bink_sound_system);
    static_assert(kInitializePlan.use_bink_open_direct_sound);
    static_assert(kInitializePlan.bink_sound_system_user_value == 0);
    static_assert(!kInitializePlan.paused_after_initialize);
    static_assert(kRestartPlan.target_frame == 1);
    static_assert(kRestartPlan.flags == 0);

    const auto fallback = cd_fallback_path(
        "Data\\Movies\\intro.bik", 4);
    assert(fallback);
    assert(*fallback == "e:\\Data\\Movies\\intro.bik");
    assert(!cd_fallback_path("Data\\Movies\\intro.bik", -1));

    const auto generic = generic_open_plan(
        "Data\\Movies\\intro.bik", 4);
    assert(generic.direct_path == "Data\\Movies\\intro.bik");
    assert(generic.fallback_path);
    assert(generic.flags == 0x04000000);
    assert(generic.apply_global_volume_after_attempt);
    assert(!generic.abort_if_all_attempts_fail);
    assert(!generic.disable_input_after_success);

    const auto global = global_open_plan(
        "Data\\Movies\\intro.bik", 4);
    assert(global.flags == 0);
    assert(global.apply_global_volume_after_attempt);
    assert(global.abort_if_all_attempts_fail);
    assert(global.disable_input_after_success);

    // Surface lock failure returns complete/error immediately.
    constexpr auto lock_fail = generic_frame_step({
        FrameMode::SkippableOneShot,
        1,
        100,
        {},
        false,
        true,
    });
    static_assert(lock_fail.complete);
    static_assert(!lock_fail.copy_to_surface);
    static_assert(!lock_fail.blit_to_backbuffer);

    // Mode 0 is a skippable one-shot.
    constexpr auto skipped = generic_frame_step({
        FrameMode::SkippableOneShot,
        10,
        100,
        {true,false,false},
        true,
        false,
    });
    static_assert(skipped.copy_to_surface);
    static_assert(skipped.unlock_surface);
    static_assert(skipped.complete);
    static_assert(!skipped.advance_frame);
    static_assert(!skipped.blit_to_backbuffer);

    constexpr auto one_shot_end = generic_frame_step({
        FrameMode::SkippableOneShot,
        100,
        100,
        {},
        true,
        false,
    });
    static_assert(one_shot_end.complete);

    // Mode 2 ignores input, but still stops at the final frame. On its first
    // nonterminal update with the latch clear, retail decodes/blits without
    // BinkNextFrame and then raises the shared advance latch.
    constexpr auto unskippable_first = generic_frame_step({
        FrameMode::UnskippableOneShot,
        1,
        100,
        {true,true,true},
        false,
        false,
    });
    static_assert(!unskippable_first.complete);
    static_assert(!unskippable_first.advance_frame);
    static_assert(!unskippable_first.wait_for_bink);
    static_assert(unskippable_first.set_frame_advance_latch);
    static_assert(unskippable_first.blit_to_backbuffer);

    constexpr auto unskippable_next = generic_frame_step({
        FrameMode::UnskippableOneShot,
        2,
        100,
        {},
        true,
        false,
    });
    static_assert(unskippable_next.advance_frame);
    static_assert(unskippable_next.wait_for_bink);
    static_assert(unskippable_next.blit_to_backbuffer);

    constexpr auto unskippable_end = generic_frame_step({
        FrameMode::UnskippableOneShot,
        100,
        100,
        {true,false,false},
        true,
        false,
    });
    static_assert(unskippable_end.complete);
    static_assert(!unskippable_end.advance_frame);

    // Mode 1 is the looping path: even with the latch initially clear and even
    // at the nominal final frame, this helper continues through NextFrame/Wait.
    constexpr auto looping_first = generic_frame_step({
        FrameMode::Looping,
        1,
        100,
        {},
        false,
        false,
    });
    static_assert(!looping_first.complete);
    static_assert(looping_first.advance_frame);
    static_assert(looping_first.wait_for_bink);
    static_assert(looping_first.set_frame_advance_latch);

    constexpr auto looping_end = generic_frame_step({
        FrameMode::Looping,
        100,
        100,
        {true,true,true},
        false,
        false,
    });
    static_assert(!looping_end.complete);
    static_assert(looping_end.advance_frame);
    static_assert(looping_end.blit_to_backbuffer);

    constexpr auto no_advance_ok =
        no_advance_frame_step({false});
    static_assert(no_advance_ok.decode_frame);
    static_assert(no_advance_ok.lock_surface);
    static_assert(no_advance_ok.copy_to_surface);
    static_assert(no_advance_ok.unlock_surface);
    static_assert(!no_advance_ok.failed);

    constexpr auto no_advance_fail =
        no_advance_frame_step({true});
    static_assert(no_advance_fail.failed);
    static_assert(!no_advance_fail.copy_to_surface);
    static_assert(!no_advance_fail.unlock_surface);

    // The global movie loop has a separate contract: it closes on end or
    // enabled skip input, otherwise it always advances/waits and blits at 0,0.
    constexpr auto global_end = global_frame_step({
        50,
        50,
        {},
        false,
        false,
    });
    static_assert(global_end.copy_to_surface);
    static_assert(global_end.unlock_surface);
    static_assert(global_end.close_movie);
    static_assert(global_end.complete);
    static_assert(!global_end.advance_frame);

    constexpr auto global_skip = global_frame_step({
        10,
        50,
        {false,true,false},
        true,
        false,
    });
    static_assert(global_skip.close_movie);
    static_assert(global_skip.complete);

    constexpr auto global_skip_disabled = global_frame_step({
        10,
        50,
        {false,true,false},
        false,
        false,
    });
    static_assert(!global_skip_disabled.close_movie);
    static_assert(!global_skip_disabled.complete);
    static_assert(global_skip_disabled.advance_frame);
    static_assert(global_skip_disabled.wait_for_bink);
    static_assert(global_skip_disabled.blit_shared_surface_at_origin);

    constexpr auto global_lock_fail = global_frame_step({
        10,
        50,
        {},
        true,
        true,
    });
    static_assert(global_lock_fail.complete);
    static_assert(!global_lock_fail.close_movie);
    static_assert(!global_lock_fail.copy_to_surface);

    static_assert(kMinimumBinkVolume == 37);
    static_assert(bink_volume_from_game_volume(0) == 37);
    static_assert(bink_volume_from_game_volume(4) == 37);
    static_assert(bink_volume_from_game_volume(5) == 1550);
    static_assert(bink_volume_from_game_volume(10) == 3100);
    static_assert(bink_volume_from_game_volume(100) == 31000);

    GlobalPlaybackState lifecycle;
    auto opened = on_global_open_success(lifecycle, 10);
    assert(lifecycle.has_movie);
    assert(!lifecycle.paused);
    assert(!lifecycle.input_processing_enabled);
    assert(opened.apply_volume);
    assert(opened.volume == 3100);
    assert(opened.disable_input_processing);

    auto closed = close_global_movie(lifecycle);
    assert(closed.call_bink_close);
    assert(closed.clear_global_handle);
    assert(closed.enable_input_processing);
    assert(!lifecycle.has_movie);
    assert(lifecycle.input_processing_enabled);

    closed = close_global_movie(lifecycle);
    assert(!closed.call_bink_close);
    assert(closed.clear_global_handle);
    assert(closed.enable_input_processing);

    GlobalPlaybackState playback{true,false,true};
    auto pause = pause_global_movie(playback);
    assert(pause.call_bink_pause);
    assert(pause.pause_value);
    assert(pause.set_volume && pause.volume == 0);
    assert(!pause.input_processing_enabled);
    assert(playback.paused);
    assert(!playback.input_processing_enabled);
    assert(!should_apply_global_volume(playback));

    auto resume = resume_global_movie(playback, 10);
    assert(resume.call_bink_pause);
    assert(!resume.pause_value);
    assert(resume.set_volume && resume.volume == 3100);
    assert(resume.input_processing_enabled);
    assert(!playback.paused);
    assert(playback.input_processing_enabled);
    assert(should_apply_global_volume(playback));

    GlobalPlaybackState no_movie;
    pause = pause_global_movie(no_movie);
    assert(!pause.call_bink_pause);
    resume = resume_global_movie(no_movie, 10);
    assert(!resume.call_bink_pause);
}
