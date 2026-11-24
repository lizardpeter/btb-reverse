#pragma once

#include "btb/herding_navigation_boundary.hpp"
#include "btb/herding_presentation.hpp"
#include "btb/herding_retail_rect_contact.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"
#include "btb/herding_source_pickles_animation.hpp"
#include <bit>
#include <cmath>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::herding {

// 004182D4..0041854F: 00443A9C initially 1; 0051078C is
// zero-filled process state. 00419600 resets camera but neither latch.
// 0043B448 is a DOUBLE 30.0, 0043B444 a FLOAT 1.5.
// Mouse uses the last drawn camera, not the proposed new camera.
inline constexpr double kRetailPicklesPointerDeadZone=30.0;
inline constexpr float kRetailPicklesMouseAnimationSpeed=15.0f;
static_assert(std::bit_cast<std::uint64_t>(kRetailPicklesPointerDeadZone)==0x403e000000000000ULL);

struct RetailPicklesControlState {
    bool mouse_mode{true};
    bool movement_latch{};
    Vec2i previous_pointer{};
    Vec2i last_camera{};
    friend bool operator==(const RetailPicklesControlState&,const RetailPicklesControlState&)=default;
};

struct RetailPicklesControlProposal {
    RetailEntityRecord32 hero{};
    RetailPicklesControlState state{};
    Vec2i food_probe{};
    float raw_x{};
    float raw_y{};
    float obstacle_x{};
    float obstacle_y{};
    int directional_axes{};
    std::size_t obstacle_contacts{};
};

// Guard malformed external records, not ordinary game coordinates.
// The original uses FTOL and 32-bit arithmetic; live map is 1272x882.
[[nodiscard]] inline bool retail_pickles_world_number(double v) noexcept {
    return std::isfinite(v) && v>-1000000.0 && v<1000000.0;
}

[[nodiscard]] inline std::optional<RetailCornerRect>
original_pickles_feet_rect(const RetailEntityRecord32& hero,float x,float y) noexcept {
    const auto w=static_cast<std::int64_t>(hero.source_right)-hero.source_left;
    const auto h=static_cast<std::int64_t>(hero.source_bottom)-hero.source_top;
    if(w<0 || h<0 || w>1000000 || h>1000000 ||
       !retail_pickles_world_number(x)||!retail_pickles_world_number(y)||
       !retail_pickles_world_number(static_cast<double>(x)+w)||
       !retail_pickles_world_number(static_cast<double>(y)+h))return std::nullopt;
    const auto bottom=static_cast<int>(static_cast<double>(y)+h);
    return RetailCornerRect{static_cast<int>(x)+18,bottom-20,
        static_cast<int>(static_cast<double>(x)+w)-18,bottom};
}

[[nodiscard]] constexpr bool original_pickles_has_obstacle(EntityType type) noexcept {
    return type==EntityType::DormantLegacy4||type==EntityType::DormantLegacy5||
           type==EntityType::DormantLegacy6||type==EntityType::TravisCab||
           type==EntityType::Trailer1||type==EntityType::Trailer2;
}

// 0041872B..00418C1C. The values are source integer rectangle
// offsets, not the visible image's full bounding box.
[[nodiscard]] inline std::optional<RetailCornerRect>
original_pickles_obstacle_rect(const RetailEntityRecord32& obstacle) noexcept {
    const auto w=static_cast<std::int64_t>(obstacle.source_right)-obstacle.source_left;
    const auto h=static_cast<std::int64_t>(obstacle.source_bottom)-obstacle.source_top;
    if(w<0||h<0||w>1000000||h>1000000||
       !retail_pickles_world_number(obstacle.x)||!retail_pickles_world_number(obstacle.y))
        return std::nullopt;
    const int width=static_cast<int>(w),height=static_cast<int>(h),x=obstacle.x,y=obstacle.y;
    switch(obstacle.entity_type()) {
    case EntityType::DormantLegacy4:
    case EntityType::DormantLegacy5:
    case EntityType::DormantLegacy6:
        return RetailCornerRect{x,y,x+width,y+height};
    case EntityType::TravisCab:
        return RetailCornerRect{x,y+75,x+width-70,y+height};
    case EntityType::Trailer1:
        return RetailCornerRect{x-40,y+75,x+width-20,y+height-20};
    case EntityType::Trailer2:
        return RetailCornerRect{x-30,y+30,x+width-20,y+height+30};
    default:return std::nullopt;
    }
}

