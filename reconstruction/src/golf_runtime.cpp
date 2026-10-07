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
        static_cast<double>(ball.speed) * kBallSpeedDecayFraction);
    if (deceleration < kBallSpeedMinimumDecay) {
        deceleration = kBallSpeedMinimumDecay;
    }

    ball.speed -= deceleration;
    if (ball.speed < kBallStopThreshold) {
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
    const auto retail_ball = retail_ball_point(ball);
    return distance(
        Vec2f{
            static_cast<float>(retail_ball.x),
            static_cast<float>(retail_ball.y),
        },
        Vec2f{
            static_cast<float>(center.x),
            static_cast<float>(center.y),
        }) < kTargetRadius;
}

LandingResolution resolve_landing(
    Vec2f ball,
    const std::array<AnimatedCourseObject,3>& targets) noexcept {

    LandingResolution result;
    result.ball_position = ball;

    for (std::size_t i = 0; i < targets.size(); ++i) {
        if (!ball_reaches_target(result.ball_position, targets[i])) {
            continue;
        }

        const auto center = target_center(targets[i]);
        result.target_index = static_cast<std::int32_t>(i);
        result.ball_position = {
            static_cast<float>(center.x),
            static_cast<float>(center.y),
        };
        result.snapped_to_target = true;
    }

    return result;
}

GolfFeedbackResult score_and_select_feedback(
    std::int32_t target_index,
    std::int32_t current_score,
    std::int32_t attempts_remaining,
    std::int32_t aim,
    Vec2f ball,
    const std::array<AnimatedCourseObject,3>& targets,
    std::uint32_t random_value) noexcept {

    GolfFeedbackResult result;
    result.target_points_added = target_point_value(target_index);
    result.score_after = current_score + result.target_points_added;

    // Final attempt is graded from the updated cumulative score regardless of
    // whether this shot hit a target.
    if (attempts_remaining == 1) {
        if (result.score_after > 4) {
            result.kind = GolfFeedbackKind::FinalScoreGood;
            result.sound_id =
                133 + static_cast<std::int32_t>(random_value % 2U);
        } else {
            result.kind = GolfFeedbackKind::FinalScoreLow;
            result.sound_id = 135;
        }
        return result;
    }

    // Earlier target hits use one broad 12-line celebration pool.
    if (target_index >= 0 && target_index < 3) {
        result.kind = GolfFeedbackKind::NonFinalTargetHit;
        result.sound_id =
            102 + static_cast<std::int32_t>(random_value % 12U);
        return result;
    }

    const auto expected = expected_target_for_aim(aim);
    result.expected_target_index = expected;

    const auto expected_center = target_center(
        targets[static_cast<std::size_t>(expected)]);
    const auto retail_ball = retail_ball_point(ball);

    result.expected_target_distance = distance(
        Vec2f{
            static_cast<float>(retail_ball.x),
            static_cast<float>(retail_ball.y),
        },
        Vec2f{
            static_cast<float>(expected_center.x),
            static_cast<float>(expected_center.y),
        });

    result.ball_right_of_expected_target =
        ball.x > static_cast<float>(expected_center.x);

    if (!aim_within_expected_target_tolerance(expected, aim)) {
        result.kind =
            GolfFeedbackKind::MissAimOutsideTargetTolerance;
        result.sound_id =
            (random_value % 2U) == 0U ? 117 : 114;
        return result;
    }

    if (result.expected_target_distance < kLargeLandingRadius) {
        result.kind = GolfFeedbackKind::MissNearExpectedTarget;
        result.sound_id =
            119 + static_cast<std::int32_t>(random_value % 4U);
        return result;
    }

    if (result.ball_right_of_expected_target) {
        result.kind =
            GolfFeedbackKind::MissFarBallRightOfExpectedTarget;
        result.sound_id =
            131 + static_cast<std::int32_t>(random_value % 2U);
    } else {
        result.kind =
            GolfFeedbackKind::MissFarBallLeftOrAtExpectedTarget;
        result.sound_id =
            129 + static_cast<std::int32_t>(random_value % 2U);
    }

    return result;
}

} // namespace btb::golf
