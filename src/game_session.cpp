#include "btb/game_session.hpp"

#include <cstdlib>
#include <exception>
#include <utility>

namespace btb::host {

game_flow::DispatcherStep Session::dispatcher() const noexcept {
    game_flow::DispatcherInput input;
    input.current_state = retail_state_;
    return game_flow::dispatch_step(input);
}

bool Session::load_dinosaur(
    const std::filesystem::path& dino_txt,
    dino::Species species,
    dino::Difficulty difficulty) {

    // Prepare the replacement in temporary state first. Failure must not
    // expose a half-loaded activity or leave the flow state in DinoRun.
    try {
        const auto species_id = static_cast<int>(species);
        const auto difficulty_id = static_cast<int>(difficulty);
        if (species_id < 0 || species_id > 2 ||
            difficulty_id < 0 || difficulty_id > 2) {
            error_ = "Dinosaur species/difficulty must be in the retail 0..2 range";
            return false;
        }

        const auto data = dino::parse_level_file(dino_txt.string());
        auto next = std::make_unique<dino::Runtime>(data);
        dinosaur_ = std::move(next);
        level_path_ = dino_txt;
        species_ = species;
        difficulty_ = difficulty;
        animations_ = {};
        dino_frame_ = {};
        pickup_offset_ = {};
        last_sound_id_ = dino::startup_presentation(species).intro_sound_id;
        error_.clear();
        screen_ = Screen::Dinosaur;
        retail_state_ = static_cast<std::int32_t>(game_flow::State::DinoRun);
        tick();
        return true;
    } catch (const std::exception& e) {
        error_ = e.what();
        return false;
    }
}

void Session::set_piece_dimensions(std::size_t slot, int width, int height) {
    if (dinosaur_ && width > 0 && height > 0) {
        dinosaur_->set_piece_dimensions(slot, width, height);
    }
}

void Session::pointer_down(dino::Vec2i cursor) {
    if (screen_ != Screen::Dinosaur || !dinosaur_ ||
        dinosaur_->mode() != dino::InteractionMode::Idle) {
        return;
    }

    const auto slot = dinosaur_->begin_drag(cursor);
    if (!slot) {
        return;
    }

    const auto& piece = dinosaur_->pieces()[*slot];
    pickup_offset_ = {
        piece.current.x - cursor.x,
        piece.current.y - cursor.y,
    };
    animations_.queue_pickup(std::rand() % 2, std::rand() % 2);
}

void Session::pointer_up(dino::Vec2i cursor) {
    if (screen_ != Screen::Dinosaur || !dinosaur_) {
        return;
    }

    if (dinosaur_->mode() != dino::InteractionMode::Carrying) {
        return;
    }

    const auto difficulty_index = dino::level_index(species_, difficulty_);
    const auto outcome = dinosaur_->try_drop(
        cursor, dino::Runtime::snap_tolerance(difficulty_index));
    if (outcome == dino::DropResult::Accepted) {
        const auto roll = std::rand() % 6;
        last_sound_id_ = dino::correct_drop_sound_id(roll);
        static_cast<void>(animations_.queue_correct_drop(roll));
    } else if (outcome == dino::DropResult::Rejected) {
        const auto roll = std::rand() % 6;
        last_sound_id_ = dino::wrong_drop_sound_id(roll);
        static_cast<void>(animations_.queue_wrong_drop(roll, std::rand() % 2));
    }
}

void Session::tick() {
    const auto dispatch = dispatcher();
    if (dispatch.intercept != game_flow::Intercept::StateTable ||
        !dispatch.dispatch) {
        return;
    }

    if (screen_ != Screen::Dinosaur && screen_ != Screen::DinosaurComplete) {
        return;
    }
    if (!dinosaur_ ||
        dispatch.dispatch->state != game_flow::State::DinoRun) {
        return;
    }

    if (dinosaur_->mode() == dino::InteractionMode::FinalizeAcceptedDrop) {
        static_cast<void>(dinosaur_->finalize_accepted_drop());
    }

    // The original game remains in DinoRun while completion audio and
    // certificate UI run. Keep that outer-state decision intact here.
    if (screen_ == Screen::Dinosaur && dinosaur_->complete()) {
        const auto gate = dino::completion_gate(true, false, 0);
        if (gate.begin_completion) {
            last_sound_id_ =
                dino::begin_completion(species_, 0).completion_sound_id;
            screen_ = Screen::DinosaurComplete;
        }
    }

    dino_frame_ = dino::compose_dino_frame(
        *dinosaur_, animations_, species_, {0, 0},
        dino::kInitialSpecialRenderOffsetIndex);
}

void Session::back_to_activity_select() noexcept {
    dinosaur_.reset();
    dino_frame_ = {};
    level_path_.clear();
    error_.clear();
    last_sound_id_ = -1;
    screen_ = Screen::ActivitySelect;
    retail_state_ =
        static_cast<std::int32_t>(game_flow::State::ActivitySelectUpdate);
}

} // namespace btb::host
