#pragma once

#include "btb/maze_data.hpp"

#include <cstdint>

namespace btb::maze {

struct ActorPosition {
    float x{};
    float y{};

    friend bool operator==(const ActorPosition&, const ActorPosition&) = default;
};

// Exact movement kernel at 0x0041CC20. Retail truncates the actor's float
// position to integer coordinates, computes the shared integer angle toward
// the target node, then applies integer speed through cosine/sine.
void move_actor_toward_node(
    const ScreenGraph& graph,
    std::int32_t target_node_id,
    ActorPosition& actor,
    std::int32_t speed);

} // namespace btb::maze
