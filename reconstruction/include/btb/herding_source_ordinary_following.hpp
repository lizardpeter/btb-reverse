#pragma once

#include "btb/herding_runtime.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"

#include <bit>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <optional>

namespace btb::herding {

// Original UpdateHerdingAnimal 0x00417603..0x00417994, recovered
// directly from the original 311,296-byte PE32 x86 instructions.
// Applies ONLY to an already-registered ORDINARY follower that has
// not entered the independent tracked-target list. This is the
// original circular formation around Farmer Pickles, NOT generic
// move-toward-player steering.
//
// Formation slot is the per-frame 0x00510770 global, reset to 0 by
// UpdateHerdingActivity and incremented on each ordinary follower.
// Distances use the original signed animal sprite anchor, but the
// formation orbit itself originates 40 units left/up from Pickles.
// This portable implementation approximates FSIN/FCOS/FSQRT/FTOL:
// exact x87 pixel parity still requires differential Win32 replay.
inline constexpr float kRetailFollowFastGain=0.835f;   // 0x43B3E8
inline constexpr float kRetailFollowFastCeiling=1.9f; // 0x43B3EC
inline constexpr float kRetailFollowSlowBand=15.f;    // 0x43B3A8
inline constexpr double kRetailFollowBrake=0.1;       // 0x43B400
inline constexpr double kRetailFollowFineBrake=0.01; // 0x43B370
inline constexpr double kRetailFollowTurnScale=0.7142857142857143; // 0x43B408
inline constexpr double kRetailFollowTurnThreshold=0.6; // 0x43B410
inline constexpr double kRetailFollowQuietSpeed=0.15;  // 0x43B3F0
static_assert(std::bit_cast<std::uint32_t>(kRetailFollowFastGain)==0x3f55c28fu);
static_assert(std::bit_cast<std::uint32_t>(kRetailFollowFastCeiling)==0x3ff33333u);

struct RetailOrdinaryFollowerStep {
    RetailEntityRecord32 moved{};
    Vec2i source_formation_target{};
    bool holds_formation_position{};
    int source_formation_ordinal{};
};

[[nodiscard]] inline std::optional<RetailOrdinaryFollowerStep>
original_herding_ordinary_follower_step(
    const RetailEntityRecord32& before,
    const RetailEntityRecord32& pickles,
    int formation_ordinal) noexcept {

    if(!is_herd_animal(before.entity_type()) ||
       before.behavior_state!=0 ||
       before.temporary_target_timer>0 ||
       pickles.entity_type()!=EntityType::FarmerPickles ||
       formation_ordinal<0 || formation_ordinal>=20 ||
       pickles.direction<0 || pickles.direction>=8 ||
       before.source_right<before.source_left ||
       before.source_bottom<before.source_top ||
       pickles.source_bottom<pickles.source_top ||
       !std::isfinite(before.x_float) ||
       !std::isfinite(before.y_float) ||
       !std::isfinite(before.movement_speed) ||
       before.movement_speed<0 ||
       before.animation_frame_countdown<
           std::numeric_limits<std::int32_t>::min()+200 ||
       before.animation_frame==std::numeric_limits<std::int32_t>::max())
        return std::nullopt;

    const std::int64_t ax=static_cast<std::int64_t>(before.x)+
        (static_cast<std::int64_t>(before.source_left)-
         before.source_right)/2;
    const std::int64_t ay=static_cast<std::int64_t>(before.y)+
        (static_cast<std::int64_t>(before.source_bottom)-
         before.source_top)/2;
    const std::int64_t hx=static_cast<std::int64_t>(pickles.x)-40;
    const std::int64_t hy=static_cast<std::int64_t>(pickles.y)+
        pickles.source_bottom-pickles.source_top-40;
    for(const auto value:{ax,ay,hx,hy}) {
        if(value<std::numeric_limits<std::int32_t>::min()+2000LL ||
           value>std::numeric_limits<std::int32_t>::max()-2000LL)
            return std::nullopt;
    }

    const double distance=std::hypot(
        static_cast<double>(ax-hx),static_cast<double>(ay-hy));
    if(!std::isfinite(distance) ||
       distance>std::numeric_limits<int>::max()-1024)
        return std::nullopt;
    // Source 0x417657 converts *difference* to int, THEN abs(),
    // and enters the no-translation/slow-braking branch for <=2.
    const int desired=formation_ordinal*30+15;
    const bool holds=std::abs(static_cast<int>(distance-desired))<=2;
    auto moved=before;
    Vec2i formation{static_cast<int>(hx),static_cast<int>(hy)};
    double speed=before.movement_speed;
    int heading=pickles.direction*45;
    int facing=pickles.direction;
    if(holds) {
        facing=pickles.direction;
        speed=speed>=kRetailFollowBrake
            ? speed-kRetailFollowBrake:0.0;
    } else {
        int orbit_degrees=(pickles.direction*5+20)*9;
        if(orbit_degrees>=360)orbit_degrees-=360;
        const int orbit_radius=formation_ordinal*25+15;
        const double radians=static_cast<double>(orbit_degrees)*
            static_cast<double>(kRetailHerdingRadiansPerDegree);
        const double tx=static_cast<double>(hx)+
            std::sin(radians)*orbit_radius;
        const double ty=static_cast<double>(hy)-
            std::cos(radians)*orbit_radius;
        if(!std::isfinite(tx)||!std::isfinite(ty))return std::nullopt;
        formation={static_cast<int>(tx),static_cast<int>(ty)};
        const auto source_heading=original_herding_integer_heading(
            static_cast<int>(ax),static_cast<int>(ay),
            formation.x,formation.y);
        if(!source_heading)return std::nullopt;
        heading=*source_heading;
        facing=(heading+22)/45;
        if(facing>=8)facing-=8;
        if(before.direction!=facing && speed>=kRetailFollowTurnThreshold)
            speed*=kRetailFollowTurnScale;
    }

    moved.direction=facing;
    // The branch re-evaluates slot spacing at 0x41782B even after
    // computing orbit direction. Speed gains depend on distance to
    // Pickles' SOURCE formation origin, not the selected orbit point.
    if(!holds) {
        const double gap=distance-(formation_ordinal*50+15);
        if(gap<=1.0) {
            speed=speed>=kRetailFollowBrake
                ? speed-kRetailFollowBrake:0.0;
        } else if(gap>=kRetailFollowSlowBand) {
            if(speed<kRetailFollowFastCeiling)
                speed=static_cast<float>(speed)+kRetailFollowFastGain;
        } else {
            if(speed>=kRetailFollowQuietSpeed)
                speed-=kRetailFollowFineBrake;
        }
        const auto step=original_herding_steering_step(
            before.x_float,before.y_float,heading,
            static_cast<float>(speed),false);
        if(!step)return std::nullopt;
        moved.x_float=step->x;
        moved.y_float=step->y;
        moved.x=step->rounded_x;
        moved.y=step->rounded_y;
        // 0x41792B: animation timer = truncate(old_timer -
        // source speed * 10). Nonmoving formation-hold skips this.
        moved.animation_frame_countdown=static_cast<int>(
            static_cast<double>(before.animation_frame_countdown)-
            speed*10.0);
        if(speed==0.0) {
            const int first=before.entity_type()==EntityType::Sheep?6:
                before.entity_type()==EntityType::Rabbit?3:0;
            moved.animation_frame=before.entity_type()==EntityType::Sheep
                ? first-1:first;
            moved.animation_frame_countdown=101;
        }
    }
    moved.movement_speed=static_cast<float>(speed);
    moved.movement_active=1;

    if(moved.animation_frame_countdown<=0) {
        moved.animation_frame_countdown=100;
        ++moved.animation_frame;
        const int first=before.entity_type()==EntityType::Sheep?6:
            before.entity_type()==EntityType::Rabbit?3:0;
        const int end=before.entity_type()==EntityType::Sheep?10:
            before.entity_type()==EntityType::Rabbit?7:6;
        if(moved.animation_frame>=end)moved.animation_frame=first;
    }
    return RetailOrdinaryFollowerStep{
        moved,formation,holds,formation_ordinal
    };
}

} // namespace btb::herding
