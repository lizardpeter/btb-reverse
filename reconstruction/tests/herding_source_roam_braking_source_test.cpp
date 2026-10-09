#include "btb/herding_source_roam_braking.hpp"

#include <cassert>
#include <cmath>

namespace h=btb::herding;

int main() {
    // Source comparison is strictly >0.4; exactly 0.4 goes to 0.1.
    const auto at_limit=h::original_herding_roam_brake_and_animate(
        h::EntityType::Sheep,0.4f,8,50);
    assert(at_limit);
    assert(at_limit->speed==0.1f);
    assert(at_limit->animation_countdown==30);
    assert(at_limit->animation_frame==8);
    assert(!at_limit->advanced_frame);

    const auto high=h::original_herding_roam_brake_and_animate(
        h::EntityType::Sheep,0.9f,9,20);
    assert(high);
    assert(std::fabs(high->speed-0.5f)<0.000001f);
    assert(high->animation_countdown==100);
    assert(high->animation_frame==6);
    assert(high->advanced_frame);

    const auto duck=h::original_herding_roam_brake_and_animate(
        h::EntityType::Duck,0.0f,5,19);
    assert(duck);
    assert(duck->speed==0.1f);
    assert(duck->animation_frame==0);
    assert(duck->animation_countdown==100);

    const auto rabbit=h::original_herding_roam_brake_and_animate(
        h::EntityType::Rabbit,0.39f,6,-5);
    assert(rabbit);
    assert(rabbit->animation_frame==3);
    assert(rabbit->animation_countdown==100);

    const auto not_yet=h::original_herding_roam_brake_and_animate(
        h::EntityType::Rabbit,0.41f,4,21);
    assert(not_yet);
    assert(not_yet->animation_frame==4);
    assert(not_yet->animation_countdown==1);
    assert(!not_yet->advanced_frame);
    assert(not_yet->speed>0);

    assert(!h::original_herding_roam_brake_and_animate(
        h::EntityType::FarmerPickles,0.2f,0,100));
    assert(!h::original_herding_roam_brake_and_animate(
        h::EntityType::Duck,-1.0f,1,100));
}
