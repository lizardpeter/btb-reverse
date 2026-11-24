#pragma once

#include "btb/maze_spud_controller.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
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
} // namespace btb::maze
