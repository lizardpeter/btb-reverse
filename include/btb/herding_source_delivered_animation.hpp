#pragma once

#include "btb/herding_runtime.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// Original UpdateHerdingAnimal state 99, 0x416ECF..0x416F39:
// face down (index 4), count down animation by 5, reload 150 and
// increment the in-home sprite frame with the retail 6/10/12
// EXCLUSIVE upper and 3/7/9 lower bounds (type 1/2/3).
struct RetailDeliveredAnimalAnimation {
    RetailEntityRecord32 updated{};
    bool advanced_frame{};
};

[[nodiscard]] constexpr std::optional<RetailDeliveredAnimalAnimation>
original_herding_delivered_animal_animation(
    const RetailEntityRecord32& original) noexcept {
    if(original.behavior_state!=99 ||
       !is_herd_animal(original.entity_type()) ||
       original.animation_frame_countdown<
           std::numeric_limits<std::int32_t>::min()+5 ||
       original.animation_frame==std::numeric_limits<std::int32_t>::max())
        return std::nullopt;
    auto updated=original;
    updated.direction=4;
    updated.animation_frame_countdown-=5;
    bool advanced=false;
    if(updated.animation_frame_countdown<=0) {
        updated.animation_frame_countdown=150;
        ++updated.animation_frame;
        int first=3,last_exclusive=6;
        switch(original.entity_type()) {
        case EntityType::Sheep:break;
        case EntityType::Rabbit:first=7;last_exclusive=10;break;
        case EntityType::Duck:first=9;last_exclusive=12;break;
        default:return std::nullopt;
        }
        if(updated.animation_frame>=last_exclusive ||
           updated.animation_frame<first)
            updated.animation_frame=first;
        advanced=true;
    }
    return RetailDeliveredAnimalAnimation{updated,advanced};
}

} // namespace btb::herding
