#pragma once

#include "btb/dino_level.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::dino {

enum class PieceState : std::int32_t {
    Loose = 0,
    Dragging = 1,
    CorrectSnapTransition = 2,
    IncorrectReturnTransition = 3,
    Placed = 4,
};

enum class InteractionMode : std::int32_t {
    Idle = 0,
    Dragging = 1,
    ResolveDrop = 2,
};

struct PieceRuntime {
    Vec2i target{};
    Vec2i start{};
    Vec2i size{};
    std::int32_t piece_id{};
    PieceState state{PieceState::Loose};
};

enum class DropResult {
    None,
    Correct,
    Incorrect,
};

class GameModel {
public:
    GameModel(LevelData level, std::vector<Vec2i> piece_sizes, std::int32_t snap_tolerance);

    [[nodiscard]] const std::vector<PieceRuntime>& pieces() const noexcept { return pieces_; }
    [[nodiscard]] InteractionMode mode() const noexcept { return mode_; }
    [[nodiscard]] std::optional<std::size_t> selected_piece() const noexcept { return selected_piece_; }
    [[nodiscard]] std::size_t completed_piece_count() const noexcept { return completed_piece_count_; }
    [[nodiscard]] bool complete() const noexcept { return completed_piece_count_ == pieces_.size(); }

    // Matches the original idle scan: the first loose piece whose current/start rectangle
    // contains the cursor becomes selected.
    bool begin_drag(Vec2i cursor);

    // Matches the original release evaluation: only the selected piece's own target can
    // accept the drop.
    DropResult release(Vec2i cursor);

    // Matches interaction mode 2: converts correct transition -> placed, or incorrect
    // transition -> loose, then returns to idle.
    void resolve_drop();

private:
    std::vector<PieceRuntime> pieces_;
    InteractionMode mode_{InteractionMode::Idle};
    std::optional<std::size_t> selected_piece_;
    std::size_t completed_piece_count_{};
    std::int32_t snap_tolerance_{};
};

inline constexpr std::int32_t kCorrectDropSoundFirst = 164;
inline constexpr std::int32_t kCorrectDropSoundLast = 169;
inline constexpr std::int32_t kIncorrectDropSoundFirst = 170;
inline constexpr std::int32_t kIncorrectDropSoundLast = 175;
inline constexpr std::int32_t kDinoIntroSoundFirst = 185;
inline constexpr std::int32_t kDinoIntroSoundLast = 187;
inline constexpr std::int32_t kDinoCompletionSoundFirst = 188;
inline constexpr std::int32_t kDinoCompletionSoundLast = 190;

} // namespace btb::dino
