#pragma once

#include "btb/herding_runtime.hpp"
#include "btb/herding_source_far_roam.hpp"

#include <cstdint>
#include <optional>

namespace btb::herding {

// Original UpdateHerdingAnimal 0x00416A30 and 0x0041751B:
// when the animal is in behavior state zero, is not already in the
// ordinary 20-slot follower table, and Pickles carries the matching
// food, local_50 < original literal 80.0 enables follower admission.
//
// IMPORTANT: the retail animal X probe is x+(left-right)/2, i.e.
// NEGATIVE half sprite width. Y is y+(bottom-top)/2. The Pickles
// reference is (x, y+bottom-top-20). Do not use rectangle centers.
[[nodiscard]] constexpr std::optional<bool>
original_herding_food_attraction_contact(
    const RetailEntityRecord32& animal,
    const RetailEntityRecord32& pickles,
    FoodType selected_food,
    bool already_following) noexcept {

    if (!is_herd_animal(animal.entity_type()) ||
        pickles.entity_type()!=EntityType::FarmerPickles ||
        animal.source_right<animal.source_left ||
        animal.source_bottom<animal.source_top ||
        pickles.source_bottom<pickles.source_top)
        return std::nullopt;
    if (animal.behavior_state!=0 ||
        animal.temporary_target_timer>0 ||
        selected_food!=required_food_for(animal.entity_type()) ||
        already_following)
        return false;

    const std::int64_t animal_x=
        static_cast<std::int64_t>(animal.x)+
        (static_cast<std::int64_t>(animal.source_left)-
         animal.source_right)/2;
    const std::int64_t animal_y=
        static_cast<std::int64_t>(animal.y)+
        (static_cast<std::int64_t>(animal.source_bottom)-
         animal.source_top)/2;
    const std::int64_t pickles_y=
        static_cast<std::int64_t>(pickles.y)+
        pickles.source_bottom-pickles.source_top-20;
    const auto dx=animal_x-pickles.x;
    const auto dy=animal_y-pickles_y;
    if(dx<=-80 || dx>=80 || dy<=-80 || dy>=80)
        return false;
    return dx*dx+dy*dy<6400;
}

static_assert(kRetailFarAnimalNoInteractionRadius==80.f);

} // namespace btb::herding
