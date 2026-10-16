#pragma once

#include "btb/herding_runtime.hpp"

#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// Keyboard-mode Farmer Pickles animation in UpdateHerdingActivity
// 0x418CE4..0x418ECE, recovered from the original PE32.
// This is separate from DrawHerdingActivity's animal sprite counter.
// Source ranges are 0..9 walking empty-handed, 20..29 walking with
// food, 12..15 idle empty-handed and 33..38 idle carrying food.
// Original keyboard mode decrements the frame countdown by 10;
// it reloads 50 on walking frame changes and 100 while idling.
struct RetailPicklesAnimationStep {
    std::int32_t frame{};
    std::int32_t countdown{};
};

[[nodiscard]] constexpr std::optional<RetailPicklesAnimationStep>
original_pickles_keyboard_animation_step(
    std::int32_t previous_frame,
    std::int32_t previous_countdown,
    bool movement_mask_nonzero,
    FoodType selected_food) noexcept {

    if(selected_food!=FoodType::None &&
       selected_food!=FoodType::DuckFood &&
       selected_food!=FoodType::RabbitFood &&
       selected_food!=FoodType::SheepFood)
        return std::nullopt;
    if(previous_countdown<std::numeric_limits<std::int32_t>::min()+10 ||
       previous_frame==std::numeric_limits<std::int32_t>::max())
        return std::nullopt;

    const bool carrying=selected_food!=FoodType::None;
    if(movement_mask_nonzero) {
        const int start=carrying?20:0;
        const int last=carrying?29:9;
        if(previous_frame<start || previous_frame>last)
            return RetailPicklesAnimationStep{start,50};
        const auto next_countdown=previous_countdown-10;
        if(next_countdown>0)
            return RetailPicklesAnimationStep{previous_frame,next_countdown};
        const int next_frame=previous_frame>=last ? start : previous_frame+1;
        return RetailPicklesAnimationStep{next_frame,50};
    }

    // The original activity enters each idle animation cycle only
    // if its current frame already belongs to that idle family.
    // Otherwise it writes the first frame without resetting countdown.
    const int idle_start=carrying?33:12;
    const int idle_last=carrying?38:15;
    if(previous_frame<idle_start || previous_frame>idle_last)
        return RetailPicklesAnimationStep{idle_start,previous_countdown};
    const auto next_countdown=previous_countdown-10;
    if(next_countdown>0)
        return RetailPicklesAnimationStep{previous_frame,next_countdown};
    const int next_frame=previous_frame>=idle_last ? idle_start : previous_frame+1;
    return RetailPicklesAnimationStep{next_frame,100};
}

static_assert(original_pickles_keyboard_animation_step(
    0,0,false,FoodType::None)->frame==12);
static_assert(original_pickles_keyboard_animation_step(
    30,0,true,FoodType::None)->frame==0);
static_assert(original_pickles_keyboard_animation_step(
    29,10,true,FoodType::RabbitFood)->frame==20);
static_assert(original_pickles_keyboard_animation_step(
    38,10,false,FoodType::SheepFood)->frame==33);

} // namespace btb::herding
