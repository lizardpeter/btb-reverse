#include "btb/dino_level.hpp"

#include <fstream>
#include <set>
#include <stdexcept>
#include <utility>

namespace btb::dino {

LevelData parse_level(std::istream& in) {
    std::int32_t count = 0;
    if (!(in >> count) || count <= 0) {
        throw std::runtime_error("invalid Dinosaur piece count");
    }

    std::vector<Vec2i> coordinates;
    for (;;) {
        Vec2i p{};
        if (!(in >> p.x >> p.y)) {
            throw std::runtime_error("Dinosaur level ended before -1 -1");
        }
        if (p.x == -1 && p.y == -1) {
            break;
        }
        coordinates.push_back(p);
    }

    const auto required = static_cast<std::size_t>(count) * 2U;
    if (coordinates.size() < required) {
        throw std::runtime_error("Dinosaur level has too few coordinate pairs");
    }

    LevelData result;
    result.target_positions.assign(
        coordinates.begin(),
        coordinates.begin() + count);
    result.start_positions.assign(
        coordinates.begin() + count,
        coordinates.begin() + count * 2);
    result.extra_positions.assign(
        coordinates.begin() + count * 2,
        coordinates.end());

    result.piece_permutation.reserve(static_cast<std::size_t>(count));
    for (std::int32_t i = 0; i < count; ++i) {
        std::int32_t id = -1;
        if (!(in >> id)) {
            throw std::runtime_error("Dinosaur level has too few permutation entries");
        }
        result.piece_permutation.push_back(id);
    }

    std::int32_t unexpected = 0;
    if (in >> unexpected) {
        throw std::runtime_error("Dinosaur level has extra permutation data");
    }

    std::set<std::int32_t> observed(
        result.piece_permutation.begin(),
        result.piece_permutation.end());
    for (std::int32_t id = 0; id < count; ++id) {
        if (!observed.contains(id)) {
            throw std::runtime_error("Dinosaur piece permutation is invalid");
        }
    }
    if (observed.size() != static_cast<std::size_t>(count)) {
        throw std::runtime_error("Dinosaur piece permutation contains duplicates");
    }

    return result;
}

LevelData parse_level_file(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("could not open Dinosaur level: " + path);
    }
    return parse_level(in);
}

bool cursor_near_point(
    Vec2i cursor,
    Vec2i point,
    std::int32_t tolerance) noexcept {
    const auto dx = cursor.x >= point.x ? cursor.x - point.x : point.x - cursor.x;
    const auto dy = cursor.y >= point.y ? cursor.y - point.y : point.y - cursor.y;
    return dx <= tolerance && dy <= tolerance;
}

bool cursor_inside_rect(
    Vec2i cursor,
    Vec2i top_left,
    Vec2i bottom_right) noexcept {
    return cursor.x >= top_left.x
        && cursor.x <= bottom_right.x
        && cursor.y >= top_left.y
        && cursor.y <= bottom_right.y;
}

} // namespace btb::dino
