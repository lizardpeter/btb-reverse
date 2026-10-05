#pragma once

#include <array>
#include <cstdint>
#include <istream>
#include <vector>

namespace btb::herding {

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

struct Data {
    // The retail initializer consumes the first six pairs directly before it
    // begins processing sentinel-delimited coordinate groups.
    std::array<Vec2i, 6> setup_positions{};

    // Remaining herd.txt coordinates, split on the literal -1 -1 sentinels.
    // The shipped file contains four groups with sizes 12, 5, 6, and 1.
    std::vector<std::vector<Vec2i>> coordinate_groups;

    [[nodiscard]] std::vector<Vec2i> retail_transformed_group0() const;
};

Data parse_data(std::istream& in);

} // namespace btb::herding
