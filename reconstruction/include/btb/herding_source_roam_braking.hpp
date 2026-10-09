#pragma once

#include "btb/herding_runtime.hpp"

#include <bit>
#include <cstdint>
#include <cmath>
#include <limits>
#include <optional>

namespace btb::herding {

// 0x417C0C..0x417C85: the original animation/speed tail of one
// free-roaming movement branch. This is NOT the 0x4173BF
// +0.01 acceleration branch, or the separate DrawHerdingActivity
// per-render animation, and must be called only by its source branch.
//
// 0x43B3E0: speed comparison 0.4f; 0x43B400 double 0.1;
// countdown at entity +0x2C decreases by 20; on expiration resets
// to 100, increments the frame and wraps through species-specific
// min/max tables (0x443AF0 and 0x443AFC).
inline constexpr float kRetailRoamSpeedBrake=0.4f;
inline constexpr float kRetailRoamSpeedFloor=0.1f;
static_assert(std::bit_cast<std::uint32_t>(
    kRetailRoamSpeedBrake)==0x3ECCCCCDu);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailRoamSpeedFloor)==0x3DCCCCCDu);

struct RetailRoamSpeedAnimationStep {
    float speed{};
    int animation_frame{};
    int animation_countdown{};
    bool advanced_frame{};
};

[[nodiscard]] constexpr std::optional<RetailRoamSpeedAnimationStep>
original_herding_roam_brake_and_animate(
    EntityType species,
    float old_speed,
    int old_frame,
    int old_countdown) noexcept {

    if (!is_herd_animal(species) ||
        !std::isfinite(old_speed) ||
        old_speed < 0 ||
        old_countdown < std::numeric_limits<int>::min()+20 ||
        old_frame == std::numeric_limits<int>::max()) {
        return std::nullopt;
    }

    // At 0x417C18 FCOMP checks C0/C3. Equality takes the floor path.
    const auto next_speed = old_speed > kRetailRoamSpeedBrake
        ? old_speed-kRetailRoamSpeedBrake
        : kRetailRoamSpeedFloor;

    int countdown=old_countdown-20;
    int frame=old_frame;
    bool advanced=false;
    if (countdown<=0) {
        countdown=100;
        ++frame;
        switch (species) {
        case EntityType::Sheep:
            if (frame>=10) frame=6;
            break;
        case EntityType::Rabbit:
            if (frame>=7) frame=3;
            break;
        case EntityType::Duck:
            if (frame>=6) frame=0;
            break;
        default: break;
        }
        advanced=true;
    }
    return RetailRoamSpeedAnimationStep{
        next_speed,frame,countdown,advanced};
}

} // namespace btb::herding
