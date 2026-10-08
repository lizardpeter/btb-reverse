#include "btb/maze_spud_controller.hpp"
#include <cassert>

using namespace btb::maze;

int main() {
    static_assert(spud_random_edge_node(0,0) == 0);
    static_assert(spud_random_edge_node(2,0) == 0);
    static_assert(spud_random_edge_node(1,0) == 15);
    static_assert(spud_random_edge_node(1,1) == 0);

    // First matching link wins, duplicates do not choose the last entry.
    static_assert(spud_direction_to_path_node({10,20,20,30},20) == 1);
    static_assert(spud_direction_to_path_node({10,20,-1,30},40) == 4);

    constexpr auto middle_even = spud_return_route_build(1,0,7);
    static_assert(middle_even.next_phase == SpudNpcPhase::TravelHome);
    static_assert(middle_even.target_node == 15);
    static_assert(middle_even.first_path_index == 0);
    static_assert(middle_even.animation_countdown == 3);
    static_assert(middle_even.clear_collision_latch);
    static_assert(middle_even.clear_carrying_flag);

    constexpr auto middle_odd = spud_return_route_build(1,1,8);
    static_assert(middle_odd.target_node == 0);
    static_assert(middle_odd.animation_countdown == 4);

    // Signed integer division truncates toward zero.
    constexpr auto west = spud_return_route_build(0,0,-7);
    static_assert(west.target_node == 0);
    static_assert(west.animation_countdown == -3);

    assert(middle_even.target_node == 15);
}
