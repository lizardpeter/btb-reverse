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

    static_assert(kMotionSubstepCount == 7);
    static_assert(kVerticalMotionProfileCount == 8);

    assert((data.horizontal_motion_deltas ==
        std::array<std::int32_t,7>{0,3,10,27,8,-4,-4}));
    assert((data.vertical_motion_deltas[0] ==
        std::array<std::int32_t,7>{0,0,1,-1,0,0,0}));
    assert((data.vertical_motion_deltas[2] ==
        std::array<std::int32_t,7>{0,0,1,-17,0,0,0}));
    assert((data.vertical_motion_deltas[5] ==
        std::array<std::int32_t,7>{0,0,19,13,0,0,0}));
    assert((data.vertical_motion_deltas[7] ==
        std::array<std::int32_t,7>{0,0,0,32,0,0,0}));

    assert((motion_delta(
        data, VerticalMotionProfile::Up16Arc, 2) == Vec2i{10,1}));
    assert((motion_delta(
        data, VerticalMotionProfile::Up16Arc, 3) == Vec2i{27,-17}));

    // Every profile shares the same net +40 horizontal movement. The eight
    // rows encode two neutral choices plus +/-16 and +/-32 vertical outcomes.
    assert((motion_profile_net_delta(
        data, VerticalMotionProfile::NeutralArc) == Vec2i{40,0}));
    assert((motion_profile_net_delta(
        data, VerticalMotionProfile::NeutralFlat) == Vec2i{40,0}));
    assert((motion_profile_net_delta(
        data, VerticalMotionProfile::Up16Arc) == Vec2i{40,-16}));
    assert((motion_profile_net_delta(
        data, VerticalMotionProfile::Up32Arc) == Vec2i{40,-32}));
    assert((motion_profile_net_delta(
        data, VerticalMotionProfile::Down16Arc) == Vec2i{40,16}));
    assert((motion_profile_net_delta(
        data, VerticalMotionProfile::Down32Arc) == Vec2i{40,32}));
    assert((motion_profile_net_delta(
        data, VerticalMotionProfile::Up32Direct) == Vec2i{40,-32}));
    assert((motion_profile_net_delta(
        data, VerticalMotionProfile::Down32Direct) == Vec2i{40,32}));

    assert(data.animation_step_delay == 7);
    assert((data.discarded_scalars ==
        std::array<std::int32_t,2>{105,79}));
    assert((data.loaded_unused_scalars ==
        std::array<std::int32_t,4>{1,2,2,3}));
    assert(data.initial_horizontal_alignment_offset == 70);

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
