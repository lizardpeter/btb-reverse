#pragma once

#include "btb/maze_data.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::maze {

struct NavigatorState {
    Screen screen{Screen::Middle};
    std::int32_t node_id{};
    float x{};
    float y{};
};

struct PortalTransition {
    NavigatorState destination;
    bool changed_screen{};
};

struct DirectionalIntent {
    std::int32_t x{};
    std::int32_t y{};

    friend bool operator==(const DirectionalIntent&, const DirectionalIntent&) = default;
};

// Exact state retained by 0x0041AEB0 DebounceMazeDirectionalInput.
//
// Retail keeps three int32 samples for each axis. A new sample is shifted into
// slot 2 every call. Movement is emitted only when all three X samples match
// AND all three Y samples match. If either axis is unstable, retail zeros both
// output axes.
class DirectionalDebounce {
public:
    [[nodiscard]] DirectionalIntent update(DirectionalIntent intent) noexcept;

    [[nodiscard]] const std::array<std::int32_t,3>& x_history() const noexcept {
        return x_history_;
    }
    [[nodiscard]] const std::array<std::int32_t,3>& y_history() const noexcept {
        return y_history_;
    }

private:
    std::array<std::int32_t,3> x_history_{};
    std::array<std::int32_t,3> y_history_{};
};

class Navigator {
public:
    Navigator(const Data& data, NavigatorState initial);

    [[nodiscard]] const NavigatorState& state() const noexcept { return state_; }
    [[nodiscard]] const Node& current_node() const;

    [[nodiscard]] bool can_move(Direction direction) const;
    [[nodiscard]] std::optional<std::int32_t> linked_node(Direction direction) const;

    // Logical node selection is separated from frame interpolation. This
    // matches the graph decisions made by UpdateMazePlayerMovement.
    bool choose_link(Direction direction);

    // Apply the retail screen-transition semantics of node_type
    // 0x80/0x100/0x200 once a portal node is reached.
    [[nodiscard]] PortalTransition apply_portal_if_needed();

    [[nodiscard]] static constexpr float portal_offset() noexcept { return 25.0f; }

private:
    [[nodiscard]] const ScreenGraph& graph(Screen screen) const;
    [[nodiscard]] const Node& node(Screen screen, std::int32_t node_id) const;

    const Data* data_{};
    NavigatorState state_{};
};

struct PathResult {
    std::vector<std::int32_t> nodes;
    std::int32_t cost{};

    friend bool operator==(const PathResult&, const PathResult&) = default;
};

// SearchMazePathRecursive has a 13-dword working path buffer. Start occupies
// slot 0, so retail can retain at most 13 nodes in one candidate path.
inline constexpr std::size_t kRetailMaximumPathNodes = 13;

[[nodiscard]] std::optional<PathResult> find_shortest_path_retail(
    const ScreenGraph& graph,
    std::int32_t start_node,
    std::int32_t target_node);

[[nodiscard]] std::int32_t truncated_edge_distance(
    const Node& a,
    const Node& b) noexcept;

} // namespace btb::maze
