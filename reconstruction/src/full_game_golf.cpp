#include "btb/full_game_golf.hpp"

#include <exception>
#include <algorithm>
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
        swing_ticks_ = 0;
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

    // This is a strictly data-backed subset of native Golf composition.
    // Later render-layer recovery must place the ball sprite, aim indicator,
    // power bar, feedback overlays, HUD and source-frame animations in their
    // proven retail order. Do not fabricate missing sprite paths or frames.
    out.draws.push_back({
        "Data\\SubGameGolf\\golfbg.bmp", 0, 0, std::nullopt, false
    });
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
    // 00414C16: column DAT_0050ad90; row trunc(44 - aim / 2.0).
    // The x87 operands omitted by Ghidra are visible in the original PE32.
    const int bob_frame = std::min(swing_ticks_/9,8);
    const int bob_width = course.bob_frame_size_retail.x;
    const int bob_height = course.bob_frame_size_retail.y;
    const int bob_left = bob_frame*bob_width;
    const int bob_top = static_cast<int>(44.0-state.aim/2.0)*bob_height;
    out.draws.push_back({
        course.bob_sprite_path,
        course.bob_position.x,
        course.bob_position.y,
        Rect{bob_left,bob_top,bob_left+bob_width,bob_top+bob_height},
        true
    });

    return out;
}

ActivityFrameOutput GolfDriver::advance(
    const ActivityFrameInput& input) {

    ActivityFrameOutput out;
    if (!round_) {
        return out;
    }

    auto action = queued_input_.value_or(golf::RoundInput{});
    if (!queued_input_) {
        if (round_->state() == golf::RoundState::Aim) {
            // 00415080: absolute pointer angle, then the original shared
            // direction-mask adjustment (Right/Down before Left/Up).
            int aim = golf::clamp_aim(88-input.pointer_x/7);
            if (input.directional_input_bits&10) --aim;
            else if (input.directional_input_bits&5) ++aim;
            action.aim_delta = golf::clamp_aim(aim)-round_->aim();
        }
        if (round_->state() == golf::RoundState::SwingAnimationDelay)
            action.swing_animation_finished = ++swing_ticks_ >= 72;
        action.feedback_audio_finished = !input.managed_sound_playing;
    }
    if (round_->state() == golf::RoundState::LaunchSetup ||
        round_->state() == golf::RoundState::Aim)
        swing_ticks_ = 0;
    // The original BBC game manual confirms that either mouse button
    // (or Space Bar, translated by the host into the input pulse) enters
    // the power-meter phase on the first press and locks it on the second.
    if (!queued_input_ && input.click_pulse) {
        if (round_->state() == golf::RoundState::Aim) {
            action.accept_aim = true;
        } else if (round_->state() == golf::RoundState::PowerMeter) {
            action.accept_power = true;
        }
    }
    action.random_value =
        static_cast<std::uint32_t>(input.random_value);
    queued_input_.reset();

    const auto frame = round_->advance(action);
    out = compose_frame(frame);

    if (frame.managed_voice) {
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
        out.next_completion_return_state=static_cast<int>(game_flow::State::AdventureChooserSetup);
        out.next_outer_state = static_cast<int>(
            game_flow::State::PlayAgainYesNoSetup);
    }
    return out;
}

bool GolfDriver::unload(std::string& error) {
    round_.reset();
    queued_input_.reset();
    swing_ticks_ = 0;
    player_index_ = -1;
    error.clear();
    return true;
}

} // namespace btb::full_game
