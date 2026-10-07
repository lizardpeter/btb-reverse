#include "btb/maze_motion.hpp"

#include "btb/maze_input.hpp"

#include <cmath>
#include <cstdint>
#include <stdexcept>

namespace btb::maze {
namespace {

constexpr long double kRetailRadiansPerDegree =
    static_cast<long double>(0.017453530803322792f);

[[nodiscard]] const Node& node_by_id(
    const ScreenGraph& graph,
    std::int32_t id) {
    for (const auto& node : graph.nodes) {
        if (node.id == id) {
            return node;
        }
    }
    throw std::runtime_error("Maze motion references unknown node");
}

[[nodiscard]] std::int32_t trunc_float_to_int(
    float value) noexcept {
    return static_cast<std::int32_t>(value);
}

} // namespace

void move_actor_toward_node(
    const ScreenGraph& graph,
    std::int32_t target_node_id,
    ActorPosition& actor,
    std::int32_t speed) {

    const auto& target = node_by_id(graph, target_node_id);

    const Vec2i actor_integer{
        trunc_float_to_int(actor.x),
        trunc_float_to_int(actor.y),
    };

    // Retail calls DistanceBetweenIntegerPoints here and immediately pops the
    // result without using it. Preserve the semantic fact without introducing
    // a fake movement gate: there is no distance-based stop in this helper.
    const auto angle =
        angle_between_integer_points_degrees(
            actor_integer,
            target.retail_position);

    const auto radians =
        static_cast<long double>(angle) * kRetailRadiansPerDegree;
    const auto speed_fp = static_cast<long double>(
        static_cast<float>(speed));

    // The x87 code updates/stores Y first, then computes X from the original
    // retained angle. Each destination is rounded to the 32-bit float global.
    actor.y = static_cast<float>(
        static_cast<long double>(actor.y) -
        std::cos(radians) * speed_fp);
    actor.x = static_cast<float>(
        static_cast<long double>(actor.x) +
        std::sin(radians) * speed_fp);
}

} // namespace btb::maze
