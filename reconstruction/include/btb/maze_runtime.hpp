#pragma once

#include "btb/maze_data.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::maze {

enum class Screen : std::int32_t {
    West = 0,
    Middle = 1,
    East = 2,
};

enum class NodeType : std::int32_t {
    Normal = 0,
    EnterWest = 0x80,
    EnterEast = 0x100,
    ReturnToMiddle = 0x200,
};

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

class Navigator {
public:
    Navigator(const Data& data, NavigatorState initial);

    [[nodiscard]] const NavigatorState& state() const noexcept { return state_; }
    [[nodiscard]] const Node& current_node() const;

    [[nodiscard]] bool can_move(Direction direction) const;
    [[nodiscard]] std::optional<std::int32_t> linked_node(Direction direction) const;

    // Move logically to a linked graph node. The retail frame-by-frame
    // interpolation is separate; this captures the exact graph transition.
    bool choose_link(Direction direction);

    // Apply the screen-portal semantics encoded by node_type 0x80/0x100/0x200.
    [[nodiscard]] PortalTransition apply_portal_if_needed();

    [[nodiscard]] static constexpr float portal_offset() noexcept { return 25.0f; }

private:
    [[nodiscard]] const ScreenGraph& graph(Screen screen) const;
    [[nodiscard]] const Node& node(Screen screen, std::int32_t node_id) const;

    const Data* data_{};
    NavigatorState state_{};
};

} // namespace btb::maze
