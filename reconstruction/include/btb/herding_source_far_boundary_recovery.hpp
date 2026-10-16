#pragma once

#include "btb/herding_source_far_roam.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// UpdateHerdingActivity original 0x419133..0x4191B0: after the
// normal 0.15/0.30-step untracked roam produces a position OUTSIDE
// herd.txt group 0, the x86 code restores PREVIOUS INTEGER X/Y
// (source +0x14/+0x18), but continues from the CANDIDATE FLOAT X/Y.
// It then uses original saved sprite geometry and the one-point
// group-3 target 0x50AF74/0x50AF70 to steer 3.0 source units,
// setting original velocity back to zero.
//
// Unlike the distinct UpdateHerdingAnimal group-2 random retry loop,
// THIS branch uses one fixed GROUP THREE point and NO rand() calls.
struct RetailFarBoundaryRecoveryStep {
    RetailEntityRecord32 animal{};
    Vec2i steering_anchor{};
    Vec2i original_group3_target{};
    std::int32_t heading_degrees{};
};

[[nodiscard]] inline std::optional<RetailFarBoundaryRecoveryStep>
original_far_untracked_animal_boundary_recovery(
    const RetailEntityRecord32& before,
    const RetailEntityRecord32& out_of_group0_candidate,
    Vec2i original_group3_target) noexcept {
    if(!is_herd_animal(before.entity_type()) ||
       before.behavior_state!=0 ||
       before.temporary_target_timer>0 ||
       before.movement_active!=0 ||
       before.entity_id!=out_of_group0_candidate.entity_id ||
       before.type!=out_of_group0_candidate.type ||
       out_of_group0_candidate.previous_x!=before.x ||
       out_of_group0_candidate.previous_y!=before.y ||
       before.source_right<before.source_left ||
       before.source_bottom<before.source_top)
        return std::nullopt;

    // The original C++ array base for the entity loop is +0x04:
    // piVar8[0xc] = sprite-left; [0xe] = sprite-right.
    // piVar8[0xf] = sprite-bottom; [0xd] = sprite-top.
    // X is NEGATIVE half width, Y is POSITIVE half height.
    const std::int64_t ax=static_cast<std::int64_t>(before.x)+
        (static_cast<std::int64_t>(before.source_left)-
         before.source_right)/2;
    const std::int64_t ay=static_cast<std::int64_t>(before.y)+
        (static_cast<std::int64_t>(before.source_bottom)-
         before.source_top)/2;
    if(ax<std::numeric_limits<std::int32_t>::min() ||
       ay<std::numeric_limits<std::int32_t>::min() ||
       ax>std::numeric_limits<std::int32_t>::max() ||
       ay>std::numeric_limits<std::int32_t>::max())
        return std::nullopt;
    const Vec2i anchor{static_cast<int>(ax),static_cast<int>(ay)};
    const auto heading=original_herding_integer_heading(
        anchor.x,anchor.y,original_group3_target.x,
        original_group3_target.y);
    if(!heading)return std::nullopt;
    const auto motion=original_herding_steering_step(
        out_of_group0_candidate.x_float,
        out_of_group0_candidate.y_float,
        *heading,kRetailHerdingFollowerStep,false);
    if(!motion)return std::nullopt;

    auto moved=out_of_group0_candidate;
    moved.x=motion->rounded_x;
    moved.y=motion->rounded_y;
    moved.x_float=motion->x;
    moved.y_float=motion->y;
    moved.direction=motion->facing_index;
    moved.movement_speed=0.0f;
    moved.movement_active=0;
    // Keep the pre-frame coordinates at +0x14/+0x18. Next frame
    // original UpdateHerdingActivity will refresh these independently.
    return RetailFarBoundaryRecoveryStep{
        moved,anchor,original_group3_target,*heading};
}

} // namespace btb::herding
