#include "btb/herding_data.hpp"

#include <array>
#include <cassert>
#include <sstream>

using namespace btb::herding;

int main() {
    std::istringstream in(R"(
719 266
527 114
376 170
478 471
93 324
100 120
44 99 202 99 353 279 461 263 530 299 966 181 966 166 966 235 1086 349 1257 405
1257 900 44 900
-1 -1
450 360 600 179 886 139 1028 304 755 360 -1 -1
30 500 500 500 500 500 1200 500 1200 850 30 850 -1 -1
92 324 -1 -1
)");

    const auto data = parse_data(in);

    assert((data.setup_positions[0] == Vec2i{719, 266}));
    assert((data.setup_positions[3] == Vec2i{478, 471}));
    assert((data.setup_positions[5] == Vec2i{100, 120}));
    static_assert(kFarmerPicklesSetupPositionIndex == 3);
    static_assert(!fixed_setup_position_has_herding_consumer(0));
    static_assert(!fixed_setup_position_has_herding_consumer(1));
    static_assert(!fixed_setup_position_has_herding_consumer(2));
    static_assert(fixed_setup_position_has_herding_consumer(3));
    static_assert(!fixed_setup_position_has_herding_consumer(4));
    static_assert(!fixed_setup_position_has_herding_consumer(5));

    assert(data.coordinate_groups.size() == 4);
    assert(data.coordinate_groups[0].size() == 12);
    assert(data.coordinate_groups[1].size() == 5);
    assert(data.coordinate_groups[2].size() == 6);
    assert(data.coordinate_groups[3].size() == 1);

    const auto transformed = data.retail_transformed_group0();
    assert((transformed[0] == Vec2i{-20, -1}));
    assert((transformed[1] == Vec2i{138, -1}));
    assert((transformed.back() == Vec2i{-20, 800}));

    assert((data.coordinate_groups[1][0] == Vec2i{450, 360}));
    assert((data.coordinate_groups[2][0] == Vec2i{30, 500}));
    assert((data.coordinate_groups[3][0] == Vec2i{92, 324}));
}
