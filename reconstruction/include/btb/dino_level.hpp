#pragma once

#include <cstddef>
#include <cstdint>
#include <istream>
#include <span>
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

    // Dino initialization consumes only the first coordinate pair after the
    // two N-sized target/start blocks. It becomes the dormant special-render
    // anchor at globals 0x004FC400/0x004FC404.
    [[nodiscard]] const Vec2i* special_render_anchor() const noexcept {
        return extra_positions.empty() ? nullptr : &extra_positions.front();
    }

    // Kept as a compatibility alias for the earlier reconstruction name.
    [[nodiscard]] const Vec2i* runtime_animation_anchor() const noexcept {
        return special_render_anchor();
    }

    // Exhaustive Dino-module xrefs show no reader for any later extra pair.
    [[nodiscard]] std::span<const Vec2i> legacy_unused_tail() const noexcept {
        return extra_positions.size() <= 1
            ? std::span<const Vec2i>{}
            : std::span<const Vec2i>{
                  extra_positions.data() + 1,
                  extra_positions.size() - 1};
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
    return 3 * static_cast<std::int32_t>(species)
         + static_cast<std::int32_t>(difficulty);
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
