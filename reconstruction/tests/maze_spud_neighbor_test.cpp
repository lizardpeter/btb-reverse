#include "btb/maze_spud_controller.hpp"
#include <cassert>

using namespace btb::maze;

int main() {
    // Pass 1: skip player node, reverse direction, and player-adjacent node.
    constexpr auto first = select_spud_return_neighbor(
        {41,42,43,44}, /*previous=*/0, /*random=*/1,
        /*player=*/42, {45,41,-1,50});
    static_assert(first.direction == 3 && first.node_id == 44);
    static_assert(first.opposite_previous_direction == 2);
    static_assert(first.attempt_pass == 1);

    // Pass 2 relaxes the reverse-direction prohibition, but still protects
    // the player node and its directly connected neighbors.
    constexpr auto backtrack = select_spud_return_neighbor(
        {-1,-1,80,-1}, /*previous=*/0, /*random=*/0,
        /*player=*/90, {-1,-1,-1,-1});
    static_assert(backtrack.direction == 2);
    static_assert(backtrack.attempt_pass == 2);

    // Pass 3 relaxes player-neighbor filtering as well, preserving the
    // authored fallback when no safe route exists.
    constexpr auto nearby = select_spud_return_neighbor(
        {-1,-1,80,-1}, /*previous=*/0, /*random=*/0,
        /*player=*/90, {80,-1,-1,-1});
    static_assert(nearby.direction == 2 && nearby.attempt_pass == 3);

    // Circular scanning wraps 3 -> 0.
    constexpr auto wrap = select_spud_return_neighbor(
        {100,-1,-1,-1}, /*previous=*/1, /*random=*/3,
        /*player=*/200, {-1,-1,-1,-1});
    static_assert(wrap.direction == 0 && wrap.attempt_pass == 1);

    // The retail fourth-pass fallthrough does not reject all-invalid links.
    constexpr auto broken = select_spud_return_neighbor(
        {-1,-1,-1,-1}, /*previous=*/2, /*random=*/3,
        /*player=*/10, {-1,-1,-1,-1});
    static_assert(broken.direction == 3 && broken.node_id == -1);
    static_assert(broken.attempt_pass == 4);
    static_assert(!broken.found_eligible_neighbor);

    assert(backtrack.attempt_pass == 2);
}
