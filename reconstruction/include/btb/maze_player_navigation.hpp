#pragma once

#include "btb/maze_data.hpp"

#include <cstdint>

namespace btb::maze {

inline constexpr std::int32_t kNodeCaptureRadius = 4;
inline constexpr long double kNodeAttractionRadius = 30.0L;

struct PlayerNodeScanState {
    std::int32_t current_node{};
    std::int32_t last_captured_node{-1};
};

struct PlayerNodeScanResult {
    PlayerNodeScanState state{};

    // Keyboard mode only: last qualifying current/linked node in the retail
    // scan order that lies inside the 30-pixel attraction radius.
    std::int32_t attraction_node{-1};

    // Node actually captured this frame at the inner radius, or -1.
    std::int32_t captured_node{-1};

    // Retail has a special latch when the current node is captured on
    // consecutive passes. Neighbor captures do not set this flag.
    bool repeated_current_capture{};
};

// Exact node-proximity/candidate scan from the front half of
// 0x0041B140 UpdateMazePlayerMovement.
//
// Important asymmetry:
// - current node capture is distance <= 4
// - linked-node capture is distance < 4
//
// In keyboard mode, candidates inside 30 pixels are overwritten while the
// four links are scanned, so the last qualifying link wins. Mouse mode never
// sets an attraction candidate here, though inner-radius capture still works.
[[nodiscard]] PlayerNodeScanResult scan_player_navigation_nodes(
    const ScreenGraph& graph,
    Vec2i player_integer,
    bool mouse_mode,
    PlayerNodeScanState state);

} // namespace btb::maze
