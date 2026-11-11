#pragma once
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"
#include "btb/herding_source_roam_braking.hpp"
#include <bit>

namespace btb::herding {
// Original PE c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05:
// 004175F0 -> 00417B1D..00417C85, non-follower within strict 80 units.
// The direction runs FROM Pickles TO the signed animal anchor. The amount
// is distance * float at 0043B3E4, not the animal's accumulated speed.
inline constexpr float kRetailNearEscapeDistanceScale=0.03333333507180214f;
static_assert(std::bit_cast<std::uint32_t>(kRetailNearEscapeDistanceScale)==0x3D088889U);
struct RetailNearEscapeStep {
    RetailEntityRecord32 animal{};
    Vec2i animal_anchor{},pickles_anchor{};
    int heading{};
    float distance{};
};
[[nodiscard]] inline std::optional<RetailNearEscapeStep>
original_herding_near_escape_step(const RetailEntityRecord32& animal,
    const RetailEntityRecord32& pickles) noexcept {
    if(!is_herd_animal(animal.entity_type()) ||
       pickles.entity_type()!=EntityType::FarmerPickles ||
       animal.behavior_state!=0 || animal.temporary_target_timer>0 ||
       animal.source_right<animal.source_left ||
       animal.source_bottom<animal.source_top ||
       pickles.source_bottom<pickles.source_top)return std::nullopt;
    const std::int64_t ax=static_cast<std::int64_t>(animal.x)+
        (static_cast<std::int64_t>(animal.source_left)-animal.source_right)/2;
    const std::int64_t ay=static_cast<std::int64_t>(animal.y)+
        (static_cast<std::int64_t>(animal.source_bottom)-animal.source_top)/2;
    const std::int64_t py=static_cast<std::int64_t>(pickles.y)+
        pickles.source_bottom-pickles.source_top-20;
    if(ax<INT32_MIN||ax>INT32_MAX||ay<INT32_MIN||ay>INT32_MAX||
       py<INT32_MIN||py>INT32_MAX)return std::nullopt;
    const auto dx=ax-pickles.x,dy=ay-py;
    if(dx<=-80||dx>=80||dy<=-80||dy>=80||dx*dx+dy*dy>=6400)
        return std::nullopt;
    // Original distance return is stored to float before multiplying.
    const auto distance=static_cast<float>(std::sqrt(
        static_cast<double>(dx*dx+dy*dy)));
    const auto heading=original_herding_integer_heading(pickles.x,
        static_cast<int>(py),static_cast<int>(ax),static_cast<int>(ay));
    const auto animation=original_herding_roam_brake_and_animate(
        animal.entity_type(),animal.movement_speed,animal.animation_frame,
        animal.animation_frame_countdown);
    if(!heading||!animation)return std::nullopt;
    const auto step=original_herding_steering_step(animal.x_float,
        animal.y_float,*heading,distance*kRetailNearEscapeDistanceScale,false);
    if(!step||step->facing_index<0||step->facing_index>7)return std::nullopt;
    auto moved=animal;
    moved.x_float=step->x;moved.y_float=step->y;
    moved.x=step->rounded_x;moved.y=step->rounded_y;
    moved.direction=step->facing_index;
    moved.movement_active=1;
    moved.movement_speed=animation->speed;
    moved.animation_frame=animation->animation_frame;
    moved.animation_frame_countdown=animation->animation_countdown;
    return RetailNearEscapeStep{moved,{static_cast<int>(ax),static_cast<int>(ay)},
        {pickles.x,static_cast<int>(py)},*heading,distance};
}
} // namespace btb::herding
