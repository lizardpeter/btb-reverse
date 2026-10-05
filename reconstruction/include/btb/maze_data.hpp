#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace btb::maze {

enum class Screen : std::int32_t {
    West = 0,
    Middle = 1,
    East = 2,
};

inline constexpr std::int32_t kPortalToWest = 0x80;
inline constexpr std::int32_t kPortalToEast = 0x100;
inline constexpr std::int32_t kPortalToMiddle = 0x200;

enum Direction : std::int32_t {
    Up = 1,
    Right = 2,
    Down = 4,
    Left = 8,
};

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

struct Node {
    std::int32_t id{};
    Vec2i source_position{};
    Vec2i retail_position{};
    std::int32_t direction_bits{};
    std::int32_t node_type{};
    std::array<std::int32_t, 4> links{{-1, -1, -1, -1}};

    [[nodiscard]] bool allows(Direction direction) const noexcept {
        return (direction_bits & static_cast<std::int32_t>(direction)) != 0;
    }

    [[nodiscard]] std::int32_t linked_node(Direction direction) const noexcept {
        switch (direction) {
            case Up: return links[0];
            case Right: return links[1];
            case Down: return links[2];
            case Left: return links[3];
        }
        return -1;
    }

    [[nodiscard]] std::optional<Screen> portal_destination() const noexcept {
        // Preserve the retail branch priority from the runtime:
        // 0x80 -> West, 0x100 -> East, 0x200 -> Middle.
        if ((node_type & kPortalToWest) != 0) return Screen::West;
        if ((node_type & kPortalToEast) != 0) return Screen::East;
        if ((node_type & kPortalToMiddle) != 0) return Screen::Middle;
        return std::nullopt;
    }
};

struct ScreenGraph {
    std::string source_marker;
    std::vector<Node> nodes;
};

struct ReferenceNode {
    Vec2i position{};
    std::int32_t node_id{};
};

struct Data {
    std::array<ScreenGraph, 3> screens;
    std::array<std::array<ReferenceNode, 4>, 3> reference_nodes{};

    float player_speed{};
    std::array<std::int32_t, 3> spud_speed_regular{};
    std::array<std::int32_t, 3> spud_speed_package{};
    std::array<std::int32_t, 3> timer_by_difficulty{};
    std::array<std::int32_t, 3> spud_spawn_frames_by_difficulty{};
    std::int32_t spud_animation_delay{};
};

Data parse_data(const std::string& text);

} // namespace btb::maze
