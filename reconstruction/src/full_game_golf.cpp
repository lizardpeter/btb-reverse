#include "btb/full_game_golf.hpp"

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
    for(int i=0;i<state.attempts_remaining;++i)
        out.draws.push_back({
            "Data\\SubGameGolf\\toolbar ball.bmp",373+i*20,437,
            std::nullopt,false
        });
    if(state.attempts_remaining>0) {
        const int ball_x=static_cast<int>(state.ball.position.x);
        const int ball_y=static_cast<int>(state.ball.position.y);
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
            ball_x,ball_y,std::nullopt,true
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
    // Bob's actual frame depends on retail aim/swing animation; this
    // source neutral request displays only the current first source frame,
    // while the native animation adapter is pending.
    out.draws.push_back({
        course.bob_sprite_path,
        course.bob_position.x,
        course.bob_position.y,
        Rect{0,0,
            course.bob_frame_size_retail.x,
            course.bob_frame_size_retail.y},
        true
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
    // Score digits, flight trail and the original swing/actor source-frame
    // indices remain separate from this recovered HUD/sprite subset.
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
            action.swing_animation_finished=++swing_animation_ticks_>=72;
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
    player_index_ = -1;
    error.clear();
    return true;
}

} // namespace btb::full_game
