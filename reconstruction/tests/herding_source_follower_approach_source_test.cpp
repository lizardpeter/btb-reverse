#include "btb/herding_source_follower_approach.hpp"

#include <cassert>
#include <cmath>

namespace h=btb::herding;

h::RetailEntityRecord32 animal() {
    h::RetailEntityRecord32 value{};
    value.entity_id=1;
    value.type=static_cast<int>(h::EntityType::Sheep);
    value.x=100;
    value.y=100;
    value.x_float=100.0f;
    value.y_float=100.0f;
    value.movement_speed=0.8f;
    value.animation_frame=2;
    value.animation_frame_countdown=4;
    return value;
}

int main() {
    const auto starting=animal();

    // 0x4172D2: strict <30, not <=30. Original call consumes a single
    // retail rand() when arrival removes the tracked follower index.
    const auto close=h::original_herding_follower_approach(
        starting,{129,100});
    assert(close);
    assert(close->reached_tracked_point);
    assert(close->remove_from_follower_table);
    assert(close->source_rand_calls==1);
    assert(close->updated.x==100);
    assert(close->updated.y==100);
    assert(close->updated.movement_active==1);
    assert(close->updated.animation_frame==2);
    assert(close->updated.animation_frame_countdown==4);

    // Exactly 30 units does not release the member. The first frame
    // moves the original 0x64-byte entity and advances the animation
    // after subtracting five ticks from a countdown of four.
    const auto edge=h::original_herding_follower_approach(
        starting,{130,100});
    assert(edge);
    assert(!edge->reached_tracked_point);
    assert(!edge->remove_from_follower_table);
    assert(edge->source_rand_calls==0);
    assert(edge->updated.x_float>100.0f);
    assert(std::fabs(edge->updated.y_float-100.0f)<0.001f);
    assert(edge->updated.direction==2);
    assert(edge->updated.movement_active==1);
    assert(edge->updated.animation_frame_countdown==100);
    assert(edge->updated.animation_frame==0);
    assert(edge->animation_advanced);

    auto ticking=starting;
    ticking.animation_frame=1;
    ticking.animation_frame_countdown=7;
    const auto not_yet=h::original_herding_follower_approach(
        ticking,{200,100});
    assert(not_yet);
    assert(!not_yet->animation_advanced);
    assert(not_yet->updated.animation_frame==1);
    assert(not_yet->updated.animation_frame_countdown==2);

    const auto north=h::original_herding_follower_approach(
        ticking,{100,0});
    assert(north);
    assert(north->updated.direction==0);
    assert(north->updated.x_float<100.0f); // native 359-degree drift
    assert(north->updated.y_float<100.0f);

    auto invalid=starting;
    invalid.type=static_cast<int>(h::EntityType::GateRight);
    assert(!h::original_herding_follower_approach(invalid,{200,100}));
    assert(starting.x_float==100.0f); // input record never mutated
}