struct RetailPicklesObstacleResult {
    float x{},y{};
    std::size_t contacts{};
};

// Retail rectangle order is feet FIRST, static object SECOND, inclusive
// corner contact (NOT symmetric AABB). For each trailer/cab, try old X,
// then old Y with the incoming X, then restore both. Keep the source's
// last tested feet rectangle until another contact causes a recompute.
[[nodiscard]] inline std::optional<RetailPicklesObstacleResult>
original_pickles_obstacle_pass(const RetailEntityRecord32& hero,
    float attempted_x,float attempted_y,std::span<const RetailEntityRecord32> entities) noexcept {
    auto rect=original_pickles_feet_rect(hero,attempted_x,attempted_y);
    if(!rect)return std::nullopt;
    RetailPicklesObstacleResult out{attempted_x,attempted_y,0};
    for(const auto& entity:entities) {
        const auto type=entity.entity_type();
        if(!original_pickles_has_obstacle(type))continue;
        const auto obstacle=original_pickles_obstacle_rect(entity);
        if(!obstacle)return std::nullopt;
        if(!original_herding_rect_contact(*rect,*obstacle))continue;
        ++out.contacts;
        const float incoming_x=out.x;
        out.x=static_cast<float>(hero.x);
        if(type==EntityType::DormantLegacy4||type==EntityType::DormantLegacy5||
           type==EntityType::DormantLegacy6) {
            out.y=static_cast<float>(hero.y);
            continue;
        }
        rect=original_pickles_feet_rect(hero,out.x,out.y);
        if(!rect)return std::nullopt;
        if(!original_herding_rect_contact(*rect,*obstacle))continue;
        out.x=incoming_x;out.y=static_cast<float>(hero.y);
        rect=original_pickles_feet_rect(hero,out.x,out.y);
        if(!rect)return std::nullopt;
        if(original_herding_rect_contact(*rect,*obstacle))
            out.x=static_cast<float>(hero.x);
    }
    return out;
}

// Source input proposal, food-probe integers, obstacle pass, and
// group-zero navigation in their original order. Food selection must
// consume food_probe, BEFORE final corrected integers are committed.
[[nodiscard]] inline std::optional<RetailPicklesControlProposal>
original_pickles_control_proposal(const RetailEntityRecord32& before,
    RetailPicklesControlState state,Vec2i pointer,std::uint8_t raw_keys,
    std::span<const RetailEntityRecord32> entities,
    const std::vector<Vec2i>& nav) noexcept {
    if(before.entity_type()!=EntityType::FarmerPickles||
       !retail_pickles_world_number(before.x_float)||!retail_pickles_world_number(before.y_float)||
       !retail_pickles_world_number(before.x)||!retail_pickles_world_number(before.y)||
       !retail_pickles_world_number(pointer.x)||!retail_pickles_world_number(pointer.y)||
       !retail_pickles_world_number(state.last_camera.x)||!retail_pickles_world_number(state.last_camera.y))
        return std::nullopt;
    const auto w=static_cast<std::int64_t>(before.source_right)-before.source_left;
    const auto h=static_cast<std::int64_t>(before.source_bottom)-before.source_top;
    if(w<0||h<0||w>1000000||h>1000000)return std::nullopt;
    RetailPicklesControlProposal out{};
    out.hero=before;out.state=state;
    out.hero.previous_x=before.x;out.hero.previous_y=before.y;
    out.hero.movement_direction_mask=0;
    out.food_probe={before.x,before.y};
    out.raw_x=before.x_float;out.raw_y=before.y_float;
    if(state.mouse_mode) {
        // Switching to keyboard still executes THIS frame's mouse
        // branch; 418E55 checks the now-updated mode for final facing.
        if(raw_keys!=0)out.state.mouse_mode=false;
        const int cx=before.x+static_cast<int>(w/2)-state.last_camera.x;
        const int cy=before.y+static_cast<int>(h/2)-state.last_camera.y;
        const auto dx=static_cast<std::int64_t>(pointer.x)-cx;
        const auto dy=static_cast<std::int64_t>(pointer.y)-cy;
        if(dx*dx+dy*dy>900) {
            const auto heading=original_herding_integer_heading(cx,cy,pointer.x,pointer.y);
            if(!heading)return std::nullopt;
            out.hero.direction=(*heading+22)/45;
            if(out.hero.direction>=8)out.hero.direction-=8;
            out.hero.movement_speed=kRetailPicklesMouseAnimationSpeed;
            if(state.movement_latch) {
                const auto step=original_herding_steering_step(before.x_float,before.y_float,
                    *heading,kPicklesKeyboardStep,false);
                if(!step)return std::nullopt;
                out.raw_x=step->x;out.raw_y=step->y;
                out.food_probe={step->rounded_x,step->rounded_y};
            }
            out.hero.movement_direction_mask=PicklesMoveRight;
            out.state.movement_latch=true;
        }
    } else {
        if(pointer!=state.previous_pointer)out.state.mouse_mode=true;
        out.state.previous_pointer=pointer;
        const auto motion=pickles_keyboard_motion(raw_keys);
        out.hero.movement_direction_mask=motion.movement_mask;
        out.directional_axes=(motion.delta_x!=0.f?1:0)+(motion.delta_y!=0.f?1:0);
        if(state.movement_latch) {
            out.raw_x=before.x_float+motion.delta_x;
            out.raw_y=before.y_float+motion.delta_y;
        }
        out.state.movement_latch=true;
    }
    const auto obstacles=original_pickles_obstacle_pass(before,out.raw_x,out.raw_y,entities);
    if(!obstacles)return std::nullopt;
    out.obstacle_x=obstacles->x;out.obstacle_y=obstacles->y;out.obstacle_contacts=obstacles->contacts;
    const auto boundary=retail_herding_navigation_boundary_step(nav,out.obstacle_x,out.obstacle_y,
        before.x,before.y,retail_herding_axis_recovery_enabled(out.directional_axes,out.state.mouse_mode?1:0));
    if(!boundary)return std::nullopt;
    out.hero.x_float=boundary->x;out.hero.y_float=boundary->y;
    out.hero.x=static_cast<int>(boundary->x);out.hero.y=static_cast<int>(boundary->y);
    // 418ECE clears every entity's movement latch after Pickles' own update.
    out.hero.movement_active=0;
    return out;
}

