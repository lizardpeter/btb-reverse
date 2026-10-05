#include "btb/maze_runtime.hpp"

#include <cmath>
#include <stdexcept>

namespace btb::maze {
namespace {

std::size_t screen_index(Screen screen) {
    return static_cast<std::size_t>(screen);
}

const Node& node_by_id(const ScreenGraph& graph, std::int32_t id) {
    for (const auto& node : graph.nodes) {
        if (node.id == id) {
            return node;
        }
    }
    throw std::runtime_error("Maze graph references unknown node");
}

} // namespace

Navigator::Navigator(const Data& data, NavigatorState initial)
    : data_(&data), state_(initial) {
    (void)node(state_.screen, state_.node_id);
}

const ScreenGraph& Navigator::graph(Screen screen) const {
    return data_->screens.at(screen_index(screen));
}

const Node& Navigator::node(Screen screen, std::int32_t node_id) const {
    return node_by_id(graph(screen), node_id);
}

const Node& Navigator::current_node() const {
    return node(state_.screen, state_.node_id);
}

bool Navigator::can_move(Direction direction) const {
    return current_node().allows(direction)
        && current_node().linked_node(direction) != -1;
}

std::optional<std::int32_t> Navigator::linked_node(Direction direction) const {
    if (!can_move(direction)) {
        return std::nullopt;
    }
    return current_node().linked_node(direction);
}

bool Navigator::choose_link(Direction direction) {
    const auto next = linked_node(direction);
    if (!next) {
        return false;
    }
    state_.node_id = *next;
    return true;
}

PortalTransition Navigator::apply_portal_if_needed() {
    const auto type = current_node().type();
    const auto destination = current_node().portal_destination();
    if (!destination) {
        return {state_, false};
    }

    const auto previous_screen = state_.screen;
    state_.screen = *destination;

    switch (type) {
        case NodeType::PortalToWest: {
            state_.node_id = 0;
            const auto& entry = node(Screen::West, 0);
            state_.x = static_cast<float>(entry.retail_position.x) - portal_offset();
            state_.y = static_cast<float>(entry.retail_position.y);
            break;
        }
        case NodeType::PortalToEast: {
            state_.node_id = 0;
            const auto& entry = node(Screen::East, 0);
            state_.x = static_cast<float>(entry.retail_position.x) + portal_offset();
            state_.y = static_cast<float>(entry.retail_position.y);
            break;
        }
        case NodeType::PortalToMiddle: {
            if (previous_screen == Screen::West) {
                state_.node_id = 0;
                const auto& entry = node(Screen::Middle, 0);
                state_.x = static_cast<float>(entry.retail_position.x) + portal_offset();
                state_.y = static_cast<float>(entry.retail_position.y);
            } else {
                state_.node_id = 15;
                const auto& entry = node(Screen::Middle, 15);
                state_.x = static_cast<float>(entry.retail_position.x) - portal_offset();
                state_.y = static_cast<float>(entry.retail_position.y);
            }
            break;
        }
        case NodeType::Normal:
            break;
    }

    return {state_, true};
}

} // namespace btb::maze
