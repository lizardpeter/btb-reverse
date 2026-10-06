#include "btb/grand_opening_runtime.hpp"

#include <cassert>

using namespace btb::grand_opening;

int main() {
    static_assert(kPlaybackPreviousStepSentinel == 999999);

    constexpr auto prepared =
        prepare_playback(2, Conductor::Wendy);
    static_assert(prepared.next_state == ActivityState::Playing);
    static_assert(prepared.progress_index == 253);
    static_assert(prepared.backing_track == "Wendymt.wav");
    static_assert(
        prepared.previous_elapsed_centiseconds ==
        kPlaybackPreviousStepSentinel);
    static_assert(prepared.shared_completion_code == 6);

    static_assert(kConductorPlaybackStartFrame == 50);
    static_assert(kConductorPlaybackEndExclusive[0] == 90);
    static_assert(kConductorPlaybackEndExclusive[1] == 85);
    static_assert(kConductorPlaybackEndExclusive[2] == 105);
    static_assert(kConductorPlaybackFrameTicks == 4);

    ConductorAnimationState bob_animation;
    for (int i = 0; i < 3; ++i) {
        const auto anim = tick_conductor_animation(
            Conductor::Bob, bob_animation);
        assert(!anim.frame_advanced);
        assert(bob_animation.frame == 0);
    }
    auto conductor_step = tick_conductor_animation(
        Conductor::Bob, bob_animation);
    assert(conductor_step.frame_advanced);
    assert(!conductor_step.wrapped);
    assert(bob_animation.frame == 50);
    assert(bob_animation.tick == 0);

    bob_animation.frame = 89;
    bob_animation.tick = 3;
    conductor_step = tick_conductor_animation(
        Conductor::Bob, bob_animation);
    assert(conductor_step.frame_advanced);
    assert(conductor_step.wrapped);
    assert(bob_animation.frame == 50);

    ConductorAnimationState wendy_animation{84, 3};
    conductor_step = tick_conductor_animation(
        Conductor::Wendy, wendy_animation);
    assert(conductor_step.wrapped);
    assert(wendy_animation.frame == 50);

    ConductorAnimationState farmer_animation{104, 3};
    conductor_step = tick_conductor_animation(
        Conductor::FarmerPickles, farmer_animation);
    assert(conductor_step.wrapped);
    assert(farmer_animation.frame == 50);

    Composition composition;
    assert(place_event(
        composition, 0, 0, MachineType::Roley1Second));
    assert(place_event(
        composition, 1, 0, MachineType::Muck2Second));
    assert(place_event(
        composition, 2, 1, MachineType::Lofty1Second));
    assert(place_event(
        composition, 4, 23, MachineType::Scoop2Second) == false);
    assert(place_event(
        composition, 4, 23, MachineType::Scoop1Second));

    std::array<std::int32_t,5> animations{};

    // State 8 seeds previous elapsed with 999999, forcing the first second to
    // trigger immediately even though elapsed centiseconds begin at zero.
    auto step = update_playback_second(
        composition,
        0,
        kPlaybackPreviousStepSentinel,
        animations);
    assert(step.second == 0);
    assert(step.second_changed);
    assert(!step.timeline_complete);
    assert(step.triggers.size() == 2);

    assert(step.triggers[0].pitch_row == 0);
    assert(step.triggers[0].machine_type == MachineType::Roley1Second);
    assert(step.triggers[0].loaded_sound_slot == 40);
    assert(step.triggers[0].machine == Machine::Roley);
    assert(step.triggers[0].machine_animation_state_written == 1);

    assert(step.triggers[1].pitch_row == 1);
    assert(step.triggers[1].machine_type == MachineType::Muck2Second);
    assert(step.triggers[1].loaded_sound_slot == 33);
    assert(step.triggers[1].machine == Machine::Muck);
    assert(step.triggers[1].machine_animation_state_written == 2);

    assert(animations[0] == 1);
    assert(animations[1] == 2);

    // No re-trigger occurs while still inside the same one-second bucket.
    step = update_playback_second(
        composition, 99, 0, animations);
    assert(step.second == 0);
    assert(!step.second_changed);
    assert(step.triggers.empty());

    // Crossing to second 1 triggers the row-2 Lofty event.
    step = update_playback_second(
        composition, 100, 99, animations);
    assert(step.second == 1);
    assert(step.second_changed);
    assert(step.triggers.size() == 1);
    assert(step.triggers[0].pitch_row == 2);
    assert(step.triggers[0].machine_type == MachineType::Lofty1Second);
    assert(step.triggers[0].loaded_sound_slot == 24);
    assert(step.triggers[0].machine_animation_state_written == 1);

    // Continuation cells are not machine triggers.
    step = update_playback_second(
        composition, 600, 599, animations);
    assert(step.second == 6);
    assert(step.second_changed);
    assert(step.triggers.empty());

    // If a machine animation is already active, retail still plays the WAV
    // but does not overwrite that machine's current animation state.
    Composition repeated;
    assert(place_event(
        repeated, 4, 2, MachineType::Roley2Second));
    animations[0] = 7;
    step = update_playback_second(
        repeated, 200, 100, animations);
    assert(step.triggers.size() == 1);
    assert(step.triggers[0].loaded_sound_slot == 1);
    assert(step.triggers[0].machine_animation_state_written == 0);
    assert(animations[0] == 7);

    step = update_playback_second(
        composition, 2300, 2299, animations);
    assert(step.second == 23);
    assert(step.triggers.size() == 1);
    assert(step.triggers[0].machine_type == MachineType::Scoop1Second);
    assert(step.triggers[0].loaded_sound_slot == 8);

    step = update_playback_second(
        composition, 2400, 2399, animations);
    assert(step.second == -1);
    assert(step.timeline_complete);
    assert(step.triggers.empty());

    constexpr auto stop =
        update_playing_state(true, true);
    static_assert(stop.next_state == ActivityState::Edit);
    static_assert(stop.stop_backing_track);

    constexpr auto ended =
        update_playing_state(false, false);
    static_assert(ended.next_state == ActivityState::Edit);
    static_assert(!ended.stop_backing_track);

    constexpr auto still_playing =
        update_playing_state(false, true);
    static_assert(still_playing.next_state == ActivityState::Playing);

    static_assert(kMusicChooserOuterState == 0x2A);
    static_assert(kPlayAgainOuterState == 0x3C);
    static_assert(kSharedMovieOuterState == 0x40);
    static_assert(kBobsBandPlayAgainContext == 0x2E);

    constexpr auto normal_edit =
        update_outer_activity({ActivityState::Edit, false, false, false});
    static_assert(!normal_edit.return_abort);
    static_assert(!normal_edit.save_and_unload);

    constexpr auto leave_to_music =
        update_outer_activity({ActivityState::Edit, false, true, false});
    static_assert(leave_to_music.return_abort);
    static_assert(leave_to_music.save_and_unload);
    static_assert(leave_to_music.clear_leave_activity_request);
    static_assert(
        leave_to_music.saved_frontend_state &&
        *leave_to_music.saved_frontend_state == 0x2A);
    static_assert(
        leave_to_music.outer_state &&
        *leave_to_music.outer_state == 0x2A);

    constexpr auto leave_to_shared_movie =
        update_outer_activity({ActivityState::Edit, false, true, true});
    static_assert(
        leave_to_shared_movie.outer_state &&
        *leave_to_shared_movie.outer_state == 0x40);

    constexpr auto quit_with_shared_movie =
        update_outer_activity({ActivityState::Edit, true, false, true});
    static_assert(quit_with_shared_movie.return_abort);
    static_assert(quit_with_shared_movie.save_and_unload);
    static_assert(
        quit_with_shared_movie.outer_state &&
        *quit_with_shared_movie.outer_state == 0x40);

    constexpr auto completion =
        update_outer_activity(
            {ActivityState::ExitToPlayAgain, false, false, false});
    static_assert(completion.return_abort);
    static_assert(completion.save_and_unload);
    static_assert(completion.prepare_play_again);
    static_assert(
        completion.outer_state &&
        *completion.outer_state == 0x3C);
    static_assert(
        completion.play_again_context &&
        *completion.play_again_context == 0x2E);
    static_assert(completion.clear_shared_transition_flag);
}
