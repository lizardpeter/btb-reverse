#include "btb/input_runtime.hpp"

#include <cassert>

using namespace btb::input;

int main() {
    static_assert(kDirectInputVersion == 0x0800);
    static_assert(kPointerDeviceCooperativeFlags == 0x05);
    static_assert(kKeyboardCooperativeFlags == 0x16);

    static_assert(kDikLeft == 0xCB);
    static_assert(kDikRight == 0xCD);
    static_assert(kDikUp == 0xC8);
    static_assert(kDikDown == 0xD0);
    static_assert(kDikSpace == 0x39);

    constexpr auto missing =
        acquire_state_step(false, true);
    static_assert(missing.return_one_when_primary_device_missing);
    static_assert(missing.return_value == 1);
    static_assert(!missing.acquire_primary);

    constexpr auto active =
        acquire_state_step(true, true);
    static_assert(active.acquire_primary);
    static_assert(active.acquire_secondary);
    static_assert(!active.unacquire_primary);
    static_assert(active.return_value == 0);

    constexpr auto inactive =
        acquire_state_step(true, false);
    static_assert(inactive.unacquire_primary);
    static_assert(inactive.unacquire_secondary);
    static_assert(!inactive.acquire_primary);

    static_assert(kRetailMouseBounds.min_x == 2);
    static_assert(kRetailMouseBounds.min_y == 2);
    static_assert(kRetailMouseBounds.max_x == 637);
    static_assert(kRetailMouseBounds.max_y == 477);

    CursorState cursor{-10,-20,4,5};
    clear_hotspot_and_clamp_nonnegative(cursor);
    assert(cursor.x == 0);
    assert(cursor.y == 0);
    assert(cursor.hotspot_x == 0);
    assert(cursor.hotspot_y == 0);

    set_hotspot(cursor, 10, 20);
    assert(cursor.hotspot_x == 10);
    assert(cursor.hotspot_y == 20);

    cursor.x = 640;
    cursor.y = 480;
    clamp_cursor_to_bounds(cursor);
    assert(cursor.x == 627); // max 637 - hotspot 10
    assert(cursor.y == 457); // max 477 - hotspot 20

    cursor.x = -20;
    cursor.y = -30;
    clamp_cursor_to_bounds(cursor);
    assert(cursor.x == -8);  // min 2 - hotspot 10
    assert(cursor.y == -18); // min 2 - hotspot 20

    constexpr auto directions =
        keyboard_directions(true,false,true,false,true);
    static_assert(directions.left());
    static_assert(!directions.right());
    static_assert(directions.up());
    static_assert(!directions.down());
    static_assert(directions.action());
    static_assert(directions.bits == (1U|4U|0x10U));

    static_assert(keyboard_cursor_step_size(false) == 1);
    static_assert(keyboard_cursor_step_size(true) == 4);

    CursorState keyboard_cursor{100,100,0,0};
    apply_keyboard_cursor_movement(
        keyboard_cursor,
        keyboard_directions(true,false,true,false,false),
        false,
        false);
    assert(keyboard_cursor.x == 99);
    assert(keyboard_cursor.y == 99);

    keyboard_cursor = {100,100,0,0};
    apply_keyboard_cursor_movement(
        keyboard_cursor,
        keyboard_directions(false,true,false,true,false),
        true,
        false);
    assert(keyboard_cursor.x == 104);
    assert(keyboard_cursor.y == 104);

    keyboard_cursor = {100,100,0,0};
    apply_keyboard_cursor_movement(
        keyboard_cursor,
        keyboard_directions(false,true,false,true,false),
        true,
        true);
    assert(keyboard_cursor.x == 100);
    assert(keyboard_cursor.y == 100);

    CursorState relative{50,60,0,0};
    apply_relative_pointer_delta(relative, 7, -3, true);
    assert(relative.x == 57);
    assert(relative.y == 57);

    relative = {50,60,0,0};
    apply_relative_pointer_delta(relative, 7, -3, false);
    assert(relative.x == 57);
    assert(relative.y == 60);

    auto movement = update_cursor_position(
        {320,240,0,0},
        keyboard_directions(false,true,false,false,false),
        false,
        false,
        5,
        7,
        true);
    assert(movement.state.x == 326);
    assert(movement.state.y == 247);
    assert(movement.moved);

    movement = update_cursor_position(
        {636,476,0,0},
        keyboard_directions(false,true,false,true,false),
        true,
        false,
        5,
        7,
        true);
    assert(movement.state.x == 637);
    assert(movement.state.y == 477);
    assert(movement.moved);

    movement = update_cursor_position(
        {320,240,0,0},
        {},
        false,
        false,
        0,
        0,
        true);
    assert(!movement.moved);

    static_assert(kInputInitPlan.direct_input_version == 0x800);
    static_assert(kInputInitPlan.create_primary_device);
    static_assert(kInputInitPlan.primary_cooperative_flags == 5);
    static_assert(kInputInitPlan.create_keyboard_device);
    static_assert(kInputInitPlan.keyboard_cooperative_flags == 0x16);
    static_assert(kInputInitPlan.apply_acquire_state_after_setup);
    static_assert(kInputInitPlan.enable_input_processing_gate);
    static_assert(kInputInitPlan.initial_cursor.x == 320);
    static_assert(kInputInitPlan.initial_cursor.y == 0);
    static_assert(kInputInitPlan.initial_bounds.max_x == 637);
    static_assert(kInputInitPlan.initial_bounds.max_y == 477);
}
