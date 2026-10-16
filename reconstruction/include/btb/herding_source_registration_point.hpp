#pragma once

#include "btb/herding_runtime.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// InitializeHerdingActivity 0x00419600 writes the three species
// registration targets to source table 0x0050B2F4 + type*8.
// The x86 values are integer pixel coordinates (NOT the later
// entrance route targets at 0x00510600).
[[nodiscard]] constexpr std::optional<Vec2i>
original_herding_species_registration_point(EntityType type) noexcept {
    switch(type) {
        case EntityType::Sheep: return Vec2i{736,183};
        case EntityType::Rabbit: return Vec2i{861,142};
        case EntityType::Duck: return Vec2i{1023,337};
        default: return std::nullopt;
    }
}

// UpdateHerdingAnimal 0x00416A30 stores its original signed animal
// anchor. In particular X is x + (left-right)/2, NOT sprite center.
// All <150 registration and <30 tracked-arrival tests use it.
[[nodiscard]] constexpr std::optional<Vec2i>
original_herding_animal_tracking_anchor(
    const RetailEntityRecord32& animal) noexcept {
    if(!is_herd_animal(animal.entity_type()) ||
       animal.source_right<animal.source_left ||
       animal.source_bottom<animal.source_top)
        return std::nullopt;
    const std::int64_t x=static_cast<std::int64_t>(animal.x)+
        (static_cast<std::int64_t>(animal.source_left)-
         animal.source_right)/2;
    const std::int64_t y=static_cast<std::int64_t>(animal.y)+
        (static_cast<std::int64_t>(animal.source_bottom)-
         animal.source_top)/2;
    if(x<std::numeric_limits<std::int32_t>::min() ||
       x>std::numeric_limits<std::int32_t>::max() ||
       y<std::numeric_limits<std::int32_t>::min() ||
       y>std::numeric_limits<std::int32_t>::max())
        return std::nullopt;
    return Vec2i{static_cast<int>(x),static_cast<int>(y)};
}

static_assert(original_herding_species_registration_point(EntityType::Sheep)->x==736);
static_assert(original_herding_species_registration_point(EntityType::Rabbit)->y==142);
static_assert(original_herding_species_registration_point(EntityType::Duck)->x==1023);

} // namespace btb::herding
