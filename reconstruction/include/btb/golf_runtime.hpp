#pragma once

#include "btb/golf_data.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::golf {

struct Vec2f {
    float x{};
    float y{};
};

enum class RoundState : std::int32_t {
    Aim = 0,
    PowerMeter = 1,
    LaunchSetup = 2,
    BallFlight = 3,
    ResolveLanding = 4,
    ScoreAndFeedback = 5,
    ResetNextAttempt = 6,
    SwingAnimationDelay = 99,
};

struct ShotParameters {
    std::int32_t angle_degrees{};
    std::int32_t power{};
    std::int32_t speed{};
};

struct BallMotion {
    Vec2f position{};
    std::int32_t angle_degrees{};
    std::int32_t speed{};

    [[nodiscard]] bool stopped() const noexcept { return speed == 0; }
};

inline constexpr std::int32_t kAimMinimum = 0;
inline constexpr std::int32_t kAimMaximum = 88;
inline constexpr std::int32_t kPowerMinimum = 0;
inline constexpr std::int32_t kPowerMaximum = 1000;
inline constexpr std::int32_t kRetailInitialAttempts = 5;
inline constexpr float kTargetRadius = 15.0f;
inline constexpr float kLargeLandingRadius = 50.0f;
inline constexpr double kBallSpeedDecayFraction = 0.012;
inline constexpr std::int32_t kBallSpeedMinimumDecay = 10;
inline constexpr std::int32_t kBallStopThreshold = 20;

[[nodiscard]] std::int32_t clamp_aim(std::int32_t aim) noexcept;
[[nodiscard]] std::int32_t quantize_aim_angle(std::int32_t aim) noexcept;

void advance_power_meter(
    std::int32_t& power,
    std::int32_t& direction,
    std::int32_t difficulty_speed) noexcept;

[[nodiscard]] ShotParameters make_shot(
    std::int32_t aim,
    std::int32_t power) noexcept;

[[nodiscard]] Vec2f velocity_for(
    std::int32_t angle_degrees,
    std::int32_t speed) noexcept;

void advance_ball(BallMotion& ball) noexcept;

[[nodiscard]] float distance(Vec2f a, Vec2f b) noexcept;
[[nodiscard]] Vec2i target_center(const AnimatedCourseObject& object) noexcept;
[[nodiscard]] bool ball_reaches_target(
    Vec2f ball,
    const AnimatedCourseObject& object) noexcept;

// The retail distance calls first convert the floating ball coordinates to
// int32 through the shared x87 truncate-toward-zero helper.
[[nodiscard]] constexpr Vec2i retail_ball_point(
    Vec2f ball) noexcept {
    return {
        static_cast<std::int32_t>(ball.x),
        static_cast<std::int32_t>(ball.y),
    };
}

enum class CourseTarget : std::int32_t {
    Flag = 0,
    Windmill = 1,
    Clown = 2,
};

inline constexpr std::array<std::int32_t,3>
kTargetPointValues{{1,3,5}};

[[nodiscard]] constexpr std::int32_t target_point_value(
    std::int32_t target_index) noexcept {
    return target_index >= 0 && target_index < 3
        ? kTargetPointValues[static_cast<std::size_t>(target_index)]
        : 0;
}

struct LandingResolution {
    std::int32_t target_index{-1};
    Vec2f ball_position{};
    bool snapped_to_target{};
    bool reset_ball_animation_frame{true};
};

// Exact state-4 behavior. Retail clears target index to -1 and scans Flag,
// Windmill, then Clown. Distance is measured using integer-truncated ball
// coordinates. Every strict <15 match updates the index and snaps the ball to
// that target center; the loop does not break.
[[nodiscard]] LandingResolution resolve_landing(
    Vec2f ball,
    const std::array<AnimatedCourseObject,3>& targets) noexcept;

inline constexpr std::int32_t kFlagAimBoundary = 24;
inline constexpr std::int32_t kClownAimBoundary = 50;

inline constexpr std::array<std::int32_t,3>
kExpectedAimByTarget{{0,60,34}};

inline constexpr std::array<std::int32_t,3>
kExpectedAimToleranceByTarget{{10,10,6}};

[[nodiscard]] constexpr std::int32_t expected_target_for_aim(
    std::int32_t aim) noexcept {
    return aim < kFlagAimBoundary
        ? static_cast<std::int32_t>(CourseTarget::Flag)
        : aim < kClownAimBoundary
            ? static_cast<std::int32_t>(CourseTarget::Clown)
            : static_cast<std::int32_t>(CourseTarget::Windmill);
}

[[nodiscard]] constexpr bool aim_within_expected_target_tolerance(
    std::int32_t target_index,
    std::int32_t aim) noexcept {
    if (target_index < 0 || target_index >= 3) {
        return false;
    }
    const auto ideal =
        kExpectedAimByTarget[static_cast<std::size_t>(target_index)];
    const auto tolerance =
        kExpectedAimToleranceByTarget[
            static_cast<std::size_t>(target_index)];
    const auto delta = aim - ideal;
    const auto magnitude = delta < 0 ? -delta : delta;
    return magnitude <= tolerance;
}

enum class GolfFeedbackKind {
    NonFinalTargetHit,
    FinalScoreGood,
    FinalScoreLow,
    MissAimOutsideTargetTolerance,
    MissNearExpectedTarget,
    MissFarBallRightOfExpectedTarget,
    MissFarBallLeftOrAtExpectedTarget,
};

struct GolfFeedbackResult {
    std::int32_t score_after{};
    std::int32_t target_points_added{};
    GolfFeedbackKind kind{
        GolfFeedbackKind::MissAimOutsideTargetTolerance};
    std::int32_t sound_id{-1};
    std::optional<std::int32_t> expected_target_index{};
    float expected_target_distance{};
    bool ball_right_of_expected_target{};
    bool clear_auxiliary_counter{true};
    RoundState next_state{RoundState::ResetNextAttempt};
};

// Exact state-5 score/voice policy. random_value is the non-negative rand()
// result consumed by the selected branch; each branch applies its own modulo.
[[nodiscard]] GolfFeedbackResult score_and_select_feedback(
    std::int32_t target_index,
    std::int32_t current_score,
    std::int32_t attempts_remaining,
    std::int32_t aim,
    Vec2f ball,
    const std::array<AnimatedCourseObject,3>& targets,
    std::uint32_t random_value) noexcept;

struct NextAttemptReset {
    Vec2f ball_position{};
    std::int32_t power{};
    std::int32_t attempts_remaining{};
    std::int32_t power_direction{1};
    RoundState state{RoundState::Aim};
};

// Exact state 6: restore integer initial ball position into floats, clear
// power, decrement attempts, force direction +1, and return to state 0.
[[nodiscard]] constexpr NextAttemptReset reset_next_attempt(
    Vec2i initial_ball_position,
    std::int32_t attempts_remaining) noexcept {
    return {
        {
            static_cast<float>(initial_ball_position.x),
            static_cast<float>(initial_ball_position.y),
        },
        0,
        attempts_remaining - 1,
        1,
        RoundState::Aim,
    };
}

} // namespace btb::golf
