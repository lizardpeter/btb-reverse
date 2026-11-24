#pragma once
#include "btb/herding_source_registration_point.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"
#include "btb/herding_source_roam_braking.hpp"
#include <bit>
#include <cmath>
#include <limits>
#include <optional>

namespace btb::herding {
// 004177A0..00417819 and 00417BB0..00417C85: nonfollower
// inside the strict 80-unit Pickles distance gate moves away.
// 0x43B3E4 contains 0x3D088889 (float 1/30).
// The caller owns food attraction and higher-priority dispatch.
// The original stores distance, sine and cosine to float individually.
inline constexpr float kRetailNearRetreatScale=0.03333333507180214f;
static_assert(std::bit_cast<std::uint32_t>(kRetailNearRetreatScale)==0x3d088889u);
struct RetailNearRetreatStep {
    RetailEntityRecord32 moved{};
    int heading{};
    float distance{};
};
[[nodiscard]] inline std::optional<RetailNearRetreatStep>
original_herding_near_retreat(const RetailEntityRecord32& animal,
                             const RetailEntityRecord32& hero) noexcept {
    if(!is_herd_animal(animal.entity_type()) ||
       animal.behavior_state!=0 || animal.temporary_target_timer>0 ||
       animal.movement_active!=0 ||
       hero.entity_type()!=EntityType::FarmerPickles ||
       hero.source_bottom<hero.source_top ||
       !std::isfinite(animal.x_float)||!std::isfinite(animal.y_float))
        return std::nullopt;
    const auto anchor=original_herding_animal_tracking_anchor(animal);
    const auto hy=static_cast<std::int64_t>(hero.y)+hero.source_bottom-hero.source_top-20;
    if(!anchor || hy<std::numeric_limits<int>::min() || hy>std::numeric_limits<int>::max())
        return std::nullopt;
    const auto dx=static_cast<std::int64_t>(anchor->x)-hero.x;
    const auto dy=static_cast<std::int64_t>(anchor->y)-hy;
    if(dx<=-80 || dx>=80 || dy<=-80 || dy>=80 || dx*dx+dy*dy>=6400)
        return std::nullopt;
    const float distance=static_cast<float>(std::sqrt(static_cast<double>(dx*dx+dy*dy)));
    const auto heading=original_herding_integer_heading(hero.x,static_cast<int>(hy),anchor->x,anchor->y);
    const auto brake=original_herding_roam_brake_and_animate(animal.entity_type(),
        animal.movement_speed,animal.animation_frame,animal.animation_frame_countdown);
    if(!heading || !brake)return std::nullopt;
    const auto radians=static_cast<double>(*heading)*kRetailHerdingRadiansPerDegree;
    const float sine=static_cast<float>(std::sin(radians)), cosine=static_cast<float>(std::cos(radians));
    const double magnitude=static_cast<double>(distance)*kRetailNearRetreatScale;
    auto moved=animal;
    moved.previous_x=animal.x;moved.previous_y=animal.y;
    moved.x_float=static_cast<float>(static_cast<double>(animal.x_float)+sine*magnitude);
    moved.y_float=static_cast<float>(static_cast<double>(animal.y_float)-cosine*magnitude);
    if(!std::isfinite(moved.x_float)||!std::isfinite(moved.y_float)||
       moved.x_float<=static_cast<float>(std::numeric_limits<int>::min()) ||
       moved.x_float>=static_cast<float>(std::numeric_limits<int>::max()) ||
       moved.y_float<=static_cast<float>(std::numeric_limits<int>::min()) ||
       moved.y_float>=static_cast<float>(std::numeric_limits<int>::max()))return std::nullopt;
    moved.x=static_cast<int>(moved.x_float);moved.y=static_cast<int>(moved.y_float);
    moved.direction=(*heading+22)/45;if(moved.direction>=8)moved.direction-=8;
    moved.movement_active=1;
    moved.movement_speed=brake->speed;
    moved.animation_frame=brake->animation_frame;
    moved.animation_frame_countdown=brake->animation_countdown;
    return RetailNearRetreatStep{moved,*heading,distance};
}
} // namespace btb::herding
