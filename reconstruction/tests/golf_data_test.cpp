#include "btb/golf_data.hpp"

#include <cassert>
#include <sstream>

using namespace btb::golf;

int main() {
    std::istringstream in(R"(
98 195
data\\subgamegolf\\golfsprite_8bit.bmp
45
159 184
16
91 140

330 178
data\\subgamegolf\\flag.bmp
99 160
22 143
15
3

230 15
data\\subgamegolf\\windmill.bmp
129 164
62 151
15
3

429 31
data\\subgamegolf\\clown.bmp
103 129
33 117
15
3

data\\subgamegolf\\wendy.bmp
20 105
88 130
100

data\\subgamegolf\\spud.bmp
92 20
97 100
100

data\\subgamegolf\\wendy.bmp
53 54
85 118
100

5 4 3
2 4 6

10 10
1
105 247
//Comments
)");

    const auto data = parse_data(in);

    assert((data.bob_position == Vec2i{98, 195}));
    assert(data.bob_rotation_count == 45);
    assert((data.bob_frame_size_from_file == Vec2i{159, 184}));
    assert((data.bob_frame_size_retail == Vec2i{133, 174}));
    assert(data.bob_frame_count == 16);

    assert((data.ball_offset_from_bob_from_file == Vec2i{91, 140}));
    assert((data.ball_offset_from_bob_retail == Vec2i{89, 135}));
    assert((data.initial_ball_position() == Vec2i{187, 330}));

    assert((data.course_objects[0].position == Vec2i{330, 178}));
    assert(data.course_objects[0].frame_count == 15);
    assert(data.course_objects[0].animation_repeats == 3);
    assert(data.course_objects[1].sprite_path.find("windmill") != std::string::npos);
    assert(data.course_objects[2].sprite_path.find("clown") != std::string::npos);

    assert(data.spectators[0].frame_count == 100);
    assert(data.spectators[1].sprite_path.find("spud") != std::string::npos);

    assert((data.attempts_by_difficulty == std::array<std::int32_t, 3>{5, 4, 3}));
    assert((data.power_bar_speed_by_difficulty == std::array<std::int32_t, 3>{2, 4, 6}));
    assert((data.ball_frame_size == Vec2i{10, 10}));
    assert(data.ball_frame_count == 1);
    assert((data.trailing_point == Vec2i{105, 247}));
    assert(!data.final_optional_pair_was_present);
}
