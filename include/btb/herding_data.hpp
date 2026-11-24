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

inline constexpr std::size_t kFarmerPicklesSetupPositionIndex = 3;

[[nodiscard]] constexpr bool fixed_setup_position_has_herding_consumer(
    std::size_t index) noexcept {
    // InitializeHerdingActivity directly reads only source pair #3 before
    // grouped parsing begins at pair #6. The other five fixed pairs have no
    // Herding runtime consumer in this retail executable.
    return index == kFarmerPicklesSetupPositionIndex;
}

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
