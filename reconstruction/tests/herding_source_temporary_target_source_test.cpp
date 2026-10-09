#include "btb/herding_source_temporary_target.hpp"

#include <cassert>
#include <cmath>
#include <limits>

namespace h=btb::herding;

h::RetailEntityRecord32 original_fixture() {
    h::RetailEntityRecord32 animal{};
    animal.entity_id=4;
    animal.type=static_cast<int>(h::EntityType::Rabbit);
    animal.x=200;animal.y=200;
    animal.x_float=200.0f;animal.y_float=200.0f;
    animal.source_left=0;animal.source_top=0;
    animal.source_right=40;animal.source_bottom=40;
    animal.temporary_target_timer=200;
    animal.movement_speed=0.1f;
    animal.animation_frame=3;
    animal.animation_frame_countdown=33;
    return animal;
}

int main() {
    auto untouched=original_fixture();
    untouched.temporary_target_timer=0;
    const auto bypass=h::original_herding_temporary_target_update(
        untouched);
    assert(bypass);
    assert(!bypass->branch_taken);
    assert(bypass->continue_normal_update);
    assert(bypass->moved.x==200 && bypass->moved.y==200);

    // Original branch computes actor steering origin as 220,220 for
    // a 40x40 cell at 200,200, then moves 3 units in the direction
    // of the provided temporary target. A nearby target must clear
    // the timer and EARLY RETURN instead of running free roaming.
    auto near=original_fixture();
    near.target_x=240;near.target_y=200;
    auto near_step=h::original_herding_temporary_target_update(near);
    assert(near_step);
    assert(near_step->branch_taken);
    assert(near_step->early_return);
    assert(!near_step->continue_normal_update);
    assert(near_step->moved.temporary_target_timer==0);
    assert(near_step->moved.previous_x==200);
    assert(near_step->moved.previous_y==200);
    assert(near_step->moved.movement_speed==3.0f);
    assert(near_step->moved.x_float>200.0f);
    assert(near_step->moved.animation_frame==3);
    assert(near_step->moved.animation_frame_countdown==33);

    // Distant target keeps temporary target active and continues into
    // the other source state update return path after a 3-unit step.
    auto far=original_fixture();
    far.target_x=600;far.target_y=600;
    const auto far_step=h::original_herding_temporary_target_update(far);
    assert(far_step);
    assert(far_step->branch_taken);
    assert(!far_step->early_return);
    assert(far_step->continue_normal_update);
    assert(far_step->moved.temporary_target_timer==200);
    assert(far_step->moved.movement_speed==3.0f);
    assert(far_step->moved.x_float>200.0f);
    assert(far_step->moved.y_float>200.0f);

    // The arrival comparison is strictly less than 120; this helper
    // does not use the normal follower 30-unit threshold or home 10.
    auto exact=original_fixture();
    exact.movement_speed=0.0f;
    exact.target_x=exact.x;
    exact.target_y=exact.y+120;
    const auto edge=h::original_herding_temporary_target_update(exact);
    assert(edge);
    assert(edge->branch_taken);
    assert(edge->moved.y_float>200.0f);
    assert(edge->moved.temporary_target_timer==0); // now within 120

    auto invalid=original_fixture();
    invalid.type=static_cast<int>(h::EntityType::GateLeft);
    assert(!h::original_herding_temporary_target_update(invalid));
    invalid=original_fixture();
    invalid.x_float=std::numeric_limits<float>::infinity();
    assert(!h::original_herding_temporary_target_update(invalid));
}
