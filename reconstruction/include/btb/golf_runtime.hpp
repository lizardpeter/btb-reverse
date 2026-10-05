#pragma once

#include "btb/golf_data.hpp"

#include <cstddef>
#include <cstdint>

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

} // namespace btb::golf
