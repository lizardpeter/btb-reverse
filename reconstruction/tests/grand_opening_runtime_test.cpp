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
}
