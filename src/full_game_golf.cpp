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
        spectator_frames_={}; spectator_countdowns_={5,7};
        flag_idle_frame_=flag_idle_ticks_=0;
        target_hit_frames_={}; target_hit_ticks_={};
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

ActivityFrameOutput GolfDriver::compose_frame(
    const golf::RoundFrame& state) {

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
            Rect{column*15,0,(column+1)*15,31},true});
    };
    if(state.score>=10)digit(state.score/10,510);
    digit(state.score%10,535);
    for(int i=0;i<state.attempts_remaining;++i)
        out.draws.push_back({
            "Data\\SubGameGolf\\toolbar ball.bmp",373+i*20,437,
            std::nullopt,true
        });
    // Exact retail draw sequence at 00414B24..0041501B.
    if(state.attempts_remaining>0 && state.state==golf::RoundState::Aim) {
        const auto v=golf::velocity_for(state.aim-2,140*80);
        out.draws.push_back({"Data\\SubGameGolf\\marker.bmp",
            static_cast<int>(static_cast<double>(state.ball.position.x)+v.x-22),
            static_cast<int>(static_cast<double>(state.ball.position.y)-v.y-22),
            std::nullopt,true});
    }
    if(state.attempts_remaining>0 && state.ball.position.y>45.0f) {
        out.draws.push_back({"Data\\SubGameGolf\\shadow.bmp",
            static_cast<int>(static_cast<double>(state.ball.position.x)+4),
            static_cast<int>(static_cast<double>(state.ball.position.y)+8),
            std::nullopt,true});
        // Retail uses the same stop BMP even during flight; the vertical
        // bounce changes only the ball Y, not the shadow or ground point.
        out.draws.push_back({"Data\\SubGameGolf\\golfballstop.bmp",
            static_cast<int>(state.ball.position.x),
            static_cast<int>(static_cast<double>(state.ball.position.y)-state.ball.height),
            std::nullopt,true});
    }
    const int w=course.bob_frame_size_retail.x;
    const int h=course.bob_frame_size_retail.y;
    const int col=std::clamp(swing_animation_ticks_/9,0,
        std::max(0,std::min(16,course.bob_frame_count)-1));
    const int row=std::clamp((88-state.aim)/2,0,
        std::max(0,std::min(45,course.bob_rotation_count)-1));
    out.draws.push_back({
        course.bob_sprite_path,course.bob_position.x,course.bob_position.y,
        Rect{col*w,row*h,(col+1)*w,(row+1)*h},true
    });
    // 00414D6D: only the first two parsed spectators are blitted.
    // Idle banks are 20 frames each, countdowns initially 5 and 7.
    // Hit-reaction RNG banks remain an explicit fidelity follow-up.
    for(std::size_t i=0;i<2;++i) {
        const auto& spectator=course.spectators[i];
        if(--spectator_countdowns_[i]<=0) {
            spectator_countdowns_[i]=5;
            spectator_frames_[i]=(spectator_frames_[i]+1)%
                std::max(1,std::min(20,spectator.frame_count));
        }
        const int left=spectator_frames_[i]*spectator.frame_size.x;
        out.draws.push_back({spectator.sprite_path,spectator.position.x,
            spectator.position.y,Rect{left+(i==0?1:0),0,
                left+spectator.frame_size.x,spectator.frame_size.y},true});
    }
    for(std::size_t i=0;i<course.course_objects.size();++i) {
        const auto& target=course.course_objects[i];
        const bool hit=state.matched_target==static_cast<int>(i);
        if(hit && i==0) {
            const int left=target_hit_frames_[i]*198;
            out.draws.push_back({"Data\\SubGameGolf\\flag01.bmp",
                target.position.x-35,target.position.y+2,
                Rect{left,0,left+198,165},true});
        } else {
            if(!hit && i==0 && ++flag_idle_ticks_>3) {
                flag_idle_ticks_=0;
                flag_idle_frame_=(flag_idle_frame_+1)%std::max(1,target.frame_count);
            }
            const int f=hit?target_hit_frames_[i]:(i==0?flag_idle_frame_:0);
            const int left=f*target.frame_size.x;
            out.draws.push_back({target.sprite_path,target.position.x,target.position.y,
                Rect{left,0,left+target.frame_size.x,target.frame_size.y},true});
        }
        if(hit && ++target_hit_ticks_[i]>3) {
            target_hit_ticks_[i]=0;
            target_hit_frames_[i]=(target_hit_frames_[i]+1)%
                (i==0?19:std::max(1,target.frame_count));
        }
    }

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
    // The two spectators' randomized hit-reaction banks still require
    // exact shared-RNG scheduling. All ordinary frames use retail assets.
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
