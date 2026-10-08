#include "btb/maze_spud_controller.hpp"
#include <cassert>

using namespace btb::maze;

int main() {
    static_assert(spud_carry_speed(0) == 1 && spud_carry_speed(1) == 1);
    static_assert(spud_carry_speed(2) == 2);

    // Both the waypoint and collision radii use strict less-than.
    constexpr auto boundary = advance_spud_carry_frame(
        3, 20, 60.0f, false, 0, 2, 1);
    static_assert(boundary.nearest_node_distance == 3);
    static_assert(boundary.next_phase == SpudNpcPhase::TravelCarryingPackage);
    static_assert(!boundary.captured_waypoint && !boundary.drop_package);

    constexpr auto near_waypoint = advance_spud_carry_frame(
        2, 1, 70.0f, false, 0, 2, 1);
    static_assert(near_waypoint.nearest_node_distance == 1);
    static_assert(near_waypoint.next_phase == SpudNpcPhase::ChooseReturnRoute);
    static_assert(near_waypoint.waypoint_voice.value() == 92);
    static_assert(!near_waypoint.collision_voice.has_value());

    // An already latched collision chooses reset path but cannot drop twice.
    constexpr auto latched = advance_spud_carry_frame(
        2, 10, 45.0f, true, 1, 0, 0);
    static_assert(latched.next_phase == SpudNpcPhase::ResetPath);
    static_assert(!latched.drop_package);

    // A collision on the same frame as waypoint arrival still chooses phase
    // 4 using the OLD latch, then marks the package dropped and latch set.
    constexpr auto simultaneous = advance_spud_carry_frame(
        2, 10, 59.99f, false, 1, 0, 1);
    static_assert(simultaneous.next_phase == SpudNpcPhase::ChooseReturnRoute);
    static_assert(simultaneous.drop_package && simultaneous.set_collision_latch);
    static_assert(simultaneous.clear_carrying_flag);
    static_assert(!simultaneous.waypoint_voice.has_value());
    static_assert(simultaneous.collision_voice.value() == 95);

    constexpr auto drop = spud_dropped_package_position(-10.99f, 15.9f);
    static_assert(drop.x == -10 && drop.y == 15);
    assert(near_waypoint.waypoint_voice.value() == 92);
}
