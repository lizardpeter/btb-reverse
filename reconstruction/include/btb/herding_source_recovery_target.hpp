#pragma once

#include "btb/herding_data.hpp"

#include <array>
#include <cstdint>
#include <optional>

namespace btb::herding {

// UpdateHerdingAnimal's out-of-bounds recovery at 0x417FFE..0x41802E.
// The original loaded data globals are four (x,y) pairs beginning
// at 0x00443AC8, NOT four equiprobable complete destinations:
//
// index:   0         1         2         3
// pair: (100,766),(400,400),(891,616),(1128,659)
//
// Crucially, source calls 0x42FFC4 TWICE, once for X and independently
// once for Y, and uses a signed rand()%4 equivalent. It therefore can
// combine 16 destinations, not merely the four literal pair entries.
//
// The caller must supply these two actual reduced results in order.
// This helper never seeds random, performs a synthetic draw, or clamps
// invalid values to another valid target.
inline constexpr std::array<Vec2i,4> kRetailRoamingRecoveryTargets{{
    {100,766}, {400,400}, {891,616}, {1128,659}
}};

struct RetailRoamingRecoveryTarget {
    Vec2i point{};
    int rand_calls{2};
    int selected_x_index{};
    int selected_y_index{};
};

[[nodiscard]] constexpr std::optional<RetailRoamingRecoveryTarget>
original_herding_recovery_target(
    int first_rand_mod_4,
    int second_rand_mod_4) noexcept {

    if (first_rand_mod_4<0 || first_rand_mod_4>=4 ||
        second_rand_mod_4<0 || second_rand_mod_4>=4) {
        return std::nullopt;
    }
    return RetailRoamingRecoveryTarget{
        {
            kRetailRoamingRecoveryTargets[
                static_cast<std::size_t>(first_rand_mod_4)].x,
            kRetailRoamingRecoveryTargets[
                static_cast<std::size_t>(second_rand_mod_4)].y,
        },
        2,first_rand_mod_4,second_rand_mod_4
    };
}

} // namespace btb::herding
