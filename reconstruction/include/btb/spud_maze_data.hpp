#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <string>
#include <vector>

namespace btb::spud_maze {

inline constexpr std::size_t kScreenCount = 5;
inline constexpr std::size_t kMaxNodesPerScreen = 30;
inline constexpr std::size_t kReferenceTriplesPerScreen = 4;
inline constexpr std::size_t kDifficultyCount = 3;
inline constexpr std::array<std::size_t, kScreenCount> kRepairPointCounts{
    8, 5, 4, 6, 9
};
inline constexpr std::size_t kRepairPointCount = 32;

struct Node {
    std::int32_t index{};
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t direction_bits{};
    std::int32_t node_type{};
    std::array<std::int32_t, 4> links{{-1,-1,-1,-1}};
};

struct ReferenceTriple {
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t node{};
};

struct Data {
    std::array<std::vector<Node>, kScreenCount> screens{};
    std::array<
        std::array<ReferenceTriple, kReferenceTriplesPerScreen>,
        kScreenCount> references{};

    float file_player_speed{};
    float retail_player_speed{4.0f};

    std::array<std::int32_t, kDifficultyCount> spud_speed_regular{};
    std::array<std::int32_t, kDifficultyCount> spud_speed_package{};
    std::array<std::int32_t, kDifficultyCount> timer_values{};
    std::array<std::int32_t, kDifficultyCount> spud_spawn_times{};

    std::int32_t spud_animation_delay{};

    std::array<std::vector<std::array<std::int32_t,2>>, kScreenCount>
        repair_points{};
};

Data parse_data(std::istream& in);

} // namespace btb::spud_maze
