#pragma once

#include "btb/herding_source_registration_point.hpp"

#include <array>
#include <cstdint>
#include <optional>

namespace btb::herding {

// 00417433..00417517: wrong-home speech follows the ordinary
// follower's failed <150px home-registration check. The executable
// initializes four points at 0050B2FC (sheep, rabbit, duck, sheep).
// The loop increments before each test, skips type-1, runs twice,
// and clears only its final indexed latch if both tests miss.
// Keep the duplicated sheep point and latch zero, not a corrected
// species loop. 0043B41C contains IEEE float 200.0 (strict <200).
// The latches at 00510778..00510784 are process BSS, not reset on
// activity initialization. Each prompt is priority 10, class 1.
struct RetailWrongHomeLatches {
    std::array<bool,4> latched{};
    friend constexpr bool operator==(const RetailWrongHomeLatches&,const RetailWrongHomeLatches&)=default;
};

struct RetailWrongHomeStep {
    RetailWrongHomeLatches next{};
    std::array<int,2> requested_sound_ids{{-1,-1}};
    std::size_t count{};
};

[[nodiscard]] constexpr bool retail_home_within(Vec2i a,Vec2i b,int radius) noexcept {
    const auto dx=static_cast<std::int64_t>(a.x)-b.x;
    const auto dy=static_cast<std::int64_t>(a.y)-b.y;
    return dx>-radius&&dx<radius&&dy>-radius&&dy<radius&&
        dx*dx+dy*dy<static_cast<std::int64_t>(radius)*radius;
}

[[nodiscard]] constexpr std::optional<RetailWrongHomeStep>
original_herding_wrong_home_step(EntityType type,Vec2i anchor,
                                RetailWrongHomeLatches before) noexcept {
    const auto own=original_herding_species_registration_point(type);
    if(!own||retail_home_within(anchor,*own,150))return std::nullopt;
    constexpr std::array<Vec2i,4> points{{{736,183},{861,142},{1023,337},{736,183}}};
    RetailWrongHomeStep result{.next=before};
    const int skip=static_cast<int>(type)-1;
    int index=0;
    int near=0;
    for(int pass=0;pass<2;++pass) {
        ++index;
        if(index==skip)++index;
        if(retail_home_within(anchor,points[static_cast<std::size_t>(index)],200)) {
            ++near;
            if(!result.next.latched[static_cast<std::size_t>(index)])
                result.requested_sound_ids[result.count++]=index==3?0x3a9:0x3a9+index;
            result.next.latched[static_cast<std::size_t>(index)]=true;
        }
    }
    if(near==0)result.next.latched[static_cast<std::size_t>(index)]=false;
    return result;
}
} // namespace btb::herding
