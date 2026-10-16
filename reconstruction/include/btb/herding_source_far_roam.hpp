#pragma once

#include "btb/herding_runtime.hpp"
#include "btb/retail_point_in_polygon.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <vector>

namespace btb::herding {

// Retail PE32 (SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05)
// binary literals at 0x0043B418, 0x0043B43C, 0x004439B4,
// 0x004439D4 and 0x0043B434. These are NOT unit direction vectors:
// diagonal X/Y use +/-0.15 and the cardinals use +/-0.30.
inline constexpr float kRetailFarAnimalNoInteractionRadius=80.0f;
inline constexpr float kRetailFarAnimalSpeedCeiling=1.4f;
inline constexpr float kRetailFarAnimalSpeedGain=0.01f;
inline constexpr std::array<float,8> kRetailAnimalStepX{{
    0.f,0.15f,0.30f,0.15f,0.f,-0.15f,-0.30f,-0.15f
}};
inline constexpr std::array<float,8> kRetailAnimalStepY{{
    -0.30f,-0.15f,0.f,0.15f,0.30f,0.15f,0.f,-0.15f
}};
static_assert(std::bit_cast<std::uint32_t>(
    kRetailFarAnimalNoInteractionRadius)==0x42A00000u);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailFarAnimalSpeedCeiling)==0x3FB33333u);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailAnimalStepX[1])==0x3E19999Au);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailAnimalStepX[2])==0x3E99999Au);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailAnimalStepY[0])==0xBE99999Au);

// Source-covered path only:
//   UpdateHerdingAnimal 0x417C85 skips nearby behavior when the
//   animal is NOT a follower and is >=80 original units from Pickles.
//   UpdateHerdingActivity 0x4190..0x4191 then sees movement_active=0,
//   accelerates by +0.01 below 1.4, applies the RETAIL direction
//   table, truncates floating positions and checks navigation group 0.
// It deliberately does NOT simulate unverified close-range pursuit,
// follower motion, or source's out-of-bounds recovery toward group 3.
enum class RetailFarAnimalRoamCoverage {
    NotCovered,
    AdvancedWithinPolygon,
    RequiresOriginalBoundaryRecovery,
    InvalidInput
};
struct RetailFarAnimalRoamStep {
    RetailFarAnimalRoamCoverage coverage{
        RetailFarAnimalRoamCoverage::NotCovered};
    RetailEntityRecord32 animal{};
};

[[nodiscard]] inline RetailFarAnimalRoamStep
original_far_untracked_animal_roam_step(
    const RetailEntityRecord32& original,
    const RetailEntityRecord32& pickles,
    const std::vector<Vec2i>& original_group0_nav) noexcept {

    RetailFarAnimalRoamStep result{};
    result.animal=original;
    if (!is_herd_animal(original.entity_type()) ||
        pickles.entity_type()!=EntityType::FarmerPickles ||
        original.behavior_state!=0 ||
        original.temporary_target_timer>0 ||
        original.movement_active!=0)
        return result;

    if (original.direction<0 || original.direction>=8 ||
        !std::isfinite(original.x_float) ||
        !std::isfinite(original.y_float) ||
        !std::isfinite(original.movement_speed) ||
        original.movement_speed<0 ||
        original.source_right<original.source_left ||
        original.source_bottom<original.source_top ||
        pickles.source_bottom<pickles.source_top ||
        original_group0_nav.size()<3) {
        result.coverage=RetailFarAnimalRoamCoverage::InvalidInput;
        return result;
    }

    // UpdateHerdingAnimal computes animal sprite center and compares
    // it to Farmer Pickles' X and sprite bottom minus 20 pixels.
    // 0x00416A30 uses (source_left-source_right)/2, which is
    // NEGATIVE for standard positive-width sprite cells. Using an
    // ordinary geometric center silently moves the 80px attraction
    // boundary by a full sprite width.
    const std::int64_t animal_cx=
        static_cast<std::int64_t>(original.x)+
        (static_cast<std::int64_t>(original.source_left)-
         original.source_right)/2;
    const std::int64_t animal_cy=
        static_cast<std::int64_t>(original.y)+
        (static_cast<std::int64_t>(original.source_bottom)-
         original.source_top)/2;
    const std::int64_t hero_y=
        static_cast<std::int64_t>(pickles.y)+
        pickles.source_bottom-pickles.source_top-20;
    const auto dx=animal_cx-static_cast<std::int64_t>(pickles.x);
    const auto dy=animal_cy-hero_y;
    // Distances inside +/-80 can enter source food/follower AI; never
    // guess those branches. Check deltas first to avoid overflow.
    if (dx>-80 && dx<80 && dy>-80 && dy<80 &&
        dx*dx+dy*dy<6400)
        return result;

    auto moved=original;
    moved.previous_x=original.x;
    moved.previous_y=original.y;
    if (moved.movement_speed<kRetailFarAnimalSpeedCeiling) {
        moved.movement_speed=static_cast<float>(
            moved.movement_speed+kRetailFarAnimalSpeedGain);
    }
    moved.x_float=static_cast<float>(moved.x_float+
        kRetailAnimalStepX[static_cast<std::size_t>(moved.direction)]*
        moved.movement_speed);
    moved.y_float=static_cast<float>(moved.y_float+
        kRetailAnimalStepY[static_cast<std::size_t>(moved.direction)]*
        moved.movement_speed);
    if (!std::isfinite(moved.x_float) ||
        !std::isfinite(moved.y_float) ||
        moved.x_float<=static_cast<float>(std::numeric_limits<int>::min()) ||
        moved.y_float<=static_cast<float>(std::numeric_limits<int>::min()) ||
        moved.x_float>=static_cast<float>(std::numeric_limits<int>::max()) ||
        moved.y_float>=static_cast<float>(std::numeric_limits<int>::max())) {
        result.coverage=RetailFarAnimalRoamCoverage::InvalidInput;
        return result;
    }
    moved.x=static_cast<int>(moved.x_float);
    moved.y=static_cast<int>(moved.y_float);
    const auto inside=retail_geometry::original_polygon_contains(
        original_group0_nav,moved.x,moved.y);
    if (!inside) {
        result.coverage=RetailFarAnimalRoamCoverage::InvalidInput;
    } else if (!*inside) {
        // The subsequent 0x419167 group-3 recovery remains a separate
        // branch. Do not commit a move or claim an accepted frame.
        result.coverage=
            RetailFarAnimalRoamCoverage::RequiresOriginalBoundaryRecovery;
    } else {
        result.coverage=
            RetailFarAnimalRoamCoverage::AdvancedWithinPolygon;
        result.animal=moved;
    }
    return result;
}

} // namespace btb::herding
