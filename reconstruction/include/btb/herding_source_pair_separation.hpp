#pragma once

#include "btb/herding_source_registration_point.hpp"
#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"
#include <bit>
#include <cmath>
#include <limits>
#include <optional>
#include <span>

namespace btb::herding {

// 00417C85..00417F68: the current nonfollowing animal, if its
// movement latch was initially clear, scans ONLY subsequent records.
// 00417D83..00417F4E handles live species 1..3. Legacy types 4..6
// have a separate rectangle path but are never spawned by the retail
// initializer. The later animal's follower/behavior/latch flags do not
// filter the pair. Do not turn this into a symmetric all-pairs solver.
inline constexpr float kRetailPairSeparationDistance=23.0f;
inline constexpr float kRetailPairSeparationStep=0.9f;
static_assert(std::bit_cast<std::uint32_t>(kRetailPairSeparationStep)==0x3f666666u);

struct RetailPairSeparation {
    RetailEntityRecord32 current{};
    RetailEntityRecord32 later{};
    bool separated{};
};

[[nodiscard]] inline std::optional<RetailPairSeparation>
original_herding_pair_separation(const RetailEntityRecord32& current,
                                  const RetailEntityRecord32& later) noexcept {
    if(!is_herd_animal(current.entity_type()) ||
       !is_herd_animal(later.entity_type()))return std::nullopt;
    const auto a=original_herding_animal_tracking_anchor(current);
    const auto b=original_herding_animal_tracking_anchor(later);
    if(!a||!b)return std::nullopt;
    const auto dx=static_cast<std::int64_t>(a->x)-b->x;
    const auto dy=static_cast<std::int64_t>(a->y)-b->y;
    if(dx<=-23||dx>=23||dy<=-23||dy>=23||dx*dx+dy*dy>=529)
        return RetailPairSeparation{current,later,false};
    const auto heading=original_herding_integer_heading(b->x,b->y,a->x,a->y);
    if(!heading)return std::nullopt;
    const auto radians=static_cast<double>(*heading)*kRetailHerdingRadiansPerDegree;
    // Original FSIN spills to binary32 at 417E96; FCOS stays on x87.
    const auto sx=static_cast<double>(static_cast<float>(std::sin(radians)))*kRetailPairSeparationStep;
    const auto cy=std::cos(radians)*kRetailPairSeparationStep;
    auto result=RetailPairSeparation{current,later,true};
    const auto move=[](RetailEntityRecord32& animal,double x,double y) {
        animal.x_float=static_cast<float>(static_cast<double>(animal.x_float)+x);
        animal.y_float=static_cast<float>(static_cast<double>(animal.y_float)+y);
        if(!std::isfinite(animal.x_float)||!std::isfinite(animal.y_float)||
           animal.x_float<static_cast<double>(std::numeric_limits<int>::min())||
           animal.x_float>static_cast<double>(std::numeric_limits<int>::max())||
           animal.y_float<static_cast<double>(std::numeric_limits<int>::min())||
           animal.y_float>static_cast<double>(std::numeric_limits<int>::max()))return false;
        animal.x=static_cast<int>(animal.x_float);
        animal.y=static_cast<int>(animal.y_float);
        animal.movement_active=1;
        animal.movement_speed=0.0f;
        return true;
    };
    if(!move(result.current,sx,-cy)||!move(result.later,-sx,cy))return std::nullopt;
    // The immediate retail write is raw 0..8; the outer dispatch
    // subsequently normalizes the current direction, not this kernel.
    result.current.direction=(*heading+22)/45;
    result.later.direction=(result.current.direction+4)%8;
    return result;
}

// Ordered pure kernel. Eligibility is checked exactly once on entry:
// a first successful separation sets the current latch but does NOT
// terminate the scan of subsequent animals.
[[nodiscard]] inline std::optional<std::size_t>
original_herding_pair_scan(std::span<RetailEntityRecord32> records,
                           std::size_t index,bool following) noexcept {
    if(index>=records.size()||!is_herd_animal(records[index].entity_type()))
        return std::nullopt;
    if(following||records[index].movement_active!=0)return 0;
    std::size_t separated=0;
    for(std::size_t j=index+1;j<records.size();++j) {
        if(!is_herd_animal(records[j].entity_type()))continue;
        const auto pair=original_herding_pair_separation(records[index],records[j]);
        if(!pair)return std::nullopt;
        if(!pair->separated)continue;
        records[index]=pair->current;
        records[j]=pair->later;
        ++separated;
    }
    return separated;
}

} // namespace btb::herding
