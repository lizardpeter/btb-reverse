#pragma once

#include "btb/dino_level.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::dino {

struct Recti {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

enum class PieceState : std::int32_t {
    Loose = 0,
    Dragging = 1,
    AcceptedDrop = 2,
    RejectedDrop = 3,
    Placed = 4,
};

enum class InteractionMode : std::int32_t {
    Idle = 0,
    Carrying = 1,
    FinalizeAcceptedDrop = 2,
};

enum class DropResult {
    NotCarrying,
    Rejected,
    Accepted,
};

struct PieceRuntime {
    Vec2i target{};
    Vec2i current{};
    PieceState state{PieceState::Loose};
    Recti source_rect{};
    std::int32_t render_mode{1};
    std::int32_t piece_id{-1};

    [[nodiscard]] Recti current_bounds() const noexcept {
        return {
            current.x,
            current.y,
            current.x + source_rect.right,
            current.y + source_rect.bottom,
        };
    }
};

inline constexpr std::array<Vec2i,8> kSpecialRenderOffsets{{
    {0,-40},
    {10,-10},
    {20,0},
    {10,10},
    {0,40},
    {-10,10},
    {-20,0},
    {-10,-10},
}};

inline constexpr std::int32_t kInitialSpecialRenderOffsetIndex = 4;

[[nodiscard]] constexpr std::optional<Vec2i> special_render_position(
    Vec2i anchor,
    std::int32_t table_index) noexcept {
    if (table_index < 0 ||
        table_index >= static_cast<std::int32_t>(kSpecialRenderOffsets.size())) {
        return std::nullopt;
    }
    const auto offset =
        kSpecialRenderOffsets[static_cast<std::size_t>(table_index)];
    return Vec2i{anchor.x + offset.x, anchor.y + offset.y};
}

class Runtime {
public:
    explicit Runtime(const LevelData& level);

    [[nodiscard]] const std::vector<PieceRuntime>& pieces() const noexcept {
        return pieces_;
    }
    [[nodiscard]] std::vector<PieceRuntime>& pieces() noexcept {
        return pieces_;
    }

    [[nodiscard]] InteractionMode mode() const noexcept { return mode_; }
    [[nodiscard]] std::optional<std::size_t> selected_piece() const noexcept {
        return selected_piece_;
    }
    [[nodiscard]] std::size_t completed_piece_count() const noexcept {
        return completed_piece_count_;
    }
    [[nodiscard]] bool complete() const noexcept {
        return completed_piece_count_ == pieces_.size();
    }

    void set_piece_dimensions(std::size_t index, std::int32_t width, std::int32_t height);

    // Equivalent to the binary's idle-mode piece scan.
    [[nodiscard]] std::optional<std::size_t> begin_drag(Vec2i cursor);

    // Equivalent to the carrying-mode target tolerance test.
    [[nodiscard]] DropResult try_drop(Vec2i cursor, std::int32_t tolerance);

    // Equivalent to the mode-2 accepted-drop finalization.
    [[nodiscard]] bool finalize_accepted_drop();

    // Matches the level-specific tolerance calculation observed at initialization.
    [[nodiscard]] static constexpr std::int32_t snap_tolerance(
        std::int32_t encoded_level_index) noexcept {
        return encoded_level_index == 2 ? 9 : 20;
    }

private:
    std::vector<PieceRuntime> pieces_;
    InteractionMode mode_{InteractionMode::Idle};
    std::optional<std::size_t> selected_piece_;
    std::size_t completed_piece_count_{};
};

} // namespace btb::dino
