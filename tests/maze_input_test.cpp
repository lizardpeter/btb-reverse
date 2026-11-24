#include "btb/maze_input.hpp"

#include <array>
#include <cassert>

using namespace btb::maze;

int main() {
    // Shared angle helper cardinal directions, including retail's 359-degree
    // vertical-up artifact from the 9999.0 zero-X sentinel. The stored\n    // 57.294998-degree conversion also makes two perfect diagonals 134/314.
    assert(angle_between_integer_points_degrees({100,100},{200,100}) == 90);
    assert(angle_between_integer_points_degrees({100,100},{100,200}) == 180);
    assert(angle_between_integer_points_degrees({100,100},{0,100}) == 270);
    assert(angle_between_integer_points_degrees({100,100},{100,0}) == 359);

    assert(angle_between_integer_points_degrees({100,100},{200,200}) == 134);
    assert(angle_between_integer_points_degrees({100,100},{0,200}) == 225);
    assert(angle_between_integer_points_degrees({100,100},{0,0}) == 314);
    assert(angle_between_integer_points_degrees({100,100},{200,0}) == 45);

    // Keyboard precedence is asymmetric and bypasses the three-sample debounce.
    assert(keyboard_direction_from_bits(0x00) == (DirectionalIntent{0,0}));
    assert(keyboard_direction_from_bits(0x01) == (DirectionalIntent{-1,0}));
    assert(keyboard_direction_from_bits(0x02) == (DirectionalIntent{1,0}));
    assert(keyboard_direction_from_bits(0x03) == (DirectionalIntent{1,0}));
    assert(keyboard_direction_from_bits(0x04) == (DirectionalIntent{0,1}));
    assert(keyboard_direction_from_bits(0x08) == (DirectionalIntent{0,-1}));
    assert(keyboard_direction_from_bits(0x0C) == (DirectionalIntent{0,-1}));
    assert(keyboard_direction_from_bits(0x0F) == (DirectionalIntent{1,-1}));

    InputModeState mode{
        false,
        10,
        20,
    };
    DirectionalDebounce debounce;

    // Cursor movement while in keyboard mode flips the persistent mode, but
    // this call still executes the keyboard path.
    auto step = compute_player_input_direction(
        mode,
        debounce,
        {100.0f,100.0f,11,20,0x01});
    assert(step.path == InputPath::Keyboard);
    assert(step.intent == (DirectionalIntent{-1,0}));
    assert(step.mode_changed_for_next_call);
    assert(mode.mouse_mode);
    assert(mode.remembered_mouse_x == 11);
    assert(mode.remembered_mouse_y == 20);
    assert(!step.debounce_applied);
    assert(step.writes_node_capture_radius_four);

    // Keyboard input while in mouse mode similarly requests keyboard mode for
    // next time but does not change the path chosen for the current call.
    step = compute_player_input_direction(
        mode,
        debounce,
        {100.0f,100.0f,200,100,0x02});
    assert(step.path == InputPath::Mouse);
    assert(step.mode_changed_for_next_call);
    assert(!mode.mouse_mode);
    assert(step.debounce_applied);
    assert(step.intent == (DirectionalIntent{0,0}));

    // Two more matching mouse samples are needed before debounce emits +X.
    mode.mouse_mode = true;
    step = compute_player_input_direction(
        mode,
        debounce,
        {100.0f,100.0f,200,100,0});
    assert(step.intent == (DirectionalIntent{0,0}));
    step = compute_player_input_direction(
        mode,
        debounce,
        {100.0f,100.0f,200,100,0});
    assert(step.intent == (DirectionalIntent{1,0}));

    // The distance comparison is <= 6.0, and the dead-zone returns before
    // debounce or the shared node-capture-radius write.
    DirectionalDebounce untouched;
    mode.mouse_mode = true;
    step = compute_player_input_direction(
        mode,
        untouched,
        {100.9f,100.9f,106,100,0});
    assert(step.path == InputPath::Mouse);
    assert(step.mouse_dead_zone);
    assert(!step.debounce_applied);
    assert(!step.writes_node_capture_radius_four);
    assert(step.intent == (DirectionalIntent{0,0}));
    assert((untouched.x_history() == std::array<std::int32_t,3>{0,0,0}));
    assert((untouched.y_history() == std::array<std::int32_t,3>{0,0,0}));

    // Just outside the dead-zone enters the direction/debounce path.
    step = compute_player_input_direction(
        mode,
        untouched,
        {100.0f,100.0f,107,100,0});
    assert(!step.mouse_dead_zone);
    assert(step.debounce_applied);
    assert(step.writes_node_capture_radius_four);

    // Pure vertical-up follows the shared 359-degree angle; after debounce it
    // resolves to Y=+1 with no X component.
    DirectionalDebounce vertical;
    mode.mouse_mode = true;
    for (int i = 0; i < 3; ++i) {
        step = compute_player_input_direction(
            mode,
            vertical,
            {100.0f,100.0f,100,0,0});
    }
    assert(step.intent == (DirectionalIntent{0,1}));
}
