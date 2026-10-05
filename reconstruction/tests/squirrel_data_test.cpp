#include "btb/squirrel_data.hpp"

#include <cassert>
#include <sstream>

using namespace btb::squirrel;

int main() {
    std::istringstream in(R"(
0 3 10 27 8 -4 -4
0 0 1 -1 0 0 0
0 0 0 0 0 0 0
0 0 1 -17 0 0 0
0 0 1 -33 0 0 0
0 0 -1 17 0 0 0
0 0 19 13 0 0 0
0 0 0 -32 0 0 0
0 0 0 32 0 0 0
7 105 79
1 2 2 3
70
110 0
234 146
384 177
362 203
172 0
)");

    static_assert(kRetailLoadedIntegerCount == 71);

    const auto data = parse_retail_data(in);

    assert((data.phase_header ==
        std::array<std::int32_t,7>{0,3,10,27,8,-4,-4}));
    assert((data.phase_table[0] ==
        std::array<std::int32_t,7>{0,0,1,-1,0,0,0}));
    assert((data.phase_table[2] ==
        std::array<std::int32_t,7>{0,0,1,-17,0,0,0}));
    assert((data.phase_table[5] ==
        std::array<std::int32_t,7>{0,0,19,13,0,0,0}));
    assert((data.phase_table[7] ==
        std::array<std::int32_t,7>{0,0,0,32,0,0,0}));

    assert(data.animation_step_delay == 7);
    assert((data.discarded_scalars ==
        std::array<std::int32_t,2>{105,79}));
    assert((data.tuning_tuple ==
        std::array<std::int32_t,4>{1,2,2,3}));
    assert(data.alignment_offset == 70);

    // Prove the retail parser stopped at exactly the original executable's
    // boundary rather than accidentally consuming the legacy coordinate tail.
    const auto tail = read_unconsumed_legacy_pairs(in);
    assert(tail.size() == 5);
    assert((tail[0] == Vec2i{110,0}));
    assert((tail[1] == Vec2i{234,146}));
    assert((tail[2] == Vec2i{384,177}));
    assert((tail[3] == Vec2i{362,203}));
    assert((tail[4] == Vec2i{172,0}));
}
