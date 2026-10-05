#pragma once

#include <array>
#include <cstdint>
#include <istream>

namespace btb::bobs_band {

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

struct Box4i {
    std::int32_t a{};
    std::int32_t b{};
    std::int32_t c{};
    std::int32_t d{};
    friend bool operator==(const Box4i&, const Box4i&) = default;
};

struct MachineData {
    std::array<Vec2i, 10> machine_positions{};
    std::array<Vec2i, 10> machine_sizes{};
    std::array<Vec2i, 3> source_conductor_positions{};
    std::array<Vec2i, 3> source_conductor_sizes{};
    std::array<std::int32_t, 3> conductor_frame_counts{};
    std::array<Box4i, 10> sound_regions{};
};

MachineData parse_machine_data(std::istream& in);

} // namespace btb::bobs_band
