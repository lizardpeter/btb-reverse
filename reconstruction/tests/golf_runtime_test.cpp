#include "btb/golf_runtime.hpp"

#include <cassert>
#include <cmath>

using namespace btb::golf;

int main() {
    static_assert(static_cast<int>(RoundState::Aim) == 0);
    static_assert(static_cast<int>(RoundState::PowerMeter) == 1);
    static_assert(static_cast<int>(RoundState::LaunchSetup) == 2);
    static_assert(static_cast<int>(RoundState::BallFlight) == 3);
    static_assert(static_cast<int>(RoundState::ResolveLanding) == 4);
    static_assert(static_cast<int>(RoundState::ScoreAndFeedback) == 5);
    static_assert(static_cast<int>(RoundState::ResetNextAttempt) == 6);
    static_assert(static_cast<int>(RoundState::SwingAnimationDelay) == 99);

    assert(clamp_aim(-1) == 0);
    assert(clamp_aim(100) == 88);
    assert(quantize_aim_angle(87) == 86);
    assert(quantize_aim_angle(88) == 88);

    int power = 998;
    int direction = 1;
    advance_power_meter(power, direction, 4);
    assert(power == 1000);
    assert(direction == -1);
    advance_power_meter(power, direction, 4);
    assert(power == 996);

    power = 1;
    direction = -1;
    advance_power_meter(power, direction, 2);
    assert(power == 0);
    assert(direction == 1);

    const auto weak = make_shot(45, 0);
    assert(weak.angle_degrees == 44);
    assert(weak.speed == 500);

    const auto strong = make_shot(88, 1000);
    assert(strong.angle_degrees == 88);
    assert(strong.speed == 833);

    auto velocity = velocity_for(0, 800);
    assert(std::fabs(velocity.x - 10.0f) < 0.001f);
    assert(std::fabs(velocity.y) < 0.001f);

    BallMotion ball{{100.0f, 100.0f}, 0, 800};
    advance_ball(ball);
    assert(std::fabs(ball.position.x - 110.0f) < 0.001f);
    assert(std::fabs(ball.position.y - 100.0f) < 0.001f);
    // floor(800 * .012) = 9, but retail enforces a minimum deceleration of 10.
    assert(ball.speed == 790);

    AnimatedCourseObject flag;
    flag.position = {330, 178};
    flag.hot_area_position = {99, 160};
    assert((target_center(flag) == Vec2i{429, 338}));
    assert(ball_reaches_target({429.0f, 338.0f}, flag));
    assert(!ball_reaches_target({444.0f, 338.0f}, flag));

    static_assert(kRetailInitialAttempts == 5);
}
