#include "btb/golf_runtime.hpp"

#include <algorithm>
#include <cmath>

namespace btb::golf {

std::int32_t clamp_aim(std::int32_t aim) noexcept {
    return std::clamp(aim, kAimMinimum, kAimMaximum);
}

std::int32_t quantize_aim_angle(std::int32_t aim) noexcept {
    aim = clamp_aim(aim);
    // Retail: float(aim) / 2.0f -> MSVC float-to-int conversion -> * 2.
    // All values are non-negative, so this is equivalent to truncating to
    // the nearest lower even integer.
    return (aim / 2) * 2;
}

void advance_power_meter(
    std::int32_t& power,
    std::int32_t& direction,
    std::int32_t difficulty_speed) noexcept {

    power += difficulty_speed * direction;

    if (power > kPowerMaximum) {
        power = kPowerMaximum;
        direction = -direction;
    } else if (power < kPowerMinimum) {
        power = kPowerMinimum;
        direction = -direction;
    }
}

ShotParameters make_shot(
    std::int32_t aim,
    std::int32_t power) noexcept {

    power = std::clamp(power, kPowerMinimum, kPowerMaximum);
    return {
        .angle_degrees = quantize_aim_angle(aim),
        .power = power,
        .speed = power / 3 + 500,
    };
}

Vec2f velocity_for(
    std::int32_t angle_degrees,
    std::int32_t speed) noexcept {

    constexpr double pi = 3.14159265358979323846;
    const double radians = static_cast<double>(angle_degrees) * (pi / 180.0);
    const auto magnitude = static_cast<float>(speed / 80);

    return {
        static_cast<float>(std::cos(radians)) * magnitude,
        static_cast<float>(std::sin(radians)) * magnitude,
    };
}

void advance_ball(BallMotion& ball) noexcept {
    if (ball.speed <= 0) {
        ball.speed = 0;
        return;
    }

    const auto velocity = velocity_for(ball.angle_degrees, ball.speed);
    ball.position.x += velocity.x;
    ball.position.y -= velocity.y;

    auto deceleration = static_cast<std::int32_t>(
        static_cast<double>(ball.speed) * 0.012);
    if (deceleration < 10) {
        deceleration = 10;
    }

    ball.speed -= deceleration;
    if (ball.speed < 20) {
        ball.speed = 0;
    }
}

float distance(Vec2f a, Vec2f b) noexcept {
    const auto dx = a.x - b.x;
    const auto dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

Vec2i target_center(const AnimatedCourseObject& object) noexcept {
    return {
        object.position.x + object.hot_area_position.x,
        object.position.y + object.hot_area_position.y,
    };
}

bool ball_reaches_target(
    Vec2f ball,
    const AnimatedCourseObject& object) noexcept {

    const auto center = target_center(object);
    return distance(
        ball,
        Vec2f{
            static_cast<float>(center.x),
            static_cast<float>(center.y),
        }) < kTargetRadius;
}

} // namespace btb::golf
