#pragma once

#include "btb/retail_crt_random.hpp"

#include <array>
#include <cstdint>
#include <optional>

namespace btb::herding {

// Original animal idle / reaction voice branch at 0x00417B1D..0x417BB0.
// Each species has three adjacent source sound IDs:
//    base 0x331 (817) + species_group*3 + rand()%3
//
// Source calls rand()%8 FIRST. If it is zero, it immediately issues
// rand()%3 and plays that sound without checking existing voices.
// Otherwise it calls the sound manager's 0x402C60 status operation for
// all three sounds, and plays a random one only when NONE is active.
//
// Retains actual original priority 10, arbitration class 0
// (two pushes 0x0,0xA before 0x402CF0 play call).
// The SoundManager statuses belong to the native device; this pure
// source helper must never fabricate them or use host elapsed time.
struct RetailAnimalChatterStep {
    std::optional<int> requested_sound_id{};
    int source_priority{10};
    int source_class{0};
    int random_calls{};
    bool used_random_eighth_override{};
    bool consulted_sound_group_status{};
};

[[nodiscard]] constexpr std::optional<RetailAnimalChatterStep>
original_herding_animal_chatter(
    retail::OriginalRetailRandom& global_rand,
    int species_group,
    std::array<bool,3> group_sound_playing) noexcept {

    if (species_group<0 || species_group>2) {
        return std::nullopt;
    }

    const int eighth=global_rand.next_rand()%8;
    const bool immediate=eighth==0;
    if (!immediate &&
        (group_sound_playing[0] || group_sound_playing[1] ||
         group_sound_playing[2])) {
        return RetailAnimalChatterStep{
            std::nullopt,10,0,1,false,true
        };
    }
    const auto choice=global_rand.next_rand()%3;
    return RetailAnimalChatterStep{
        0x331+species_group*3+choice,
        10,0,2,immediate,!immediate
    };
}

} // namespace btb::herding
