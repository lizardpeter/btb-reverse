#include "btb/herding_source_steering.hpp"

#include <cassert>
#include <cmath>
#include <limits>

namespace h=btb::herding;

bool close(float a,float b,float tolerance=0.0002f) {
    return std::fabs(a-b)<=tolerance;
}

int main() {
    constexpr auto start_x=478.0f;
    constexpr auto start_y=471.0f;

    const auto north=h::original_herding_steering_step(
        start_x,start_y,0,3.0f,false);
    assert(north);
    assert(close(north->x,478));
    assert(close(north->y,468));
    assert(north->rounded_x==478 && north->rounded_y==468);
    assert(north->facing_index==0);
    assert(north->next_speed==3.0f);

    const auto east=h::original_herding_steering_step(
        start_x,start_y,90,3.0f,false);
    assert(east);
    assert(close(east->x,481));
    assert(close(east->y,471));
    assert(east->facing_index==2);
    assert(east->next_speed==3.0f);

    const auto south=h::original_herding_steering_step(
        start_x,start_y,180,3.0f,false);
    assert(south);
    assert(close(south->x,478));
    assert(close(south->y,474));
    assert(south->facing_index==4);

    const auto west=h::original_herding_steering_step(
        start_x,start_y,270,3.0f,false);
    assert(west);
    assert(close(west->x,475));
    assert(close(west->y,471));
    assert(west->facing_index==6);

    const auto wrap=h::original_herding_steering_step(
        start_x,start_y,360,3.0f,false);
    assert(wrap && wrap->facing_index==0);

    const auto just_before=h::original_herding_steering_step(
        10.5f,20.5f,22,0.79f,true);
    assert(just_before && just_before->facing_index==0);
    assert(close(just_before->next_speed,0.8f));

    const auto right_after=h::original_herding_steering_step(
        10.5f,20.5f,23,0.8f,true);
    assert(right_after && right_after->facing_index==1);
    assert(right_after->next_speed==0.8f);

    const auto stationary=h::original_herding_steering_step(
        10.5f,20.5f,90,0.0f,false);
    assert(stationary && stationary->x==10.5f &&
           stationary->y==20.5f);
    assert(stationary->rounded_x==10 &&
           stationary->rounded_y==20);

    const auto negative=h::original_herding_steering_step(
        -10.5f,-20.5f,90,0.0f,false);
    assert(negative && negative->rounded_x==-10 &&
           negative->rounded_y==-20); // x87 truncation, not floor

    assert(!h::original_herding_steering_step(
        start_x,start_y,0,-1.0f,false));
    assert(!h::original_herding_steering_step(
        std::numeric_limits<float>::infinity(),
        start_y,0,1.0f,false));
}
