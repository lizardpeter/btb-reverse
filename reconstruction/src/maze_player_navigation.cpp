#include "btb/maze_player_navigation.hpp"

#include <cmath>
#include <stdexcept>

namespace btb::maze {
namespace {

[[nodiscard]] const Node& node_by_id(
    const ScreenGraph& graph,
    std::int32_t id) {
    for (const auto& node : graph.nodes) {
        if (node.id == id) {
            return node;
        }
    }
    throw std::runtime_error("Maze player navigation references unknown node");
}

[[nodiscard]] long double distance(
    Vec2i a,
    Vec2i b) noexcept {
    const auto dx =
        static_cast<long double>(a.x) -
        static_cast<long double>(b.x);
    const auto dy =
        static_cast<long double>(a.y) -
        static_cast<long double>(b.y);
    return std::sqrt(dx * dx + dy * dy);
}

} // namespace

PlayerNodeScanResult scan_player_navigation_nodes(
    const ScreenGraph& graph,
    Vec2i player_integer,
    bool mouse_mode,
    PlayerNodeScanState state) {

    PlayerNodeScanResult result;
    result.state = state;

    const auto& current =
        node_by_id(graph, state.current_node);
    const auto current_distance =
        distance(player_integer, current.retail_position);

    if (current_distance < kNodeAttractionRadius &&
        current_distance > static_cast<long double>(kNodeCaptureRadius) &&
        !mouse_mode) {
        result.attraction_node = current.id;
    }

    // Current-node capture uses <=, unlike linked nodes below.
    if (current_distance <=
        static_cast<long double>(kNodeCaptureRadius)) {

        result.captured_node = current.id;
        result.repeated_current_capture =
            state.last_captured_node == current.id;

        if (!result.repeated_current_capture) {
            result.state.last_captured_node = current.id;
        }
        return result;
    }

    // Retail only clears the capture latch when strictly outside 30.
    if (current_distance > kNodeAttractionRadius) {
        result.state.last_captured_node = -1;
    }

    for (const auto linked_id : current.links) {
        if (linked_id == -1) {
            continue;
        }

        const auto& linked = node_by_id(graph, linked_id);
        const auto linked_distance =
            distance(player_integer, linked.retail_position);

        // Neighbor capture is strictly <4 and takes effect immediately.
        if (linked_distance <
            static_cast<long double>(kNodeCaptureRadius)) {
            result.attraction_node = -1;
            result.captured_node = linked_id;
            result.state.last_captured_node = linked_id;
            result.state.current_node = linked_id;
            return result;
        }

        if (linked_distance < kNodeAttractionRadius &&
            !mouse_mode) {
            // Deliberately overwrite: retail keeps the last qualifying link,
            // not the nearest one.
            result.attraction_node = linked_id;
        }
    }

    return result;
}

} // namespace btb::maze