// 00418CE4..00418ECE. Unlike the earlier keyboard-only model, a new
// walking family resets countdown to 50 AND decrements on entry.
// Mouse mode decrements by truncated +4C speed, keyboard by 10.
[[nodiscard]] inline bool finish_original_pickles_control(
    RetailPicklesControlProposal& out,FoodType selected_food) noexcept {
    auto& hero=out.hero;
    if(selected_food!=FoodType::None && selected_food!=FoodType::DuckFood &&
       selected_food!=FoodType::RabbitFood && selected_food!=FoodType::SheepFood)return false;
    const bool carrying=selected_food!=FoodType::None;
    if(hero.movement_direction_mask!=0) {
        const int first=carrying?20:0,last=carrying?29:9;
        if(hero.animation_frame<first||hero.animation_frame>last) {
            hero.animation_frame=first;hero.animation_frame_countdown=50;
        }
        if(!std::isfinite(hero.movement_speed)||hero.movement_speed<0||hero.movement_speed>1000.f||
           hero.animation_frame_countdown<std::numeric_limits<int>::min()+1000)return false;
        hero.animation_frame_countdown-=out.state.mouse_mode?
            static_cast<int>(hero.movement_speed):10;
        if(hero.animation_frame_countdown<=0) {
            hero.animation_frame=hero.animation_frame>=last?first:hero.animation_frame+1;
            hero.animation_frame_countdown=50;out.state.movement_latch=true;
        }
        if(!out.state.mouse_mode) {
            if(const auto facing=pickles_facing_direction_from_mask(hero.movement_direction_mask))
                hero.direction=*facing;
        }
    } else {
        const auto first=carrying?33:12,last=carrying?38:15;
        const bool old_idle=hero.animation_frame>=first&&hero.animation_frame<=last;
        const auto anim=original_pickles_keyboard_animation_step(hero.animation_frame,
            hero.animation_frame_countdown,false,selected_food);
        if(!anim)return false;
        if(old_idle&&hero.animation_frame_countdown<=10)out.state.movement_latch=true;
        hero.animation_frame=anim->frame;hero.animation_frame_countdown=anim->countdown;
    }
    const auto camera=herding_camera_plan(hero.x,hero.y);
    out.state.last_camera={camera.camera_x,camera.camera_y};
    return true;
}

} // namespace btb::herding
