#include "btb/spud_maze_data.hpp"

#include <cassert>
#include <sstream>

using namespace btb::spud_maze;

int main() {
    std::ostringstream src;

    src << "WestScreen\n";
    src << "n0 0 100 200 8 256 -1 -1 -1 1\n";
    src << "NEXT_2nd Screen\n";
    src << "n0 0 20 30 2 128 -1 1 -1 -1\n";
    src << "NEXT_3rdScreen\n";
    src << "n0 0 40 50 1 0 1 -1 -1 -1\n";
    src << "NEXT_4thScreen\n";
    src << "n0 0 60 70 1 0 1 -1 -1 -1\n";
    src << "NEXT_LastRightScreen\n";
    src << "n0 0 80 90 1 0 1 -1 -1 -1\n";
    src << "END\n";

    for (int screen = 0; screen < 5; ++screen) {
        for (int i = 0; i < 4; ++i) {
            src << (screen*100+i) << ' '
                << (screen*100+i+10) << ' '
                << i << '\n';
        }
    }

    src << "2.0\n";
    src << "1 1 2\n";
    src << "1 1 2\n";
    src << "500 400 300\n";
    src << "-1 1000 200\n";
    src << "6\n";

    for (int count : {8,5,4,6,9}) {
        for (int i = 0; i < count; ++i) {
            src << i << ' ' << (i+1) << '\n';
        }
    }

    std::istringstream in(src.str());
    const auto data = parse_data(in);

    assert(data.screens[0].size() == 1);
    assert(data.screens[4].size() == 1);
    assert(data.screens[0][0].x == 100);
    assert(data.screens[0][0].y == 189);
    assert(data.screens[0][0].node_type == 256);
    assert(data.screens[1][0].node_type == 128);

    assert(data.references[0][0].x == 0);
    assert(data.references[4][3].x == 403);

    assert(data.file_player_speed == 2.0f);
    assert(data.retail_player_speed == 4.0f);
    assert((data.spud_speed_regular == std::array<std::int32_t,3>{1,1,2}));
    assert((data.spud_speed_package == std::array<std::int32_t,3>{1,1,2}));
    assert((data.timer_values == std::array<std::int32_t,3>{500,400,300}));
    assert((data.spud_spawn_times == std::array<std::int32_t,3>{-1,1000,200}));
    assert(data.spud_animation_delay == 6);

    assert(data.repair_points[0].size() == 8);
    assert(data.repair_points[1].size() == 5);
    assert(data.repair_points[2].size() == 4);
    assert(data.repair_points[3].size() == 6);
    assert(data.repair_points[4].size() == 9);
}
