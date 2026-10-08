#include "btb/golf_activity.hpp"

#include <algorithm>
#include <utility>

namespace btb::golf {

Round::Round(Data data, int difficulty)
    : data_(std::move(data)),
      difficulty_(std::clamp(difficulty, 0, 2)) {
    // Native InitializeGolfActivity uses the hard-coded 5, not the parsed
    // data.attempts_by_difficulty table {5,4,3}.
    attempts_remaining_ = kRetailInitialAttempts;
    const auto start = data_.initial_ball_position();
    ball_.position = {
        static_cast<float>(start.x), static_cast<float>(start.y)
    };
}

RoundFrame Round::snapshot() const noexcept {
    return {
        state_,ball_,aim_,power_,score_,attempts_remaining_,target_index_,
        std::nullopt,false,finished_,false
    };
}

RoundFrame Round::advance(const RoundInput& input) noexcept {
    auto out = snapshot();
    if (finished_) {
        return out;
    }

    switch (state_) {
    case RoundState::Aim:
        aim_ = clamp_aim(aim_ + input.aim_delta);
        if (input.accept_aim) {
            state_ = RoundState::PowerMeter;
        }
        break;

    case RoundState::PowerMeter:
        // Aim and power confirmation are separate UI actions. A confirmed
        // frame freezes the previously displayed power; all other frames
        // advance using the difficulty-dependent native oscillation.
        if (input.accept_power) {
            state_ = RoundState::LaunchSetup;
        } else {
            advance_power_meter(
                power_,direction_,
                data_.power_bar_speed_by_difficulty[
                    static_cast<std::size_t>(difficulty_)]);
        }
        break;

    case RoundState::LaunchSetup: {
        const auto shot = make_shot(aim_,power_);
        ball_.angle_degrees = shot.angle_degrees;
        ball_.speed = shot.speed;
        state_ = RoundState::SwingAnimationDelay;
        break;
    }

    case RoundState::SwingAnimationDelay:
        // Original code contains an 8x8 nested Bob-swing timing counter.
        // The host's original-sprite animator must supply the completion
        // event; running physics prematurely is less faithful than waiting.
        if (input.swing_animation_finished) {
            state_ = RoundState::BallFlight;
        }
        break;

    case RoundState::BallFlight:
        advance_ball(ball_);
        if (ball_.stopped()) {
            state_ = RoundState::ResolveLanding;
        }
        break;

    case RoundState::ResolveLanding: {
        const auto landed =
            resolve_landing(ball_.position,data_.course_objects);
        target_index_ = landed.target_index;
        ball_.position = landed.ball_position;
        state_ = RoundState::ScoreAndFeedback;
        feedback_has_started_ = false;
        break;
    }

    case RoundState::ScoreAndFeedback:
        if (!feedback_has_started_) {
            const auto result = score_and_select_feedback(
                target_index_,score_,attempts_remaining_,aim_,
                ball_.position,data_.course_objects,input.random_value);
            score_ = result.score_after;
            out.managed_voice = result.sound_id;
            out.feedback_started = true;
            feedback_has_started_ = true;
        } else if (input.feedback_audio_finished) {
            // Outer 0x00415830 waits for the result voice before allowing
            // the next attempt or Play Again path. The host must confirm.
            state_ = RoundState::ResetNextAttempt;
        }
        break;

    case RoundState::ResetNextAttempt: {
        const auto next = reset_next_attempt(
            data_.initial_ball_position(),attempts_remaining_);
        ball_.position = next.ball_position;
        ball_.speed = 0;
        power_ = next.power;
        attempts_remaining_ = next.attempts_remaining;
        direction_ = next.power_direction;
        target_index_ = -1;
        feedback_has_started_ = false;
        state_ = next.state;

        if (attempts_remaining_ <= 0) {
            finished_ = true;
            out.finished = true;
            out.progress_golf_completion = true;
        }
        break;
    }
    out.state = state_;
    out.ball = ball_;
    out.aim = aim_;
    out.power = power_;
    out.score = score_;
    out.attempts_remaining = attempts_remaining_;
    out.matched_target = target_index_;
    out.finished = finished_;
    return out;
}

} // namespace btb::golf
