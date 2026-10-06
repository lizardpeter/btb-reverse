#pragma once

#include <cstdint>

namespace btb::input {

inline constexpr std::uint32_t kDirectInputVersion = 0x0800;
inline constexpr std::uint32_t kPointerDeviceCooperativeFlags = 0x05;
inline constexpr std::uint32_t kKeyboardCooperativeFlags = 0x16;

inline constexpr std::uint8_t kDikLeft = 0xCB;
inline constexpr std::uint8_t kDikRight = 0xCD;
inline constexpr std::uint8_t kDikUp = 0xC8;
inline constexpr std::uint8_t kDikDown = 0xD0;
inline constexpr std::uint8_t kDikSpace = 0x39;

inline constexpr std::uint32_t kDirectionLeft = 0x01;
inline constexpr std::uint32_t kDirectionRight = 0x02;
inline constexpr std::uint32_t kDirectionUp = 0x04;
inline constexpr std::uint32_t kDirectionDown = 0x08;
inline constexpr std::uint32_t kDirectionAction = 0x10;

struct AcquireStateStep {
    bool return_one_when_primary_device_missing{};
    bool acquire_primary{};
    bool acquire_secondary{};
    bool unacquire_primary{};
    bool unacquire_secondary{};
    std::int32_t return_value{};
};

// Exact 0x00407E10. If the first device pointer is null retail returns 1.
// Otherwise the global active-frame flag decides whether both devices are
// acquired or unacquired, and the function returns 0.
[[nodiscard]] constexpr AcquireStateStep acquire_state_step(
    bool primary_device_exists,
    bool frame_active) noexcept {

    if (!primary_device_exists) {
        return {true,false,false,false,false,1};
    }

    if (frame_active) {
        return {false,true,true,false,false,0};
    }

    return {false,false,false,true,true,0};
}

struct MouseBounds {
    std::int32_t min_x{};
    std::int32_t min_y{};
    std::int32_t max_x{};
    std::int32_t max_y{};
};

inline constexpr MouseBounds kRetailMouseBounds{2,2,637,477};

struct CursorState {
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t hotspot_x{};
    std::int32_t hotspot_y{};
};

constexpr void set_hotspot(
    CursorState& state,
    std::int32_t x,
    std::int32_t y) noexcept {
    state.hotspot_x = x;
    state.hotspot_y = y;
}

// 0x00407A90: zero hotspot and clamp only negative cursor coordinates to zero.
constexpr void clear_hotspot_and_clamp_nonnegative(
    CursorState& state) noexcept {
    state.hotspot_x = 0;
    state.hotspot_y = 0;
    if (state.x < 0) {
        state.x = 0;
    }
    if (state.y < 0) {
        state.y = 0;
    }
}

// Retail bounds test is applied to cursor+hotspot. The stored cursor position
// is adjusted to boundary-hotspot when it exceeds either side.
constexpr void clamp_cursor_to_bounds(
    CursorState& state,
    const MouseBounds& bounds = kRetailMouseBounds) noexcept {

    if (state.x + state.hotspot_x > bounds.max_x) {
        state.x = bounds.max_x - state.hotspot_x;
    }
    if (state.y + state.hotspot_y > bounds.max_y) {
        state.y = bounds.max_y - state.hotspot_y;
    }
    if (state.x + state.hotspot_x < bounds.min_x) {
        state.x = bounds.min_x - state.hotspot_x;
    }
    if (state.y + state.hotspot_y < bounds.min_y) {
        state.y = bounds.min_y - state.hotspot_y;
    }
}

struct KeyboardDirections {
    std::uint32_t bits{};

    [[nodiscard]] constexpr bool left() const noexcept {
        return (bits & kDirectionLeft) != 0;
    }
    [[nodiscard]] constexpr bool right() const noexcept {
        return (bits & kDirectionRight) != 0;
    }
    [[nodiscard]] constexpr bool up() const noexcept {
        return (bits & kDirectionUp) != 0;
    }
    [[nodiscard]] constexpr bool down() const noexcept {
        return (bits & kDirectionDown) != 0;
    }
    [[nodiscard]] constexpr bool action() const noexcept {
        return (bits & kDirectionAction) != 0;
    }
};

[[nodiscard]] constexpr KeyboardDirections keyboard_directions(
    bool left,
    bool right,
    bool up,
    bool down,
    bool space) noexcept {

    std::uint32_t bits = 0;
    if (left)  bits |= kDirectionLeft;
    if (right) bits |= kDirectionRight;
    if (up)    bits |= kDirectionUp;
    if (down)  bits |= kDirectionDown;
    if (space) bits |= kDirectionAction;
    return {bits};
}

[[nodiscard]] constexpr std::int32_t keyboard_cursor_step_size(
    bool fast_cursor_mode) noexcept {
    return fast_cursor_mode ? 4 : 1;
}

constexpr void apply_keyboard_cursor_movement(
    CursorState& state,
    KeyboardDirections directions,
    bool fast_cursor_mode,
    bool movement_suppressed) noexcept {

    if (movement_suppressed) {
        return;
    }

    const auto step = keyboard_cursor_step_size(fast_cursor_mode);

    if (directions.left()) {
        state.x -= step;
    }
    if (directions.right()) {
        state.x += step;
    }
    if (directions.down()) {
        state.y += step;
    }
    if (directions.up()) {
        state.y -= step;
    }
}

constexpr void apply_relative_pointer_delta(
    CursorState& state,
    std::int32_t delta_x,
    std::int32_t delta_y,
    bool apply_vertical_delta = true) noexcept {

    state.x += delta_x;
    if (apply_vertical_delta) {
        state.y += delta_y;
    }
}

struct CursorMovementResult {
    CursorState state{};
    bool moved{};
};

// Shared end-of-poll position path: keyboard movement, relative pointer deltas,
// then retail hotspot-aware bounds clamping. The caller supplies the vertical
// mouse-delta gate because the shipped routine derives it from its timing/
// pointer-device branch before this common clamp.
[[nodiscard]] constexpr CursorMovementResult update_cursor_position(
    CursorState state,
    KeyboardDirections directions,
    bool fast_cursor_mode,
    bool movement_suppressed,
    std::int32_t delta_x,
    std::int32_t delta_y,
    bool apply_vertical_delta,
    const MouseBounds& bounds = kRetailMouseBounds) noexcept {

    const auto old_x = state.x;
    const auto old_y = state.y;

    apply_keyboard_cursor_movement(
        state, directions, fast_cursor_mode, movement_suppressed);
    apply_relative_pointer_delta(
        state, delta_x, delta_y, apply_vertical_delta);
    clamp_cursor_to_bounds(state, bounds);

    return {
        state,
        state.x != old_x || state.y != old_y,
    };
}

struct InputInitPlan {
    std::uint32_t direct_input_version{kDirectInputVersion};
    bool create_primary_device{true};
    std::uint32_t primary_cooperative_flags{
        kPointerDeviceCooperativeFlags};
    bool create_keyboard_device{true};
    std::uint32_t keyboard_cooperative_flags{
        kKeyboardCooperativeFlags};
    bool apply_acquire_state_after_setup{true};
    bool enable_input_processing_gate{true};
    CursorState initial_cursor{320,0,0,0};
    MouseBounds initial_bounds{kRetailMouseBounds};
};

inline constexpr InputInitPlan kInputInitPlan{};

} // namespace btb::input
