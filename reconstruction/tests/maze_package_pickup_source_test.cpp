#include "btb/maze_package_pickup.hpp"
#include "btb/maze_controller.hpp"

#include <cstdlib>
#include <cstdio>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"retail Maze pickup assertion: %s at %s:%d\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)

int main() {
    using namespace btb::maze;
    static_assert(kRetailPickupRadiusExclusive==46);
    static_assert(classify_end_condition(0,200,0)==EndCondition::Success);
    static_assert(classify_end_condition(1,-1,0)==EndCondition::Timeout);
    static_assert(classify_end_condition(0,100,1)==EndCondition::None);

    Data data{};
    data.box_locations[1][0].position={100,100};
    data.box_locations[1][1].position={150,100};
    data.box_locations[1][2].position={200,100};
    data.box_locations[1][3].position={250,100};

    BoxAllocation boxes{};
    boxes.total=4;
    boxes.remaining=4;
    boxes.screens[1][0]={0,1};
    boxes.screens[1][1]={1,2};
    boxes.screens[1][2]={2,3,0,260,100};
    boxes.screens[1][3]={3,1};

    // Original radius is exclusive, while float player coords are
    // truncated as in source helper __ftol.
    assert(!collect_source_box(boxes,data,1,54.0f,100.0f));
    assert(boxes.remaining==4);
    auto result=collect_source_box(boxes,data,1,54.1f,100.0f);
    assert(!result); // truncated to 54, exactly 46 away
    result=collect_source_box(boxes,data,1,55.8f,100.0f);
    assert(result && result->slot==0 && result->remaining==3);
    assert(boxes.screens[1][0].status==0);
    // Source status 2 belongs to the Spud NPC; the player cannot collect.
    assert(!collect_source_box(boxes,data,1,150,100));
    assert(boxes.remaining==3);
    // Status 3 picks up at live dropped coords rather than source slot 2.
    result=collect_source_box(boxes,data,1,260,100);
    assert(result && result->slot==2 && result->remaining==2);
    assert(boxes.screens[1][2].status==0);
    assert(!collect_source_box(boxes,data,0,250,100));
    result=collect_source_box(boxes,data,1,250,100);
    assert(result && result->slot==3 && result->remaining==1);
    boxes.screens[1][1].status=1; // Spud returned an available box
    result=collect_source_box(boxes,data,1,150,100);
    assert(result && result->slot==1 && result->remaining==0);
    assert(classify_end_condition(boxes.remaining,200,0)==EndCondition::Success);
    assert(success_transition_code(0)==-1);
    assert(success_transition_code(1)==3);
    // Never award the same source package twice.
    assert(!collect_source_box(boxes,data,1,150,100));
    assert(boxes.remaining==0);
}
