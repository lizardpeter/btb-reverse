#pragma once

#include "btb/herding_runtime.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// Original UpdateHerdingAnimal positive temporary-target branch:
// 0x00416F57..0x00417094. Temporary target lives in original
// entity +0x5C/+0x60 and positive timer in +0x58.
//
// Unlike the roaming recovery loop, this branch computes its steering
// origin with POSITIVE half-cell dimensions using original
// abs(left-right)/2 + current integer x, similarly for y.
// Movement magnitude is original 3.0 at 0x43B430.
// Once moved, it compares point distance to ORIGINAL target against
// 120.0f at 0x43B42C. A strict <120 clears the temporary-target
// timer and RETURNS through the immediate 0x41708B epilogue.
// A distant result RETURNS through the shared 0x418101 epilogue.
// Neither outcome dispatches another AI branch this update.
//
// The caller must dispatch this before the ordinary entity behavior
// state branch, and MUST NOT run another animal branch after the early
// return. Native FSIN/FCOS/FPATAN rounding is not yet bit-perfect.
inline constexpr float kRetailTemporaryTargetCompletionRadius=120.0f;

struct RetailTemporaryTargetUpdate {
    RetailEntityRecord32 moved{};
    bool branch_taken{};
    bool early_return{};
    bool continue_normal_update{};
    std::int32_t heading_degrees{};
};

[[nodiscard]] inline std::optional<RetailTemporaryTargetUpdate>
original_herding_temporary_target_update(
    const RetailEntityRecord32& original) noexcept {

    RetailTemporaryTargetUpdate out{};
    out.moved=original;
    if (original.temporary_target_timer<=0) {
        out.continue_normal_update=true;
        return out;
    }
    if (!is_herd_animal(original.entity_type())) {
        return std::nullopt;
    }

    const auto width=static_cast<std::int64_t>(
        original.source_right)-original.source_left;
    const auto height=static_cast<std::int64_t>(
        original.source_bottom)-original.source_top;
    const auto abs_width=width>=0?width:-width;
    const auto abs_height=height>=0?height:-height;
    const auto anchor_x=static_cast<std::int64_t>(original.x)+abs_width/2;
    const auto anchor_y=static_cast<std::int64_t>(original.y)+abs_height/2;
    if (anchor_x<std::numeric_limits<std::int32_t>::min() ||
        anchor_x>std::numeric_limits<std::int32_t>::max() ||
        anchor_y<std::numeric_limits<std::int32_t>::min() ||
        anchor_y>std::numeric_limits<std::int32_t>::max()) {
        return std::nullopt;
    }

    const auto heading=original_herding_integer_heading(
        static_cast<std::int32_t>(anchor_x),
        static_cast<std::int32_t>(anchor_y),
        original.target_x,original.target_y);
    if (!heading) return std::nullopt;
    const auto motion=original_herding_steering_step(
        original.x_float,original.y_float,
        *heading,kRetailHerdingFollowerStep,false);
    if (!motion) return std::nullopt;

    out.branch_taken=true;
    out.heading_degrees=*heading;
    out.moved.previous_x=original.x;
    out.moved.previous_y=original.y;
    out.moved.x_float=motion->x;
    out.moved.y_float=motion->y;
    out.moved.x=motion->rounded_x;
    out.moved.y=motion->rounded_y;
    out.moved.direction=motion->facing_index;
    out.moved.movement_speed=kRetailHerdingFollowerStep;

    // Original 0x415D30 computes the x87 square root of the sum of
    // squared integer distances. For strict <120 the integer-domain
    // equivalent is squared distance <14400. It deliberately tests
    // the NEW integer position, not the pre-move actor anchor.
    const auto dx=static_cast<std::int64_t>(out.moved.x)-original.target_x;
    const auto dy=static_cast<std::int64_t>(out.moved.y)-original.target_y;
    if (dx>-120 && dx<120 && dy>-120 && dy<120 &&
        dx*dx+dy*dy<14400) {
        out.moved.temporary_target_timer=0;
        out.early_return=true;
        out.continue_normal_update=false;
    } else {
        // x86 0x41707B branches directly to the shared 0x418101
        // function epilogue, not to the ordinary AI dispatcher.
        out.continue_normal_update=false;
    }
    return out;
}

} // namespace btb::herding
