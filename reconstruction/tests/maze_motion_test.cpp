#include "btb/maze_motion.hpp"

#include <cassert>
#include <cmath>

using namespace btb::maze;

namespace {

Node target(int id, int x, int y) {
    Node n;
    n.id = id;
    n.source_position = {x,y + 11};
    n.retail_position = {x,y};
    return n;
}

bool near(float a, float b, float tolerance = 0.001f) {
    return std::fabs(a - b) <= tolerance;
}

} // namespace

int main() {
    ScreenGraph graph;
    graph.nodes = {
        target(0,200,100),
        target(1,100,200),
        target(2,0,100),
        target(3,100,0),
    };

    ActorPosition right{100.0f,100.0f};
    move_actor_toward_node(graph,0,right,5);
    assert(near(right.x,105.0f));
    assert(near(right.y,100.0f));

    ActorPosition down{100.0f,100.0f};
    move_actor_toward_node(graph,1,down,5);
    assert(near(down.x,100.0f));
    assert(near(down.y,105.0f));

    ActorPosition left{100.0f,100.0f};
    move_actor_toward_node(graph,2,left,5);
    assert(near(left.x,95.0f));
    assert(near(left.y,100.0f));

    // Exact vertical-up does not use 0 degrees. The 9999.0 sentinel in
    // 0x00415D70 truncates to 359 degrees, creating a small leftward drift.
    ActorPosition up{100.0f,100.0f};
    move_actor_toward_node(graph,3,up,5);
    assert(up.x < 100.0f);
    assert(near(up.x,99.9127f,0.002f));
    assert(near(up.y,95.0008f,0.002f));

    // Actor coordinates are truncated before the angle calculation.
    ActorPosition fractional{100.9f,100.9f};
    move_actor_toward_node(graph,0,fractional,2);
    assert(fractional.x > 102.8f);
    assert(fractional.y > 100.8f && fractional.y < 101.0f);
}
