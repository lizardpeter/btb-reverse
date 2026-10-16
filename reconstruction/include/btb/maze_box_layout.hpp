#pragma once

#include "btb/maze_spud_controller.hpp"
#include "btb/maze_data.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <stdexcept>

namespace btb::maze {

// Source 0x0041A730 allocates four 20-byte package slots per screen.
// It fills 1/2/1, 3/3/2, or 4/4/4 slots from left/middle/right according
// to difficulty. For every filled screen the retail executable obtains a
// location index via repeated rand()%4 until it is unique on that screen.
// The remaining unused slots keep the original -1 sentinel and status 1.
inline constexpr std::array<std::array<int,3>,3>
kSourceBoxCounts{{
    {{1,2,1}}, {{3,3,2}}, {{4,4,4}}
}};

struct BoxAllocation {
    std::array<std::array<SpudPackageSlot,4>,3> screens{};
    int total{};
    int remaining{};
};

// Caller supplies original CRT rand() results; does not force an invented
// seed or a different PRNG. An exhausted/degenerate provider is a hard
// failure, not permission to silently pick a different retail layout.
template<class RandomSource>
[[nodiscard]] BoxAllocation allocate_source_boxes(
    int difficulty,RandomSource&& next_random) {
    if(difficulty<0||difficulty>2)
        throw std::invalid_argument("original Maze difficulty must be 0..2");
    BoxAllocation allocation;
    for(auto& bank:allocation.screens)
        for(auto& slot:bank)slot={-1,kSpudAvailablePackageStatus};
    const auto& counts=kSourceBoxCounts[static_cast<std::size_t>(difficulty)];
    for(std::size_t screen=0;screen<counts.size();++screen) {
        for(int slot=0;slot<counts[screen];++slot) {
            int location=-1;
            // Retail samples again when a chosen index already appears
            // earlier in this screen. Keep the collision-driven call count.
            for(int retries=0;retries<4096;++retries) {
                const auto candidate=
                    static_cast<int>(next_random()&3U);
                bool duplicate=false;
                for(int earlier=0;earlier<slot;++earlier)
                    if(allocation.screens[screen][earlier].node_id==candidate)
                        duplicate=true;
                if(!duplicate) {
                    location=candidate;
                    break;
                }
            }
            if(location<0)
                throw std::runtime_error(
                    "original Maze package RNG did not yield a free location");
            allocation.screens[screen][static_cast<std::size_t>(slot)] = {
                location,kSpudAvailablePackageStatus
            };
            ++allocation.total;
        }
    }
    allocation.remaining=allocation.total;
    return allocation;
}
// Exact 0x0041B140 source global at 0x0043B460, read from matching
// retail PE SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
// IEEE754 bits 0x42380000 == 46.0f, with STRICT less-than comparison.
inline constexpr std::int32_t kRetailPackagePickupRadius = 46;

struct PackagePickupResult {
    int collected{};
    int remaining{};
    bool begin_player_collection_animation{};
};

// The retail player routine uses __ftol (truncate toward zero) for actor
// coordinates, then scans all four 20-byte package slots of the active
// screen. Available box status 1 becomes 0, and 0x00510BA8 decrements for
// EACH close package. Spud-dropped status 3 requires an actual NPC supplied
// dropped-position record; no invented position is used in this function.
[[nodiscard]] inline PackagePickupResult pickup_source_packages(
    BoxAllocation& allocation,const Data& data,int player_screen,
    float player_x,float player_y) noexcept {
    PackagePickupResult out{0,allocation.remaining,false};
    if(player_screen<0||player_screen>=3 ||
       !std::isfinite(player_x)||!std::isfinite(player_y))
        return out;
    const auto px=static_cast<std::int64_t>(
        static_cast<std::int32_t>(player_x));
    const auto py=static_cast<std::int64_t>(
        static_cast<std::int32_t>(player_y));
    const auto screen=static_cast<std::size_t>(player_screen);
    for(auto& slot:allocation.screens[screen]) {
        if(slot.node_id<0 || slot.node_id>=4 ||
           slot.status!=kSpudAvailablePackageStatus)continue;
        const auto point=data.box_locations[screen][
            static_cast<std::size_t>(slot.node_id)].position;
        const auto dx=px-point.x,dy=py-point.y;
        if(dx*dx+dy*dy >=
           static_cast<std::int64_t>(kRetailPackagePickupRadius)*
           kRetailPackagePickupRadius)continue;
        slot.status=0;
        --allocation.remaining;
        ++out.collected;
        out.begin_player_collection_animation=true;
    }
    out.remaining=allocation.remaining;
    return out;
}

} // namespace btb::maze
