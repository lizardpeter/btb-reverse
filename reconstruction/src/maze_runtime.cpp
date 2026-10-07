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

bool history_is_stable(
    const std::array<std::int32_t,3>& history) noexcept {
    return history[0] == history[1] &&
           history[0] == history[2];
}

void shift_history(
    std::array<std::int32_t,3>& history,
    std::int32_t newest) noexcept {
    history[0] = history[1];
    history[1] = history[2];
    history[2] = newest;
}

} // namespace

DirectionalIntent DirectionalDebounce::update(
    DirectionalIntent intent) noexcept {

    shift_history(x_history_, intent.x);
    shift_history(y_history_, intent.y);

    // 0x0041AEB0 does not debounce the axes independently at its output.
    // Any instability suppresses the complete direction pair.
    if (!history_is_stable(x_history_) ||
        !history_is_stable(y_history_)) {
        intent.x = 0;
        intent.y = 0;
    }

    return intent;
}

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


namespace {

bool path_contains(
    const std::vector<std::int32_t>& path,
    std::int32_t id) noexcept {
    for (const auto value : path) {
        if (value == id) {
            return true;
        }
    }
    return false;
}

void search_path_recursive(
    const ScreenGraph& graph,
    std::int32_t target,
    std::vector<std::int32_t>& working,
    std::int32_t accumulated_cost,
    std::optional<PathResult>& best) {

    if (working.size() >= kRetailMaximumPathNodes) {
        return;
    }

    const auto& current = node_by_id(graph, working.back());
    for (std::size_t direction = 0; direction < current.links.size(); ++direction) {
        const auto next_id = current.links[direction];
        if (next_id == -1 || path_contains(working, next_id)) {
            continue;
        }

        const auto& next = node_by_id(graph, next_id);
        const auto candidate_cost =
            accumulated_cost + truncated_edge_distance(current, next);

        if (best && candidate_cost >= best->cost) {
            continue;
        }

        working.push_back(next_id);
        if (next_id == target) {
            best = PathResult{working, candidate_cost};
        } else {
            search_path_recursive(
                graph, target, working, candidate_cost, best);
        }
        working.pop_back();
    }
}

} // namespace

std::int32_t truncated_edge_distance(
    const Node& a,
    const Node& b) noexcept {
    const auto dx = static_cast<double>(
        a.retail_position.x - b.retail_position.x);
    const auto dy = static_cast<double>(
        a.retail_position.y - b.retail_position.y);
    return static_cast<std::int32_t>(std::sqrt(dx * dx + dy * dy));
}

std::optional<PathResult> find_shortest_path_retail(
    const ScreenGraph& graph,
    std::int32_t start_node,
    std::int32_t target_node) {

    (void)node_by_id(graph, start_node);
    (void)node_by_id(graph, target_node);

    if (start_node == target_node) {
        return PathResult{{start_node}, 0};
    }

    std::vector<std::int32_t> working;
    working.reserve(kRetailMaximumPathNodes);
    working.push_back(start_node);

    std::optional<PathResult> best;
    search_path_recursive(graph, target_node, working, 0, best);
    return best;
}

} // namespace btb::maze
