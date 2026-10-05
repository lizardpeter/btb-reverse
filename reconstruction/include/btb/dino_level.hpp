#pragma once

#include <cstdint>
#include <istream>
#include <string>
#include <vector>

namespace btb::dino {

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};

    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

struct LevelData {
    std::vector<Vec2i> target_positions;
    std::vector<Vec2i> start_positions;
    std::vector<Vec2i> extra_positions;
    std::vector<std::int32_t> piece_permutation;

    [[nodiscard]] std::size_t piece_count() const noexcept {
        return piece_permutation.size();
    }

    [[nodiscard]] const Vec2i* runtime_animation_anchor() const noexcept {
        return extra_positions.empty() ? nullptr : &extra_positions.front();
    }
};

enum class Species : std::int32_t {
    Raptor = 0,
    Triceratops = 1,
    Tyrannosaurus = 2,
};

enum class Difficulty : std::int32_t {
    Easy = 0,
    Medium = 1,
    Hard = 2,
};

[[nodiscard]] constexpr std::int32_t level_index(
    Species species,
    Difficulty difficulty) noexcept {
    return static_cast<std::int32_t>(species)
         + 3 * static_cast<std::int32_t>(difficulty);
}

LevelData parse_level(std::istream& in);
LevelData parse_level_file(const std::string& path);

[[nodiscard]] bool cursor_near_point(
    Vec2i cursor,
    Vec2i point,
    std::int32_t tolerance) noexcept;

[[nodiscard]] bool cursor_inside_rect(
    Vec2i cursor,
    Vec2i top_left,
    Vec2i bottom_right) noexcept;

} // namespace btb::dino
