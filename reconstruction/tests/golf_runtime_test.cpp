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

    static_assert(target_point_value(0) == 1);
    static_assert(target_point_value(1) == 3);
    static_assert(target_point_value(2) == 5);
    static_assert(target_point_value(-1) == 0);
    static_assert(target_point_value(3) == 0);

    static_assert(expected_target_for_aim(0) == 0);
    static_assert(expected_target_for_aim(23) == 0);
    static_assert(expected_target_for_aim(24) == 2);
    static_assert(expected_target_for_aim(49) == 2);
    static_assert(expected_target_for_aim(50) == 1);
    static_assert(expected_target_for_aim(88) == 1);

    static_assert(kExpectedAimByTarget[0] == 0);
    static_assert(kExpectedAimByTarget[1] == 60);
    static_assert(kExpectedAimByTarget[2] == 34);
    static_assert(kExpectedAimToleranceByTarget[0] == 10);
    static_assert(kExpectedAimToleranceByTarget[1] == 10);
    static_assert(kExpectedAimToleranceByTarget[2] == 6);

    static_assert(aim_within_expected_target_tolerance(0, 10));
    static_assert(!aim_within_expected_target_tolerance(0, 11));
    static_assert(aim_within_expected_target_tolerance(1, 50));
    static_assert(aim_within_expected_target_tolerance(1, 70));
    static_assert(!aim_within_expected_target_tolerance(1, 49));
    static_assert(aim_within_expected_target_tolerance(2, 28));
    static_assert(aim_within_expected_target_tolerance(2, 40));
    static_assert(!aim_within_expected_target_tolerance(2, 27));

    std::array<AnimatedCourseObject,3> targets{};
    targets[0].position = {330,178};
    targets[0].hot_area_position = {99,160}; // Flag -> 429,338
    targets[1].position = {230,15};
    targets[1].hot_area_position = {129,164}; // Windmill -> 359,179
    targets[2].position = {429,31};
    targets[2].hot_area_position = {103,129}; // Clown -> 532,160

    // State 4 clears target index, scans in data order, and snaps on strict
    // integer-distance <15.
    auto landing = resolve_landing({429.9f,338.9f}, targets);
    assert(landing.snapped_to_target);
    assert(landing.target_index == 0);
    assert(std::fabs(landing.ball_position.x - 429.0f) < 0.001f);
    assert(std::fabs(landing.ball_position.y - 338.0f) < 0.001f);
    assert(landing.reset_ball_animation_frame);

    landing = resolve_landing({444.0f,338.0f}, targets);
    assert(!landing.snapped_to_target);
    assert(landing.target_index == -1);

    // Non-final target hits score 1/3/5 by target index and use CG2 IDs
    // 102..113.
    auto feedback = score_and_select_feedback(
        0, 0, 5, 0, {429,338}, targets, 0);
    assert(feedback.target_points_added == 1);
    assert(feedback.score_after == 1);
    assert(feedback.kind == GolfFeedbackKind::NonFinalTargetHit);
    assert(feedback.sound_id == 102);

    feedback = score_and_select_feedback(
        1, 1, 4, 60, {359,179}, targets, 11);
    assert(feedback.target_points_added == 3);
    assert(feedback.score_after == 4);
    assert(feedback.sound_id == 113);

    feedback = score_and_select_feedback(
        2, 4, 2, 34, {532,160}, targets, 5);
    assert(feedback.target_points_added == 5);
    assert(feedback.score_after == 9);
    assert(feedback.sound_id == 107);

    // Final attempt includes the current target's points before grading.
    feedback = score_and_select_feedback(
        2, 0, 1, 34, {532,160}, targets, 0);
    assert(feedback.score_after == 5);
    assert(feedback.kind == GolfFeedbackKind::FinalScoreGood);
    assert(feedback.sound_id == 133);

    feedback = score_and_select_feedback(
        -1, 4, 1, 34, {500,160}, targets, 99);
    assert(feedback.score_after == 4);
    assert(feedback.kind == GolfFeedbackKind::FinalScoreLow);
    assert(feedback.sound_id == 135);

    feedback = score_and_select_feedback(
        0, 4, 1, 0, {429,338}, targets, 1);
    assert(feedback.score_after == 5);
    assert(feedback.sound_id == 134);

    // A miss whose aim is outside the selected target's tolerance uses only
    // CG2_BOB_16 (117) or CG2_BOB_02 (114).
    feedback = score_and_select_feedback(
        -1, 0, 5, 11, {300,300}, targets, 0);
    assert(
        feedback.kind ==
        GolfFeedbackKind::MissAimOutsideTargetTolerance);
    assert(feedback.expected_target_index == 0);
    assert(feedback.sound_id == 117);

    feedback = score_and_select_feedback(
        -1, 0, 5, 11, {300,300}, targets, 1);
    assert(feedback.sound_id == 114);

    // Aim 34 selects Clown. Within 50 integer pixels uses CG2_BOB_06..09.
    feedback = score_and_select_feedback(
        -1, 0, 5, 34, {500,160}, targets, 3);
    assert(feedback.expected_target_index == 2);
    assert(feedback.expected_target_distance < 50.0f);
    assert(feedback.kind == GolfFeedbackKind::MissNearExpectedTarget);
    assert(feedback.sound_id == 122);

    // Far misses on the right side of the expected target use WEN_07/08
    // (131/132); left-or-equal uses WEN_05/06 (129/130).
    feedback = score_and_select_feedback(
        -1, 0, 5, 0, {600,338}, targets, 1);
    assert(feedback.expected_target_index == 0);
    assert(feedback.expected_target_distance >= 50.0f);
    assert(feedback.ball_right_of_expected_target);
    assert(
        feedback.kind ==
        GolfFeedbackKind::MissFarBallRightOfExpectedTarget);
    assert(feedback.sound_id == 132);

    feedback = score_and_select_feedback(
        -1, 0, 5, 0, {300,338}, targets, 1);
    assert(!feedback.ball_right_of_expected_target);
    assert(
        feedback.kind ==
        GolfFeedbackKind::MissFarBallLeftOrAtExpectedTarget);
    assert(feedback.sound_id == 130);

    // State 6 is a pure reset/decrement and always forces direction +1.
    constexpr auto reset = reset_next_attempt({187,330}, 5);
    static_assert(reset.ball_position.x == 187.0f);
    static_assert(reset.ball_position.y == 330.0f);
    static_assert(reset.power == 0);
    static_assert(reset.attempts_remaining == 4);
    static_assert(reset.power_direction == 1);
    static_assert(reset.state == RoundState::Aim);
}
