#pragma once

#include "btb/herding_runtime.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

namespace btb::herding {

// Original UpdateHerdingActivity, Scruffty/type-7 branch:
// 0x418F23..0x41908D. This is not a guessed generic pursuit agent.
//
// Retail globals:
//   0x510628: five herd.txt group-1 patrol waypoints (X,Y pairs)
//   0x510768: original point count
//   0x51076C: current patrol waypoint index
//   0x443B20: movement-animation latch, shared activity state
//
// Executable constants:
//   0x43B378: 10.0f (strict waypoint arrival distance)
//   0x43B338: 0.5 (double; speed multiplier on arrival)
//   0x43B440: 1.6f (strict upper speed ramp test)
//   0x43B434: 0.01f (speed ramp per original update)
//   0x43B378: 10.0f (movement magnitude multiplier)
//   countdown: subtract 5, reload 25, animation wrap at 7
inline constexpr float kRetailScrufftyWaypointRadius=10.0f;
inline constexpr double kRetailScrufftyArrivalSpeedFactor=0.5;
inline constexpr float kRetailScrufftySpeedThreshold=1.6f;
inline constexpr float kRetailScrufftySpeedGain=0.01f;
inline constexpr float kRetailScrufftyMovementMagnitude=10.0f;
inline constexpr int kRetailScrufftyAnimationTicks=5;
inline constexpr int kRetailScrufftyAnimationReload=25;
inline constexpr int kRetailScrufftyFrameCount=7;
static_assert(std::bit_cast<std::uint32_t>(
    kRetailScrufftySpeedThreshold)==0x3FCCCCCDu);

struct RetailScrufftyPatrolState {
    std::size_t waypoint_index{};
    bool movement_frame_latch{}; // global 0x443B20, default data word zero
};

struct RetailScrufftyPatrolStep {
    RetailEntityRecord32 animal{};
    RetailScrufftyPatrolState state{};
    Vec2i waypoint{};
    int native_heading{};
    bool reached_waypoint{};
    bool movement_applied{};
    bool animation_advanced{};
};

[[nodiscard]] inline std::optional<RetailScrufftyPatrolStep>
original_scruffty_patrol_step(
    const RetailEntityRecord32& scruffty,
    const std::vector<Vec2i>& original_group1_waypoints,
    RetailScrufftyPatrolState source_state) noexcept {

    if (scruffty.entity_type()!=EntityType::Scruffty ||
        original_group1_waypoints.empty() ||
        source_state.waypoint_index>=original_group1_waypoints.size() ||
        !std::isfinite(scruffty.x_float) ||
        !std::isfinite(scruffty.y_float) ||
        !std::isfinite(scruffty.movement_speed) ||
        scruffty.movement_speed<0 ||
        scruffty.animation_frame<0 ||
        scruffty.animation_frame>=kRetailScrufftyFrameCount ||
        scruffty.animation_frame_countdown<
            std::numeric_limits<int>::min()+kRetailScrufftyAnimationTicks) {
        return std::nullopt;
    }

    RetailScrufftyPatrolStep result{};
    result.animal=scruffty;
    result.state=source_state;
    auto target=original_group1_waypoints[result.state.waypoint_index];

    // 0x418F4A: native 0x415D30 sqrt(integer dx^2+dy^2).
    // In the original map domain, squared integer distance <100
    // has the same strict boundary without host sqrt differences.
    const std::int64_t dx=static_cast<std::int64_t>(scruffty.x)-target.x;
    const std::int64_t dy=static_cast<std::int64_t>(scruffty.y)-target.y;
    if (dx>-10 && dx<10 && dy>-10 && dy<10 && dx*dx+dy*dy<100) {
        result.reached_waypoint=true;
        // 0x418F5F: speed *= original double 0.5, then increment and
        // wrap the patrol index BEFORE selecting movement heading.
        result.animal.movement_speed=static_cast<float>(
            static_cast<double>(result.animal.movement_speed) *
            kRetailScrufftyArrivalSpeedFactor);
        ++result.state.waypoint_index;
        if (result.state.waypoint_index>=original_group1_waypoints.size()) {
            result.state.waypoint_index=0;
        }
        target=original_group1_waypoints[result.state.waypoint_index];
    }

    result.waypoint=target;
    const auto heading=original_herding_integer_heading(
        result.animal.x,result.animal.y,target.x,target.y);
    if (!heading) return std::nullopt;
    result.native_heading=*heading;
    const bool movement_window=
        result.state.movement_frame_latch &&
        (result.animal.animation_frame==3 ||
         result.animal.animation_frame==4);

    const auto movement=original_herding_steering_step(
        result.animal.x_float,result.animal.y_float,*heading,
        movement_window ?
            result.animal.movement_speed*kRetailScrufftyMovementMagnitude
            : 0.0f,
        false);
    if (!movement) return std::nullopt;
    // 0x418FC8 writes facing every frame even if movement is gated.
    result.animal.direction=movement->facing_index;

    if (movement_window) {
        result.movement_applied=true;
        result.animal.x_float=movement->x;
        result.animal.y_float=movement->y;
        result.animal.x=movement->rounded_x;
        result.animal.y=movement->rounded_y;
        result.animal.movement_active=1;
        // 0x419041 clears 0x443B20 after the gated step.
        result.state.movement_frame_latch=false;
    }

    // 0x419047..0x419063: ramp for values STRICTLY below 1.6,
    // regardless of whether the animation permitted translation.
    if (result.animal.movement_speed<kRetailScrufftySpeedThreshold) {
        result.animal.movement_speed=static_cast<float>(
            result.animal.movement_speed+kRetailScrufftySpeedGain);
    }
    result.animal.animation_frame_countdown-=kRetailScrufftyAnimationTicks;
    if (result.animal.animation_frame_countdown<=0) {
        result.animation_advanced=true;
        result.animal.animation_frame_countdown=kRetailScrufftyAnimationReload;
        ++result.animal.animation_frame;
        if (result.animal.animation_frame>=kRetailScrufftyFrameCount) {
            result.animal.animation_frame=0;
        }
        // 0x41907C sets the activity-global movement latch to 1
        // whenever the sprite animation advances.
        result.state.movement_frame_latch=true;
    }
    return result;
}

// Original initializer 0x41A2BB..0x41A331 creates exactly one type-7
// entity for Medium/Hard. It takes the FIRST group-1 waypoint as its
// original integer and float position. Cell 0 is 101 x 116, source
// rectangle starts (0,0), and the live 32-bit DD surface pointer is
// supplied by native asset loader. This helper does not invent a
// pointer, seed, or the other generic constructor fields.
[[nodiscard]] inline std::optional<RetailEntityRecord32>
original_scruffty_initial_record(
    int difficulty,
    std::int32_t source_entity_index,
    const std::vector<Vec2i>& original_group1_waypoints,
    std::uint32_t retail_surface_ptr32=0) noexcept {
    if (!scruffty_enabled(difficulty) ||
        difficulty>2 || source_entity_index<0 ||
        original_group1_waypoints.empty()) {
        return std::nullopt;
    }
    RetailEntityRecord32 record{};
    record.entity_id=source_entity_index;
    record.type=static_cast<int>(EntityType::Scruffty);
    record.x=original_group1_waypoints.front().x;
    record.y=original_group1_waypoints.front().y;
    record.x_float=static_cast<float>(record.x);
    record.y_float=static_cast<float>(record.y);
    record.source_left=0;
    record.source_top=0;
    record.source_right=101;
    record.source_bottom=116;
    record.surface_ptr32=retail_surface_ptr32;
    return record;
}

} // namespace btb::herding
