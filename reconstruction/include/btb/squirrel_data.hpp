#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <vector>

namespace btb::squirrel {

inline constexpr std::size_t kMotionSubstepCount = 7;
inline constexpr std::size_t kVerticalMotionProfileCount = 8;
inline constexpr std::size_t kRetailLoadedIntegerCount = 71;

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

enum class VerticalMotionProfile : std::size_t {
    NeutralArc = 0,   // net Y 0: 0,0,+1,-1,0,0,0
    NeutralFlat = 1,  // net Y 0
    Up16Arc = 2,      // net Y -16
    Up32Arc = 3,      // net Y -32
    Down16Arc = 4,    // net Y +16
    Down32Arc = 5,    // net Y +32
    Up32Direct = 6,   // net Y -32 at substep 3
    Down32Direct = 7, // net Y +32 at substep 3
};

// Exact motion data consumed by 0x00425F40 UpdateAndDrawSquirrelRunAssembly.
// Every animated run movement uses the same seven horizontal deltas and one of
// eight seven-step vertical profiles. Retail executes substeps 0..2 in the
// first movement phase and 3..6 in the second.
struct Data {
    std::array<std::int32_t, kMotionSubstepCount>
        horizontal_motion_deltas{};
    std::array<
        std::array<std::int32_t, kMotionSubstepCount>,
        kVerticalMotionProfileCount>
        vertical_motion_deltas{};

    std::int32_t animation_step_delay{}; // shipped value 7

    // Retail reads these two integers into a stack temporary and discards them.
    std::array<std::int32_t, 2> discarded_scalars{};

    // These four values are stored at 0x51505C, 0x515060, 0x515064, 0x514F04,
    // but an exhaustive executable xref sweep finds no later read of any of
    // those globals in this build.
    std::array<std::int32_t, 4> loaded_unused_scalars{};

    // Stored at 0x515084. InitializeSquirrelActivity subtracts it from the
    // fixed run X base before writing 0x514F94. Shipped value 70.
    std::int32_t initial_horizontal_alignment_offset{};
};

[[nodiscard]] constexpr Vec2i motion_delta(
    const Data& data,
    VerticalMotionProfile profile,
    std::size_t substep) noexcept {
    return {
        data.horizontal_motion_deltas[substep],
        data.vertical_motion_deltas[
            static_cast<std::size_t>(profile)][substep],
    };
}

[[nodiscard]] constexpr Vec2i motion_profile_net_delta(
    const Data& data,
    VerticalMotionProfile profile) noexcept {
    Vec2i total{};
    const auto row =
        static_cast<std::size_t>(profile);
    for (std::size_t i = 0; i < kMotionSubstepCount; ++i) {
        total.x += data.horizontal_motion_deltas[i];
        total.y += data.vertical_motion_deltas[row][i];
    }
    return total;
}

Data parse_retail_data(std::istream& in);

// The shipped file contains five extra coordinate pairs after the exact retail
// read boundary. The 2002 executable closes the file before reading them.
// This helper exists only to inventory/preserve those legacy bytes in tooling.
std::vector<Vec2i> read_unconsumed_legacy_pairs(std::istream& in);

} // namespace btb::squirrel
