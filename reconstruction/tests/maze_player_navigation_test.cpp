#include "btb/maze_player_navigation.hpp"

#include <cassert>

using namespace btb::maze;

namespace {

Node node(
    int id,
    int x,
    int y,
    std::array<int,4> links = {-1,-1,-1,-1}) {
    Node n;
    n.id = id;
    n.source_position = {x,y + 11};
    n.retail_position = {x,y};
    n.links = links;
    return n;
}

} // namespace

int main() {
    ScreenGraph graph;
    graph.nodes = {
        node(0,100,100,{1,2,3,4}),
        node(1,120,100),
        node(2,100,120),
        node(3,80,100),
        node(4,100,80),
    };

    // Keyboard mode scans every link and keeps the last qualifying node.
    auto result = scan_player_navigation_nodes(
        graph,{110,110},false,{0,-1});
    assert(result.captured_node == -1);
    assert(result.attraction_node == 4);
    assert(result.state.current_node == 0);

    // Mouse mode suppresses attraction but retains inner capture semantics.
    result = scan_player_navigation_nodes(
        graph,{110,110},true,{0,-1});
    assert(result.captured_node == -1);
    assert(result.attraction_node == -1);

    // Current node uses <=4.
    result = scan_player_navigation_nodes(
        graph,{104,100},false,{0,-1});
    assert(result.captured_node == 0);
    assert(!result.repeated_current_capture);
    assert(result.state.last_captured_node == 0);

    // Capturing the same current node again trips the dedicated latch.
    result = scan_player_navigation_nodes(
        graph,{104,100},false,result.state);
    assert(result.captured_node == 0);
    assert(result.repeated_current_capture);

    // A linked node at exactly 4 is NOT captured; it can still become the
    // keyboard attraction target.
    result = scan_player_navigation_nodes(
        graph,{116,100},false,{0,-1});
    assert(result.captured_node == -1);
    assert(result.attraction_node == 1);

    // Move one pixel closer and the neighbor is captured, becomes current,
    // clears the attraction candidate, and seeds the capture latch.
    result = scan_player_navigation_nodes(
        graph,{117,100},false,{0,-1});
    assert(result.captured_node == 1);
    assert(result.attraction_node == -1);
    assert(result.state.current_node == 1);
    assert(result.state.last_captured_node == 1);
    assert(!result.repeated_current_capture);

    // Exactly 30 does not qualify as attraction but also does not clear the
    // previous capture latch; strictly beyond 30 clears it.
    ScreenGraph isolated;
    isolated.nodes = {node(9,100,100)};

    result = scan_player_navigation_nodes(
        isolated,{130,100},false,{9,7});
    assert(result.attraction_node == -1);
    assert(result.state.last_captured_node == 7);

    result = scan_player_navigation_nodes(
        isolated,{131,100},false,{9,7});
    assert(result.state.last_captured_node == -1);
}
