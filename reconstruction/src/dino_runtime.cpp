#include "btb/dino_runtime.hpp"

#include <stdexcept>

namespace btb::dino {

Runtime::Runtime(const LevelData& level) {
    const auto count = level.piece_count();
    if (level.target_positions.size() != count ||
        level.start_positions.size() != count) {
        throw std::runtime_error("Dinosaur LevelData arrays disagree on piece count");
    }

    pieces_.reserve(count);
    for (std::size_t slot = 0; slot < count; ++slot) {
        const auto id = level.piece_permutation[slot];
        if (id < 0 || static_cast<std::size_t>(id) >= count) {
            throw std::runtime_error("Dinosaur piece ID outside target-position table");
        }

        PieceRuntime piece;
        piece.target = level.target_positions[static_cast<std::size_t>(id)];
        piece.current = level.start_positions[slot];
        piece.state = PieceState::Loose;
        piece.source_rect = {};
        piece.draw_loose_piece = true;
        piece.piece_id = id;
        pieces_.push_back(piece);
    }
}

void Runtime::set_piece_dimensions(
    std::size_t index,
    std::int32_t width,
    std::int32_t height) {
    auto& piece = pieces_.at(index);
    piece.source_rect = {0, 0, width, height};
}

std::optional<std::size_t> Runtime::begin_drag(Vec2i cursor) {
    if (mode_ != InteractionMode::Idle) {
        return std::nullopt;
    }

    for (std::size_t i = 0; i < pieces_.size(); ++i) {
        auto& piece = pieces_[i];
        if (piece.state != PieceState::Loose) {
            continue;
        }
        if (!cursor_inside_rect(
                cursor,
                {piece.current_bounds().left, piece.current_bounds().top},
                {piece.current_bounds().right, piece.current_bounds().bottom})) {
            continue;
        }

        selected_piece_ = i;
        mode_ = InteractionMode::Carrying;
        piece.state = PieceState::Dragging;
        piece.draw_loose_piece = false;
        return i;
    }

    return std::nullopt;
}

DropResult Runtime::try_drop(Vec2i cursor, std::int32_t tolerance) {
    if (mode_ != InteractionMode::Carrying || !selected_piece_) {
        return DropResult::NotCarrying;
    }

    auto& piece = pieces_.at(*selected_piece_);
    if (cursor_near_point(cursor, piece.target, tolerance)) {
        piece.state = PieceState::AcceptedDrop;
        mode_ = InteractionMode::FinalizeAcceptedDrop;
        return DropResult::Accepted;
    }

    piece.state = PieceState::RejectedDrop;
    // The retail game keeps carrying the piece after a bad drop.
    mode_ = InteractionMode::Carrying;
    return DropResult::Rejected;
}

bool Runtime::finalize_accepted_drop() {
    if (mode_ != InteractionMode::FinalizeAcceptedDrop || !selected_piece_) {
        return false;
    }

    auto& piece = pieces_.at(*selected_piece_);
    if (piece.state == PieceState::AcceptedDrop) {
        piece.state = PieceState::Placed;
        piece.draw_loose_piece = true;
        ++completed_piece_count_;
    } else {
        // Defensive path present in the original mode-2 code.
        piece.state = PieceState::Loose;
        piece.draw_loose_piece = true;
    }

    selected_piece_.reset();
    mode_ = InteractionMode::Idle;
    return true;
}

} // namespace btb::dino
