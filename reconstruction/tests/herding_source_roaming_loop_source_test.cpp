#include "btb/herding_source_roaming_loop.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

namespace h=btb::herding;
namespace r=btb::retail;

h::RetailEntityRecord32 original_test_entity() {
    h::RetailEntityRecord32 e{};
    e.entity_id=1;
    e.type=static_cast<int>(h::EntityType::Sheep);
    e.x=478;
    e.y=471;
    e.x_float=478.0f;
    e.y_float=471.0f;
    e.source_left=0;
    e.source_right=40;
    e.source_top=0;
    e.source_bottom=40;
    e.movement_speed=0.8f;
    return e;
}

int main() {
    const auto entity=original_test_entity();
    const auto anchor=h::original_herding_recovery_anchor(entity);
    // Retail starts from x+(left-right)/2, NOT x+width/2.
    assert(anchor && *anchor==(h::Vec2i{458,451}));
    auto odd=entity;
    odd.source_right=41;
    odd.source_bottom=43;
    assert(h::original_herding_recovery_anchor(odd) ==
           (h::Vec2i{458,450})); // negative signed halves truncate

    // A world whose legal region begins at Y=477. Original seed-one
    // movement needs THREE recovery attempts in the SAME frame:
    //  1: (400,659) -> Y ~473 (outside)
    //  2: (891,766) -> Y ~475 (outside)
    //  3: (400,766) -> Y ~478 (inside)
    const std::vector<h::Vec2i> world{{
        {0,477},{1200,477},{1200,650},{0,650}
    }};
    r::OriginalRetailRandom shared{1};
    h::OriginalRoamingLoop state{.entity=entity};

    auto status=h::resume_original_herding_roaming_loop(
        state,shared,world,1);
    assert(status==h::OriginalRoamingLoopStatus::Pending);
    assert(state.started && !state.finished);
    assert(state.attempts==1 && state.consumed_random_calls==2);
    assert(state.entity.x==477);
    assert(state.entity.y==473);
    assert(state.entity.previous_x==478);
    assert(state.entity.previous_y==471);
    assert(state.entity.movement_speed==0.0f);

    status=h::resume_original_herding_roaming_loop(
        state,shared,world,1);
    assert(status==h::OriginalRoamingLoopStatus::Pending);
    assert(state.attempts==2 && state.consumed_random_calls==4);
    assert(state.entity.x==479);
    assert(state.entity.y==475);
    assert(state.entity.previous_x==477);
    assert(state.entity.previous_y==473);

    status=h::resume_original_herding_roaming_loop(
        state,shared,world,1);
    assert(status==h::OriginalRoamingLoopStatus::Accepted);
    assert(state.finished);
    assert(state.attempts==3 && state.consumed_random_calls==6);
    assert(state.entity.x==479);
    assert(state.entity.y==478);
    assert(state.entity.previous_x==479);
    assert(state.entity.previous_y==475);
    assert(shared.next_rand()==11478); // source rolls advance exactly
    assert(h::resume_original_herding_roaming_loop(
        state,shared,world,1)==h::OriginalRoamingLoopStatus::Accepted);
    assert(state.attempts==3 && state.consumed_random_calls==6);

    // Performing three attempts in one chunk MUST equal the resumed
    // same-frame progression and consume the same original RNG calls.
    r::OriginalRetailRandom continuous_rand{1};
    h::OriginalRoamingLoop continuous{.entity=entity};
    assert(h::resume_original_herding_roaming_loop(
        continuous,continuous_rand,world,3)==
        h::OriginalRoamingLoopStatus::Accepted);
    assert(continuous.entity.x==state.entity.x);
    assert(continuous.entity.y==state.entity.y);
    assert(continuous.entity.x_float==state.entity.x_float);
    assert(continuous.entity.y_float==state.entity.y_float);
    assert(continuous.attempts==state.attempts);
    // The resumed test already consumed an extra seventh rand() above;
    // compare both runs against an independent six-call sequence below.

    r::OriginalRetailRandom exact_six{1};
    for (int i=0;i<6;++i) static_cast<void>(exact_six.next_rand());
    assert(continuous_rand.state()==exact_six.state());

    // If already inside, the source returns without drawing even one
    // new pair of random values.
    auto inside_entity=entity;
    inside_entity.y=500;
    inside_entity.y_float=500.0f;
    h::OriginalRoamingLoop already{.entity=inside_entity};
    r::OriginalRetailRandom unused{1};
    assert(h::resume_original_herding_roaming_loop(
        already,unused,world,2)==
        h::OriginalRoamingLoopStatus::AlreadyInside);
    assert(already.attempts==0 && already.consumed_random_calls==0);
    assert(unused.state()==1);

    // Invalid polygons and nonfinite floats must not drain the
    // global RNG or report a fabricated successful recovery.
    h::OriginalRoamingLoop bad{.entity=entity};
    const std::vector<h::Vec2i> no_vertices{};
    assert(h::resume_original_herding_roaming_loop(
        bad,unused,no_vertices,1)==
        h::OriginalRoamingLoopStatus::InvalidInput);
    assert(unused.state()==1);

    bad={.entity=entity};
    bad.entity.x_float=std::numeric_limits<float>::infinity();
    assert(h::resume_original_herding_roaming_loop(
        bad,unused,world,1)==
        h::OriginalRoamingLoopStatus::InvalidInput);
    assert(unused.state()==1);
}
