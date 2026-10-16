#pragma once

#include "btb/herding_source_home_arrival.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

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

struct RetailHomeMotionStep {
    RetailEntityRecord32 moved{};
    Vec2i source_anchor{};
    Vec2i source_target{};
    bool source_arrived{};
    bool second_stage{};
};

// UpdateHerdingAnimal state 10..15 branch 0x416B2F and 20..25
// branch 0x416C9A. Both call integer-heading 0x415D70 then native
// sin/cos steering with current speed, followed by +0.01 speed gain
// strictly below 0.8, and strict <10 PRE-MOVE actor distance.
// No route counter or undelivered decrement happens in this helper:
// those remain transactional confirmed events in the event simulation.
[[nodiscard]] inline std::optional<RetailHomeMotionStep>
original_herding_home_route_motion(
    const RetailEntityRecord32& before) noexcept {
    const auto state=before.behavior_state;
    const bool first=state>=10 && state<=14;
    const bool second=state>=20 && state<=24;
    if(!first && !second)return std::nullopt;
    const auto anchor=original_herding_home_motion_anchor(before);
    const auto target=original_herding_current_home_target(
        before.entity_type(),state);
    if(!anchor || !target ||
       before.animation_frame_countdown<
           std::numeric_limits<std::int32_t>::min()+10 ||
       before.animation_frame==std::numeric_limits<std::int32_t>::max())
        return std::nullopt;
    const auto heading=original_herding_integer_heading(
        anchor->x,anchor->y,target->x,target->y);
    if(!heading)return std::nullopt;
    const auto movement=original_herding_steering_step(
        before.x_float,before.y_float,*heading,
        before.movement_speed,true);
    if(!movement)return std::nullopt;

    auto moved=before;
    moved.x_float=movement->x;
    moved.y_float=movement->y;
    moved.x=movement->rounded_x;
    moved.y=movement->rounded_y;
    moved.direction=movement->facing_index;
    moved.movement_active=1;
    moved.movement_speed=movement->next_speed;

    moved.animation_frame_countdown-=first?10:5;
    if(moved.animation_frame_countdown<=0) {
        const auto species=before.entity_type();
        ++moved.animation_frame;
        if(species==EntityType::Duck) {
            // Retail duck home sprites 9..11. In the first-stage
            // branch 0x416BCE, duck skips the usual reload-100 write.
            if(moved.animation_frame<9 || moved.animation_frame>11)
                moved.animation_frame=9;
            if(second)moved.animation_frame_countdown=100;
        } else {
            const int start=species==EntityType::Sheep?6:3;
            const int last=species==EntityType::Sheep?9:6;
            if(moved.animation_frame<start || moved.animation_frame>last)
                moved.animation_frame=start;
            moved.animation_frame_countdown=100;
        }
    }
    return RetailHomeMotionStep{
        moved,*anchor,*target,
        original_herding_within_home_radius(*anchor,*target),second
    };
}

} // namespace btb::herding
