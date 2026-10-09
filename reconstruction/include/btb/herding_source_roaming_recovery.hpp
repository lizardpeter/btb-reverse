#pragma once

#include "btb/herding_source_recovery_target.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"
#include "btb/retail_point_in_polygon.hpp"

#include <optional>
#include <vector>

namespace btb::herding {

// Source 0x417FFE..0x4180FB performs ONE roaming/exclusion recovery
// attempt. The previous code (0x417FB2..0x417FFD) restores the current
// integer top-left and computes actor's CENTER using its original sprite
// cell dimensions. This center must be supplied explicitly. In
// particular it must NOT be confused with the entity's top-left.
//
// The source then:
//  - consumes two separate rand()%4 values for X and Y
//  - computes native 0x415D70 heading from actor center to that point
//  - applies native FSIN/FCOS movement with literal magnitude 3.0
//  - truncates float X/Y to signed int and saves facing (heading+22)/45
//  - sets speed +0x4C to zero
//  - checks the transformed group-0 polygon with the original
//    0=inside / 1=outside PointInPolygon helper
// An outside result repeats from 0x417FB2 with ANOTHER pair of RNG
// calls. That retry loop remains caller-owned to preserve x87 and
// original frame-specific base-position updates.
struct RetailRoamingRecoveryAttempt {
    RetailEntityRecord32 moved{};
    Vec2i randomly_selected_target{};
    std::int32_t native_heading{};
    bool accepted_inside_polygon{};
    int consumed_rand_calls{2};
};

[[nodiscard]] inline std::optional<RetailRoamingRecoveryAttempt>
original_herding_roaming_recovery_attempt(
    const RetailEntityRecord32& entity,
    Vec2i original_actor_center,
    int random_x_mod4,
    int random_y_mod4,
    const std::vector<Vec2i>& transformed_navigation_polygon) noexcept {

    const auto target=original_herding_recovery_target(
        random_x_mod4,random_y_mod4);
    if (!target) return std::nullopt;

    const auto heading=original_herding_integer_heading(
        original_actor_center.x,original_actor_center.y,
        target->point.x,target->point.y);
    if (!heading) return std::nullopt;

    const auto motion=original_herding_steering_step(
        entity.x_float,entity.y_float,*heading,
        kRetailHerdingFollowerStep,false);
    if (!motion) return std::nullopt;

    const auto inside=retail_geometry::original_polygon_contains(
        transformed_navigation_polygon,
        motion->rounded_x,motion->rounded_y);
    if (!inside) return std::nullopt;

    auto candidate=entity;
    candidate.x_float=motion->x;
    candidate.y_float=motion->y;
    candidate.x=motion->rounded_x;
    candidate.y=motion->rounded_y;
    candidate.direction=motion->facing_index;
    candidate.movement_speed=0.0f;

    return RetailRoamingRecoveryAttempt{
        candidate,target->point,*heading,*inside,
        target->rand_calls
    };
}

// Production-facing recovery attempt: consume the actual shared
// MSVC rand sequence instead of asking the caller to invent/replay
// the two modulus results. The explicit-roll overload remains useful
// for exact differential fixture playback.
[[nodiscard]] inline std::optional<RetailRoamingRecoveryAttempt>
original_herding_roaming_recovery_attempt(
    const RetailEntityRecord32& entity,
    Vec2i original_actor_center,
    retail::OriginalRetailRandom& shared_rng,
    const std::vector<Vec2i>& transformed_navigation_polygon) noexcept {

    // Refuse obviously invalid external input before burning the
    // original process-global random sequence.
    if (!std::isfinite(entity.x_float) ||
        !std::isfinite(entity.y_float) ||
        transformed_navigation_polygon.empty()) {
        return std::nullopt;
    }
    const auto random=next_original_herding_recovery_target(shared_rng);
    return original_herding_roaming_recovery_attempt(
        entity,original_actor_center,
        random.selected_x_index,random.selected_y_index,
        transformed_navigation_polygon);
}

} // namespace btb::herding
