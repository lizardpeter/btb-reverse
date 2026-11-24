#pragma once

#include <cstdint>
#include <optional>

namespace btb::maze {

inline constexpr std::int32_t kLowTimeWarningSecond = 15;
inline constexpr std::int32_t kLowTimeWarningSoundId = 83;
inline constexpr std::int32_t kOutcomeFastSuccessSoundId = 84;
inline constexpr std::int32_t kOutcomeSlowSuccessSoundId = 85;
inline constexpr std::int32_t kOutcomeTimeoutSoundId = 86;
inline constexpr std::int32_t kMazeFeedbackPriority = 90;
inline constexpr std::int32_t kMazeFeedbackArbitrationClass = 1;

enum class EndCondition {
    None,
    Success,
    Timeout,
};

// Exact gate at the front of 0x0041D4D0.
//
// Both success and timeout are held off while the horizontal screen transition
// controller is active.
[[nodiscard]] constexpr EndCondition classify_end_condition(
    std::int32_t packages_remaining,
    std::int32_t remaining_seconds,
    std::int32_t screen_transition_state) noexcept {

    if (screen_transition_state != 0) {
        return EndCondition::None;
    }
    if (packages_remaining <= 0) {
        return EndCondition::Success;
    }
    if (remaining_seconds < 0) {
        return EndCondition::Timeout;
    }
    return EndCondition::None;
}

[[nodiscard]] constexpr bool should_play_low_time_warning(
    std::int32_t remaining_seconds) noexcept {
    // Retail has no separate one-shot latch in this branch.
    return remaining_seconds == kLowTimeWarningSecond;
}

struct FeedbackPlan {
    bool stop_managed_sound{true};
    std::int32_t sound_id{};
    std::int32_t priority{kMazeFeedbackPriority};
    std::int32_t arbitration_class{kMazeFeedbackArbitrationClass};
    bool draw_activity{true};
    std::int32_t next_outcome_phase{1};
};

// Phase-0 outcome sound selection at 0x0041D55F..0x0041D5A9.
[[nodiscard]] constexpr std::optional<FeedbackPlan> begin_outcome_feedback(
    EndCondition condition,
    std::int32_t remaining_seconds) noexcept {

    switch (condition) {
    case EndCondition::None:
        return std::nullopt;
    case EndCondition::Success:
        return FeedbackPlan{
            true,
            remaining_seconds > kLowTimeWarningSecond
                ? kOutcomeFastSuccessSoundId
                : kOutcomeSlowSuccessSoundId,
            kMazeFeedbackPriority,
            kMazeFeedbackArbitrationClass,
            true,
            1,
        };
    case EndCondition::Timeout:
        return FeedbackPlan{
            true,
            kOutcomeTimeoutSoundId,
            kMazeFeedbackPriority,
            kMazeFeedbackArbitrationClass,
            true,
            1,
        };
    }

    return std::nullopt;
}

struct OutcomePhaseOnePlan {
    bool draw_activity{true};
    bool wait_for_managed_feedback{};
    bool advance_phase{};
};

[[nodiscard]] constexpr OutcomePhaseOnePlan outcome_phase_one(
    bool managed_feedback_playing) noexcept {
    return {
        true,
        managed_feedback_playing,
        !managed_feedback_playing,
    };
}

// On successful Maze completion the adjacent player-progress value at +0xC8
// selects whether the shared post-game movie code becomes 3 or remains -1.
[[nodiscard]] constexpr std::int32_t success_transition_code(
    std::int32_t adjacent_progress_value) noexcept {
    return adjacent_progress_value == 1 ? 3 : -1;
}

inline constexpr std::int32_t kPlayAgainOuterState = 0x3C;
inline constexpr std::int32_t kMazePlayAgainUiContext = 0x20;
inline constexpr std::int32_t kPostOutcomeTimerSentinel = 999;

struct PlayAgainPlan {
    bool prepare_play_again{true};
    std::int32_t remaining_seconds{kPostOutcomeTimerSentinel};
    std::int32_t outer_state{kPlayAgainOuterState};
    std::int32_t ui_context{kMazePlayAgainUiContext};
    bool unload_maze_resources{true};
    bool set_shared_transition_flag{true};
};

inline constexpr PlayAgainPlan kPlayAgainPlan{};

struct StartupVoicePlan {
    bool stop_other_activity_sound{true};
    std::int32_t shared_sound_group{5};
    bool stop_managed_sound{true};
    std::int32_t sound_id{};
    std::int32_t priority{kMazeFeedbackPriority};
    std::int32_t arbitration_class{kMazeFeedbackArbitrationClass};
    bool mark_input_interruptible{true};
};

[[nodiscard]] constexpr StartupVoicePlan startup_voice(
    std::int32_t difficulty_index) noexcept {
    return {
        true,
        5,
        true,
        0x49 + difficulty_index,
        kMazeFeedbackPriority,
        kMazeFeedbackArbitrationClass,
        true,
    };
}

inline constexpr std::int32_t kMazeChooserOuterState = 0x1C;
inline constexpr std::int32_t kSharedMovieOuterState = 0x40;

struct LeavePlan {
    bool unload_maze_resources{true};
    bool clear_leave_request{};
    std::optional<std::int32_t> outer_state{};
};

// Exact exit tail at 0x0041D775. A normal leave request first selects 0x1C,
// then any shared transition code other than -1 overrides it with movie state
// 0x40. A fatal/forced exit with neither condition does not write outer state.
[[nodiscard]] constexpr LeavePlan leave_plan(
    bool leave_requested,
    std::int32_t shared_transition_code) noexcept {

    LeavePlan plan;
    plan.clear_leave_request = leave_requested;

    if (leave_requested) {
        plan.outer_state = kMazeChooserOuterState;
    }
    if (shared_transition_code != -1) {
        plan.outer_state = kSharedMovieOuterState;
    }

    return plan;
}

} // namespace btb::maze
