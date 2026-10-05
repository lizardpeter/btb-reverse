#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>

namespace btb::grand_opening {

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

struct RectI {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
    friend bool operator==(const RectI&, const RectI&) = default;
};

inline constexpr std::size_t kMachineCount = 10;
inline constexpr std::size_t kConductorCount = 3;
inline constexpr std::size_t kStaticAnimationCount = 10;
inline constexpr std::size_t kRetailLoadedIntegerCount = 95;

struct MachineData {
    std::array<Vec2i, kMachineCount> machine_positions{};
    std::array<Vec2i, kMachineCount> machine_sizes{};

    // LoadGrandOpeningMachineData parses these into local stack arrays but
    // never copies them into persistent runtime storage in this retail build.
    std::array<Vec2i, kConductorCount> parsed_but_unused_conductor_positions{};
    std::array<Vec2i, kConductorCount> parsed_but_unused_conductor_sizes{};

    std::array<std::int32_t, kConductorCount> conductor_frame_counts{};
    std::array<RectI, kStaticAnimationCount> static_animation_rects{};
};

MachineData parse_machine_data(std::istream& in);

} // namespace btb::grand_opening
