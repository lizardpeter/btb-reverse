#pragma once

#include "btb/grand_opening_sequence.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::grand_opening {

inline constexpr std::int32_t kPlaybackPreviousStepSentinel = 999999;

struct PlaybackTrigger {
    std::size_t pitch_row{};
    std::size_t second{};
    MachineType machine_type{MachineType::Roley1Second};
    std::int32_t loaded_sound_slot{};
    Machine machine{Machine::Roley};
    std::int32_t machine_animation_state_written{};
};

struct PlaybackSecondStep {
    std::int32_t second{-1};
    bool timeline_complete{};
    bool second_changed{};
    std::vector<PlaybackTrigger> triggers{};
};

// Exact one-second sound-trigger branch in
// 0x0041FC90 PlayAndDrawGrandOpeningComposition.
//
// The caller owns the five machine-animation state values corresponding to
// Roley/Muck/Lofty/Dizzy/Scoop. Retail writes parity+1 (1 for short, 2 for
// long) only when that machine's animation state is currently zero.
[[nodiscard]] PlaybackSecondStep update_playback_second(
    const Composition& composition,
    std::int32_t elapsed_centiseconds,
    std::int32_t previous_elapsed_centiseconds,
    std::array<std::int32_t,5>& machine_animation_states);

struct PreparePlaybackStep {
    ActivityState next_state{ActivityState::Playing};
    std::size_t progress_index{};
    std::string_view backing_track{};
    std::int32_t previous_elapsed_centiseconds{
        kPlaybackPreviousStepSentinel};
};

// Exact state-8 setup: mark current conductor progress, capture the performance
// counter start externally, start the conductor backing track, initialize the
// previous elapsed-time sentinel, and advance to state 9.
[[nodiscard]] constexpr PreparePlaybackStep prepare_playback(
    std::size_t zero_based_player_profile,
    Conductor conductor) noexcept {
    return {
        ActivityState::Playing,
        conductor_progress_index(zero_based_player_profile, conductor),
        backing_track_filename(conductor),
        kPlaybackPreviousStepSentinel,
    };
}

struct PlayingStateStep {
    ActivityState next_state{ActivityState::Playing};
    bool stop_backing_track{};
};

// During state 9, the Stop toolbar action returns directly to edit mode.
// Retail also returns to edit mode automatically when the conductor backing
// track is no longer playing.
[[nodiscard]] constexpr PlayingStateStep update_playing_state(
    bool stop_control_activated,
    bool backing_track_is_playing) noexcept {
    if (stop_control_activated) {
        return {ActivityState::Edit, true};
    }
    if (!backing_track_is_playing) {
        return {ActivityState::Edit, false};
    }
    return {};
}

} // namespace btb::grand_opening
