#include "btb/golf_activity.hpp"

#include <algorithm>
#include <utility>

namespace btb::golf {

Round::Round(Data data, int difficulty,std::uint32_t initial_flight_counter)
    : data_(std::move(data)),
      difficulty_(std::clamp(difficulty, 0, 2)),
      flight_tick_(initial_flight_counter) {
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
        std::nullopt,false,finished_,false,bounce_height_
    };
}

RoundFrame Round::advance(const RoundInput& input) noexcept {
    auto out = snapshot();
    if (finished_) {
        // Outer 00415830 waits only after the final attempt is exhausted.
        if(input.feedback_audio_finished && !completion_emitted_) {
            completion_emitted_=true;
            out.progress_golf_completion=true;
        }
        return out;
    }

    switch (state_) {
    case RoundState::Aim:
        aim_ = clamp_aim(clamp_aim(input.source_mouse_aim.value_or(aim_)) +
                         input.aim_delta);
        if (input.accept_aim) {
            // Retail clears the last matched target only when aiming is accepted.
            target_index_=-1;
            state_ = RoundState::PowerMeter;
        }
        break;

    case RoundState::PowerMeter:
        // 00415080 increments the power meter before reading the
        // confirmation pulse, including the confirmation frame itself.
        advance_power_meter(power_,direction_,
            data_.power_bar_speed_by_difficulty[static_cast<std::size_t>(difficulty_)]);
        if(input.accept_power)state_=RoundState::LaunchSetup;
        break;

    case RoundState::LaunchSetup: {
        const auto shot = make_shot(aim_,power_);
        ball_.angle_degrees = shot.angle_degrees;
        ball_.speed = shot.speed;
        // 00415080 state 2: ground coordinates and airborne height
        // are independent; power selects an integer initial bounce.
        bounce_height_=0;
        bounce_direction_=1;
        bounce_amplitude_=((power_/2+500)*16)/1000;
        bounce_remaining_=bounce_amplitude_;
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
        // Physical display mode selects the original modulo gate; the
        // swing animation advances independently of the physics cadence.
        if(!advance_flight_cadence(flight_tick_,input.original_windowed_display).advance_physics)break;
        advance_ball(ball_);
        // 00415080 state 3 (0050AE50 / 0050AC04 / 0050ABCC):
        // rising steps shrink by one, falling steps use the current
        // amplitude capped at eight. Each ground contact reduces the
        // next amplitude by a third, with amplitudes below three disabled.
        if(bounce_height_>0 || bounce_remaining_>0) {
            const int step=std::min(8,bounce_remaining_);
            if(bounce_direction_==1) {
                bounce_height_+=step;
                if(--bounce_remaining_==0) {
                    bounce_direction_=-1;
                    bounce_remaining_=bounce_amplitude_;
                }
            } else {
                bounce_height_-=step;
                if(bounce_height_<=0) {
                    bounce_height_=0;
                    bounce_amplitude_-=bounce_amplitude_/3;
                    bounce_direction_=1;
                    bounce_remaining_=bounce_amplitude_>=3?bounce_amplitude_:0;
                }
            }
        }
        // LAB_004153FC is reached only at ground height zero. A ball
        // passing over a target while airborne does not enter its hole.
        // Keep the typed landing state even when speed and a hit coincide.
        if(ball_.stopped() || (bounce_height_==0 &&
            std::any_of(data_.course_objects.begin(),
            data_.course_objects.end(),[&](const auto& target){
                return ball_reaches_target(ball_.position,target);
            }))) {
            ball_.speed=0;
            state_=RoundState::ResolveLanding;
        }
        break;

    case RoundState::ResolveLanding: {
        const auto landed =
            resolve_landing(ball_.position,data_.course_objects);
        target_index_ = landed.target_index;
        ball_.position = landed.ball_position;
        state_ = RoundState::ScoreAndFeedback;
        break;
    }

    case RoundState::ScoreAndFeedback: {
        // 00415080 state 5 increments immediately after queuing speech.
        // Final low-score ID135 is the sole no-rand scoring branch.
        const int next_score=score_+target_point_value(target_index_);
        const bool calls_rand=attempts_remaining_!=1 || next_score>4;
        const auto random=calls_rand && input.random_stream
            ?static_cast<std::uint32_t>(input.random_stream->next_rand())
            :input.random_value;
        const auto result=score_and_select_feedback(
            target_index_,score_,attempts_remaining_,aim_,
            ball_.position,data_.course_objects,random);
        score_=result.score_after;
        out.managed_voice=result.sound_id;
        out.feedback_started=true;
        state_=RoundState::ResetNextAttempt;
        break;
    }

    case RoundState::ResetNextAttempt: {
        const auto next = reset_next_attempt(
            data_.initial_ball_position(),attempts_remaining_);
        ball_.position = next.ball_position;
        ball_.speed = 0;
        // 00415080 state 6 restores ground position and power only.
        // An airborne stop can leave a nonzero height until state 2 of
        // the next shot explicitly resets the bounce globals.
        power_ = next.power;
        attempts_remaining_ = next.attempts_remaining;
        direction_ = next.power_direction;
        state_ = next.state;

        if (attempts_remaining_ <= 0) {
            finished_ = true;
            out.finished = true;
        }
        break;
    }
    } // switch (state_): all original gameplay phases
    out.state = state_;
    out.ball = ball_;
    out.aim = aim_;
    out.power = power_;
    out.score = score_;
    out.attempts_remaining = attempts_remaining_;
    out.matched_target = target_index_;
    out.finished = finished_;
    out.bounce_height = bounce_height_;
    return out;
}

} // namespace btb::golf
