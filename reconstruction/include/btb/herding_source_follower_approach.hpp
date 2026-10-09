#pragma once

#include "btb/herding_runtime.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"
#include "btb/retail_crt_random.hpp"

#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// Narrow recovery of 0x004172B0..0x00417432. This is the tracked-target
// approach branch reached after the entity was found in the retail
// 20-slot follower table 0x50AF78. Its input target comes from the
// species-specific 0x50B2F4/0x50B2F8 point table. Callers must resolve
// that table and membership in the original update order; this helper
// does not invent a target or choose which animal should follow.
//
// 0x4172D2: source distance < float(30.0f) triggers removal from
// the 20-slot TRACKED-TARGET table 0x50AF78, writes behavior_state
// (+0x50) to 1 and returns 1; it does NOT mutate movement_active
// (+0x48) in that arrival branch. The separate ordinary FOLLOWER
// table is at 0x50AF14. Otherwise move with existing speed, ramp
// speed by +0.01 when below 0.8, and update 3-frame animation.
//
// The source also consumes one 0x42FFC4 rand() result on the arrival
// branch without using its value. This is observable in later RNG
// decisions, so callers must preserve that draw.
inline constexpr float kRetailFollowerApproachDistance=30.0f;
inline constexpr std::int32_t kRetailFollowerAnimationDecrement=5;
inline constexpr std::int32_t kRetailFollowerAnimationReload=100;
inline constexpr std::int32_t kRetailFollowerAnimationFrames=3;

struct RetailFollowerApproachStep {
    RetailEntityRecord32 updated{};
    bool reached_tracked_point{};
    bool remove_from_follower_table{};
    std::int32_t source_rand_calls{};
    bool animation_advanced{};
};

// Accept the exact original 0x64-byte entity record. On success the
// returned record contains the new float/int coordinates, facing and
// animation; other AI state remains unchanged. Trig uses the existing
// portable x87-approximate kernel and is not claimed byte-identical.
[[nodiscard]] inline std::optional<RetailFollowerApproachStep>
original_herding_follower_approach(
    const RetailEntityRecord32& original,
    Vec2i tracked_point) noexcept {

    if (!is_herd_animal(original.entity_type()) ||
        !std::isfinite(original.x_float) ||
        !std::isfinite(original.y_float) ||
        !std::isfinite(original.movement_speed) ||
        original.movement_speed<0) {
        return std::nullopt;
    }

    const auto dx=static_cast<std::int64_t>(original.x)-tracked_point.x;
    const auto dy=static_cast<std::int64_t>(original.y)-tracked_point.y;

    // Original 0x415D30 calculates FSQRT from integer deltas; for
    // practical retail map coordinates d^2 < 900 is equivalent to
    // FSQRT < 30 without double rounding or overflow.
    if (dx>-30 && dx<30 && dy>-30 && dy<30 &&
        dx*dx+dy*dy<900) {
        auto updated=original;
        // Original 0x417314 writes +0x50 (behavior_state=1), not
        // +0x48. The next animal update dispatches this state to
        // the separately recovered begin-home-route code.
        updated.behavior_state=1;
        return RetailFollowerApproachStep{
            updated,true,true,1,false
        };
    }

    const auto heading=original_herding_integer_heading(
        original.x,original.y,tracked_point.x,tracked_point.y);
    if (!heading) return std::nullopt;

    const auto movement=original_herding_steering_step(
        original.x_float,original.y_float,*heading,
        original.movement_speed,true);
    if (!movement) return std::nullopt;

    auto updated=original;
    updated.x_float=movement->x;
    updated.y_float=movement->y;
    updated.x=movement->rounded_x;
    updated.y=movement->rounded_y;
    updated.direction=movement->facing_index;
    updated.movement_active=1;
    updated.movement_speed=movement->next_speed;

    // The retail countdown is a signed 32-bit integer at +0x2C.
    // Normalize large external values explicitly rather than invoke
    // signed-overflow UB; original reachable countdowns are <=100.
    if (updated.animation_frame_countdown <
        std::numeric_limits<std::int32_t>::min()+
            kRetailFollowerAnimationDecrement) {
        return std::nullopt;
    }
    updated.animation_frame_countdown -=
        kRetailFollowerAnimationDecrement;
    bool advanced=false;
    if (updated.animation_frame_countdown<=0) {
        updated.animation_frame_countdown=
            kRetailFollowerAnimationReload;
        if (updated.animation_frame==
            std::numeric_limits<std::int32_t>::max()) {
            return std::nullopt;
        }
        ++updated.animation_frame;
        if (updated.animation_frame>=kRetailFollowerAnimationFrames) {
            updated.animation_frame=0;
        }
        advanced=true;
    }
    return RetailFollowerApproachStep{
        updated,false,false,0,advanced
    };
}

// The 0x417309 arrival path consumes one rand() AFTER its strict
// 30-unit distance test. The original process-global generator must
// advance even though the result is discarded. The pure overload
// above remains available to examine the branch without mutations.
[[nodiscard]] inline std::optional<RetailFollowerApproachStep>
original_herding_follower_approach(
    const RetailEntityRecord32& original,
    Vec2i tracked_point,
    retail::OriginalRetailRandom& shared_rng) noexcept {

    const auto step=original_herding_follower_approach(
        original,tracked_point);
    if (!step) return std::nullopt;
    for (int i=0;i<step->source_rand_calls;++i) {
        static_cast<void>(shared_rng.next_rand());
    }
    return step;
}

} // namespace btb::herding
