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

    (void)state;
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
        out.next_outer_state = static_cast<int>(
            game_flow::State::PlayAgainYesNoSetup);
    }
    return out;
}

bool GolfDriver::unload(std::string& error) {
    round_.reset();
    queued_input_.reset();
    player_index_ = -1;
    error.clear();
    return true;
}

} // namespace btb::full_game
