#pragma once

#include "btb/maze_data.hpp"
#include "btb/maze_runtime.hpp"

#include <cstdint>

namespace btb::maze {

enum class InputPath {
    Mouse,
    Keyboard,
};

struct InputModeState {
    bool mouse_mode{true};
    std::int32_t remembered_mouse_x{};
    std::int32_t remembered_mouse_y{};
};

struct PlayerInputSample {
    float player_x{};
    float player_y{};
    std::int32_t mouse_x{};
    std::int32_t mouse_y{};
    std::uint8_t direction_bits{};
};

struct PlayerInputStep {
    DirectionalIntent intent{};
    InputPath path{InputPath::Mouse};

    // The mode switch is written during this call but takes effect only on the
    // next call because retail branches on the entry value of 0x00512124.
    bool mode_changed_for_next_call{};

    bool mouse_dead_zone{};
    bool debounce_applied{};

    // Retail writes shared input phase 0x00443D64 = 4 on every keyboard call
    // and on mouse calls that get as far as directional/debounce processing.
    // The <=6-pixel mouse dead-zone exits before this write.
    bool reset_shared_input_phase{};
};

// Exact semantic reconstruction of 0x00415D70.
//
// This is deliberately not atan2. Retail computes atan(abs(dy)/abs(dx)), but
// substitutes ratio 9999.0 when abs(dx)==0, multiplies by the stored float
// 57.2949981689453125, applies manual quadrant correction, then truncates
// toward zero. Consequently a perfectly vertical upward vector is 359 degrees.
[[nodiscard]] std::int32_t angle_between_integer_points_degrees(
    Vec2i from,
    Vec2i to) noexcept;

[[nodiscard]] DirectionalIntent keyboard_direction_from_bits(
    std::uint8_t direction_bits) noexcept;

// Exact 0x0041AF60 path selection, dead-zone, component thresholding and
// debounce behavior. The mode and debounce objects are persistent retail state.
[[nodiscard]] PlayerInputStep compute_player_input_direction(
    InputModeState& mode,
    DirectionalDebounce& debounce,
    const PlayerInputSample& sample) noexcept;

} // namespace btb::maze
