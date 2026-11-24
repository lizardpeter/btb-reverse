#include "btb/herding_source_scruffty_patrol.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

namespace h=btb::herding;

int main() {
    const std::vector<h::Vec2i> patrol{
        {100,200},{200,200},{200,300},{100,300},{100,200}
    };
    assert(!h::original_scruffty_initial_record(0,11,patrol));
    assert(!h::original_scruffty_initial_record(3,11,patrol));
    assert(!h::original_scruffty_initial_record(1,-1,patrol));
    assert(!h::original_scruffty_initial_record(2,11,{}));
    const auto created=h::original_scruffty_initial_record(
        1,11,patrol,0x12345678u);
    assert(created);
    assert(created->entity_id==11);
    assert(created->entity_type()==h::EntityType::Scruffty);
    assert(created->x==100 && created->y==200);
    assert(created->x_float==100.0f && created->y_float==200.0f);
    assert(created->source_left==0 && created->source_top==0);
    assert(created->source_right==101 && created->source_bottom==116);
    assert(created->surface_ptr32==0x12345678u);

    // PE .data[0x443B20] starts at one. A move only happens if the
    // sprite frame is 3 or 4 and the original movement latch is set.
    h::RetailScrufftyPatrolState state{};
    assert(state.waypoint_index==0 && state.movement_frame_latch);
    auto dog=*created;
    dog.movement_speed=1.0f;
    dog.animation_frame=3;
    dog.animation_frame_countdown=12;
    dog.movement_active=1; // outer activity resets it to zero

    // Start on waypoint 0: the source halves speed, increments the
    // waypoint index BEFORE calculating direction, and steps toward
    // waypoint 1 (East). The gated movement magnitude is 10*0.5.
    const auto first=h::original_scruffty_patrol_step(dog,patrol,state);
    assert(first);
    assert(first->reached_waypoint);
    assert(first->state.waypoint_index==1);
    assert(first->waypoint==(h::Vec2i{200,200}));
    assert(first->native_heading==90);
    assert(first->animal.direction==2);
    assert(first->movement_applied);
    assert(first->animal.movement_active==1);
    assert(first->animal.x==105 && first->animal.y==200);
    assert(first->animal.x_float==105.0f);
    assert(std::fabs(first->animal.movement_speed-0.51f)<0.00001f);
    assert(first->animal.animation_frame_countdown==7);
    assert(first->animal.animation_frame==3);
    assert(!first->state.movement_frame_latch);

    // With latch cleared, speed still ramps but position does not
    // change. The game waits for the 5-tick animation countdown.
    const auto second=h::original_scruffty_patrol_step(
        first->animal,patrol,first->state);
    assert(second);
    assert(!second->reached_waypoint && !second->movement_applied);
    assert(second->animal.x==105);
    assert(second->animal.movement_active==0);
    assert(second->animal.animation_frame_countdown==2);
    assert(std::fabs(second->animal.movement_speed-0.52f)<0.00001f);
    assert(!second->state.movement_frame_latch);

    const auto third=h::original_scruffty_patrol_step(
        second->animal,patrol,second->state);
    assert(third);
    assert(!third->movement_applied);
    assert(third->animation_advanced);
    assert(third->animal.animation_frame==4);
    assert(third->animal.animation_frame_countdown==25);
    assert(third->state.movement_frame_latch);

    // The frame-4 latch permits a second actual movement step.
    const auto fourth=h::original_scruffty_patrol_step(
        third->animal,patrol,third->state);
    assert(fourth);
    assert(fourth->movement_applied);
    assert(fourth->animal.x>third->animal.x);
    assert(!fourth->state.movement_frame_latch);
    assert(fourth->animal.animation_frame==4);

    // The native threshold is strictly <1.6, and a waypoint is
    // reached only at distance <10 rather than <=10.
    auto boundary=dog;
    boundary.x=110;boundary.y=200;
    boundary.x_float=110.0f;boundary.y_float=200.0f;
    boundary.animation_frame=0;
    boundary.movement_speed=1.6f;
    const auto exact_ten=h::original_scruffty_patrol_step(
        boundary,patrol,{0,false});
    assert(exact_ten);
    assert(!exact_ten->reached_waypoint);
    assert(exact_ten->state.waypoint_index==0);
    assert(exact_ten->animal.movement_speed==1.6f);
    assert(!exact_ten->movement_applied);
    assert(exact_ten->animal.movement_active==0);

    // Reaching last waypoint wraps the global index to zero.
    const auto wrapped=h::original_scruffty_patrol_step(
        dog,patrol,{4,false});
    assert(wrapped);
    assert(wrapped->reached_waypoint);
    assert(wrapped->state.waypoint_index==0);
    assert(!wrapped->movement_applied);

    auto invalid=dog;
    invalid.type=static_cast<int>(h::EntityType::Sheep);
    assert(!h::original_scruffty_patrol_step(invalid,patrol,{}));
    assert(!h::original_scruffty_patrol_step(dog,{},{}));
    assert(!h::original_scruffty_patrol_step(
        dog,patrol,{patrol.size(),true}));
    invalid=dog;
    invalid.movement_speed=std::numeric_limits<float>::infinity();
    assert(!h::original_scruffty_patrol_step(invalid,patrol,{}));
}
