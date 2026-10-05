#include "btb/dino_game.hpp"

#include <stdexcept>
#include <utility>

namespace btb::dino {

GameModel::GameModel(
    LevelData level,
    std::vector<Vec2i> piece_sizes,
    std::int32_t snap_tolerance)
    : snap_tolerance_(snap_tolerance) {

    if (snap_tolerance < 0) {
        throw std::invalid_argument("snap tolerance must be non-negative");
    }
    if (piece_sizes.size() != level.piece_count()) {
        throw std::invalid_argument("piece size count must match Dinosaur piece count");
    }

    pieces_.reserve(level.piece_count());
    for (std::size_t slot = 0; slot < level.piece_count(); ++slot) {
        const auto piece_id = level.piece_permutation.at(slot);
        const auto id = static_cast<std::size_t>(piece_id);
        pieces_.push_back(PieceRuntime{
            .target = level.target_positions.at(id),
            .start = level.start_positions.at(slot),
            .size = piece_sizes.at(id),
            .piece_id = piece_id,
            .state = PieceState::Loose,
        });
    }
}

bool GameModel::begin_drag(Vec2i cursor) {
    if (mode_ != InteractionMode::Idle) {
        return false;
    }

    for (std::size_t i = 0; i < pieces_.size(); ++i) {
        auto& piece = pieces_[i];
        if (piece.state != PieceState::Loose) {
            continue;
        }

        const Vec2i bottom_right{
            piece.start.x + piece.size.x,
            piece.start.y + piece.size.y,
        };
        if (!cursor_inside_rect(cursor, piece.start, bottom_right)) {
            continue;
        }

        piece.state = PieceState::Dragging;
        selected_piece_ = i;
        mode_ = InteractionMode::Dragging;
        return true;
    }

    return false;
}

DropResult GameModel::release(Vec2i cursor) {
    if (mode_ != InteractionMode::Dragging || !selected_piece_) {
        return DropResult::None;
    }

    auto& piece = pieces_.at(*selected_piece_);
    mode_ = InteractionMode::ResolveDrop;

    if (cursor_near_point(cursor, piece.target, snap_tolerance_)) {
        piece.state = PieceState::CorrectSnapTransition;
        return DropResult::Correct;
    }

    piece.state = PieceState::IncorrectReturnTransition;
    return DropResult::Incorrect;
}

void GameModel::resolve_drop() {
    if (mode_ != InteractionMode::ResolveDrop || !selected_piece_) {
        return;
    }

    auto& piece = pieces_.at(*selected_piece_);
    if (piece.state == PieceState::CorrectSnapTransition) {
        piece.state = PieceState::Placed;
        ++completed_piece_count_;
    } else if (piece.state == PieceState::IncorrectReturnTransition) {
        piece.state = PieceState::Loose;
    } else {
        throw std::logic_error("unexpected Dinosaur piece state while resolving drop");
    }

    selected_piece_.reset();
    mode_ = InteractionMode::Idle;
}

} // namespace btb::dino
