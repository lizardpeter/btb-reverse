#include "btb/herding_source_roaming_recovery.hpp"

#include <cassert>
#include <cmath>
#include <limits>
#include <set>
#include <utility>
#include <vector>

namespace h=btb::herding;

int main() {
    // X and Y are selected with INDEPENDENT retail rand()%4 calls.
    // Verify all 16 combinations are distinct source-supported targets.
    std::set<std::pair<int,int>> observed{};
    for (int x=0;x<4;++x) {
        for (int y=0;y<4;++y) {
            const auto target=h::original_herding_recovery_target(x,y);
            assert(target);
            assert(target->rand_calls==2);
            assert(target->point.x==h::kRetailRoamingRecoveryTargets[x].x);
            assert(target->point.y==h::kRetailRoamingRecoveryTargets[y].y);
            observed.insert({target->point.x,target->point.y});
        }
    }
    assert(observed.size()==16);
    assert(h::original_herding_recovery_target(0,3)->point ==
           (h::Vec2i{100,659}));
    assert(h::original_herding_recovery_target(3,0)->point ==
           (h::Vec2i{1128,766}));
    assert(!h::original_herding_recovery_target(-1,1));
    assert(!h::original_herding_recovery_target(1,4));

    const std::vector<h::Vec2i> original_navigation{{
        {-20,-1},{138,-1},{289,179},{397,163},
        {466,199},{902,81},{902,66},{902,135},
        {1022,249},{1193,305},{1193,800},{-20,800}
    }};

    h::RetailEntityRecord32 animal{};
    animal.entity_id=1;
    animal.type=static_cast<int>(h::EntityType::Sheep);
    animal.x=478; animal.y=471;
    animal.x_float=478.0f;
    animal.y_float=471.0f;
    animal.direction=0;
    animal.movement_speed=0.8f;

    // The source passes a sprite-center integer actor position to
    // 0x415D70. It does not pass the raw top-left. With valid center
    // and the pair (100,400) this is one 3-unit recovery step.
    const auto attempt=h::original_herding_roaming_recovery_attempt(
        animal,{498,491},0,1,original_navigation);
    assert(attempt);
    assert(attempt->randomly_selected_target ==
           (h::Vec2i{100,400}));
    assert(attempt->consumed_rand_calls==2);
    assert(attempt->moved.movement_speed==0.0f);
    assert(attempt->moved.x_float<478.0f);
    assert(attempt->moved.x==static_cast<int>(attempt->moved.x_float));
    assert(attempt->moved.y==static_cast<int>(attempt->moved.y_float));
    assert(attempt->accepted_inside_polygon);
    assert(animal.x==478 && animal.movement_speed==0.8f);

    // We must preserve polygon rejection rather than normalizing the
    // next attempt into a rectangle clamp or discarding the RNG draw.
    const std::vector<h::Vec2i> far_polygon{{
        {1000,1000},{1100,1000},{1100,1100},{1000,1100}
    }};
    const auto rejected=h::original_herding_roaming_recovery_attempt(
        animal,{498,491},3,2,far_polygon);
    assert(rejected);
    assert(!rejected->accepted_inside_polygon);
    assert(rejected->consumed_rand_calls==2);

    assert(!h::original_herding_roaming_recovery_attempt(
        animal,{498,491},4,2,original_navigation));

    // The process-global original rand stream performs X then Y.
    // srand(1) -> first pair (41%4=1,18467%4=3), followed by
    // (6334%4=2,26500%4=0). This is not a per-animal PRNG.
    btb::retail::OriginalRetailRandom shared{1};
    const auto automatic=h::original_herding_roaming_recovery_attempt(
        animal,{498,491},shared,original_navigation);
    assert(automatic);
    assert(automatic->randomly_selected_target ==
           (h::Vec2i{400,659}));
    assert(automatic->consumed_rand_calls==2);
    assert(shared.next_rand()==6334);

    shared.seed(1);
    auto nonexistent=animal;
    nonexistent.x_float=std::numeric_limits<float>::infinity();
    const auto old_random=shared.state();
    assert(!h::original_herding_roaming_recovery_attempt(
        nonexistent,{498,491},shared,original_navigation));
    assert(shared.state()==old_random);
}
