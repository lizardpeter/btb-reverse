#pragma once

#include "btb/herding_runtime.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// Original PE32 helper at 0x415D30:
// sqrt(float(abs(X1-X2)^2 + abs(Y1-Y2)^2)) via x87 FILD/FMUL/FSQRT.
// At 0x416D50/0x416ECF the animal AI compares this distance strictly
// below original float 10.0 (0x43B378 = 0x41200000).
//
// For integer coordinates, compare squared values to exactly 100;
// that is mathematically identical to the retail strict <10 result
// throughout the original map domain, with no sqrt precision noise.
// Check each delta before squaring to avoid overflow at arbitrary
// user-provided int32 coordinate extremes.
[[nodiscard]] constexpr bool original_herding_within_home_radius(
    Vec2i actual,
    Vec2i target) noexcept {

    const auto dx=static_cast<std::int64_t>(actual.x)-target.x;
    const auto dy=static_cast<std::int64_t>(actual.y)-target.y;
    if (dx <= -10 || dx >= 10 ||
        dy <= -10 || dy >= 10) {
        return false;
    }
    return dx*dx + dy*dy < 100;
}

[[nodiscard]] constexpr std::optional<Vec2i>
original_herding_current_home_target(
    EntityType species,
    std::int32_t behavior_state) noexcept {

    if (behavior_state>=10 && behavior_state<=14) {
        const auto home=home_entrance_target(species);
        return home.x==-1 ? std::nullopt
                           : std::optional<Vec2i>{home};
    }
    if (behavior_state>=20 && behavior_state<=24) {
        return home_entry_target(species,behavior_state);
    }
    return std::nullopt;
}

// 0x00416A30: after behavior becomes nonzero, the animal movement
// origin is (x - original sprite-anchor X, y + 40), not its top-left.
// 0x00443B08 stores species X offsets 40 / 48 / 50.
// The source state-10/20 target coordinates are already normalized in
// home_entrance_target() and home_entry_target() respectively.
[[nodiscard]] constexpr std::optional<Vec2i>
original_herding_home_motion_anchor(
    const RetailEntityRecord32& animal) noexcept {
    int offset{};
    switch(animal.entity_type()) {
        case EntityType::Sheep: offset=40;break;
        case EntityType::Rabbit: offset=48;break;
        case EntityType::Duck: offset=50;break;
        default: return std::nullopt;
    }
    const auto x=static_cast<std::int64_t>(animal.x)-offset;
    const auto y=static_cast<std::int64_t>(animal.y)+40;
    if(x<std::numeric_limits<std::int32_t>::min() ||
       x>std::numeric_limits<std::int32_t>::max() ||
       y<std::numeric_limits<std::int32_t>::min() ||
       y>std::numeric_limits<std::int32_t>::max())
        return std::nullopt;
    return Vec2i{static_cast<int>(x),static_cast<int>(y)};
}

[[nodiscard]] constexpr std::optional<Vec2i>
original_herding_home_entity_origin_for_target(
    EntityType species, Vec2i anchor_target) noexcept {
    int offset{};
    switch(species) {
        case EntityType::Sheep:offset=40;break;
        case EntityType::Rabbit:offset=48;break;
        case EntityType::Duck:offset=50;break;
        default:return std::nullopt;
    }
    const auto x=static_cast<std::int64_t>(anchor_target.x)+offset;
    const auto y=static_cast<std::int64_t>(anchor_target.y)-40;
    if(x<std::numeric_limits<std::int32_t>::min() ||
       x>std::numeric_limits<std::int32_t>::max() ||
       y<std::numeric_limits<std::int32_t>::min() ||
       y>std::numeric_limits<std::int32_t>::max())
       return std::nullopt;
    return Vec2i{static_cast<int>(x),static_cast<int>(y)};
}

[[nodiscard]] constexpr bool original_herding_home_arrival(
    const RetailEntityRecord32& animal) noexcept {

    const auto target=original_herding_current_home_target(
        animal.entity_type(),animal.behavior_state);
    const auto actor=original_herding_home_motion_anchor(animal);
    return target && actor &&
        original_herding_within_home_radius(*actor,*target);
}

} // namespace btb::herding
