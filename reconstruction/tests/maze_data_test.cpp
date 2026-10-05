#include "btb/maze_data.hpp"

#include <cassert>
#include <string>

using namespace btb::maze;

int main() {
    const std::string text = R"(
header
zero 0 587 188 8 512 -1 -1 -1 1
one 1 464 188 7 0 8 0 12 -1
NEXT_MiddleScreen
LeftExit 0 40 182 2 128 -1 1 -1 -1
nexttoEx 1 83 182 13 0 2 -1 10 0
NEXT_EastScreen
Exit 0 40 180 2 512 -1 1 -1 -1
Next_to_ex 1 96 180 15 0 5 2 10 0
END
87 67 2 548 67 5 66 320 15 568 320 17
89 98 2 549 98 5 60 314 10 563 314 13
526 69 7 89 323 14 392 249 12 546 323 16
2.0
1 1 2
5 6 7
240 180 120
1500 1000 200
6
)";

    const auto data = parse_data(text);
    assert(data.screens[0].nodes.size() == 2);
    assert(data.screens[1].nodes.size() == 2);
    assert(data.screens[2].nodes.size() == 2);

    const auto& west0 = data.screens[0].nodes[0];
    assert(west0.id == 0);
    assert((west0.source_position == Vec2i{587, 188}));
    assert((west0.retail_position == Vec2i{587, 177}));
    assert(west0.direction_bits == 8);
    assert(west0.type() == NodeType::PortalToMiddle);
    assert(west0.portal_destination() == Screen::Middle);
    assert(west0.allows(Direction::Left));
    assert(!west0.allows(Direction::Right));
    assert(west0.linked_node(Direction::Left) == 1);
    assert(west0.portal_destination() == Screen::Middle);

    const auto& middle_exit = data.screens[1].nodes[0];
    assert(middle_exit.portal_destination() == Screen::West);

    Node east_exit;
    east_exit.node_type = kPortalToEast;
    assert(east_exit.portal_destination() == Screen::East);

    const auto& middle_exit_west = data.screens[1].nodes[0];
    assert(middle_exit_west.type() == NodeType::PortalToWest);
    assert(middle_exit_west.portal_destination() == Screen::West);

    const auto& east_exit = data.screens[2].nodes[0];
    assert(east_exit.type() == NodeType::PortalToMiddle);
    assert(east_exit.portal_destination() == Screen::Middle);

    const auto& west1 = data.screens[0].nodes[1];
    assert(west1.allows(Direction::Up));
    assert(west1.allows(Direction::Right));
    assert(west1.allows(Direction::Down));
    assert(!west1.allows(Direction::Left));

    assert((data.reference_nodes[0][0].position == Vec2i{87, 67}));
    assert(data.reference_nodes[0][0].node_id == 2);

    assert(data.player_speed == 2.0f);
    assert((data.spud_speed_regular == std::array<std::int32_t,3>{1,1,2}));
    assert((data.spud_speed_package == std::array<std::int32_t,3>{5,6,7}));
    assert((data.timer_by_difficulty == std::array<std::int32_t,3>{240,180,120}));
    assert((data.spud_spawn_frames_by_difficulty == std::array<std::int32_t,3>{1500,1000,200}));
    assert(data.spud_animation_delay == 6);
}
