#include "btb/herding_source_partial_dispatcher.hpp"

#include <cassert>
#include <cmath>

namespace h=btb::herding;
namespace r=btb::retail;

h::RetailEntityRecord32 animal(std::int32_t index,int x,int y) {
    h::RetailEntityRecord32 e{};
    e.entity_id=index;
    e.type=static_cast<int>(h::EntityType::Sheep);
    e.x=x;e.y=y;e.x_float=static_cast<float>(x);
    e.y_float=static_cast<float>(y);
    e.source_left=0;e.source_top=0;
    e.source_right=40;e.source_bottom=40;
    e.movement_speed=0.5f;
    e.animation_frame_countdown=4;
    e.animation_frame=2;
    return e;
}

int main() {
    h::FollowerList ordinary;
    assert(ordinary.add(3));
    h::CoveredAnimalMotionDispatcher source{1};
    r::OriginalRetailRandom rng{1};

    // 0x416F57 temporary target takes precedence, before either table.
    auto temporary=animal(3,100,100);
    temporary.temporary_target_timer=200;
    temporary.target_x=120;
    temporary.target_y=100;
    const auto temp=source.advance(3,temporary,ordinary,{200,100},rng);
    assert(temp.branch==h::CoveredAnimalBranch::TemporaryTargetCleared);
    assert(temp.entity.temporary_target_timer==0);
    assert(temp.entity.movement_speed==3.0f);
    assert(temp.rand_calls==0);
    assert(rng.state()==1);
    assert(!source.tracked().contains(3));

    // With no temporary target, original ordinary-follower table
    // registers the animal for tracked approach when distance <150.
    auto sheep=animal(3,100,100);
    auto registered=source.advance(3,sheep,ordinary,{200,100},rng);
    assert(registered.branch==
           h::CoveredAnimalBranch::RegisteredTrackedTarget);
    assert(registered.remaining_to_register==0);
    assert(registered.last_target_registered);
    assert(source.tracked().contains(3));
    assert(ordinary.contains(3));
    assert(rng.state()==1); // enrollment burns no rand

    // Tracked table is checked before ordinary list. A distant tracked
    // animal must take the source <30 approach branch, not register
    // again or enter unrelated autonomous free roaming.
    const auto approaching=source.advance(
        3,sheep,ordinary,{200,100},rng);
    assert(approaching.branch==
           h::CoveredAnimalBranch::TrackedTargetApproaching);
    assert(approaching.entity.behavior_state==0);
    assert(approaching.entity.movement_active==1);
    assert(approaching.entity.movement_speed>0.5f);
    assert(approaching.entity.x_float>100);
    assert(approaching.rand_calls==0);
    assert(rng.state()==1);

    // At distance 20<30 the tracked entry (0x50AF78), not the
    // ordinary followers (0x50AF14), is removed. The source writes
    // behavior +0x50=1 and burns exactly one otherwise unused rand.
    auto close=animal(3,180,100);
    const auto arrived=source.advance(3,close,ordinary,{200,100},rng);
    assert(arrived.branch==
           h::CoveredAnimalBranch::TrackedTargetArrived);
    assert(arrived.entity.behavior_state==1);
    assert(arrived.entity.movement_active==0);
    assert(arrived.rand_calls==1);
    assert(arrived.rand_state_before==1);
    assert(!source.tracked().contains(3));
    assert(ordinary.contains(3));
    assert(rng.next_rand()==18467); // first rand 41 was discarded

    // Next original update must hand state 1 to the source home-route
    // dispatcher. Do not re-register it as an ordinary follower.
    const auto next=source.advance(
        3,arrived.entity,ordinary,{200,100},rng);
    assert(next.branch==h::CoveredAnimalBranch::InactiveBehaviorState);
    assert(!source.tracked().contains(3));

    // Untracked/unfollowing branch is NOT reconstructed; never
    // falsely report a completed roaming physics update.
    auto lone=animal(7,100,100);
    const auto unknown=source.advance(
        7,lone,ordinary,{200,100},rng);
    assert(unknown.branch==
           h::CoveredAnimalBranch::UncoveredOriginalBranch);
    assert(!unknown.covered());

    // A wrong record index is an invalid source event, not
    // permission to write into an unrelated original animal.
    const auto malformed=source.advance(
        8,lone,ordinary,{200,100},rng);
    assert(malformed.branch==h::CoveredAnimalBranch::InvalidSourceState);
    assert(!malformed.covered());
}
