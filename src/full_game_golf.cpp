#include "btb/full_game_golf.hpp"

#include <algorithm>
#include <cmath>
#include <exception>
#include <fstream>
#include <utility>

namespace btb::full_game {

bool GolfDriver::initialize(int player_index, std::string& error) {
    if (round_) {
        error = "Golf is already initialized; unload the current activity";
        return false;
    }
    if (player_index < 0 ||
        player_index >= static_cast<int>(progress::kPlayerCount)) {
        error = "Golf player index outside five original profiles";
        return false;
    }
    if (difficulty_ < 0 || difficulty_ > 2) {
        error = "Golf difficulty must be a retail value from 0 to 2";
        return false;
    }

    std::ifstream source(golf_data_dir_ / "golfdata.txt");
    if (!source) {
        error = "cannot open original Data/SubGameGolf/golfdata.txt";
        return false;
    }
    try {
        auto data = golf::parse_data(source);
        auto candidate = std::make_unique<golf::Round>(
            std::move(data),difficulty_);
        round_ = std::move(candidate);
        player_index_ = player_index;
        queued_input_.reset();
        swing_animation_ticks_=0;
        feedback_voice_started_=false;
        intro_stage_=0;
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

ActivityFrameOutput GolfDriver::compose_frame(
    const golf::RoundFrame& state) const {

    ActivityFrameOutput out;
    if (!round_) {
        return out;
    }
    const auto& course = round_->data();

    // Original 0x00414A60 starts with the backdrop, a 20px-spaced row
    // of original toolbar golf balls for remaining attempts, and the
    // live source ball sprites. These are the exact original bitmap names
    // and the authored x=0x175/y=0x1B5 attempt HUD coordinates.
    out.draws.push_back({
        "Data\\SubGameGolf\\golfbg.bmp",0,0,std::nullopt,false
    });
    // The shipped 150x32 score sheet is ordered 1..9,0, not 0..9.
    const auto digit=[&](int value,int x) {
        const int column=(value+9)%10;
        out.draws.push_back({"Data\\SubGameGolf\\golfnumbers.bmp",x,426,
            Rect{column*15,0,(column+1)*15,32},true});
    };
    if(state.score>=10)digit(state.score/10,510);
    digit(state.score%10,535);
    for(int i=0;i<state.attempts_remaining;++i)
        out.draws.push_back({
            "Data\\SubGameGolf\\toolbar ball.bmp",373+i*20,437,
            std::nullopt,true
        });
    if(state.attempts_remaining>0) {
        const int ball_x=static_cast<int>(state.ball.position.x);
        const int ball_y=static_cast<int>(state.ball.position.y);
        if(state.state==golf::RoundState::Aim) {
            // 00414A60 projects a 140-pixel aiming marker at aim minus 2.
            constexpr double rad=3.14159265358979323846/180.0;
            const double angle=(state.aim-2)*rad;
            out.draws.push_back({"Data\\SubGameGolf\\marker.bmp",
                static_cast<int>(state.ball.position.x+140*std::cos(angle)),
                static_cast<int>(state.ball.position.y-140*std::sin(angle)),
                std::nullopt,true});
        }
        // Native DrawGolfActivity checks the current float ball location
        // before submitting the magenta-keyed shadow and stopped-ball
        // sprites. The original asset resolver keeps both physical BMPs.
        out.draws.push_back({
            "Data\\SubGameGolf\\shadow.bmp",ball_x,ball_y,
            std::nullopt,true
        });
        out.draws.push_back({
            state.ball.speed>0
                ? "Data\\SubGameGolf\\golfballgo.bmp"
                : "Data\\SubGameGolf\\golfballstop.bmp",
            ball_x,ball_y-state.bounce_height,std::nullopt,true
        });
    }
    for (const auto& target : course.course_objects) {
        out.draws.push_back({
            target.sprite_path,
            target.position.x,target.position.y,
            target.frame_size.x > 0 && target.frame_size.y > 0
                ? std::optional<Rect>{
                    Rect{0,0,target.frame_size.x,target.frame_size.y}
                  }
                : std::nullopt,
            true
        });
    }
    for (const auto& spectator : course.spectators) {
        out.draws.push_back({
            spectator.sprite_path,
            spectator.position.x,spectator.position.y,
            spectator.frame_size.x > 0 && spectator.frame_size.y > 0
                ? std::optional<Rect>{
                    Rect{0,0,spectator.frame_size.x,spectator.frame_size.y}
                  }
                : std::nullopt,
            true
        });
    }
    // 00414A60/00415080: 45 aim rows, 16 swing columns; the actual
    // shipped 2128x7830 sheet uses 133x174 cells.
    const int w=course.bob_frame_size_retail.x;
    const int h=course.bob_frame_size_retail.y;
    const int col=std::clamp(swing_animation_ticks_/9,0,
        std::max(0,std::min(16,course.bob_frame_count)-1));
    const int row=std::clamp(state.aim/2,0,
        std::max(0,std::min(45,course.bob_rotation_count)-1));
    out.draws.push_back({
        course.bob_sprite_path,course.bob_position.x,course.bob_position.y,
        Rect{col*w,row*h,(col+1)*w,(row+1)*h},true
    });

    // 0x00414A60 submits the real powerbar at (0x53,0x1A0).
    // The source-width is trunc(power * 0x101 / 1000), height 0x35.
    // A zero-width blit has no drawable pixels and is omitted.
    if(state.state!=golf::RoundState::Aim && state.power>0) {
        const int width=(state.power*257)/1000;
        if(width>0)
            out.draws.push_back({
                "Data\\SubGameGolf\\powerbar.bmp",83,416,
                Rect{0,0,width,53},true
            });
    }
    // Spectator reactions and target-specific animation banks remain
    // separate from this recovered presentation.
    return out;
}

ActivityFrameOutput GolfDriver::advance(
    const ActivityFrameInput& input) {

    ActivityFrameOutput out;
    if (!round_) {
        return out;
    }

    // Input translation is a separate retail-DirectInput/UI responsibility.
    // No guessed click-zone/aim-angle mapping is embedded in this driver.
    auto action = queued_input_.value_or(golf::RoundInput{});
    // The original BBC game manual confirms that either mouse button
    // (or Space Bar, translated by the host into the input pulse) enters
    // the power-meter phase on the first press and locks it on the second.
    // We still require explicit native input translation for left/right
    // aim movement and for animation/audio completion.
    if (!queued_input_ && input.click_pulse) {
        if (round_->state() == golf::RoundState::Aim) {
            action.accept_aim = true;
        } else if (round_->state() == golf::RoundState::PowerMeter) {
            action.accept_power = true;
        }
    }
    if (!queued_input_) {
        switch(round_->state()) {
        case golf::RoundState::Aim:
            // Original 0x00415080: aim = clamp(88 - pointer_x / 7).
            // Right/down direction bits subtract one, left/up add one.
            action.source_mouse_aim=88-input.pointer_x/7;
            if(input.directional_input_bits & 0x0aU)action.aim_delta=-1;
            else if(input.directional_input_bits & 0x05U)action.aim_delta=1;
            break;
        case golf::RoundState::SwingAnimationDelay:
            // 0x0050ADE8 increments until nine, then 0x0050AD90 until
            // eight. Native completion is triggered on the 72nd tick.
            action.swing_animation_finished=swing_animation_ticks_+1>=72;
            break;
        case golf::RoundState::ScoreAndFeedback:
            // First scoring frame queues original managed voice;
            // subsequent frames read the REAL DirectSound busy observation.
            action.feedback_audio_finished=
                feedback_voice_started_ && !input.managed_sound_playing;
            break;
        default:break;
        }
    }
    const auto prior=round_->state();
    action.random_value =
        static_cast<std::uint32_t>(input.random_value);
    queued_input_.reset();

    const auto frame = round_->advance(action);
    if(prior==golf::RoundState::LaunchSetup &&
       frame.state==golf::RoundState::SwingAnimationDelay)
        swing_animation_ticks_=0;
    else if(prior==golf::RoundState::SwingAnimationDelay) {
        ++swing_animation_ticks_;
        if(frame.state==golf::RoundState::BallFlight)
            swing_animation_ticks_=std::max(72,swing_animation_ticks_);
    } else if(prior==golf::RoundState::BallFlight)
        swing_animation_ticks_=std::min(135,swing_animation_ticks_+1);
    else if(prior==golf::RoundState::ResolveLanding)
        swing_animation_ticks_=0;
    if(prior==golf::RoundState::ScoreAndFeedback &&
       frame.state!=golf::RoundState::ScoreAndFeedback)
        feedback_voice_started_=false;
    out = compose_frame(frame);

    if (frame.managed_voice) {
        feedback_voice_started_=true;
        out.audio.push_back({
            AudioOperation::ManagedSoundId, {},
            *frame.managed_voice, 50, 1
        });
    }

    // 00415830 plays the activity music and initial 123 voice, then
    // the interruptible 101 line after the managed sound bank drains.
    if(intro_stage_==0) {
        out.audio.push_back({AudioOperation::StartBackingTrack,"Data/music/golf.wav"});
        out.audio.push_back({AudioOperation::ManagedSoundId,{},123,50,1,true});
        intro_stage_=1;
    } else if(intro_stage_==1 && !input.managed_sound_playing && !frame.managed_voice) {
        out.audio.push_back({AudioOperation::ManagedSoundId,{},101,50,1,true});
        intro_stage_=2;
    }
    if (frame.progress_golf_completion) {
        out.progress_writes.push_back({progress::Slot::Golf,1});
        out.save_and_unload = true;
        out.next_saved_state =
            static_cast<int>(game_flow::State::GolfInit);
        out.next_replay_class = 1;
        out.next_outer_state = static_cast<int>(
            game_flow::State::PlayAgainYesNoSetup);
    }
    return out;
}

bool GolfDriver::unload(std::string& error) {
    round_.reset();
    queued_input_.reset();
    swing_animation_ticks_=0;
    feedback_voice_started_=false;
    intro_stage_=0;
    player_index_ = -1;
    error.clear();
    return true;
}

} // namespace btb::full_game
