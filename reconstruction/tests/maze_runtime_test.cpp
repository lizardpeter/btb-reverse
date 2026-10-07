#include "btb/maze_runtime.hpp"

#include <cassert>
#include <vector>

using namespace btb::maze;

static Node node(
    int id,
    int x,
    int y,
    int type,
    std::array<int, 4> links = {-1, -1, -1, -1}) {
    Node n;
    n.id = id;
    n.source_position = {x, y + 11};
    n.retail_position = {x, y};
    n.node_type = type;
    n.links = links;
    return n;
}

static Data navigation_data() {
    Data data;

    data.screens[0].source_marker = "West";
    data.screens[0].nodes = {
        node(0, 587, 177, 0x200, {-1, -1, -1, 1}),
        node(1, 464, 177, 0, {-1, -1, -1, 0}),
    };

    data.screens[1].source_marker = "Middle";
    data.screens[1].nodes = {
        node(0, 40, 171, 0x80, {-1, 1, -1, -1}),
        node(1, 83, 171, 0, {-1, 15, -1, 0}),
        node(15, 603, 171, 0x100, {-1, -1, -1, 1}),
    };

    data.screens[2].source_marker = "East";
    data.screens[2].nodes = {
        node(0, 40, 169, 0x200, {-1, 1, -1, -1}),
        node(1, 96, 169, 0, {-1, -1, -1, 0}),
    };

    return data;
}

int main() {
    auto data = navigation_data();

    Navigator to_west(data, {Screen::Middle, 0, 40.0f, 171.0f});
    const auto west = to_west.apply_portal_if_needed();
    assert(west.changed_screen);
    assert(west.destination.screen == Screen::West);
    assert(west.destination.node_id == 0);
    assert(west.destination.x == 562.0f);
    assert(west.destination.y == 177.0f);

    const auto middle_from_west = to_west.apply_portal_if_needed();
    assert(middle_from_west.changed_screen);
    assert(middle_from_west.destination.screen == Screen::Middle);
    assert(middle_from_west.destination.node_id == 0);
    assert(middle_from_west.destination.x == 65.0f);
    assert(middle_from_west.destination.y == 171.0f);

    Navigator to_east(data, {Screen::Middle, 15, 603.0f, 171.0f});
    const auto east = to_east.apply_portal_if_needed();
    assert(east.changed_screen);
    assert(east.destination.screen == Screen::East);
    assert(east.destination.node_id == 0);
    assert(east.destination.x == 65.0f);
    assert(east.destination.y == 169.0f);

    const auto middle_from_east = to_east.apply_portal_if_needed();
    assert(middle_from_east.changed_screen);
    assert(middle_from_east.destination.screen == Screen::Middle);
    assert(middle_from_east.destination.node_id == 15);
    assert(middle_from_east.destination.x == 578.0f);
    assert(middle_from_east.destination.y == 171.0f);

    ScreenGraph graph;
    graph.nodes = {
        node(0, 0, 0, 0, {1, 2, -1, -1}),
        node(1, 10, 0, 0, {-1, 3, -1, 0}),
        node(2, 0, 10, 0, {0, -1, 3, -1}),
        node(3, 10, 10, 0),
    };

    const auto path = find_shortest_path_retail(graph, 0, 3);
    assert(path.has_value());
    assert((path->nodes == std::vector<std::int32_t>{0, 1, 3}));
    assert(path->cost == 20);

    assert(truncated_edge_distance(
        node(10, 0, 0, 0),
        node(11, 2, 2, 0)) == 2);

    const auto same = find_shortest_path_retail(graph, 1, 1);
    assert(same.has_value());
    assert((same->nodes == std::vector<std::int32_t>{1}));
    assert(same->cost == 0);

    // 0x0041AEB0 keeps three samples per axis and suppresses both axes
    // until both three-sample histories are stable.
    DirectionalDebounce debounce;

    auto intent = debounce.update({1, 0});
    assert(intent.x == 0 && intent.y == 0);
    assert((debounce.x_history() == std::array<std::int32_t,3>{0,0,1}));

    intent = debounce.update({1, 0});
    assert(intent.x == 0 && intent.y == 0);
    assert((debounce.x_history() == std::array<std::int32_t,3>{0,1,1}));

    intent = debounce.update({1, 0});
    assert(intent.x == 1 && intent.y == 0);
    assert((debounce.x_history() == std::array<std::int32_t,3>{1,1,1}));

    // Changing only Y still suppresses X because retail zeros the pair when
    // either axis history is unstable.
    intent = debounce.update({1, -1});
    assert(intent.x == 0 && intent.y == 0);
    intent = debounce.update({1, -1});
    assert(intent.x == 0 && intent.y == 0);
    intent = debounce.update({1, -1});
    assert(intent.x == 1 && intent.y == -1);

    // Releasing X likewise suppresses the otherwise-stable Y intent for two
    // frames, then emits the new stable pair on the third sample.
    intent = debounce.update({0, -1});
    assert(intent.x == 0 && intent.y == 0);
    intent = debounce.update({0, -1});
    assert(intent.x == 0 && intent.y == 0);
    intent = debounce.update({0, -1});
    assert(intent.x == 0 && intent.y == -1);
}
