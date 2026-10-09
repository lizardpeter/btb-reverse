#include "btb/herding_source_tracked_targets.hpp"

#include <cassert>

namespace h=btb::herding;

int main() {
    h::FollowerList ordinary;
    h::RetailTrackedTargetSlots tracked;
    assert(ordinary.add(3));
    assert(ordinary.add(6));
    assert(tracked.route_for(3,ordinary)==
           h::RetailTrackedRoute::OrdinaryFollower);
    assert(tracked.route_for(9,ordinary)==
           h::RetailTrackedRoute::UntrackedFreeRoam);

    int remaining=2;
    // 150 exactly is not admitted; native 0x43B428 comparison is <150.
    auto result=tracked.admit_from_follower(
        3,{150,0},{0,0},ordinary,remaining);
    assert(result.status==
           h::RetailTrackedAdmissionStatus::NotWithinThreshold);
    assert(remaining==2 && !tracked.contains(3));

    result=tracked.admit_from_follower(
        3,{149,0},{0,0},ordinary,remaining);
    assert(result.status==h::RetailTrackedAdmissionStatus::Inserted);
    assert(result.slot==0);
    assert(remaining==1);
    assert(!result.reached_registration_target);
    assert(tracked.route_for(3,ordinary)==
           h::RetailTrackedRoute::TrackedTargetApproach);
    assert(ordinary.contains(3)); // original tables are independent

    result=tracked.admit_from_follower(
        3,{0,0},{0,0},ordinary,remaining);
    assert(result.status==
           h::RetailTrackedAdmissionStatus::AlreadyTracked);
    assert(remaining==1);

    result=tracked.admit_from_follower(
        6,{8,6},{0,0},ordinary,remaining);
    assert(result.status==h::RetailTrackedAdmissionStatus::Inserted);
    assert(result.slot==1);
    assert(remaining==0);
    assert(result.reached_registration_target);

    assert(!tracked.remove(99));
    assert(tracked.remove(3));
    assert(!tracked.contains(3));
    assert(ordinary.contains(3));
    assert(tracked.route_for(3,ordinary)==
           h::RetailTrackedRoute::OrdinaryFollower);
    assert(!tracked.remove(3));

    h::RetailTrackedTargetSlots empty;
    int one=1;
    result=empty.admit_from_follower(
        7,{0,0},{0,0},ordinary,one);
    assert(result.status==
           h::RetailTrackedAdmissionStatus::MissingOrdinaryFollower);
    assert(one==1);

    // The original 20-slot array neither expands to an STL vector nor
    // allows a new entry to overwrite an existing tracked entity.
    for (int n=0;n<20;++n) {
        empty.slot[static_cast<std::size_t>(n)]=100+n;
    }
    result=empty.admit_from_follower(
        3,{0,0},{0,0},ordinary,one);
    assert(result.status==
           h::RetailTrackedAdmissionStatus::TableFull);
    assert(one==1);
}
