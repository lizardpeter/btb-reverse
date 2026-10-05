#include "btb/dino_level.hpp"

#include <cassert>
#include <sstream>

using namespace btb::dino;

int main() {
    // Exact Raptor Easy source data. This level is intentionally useful
    // because it has three extra coordinates rather than the usual two.
    std::istringstream in(R"(
7
149 147
215 161
223 220
275 225
330 228
274 288
382 199
362 396
33 92
21 199
32 314
121 376
210 405
504 330
456 248
80 300
100 120
-1 -1
6 3 0 1 4 2 5
)");

    const auto level = parse_level(in);
    assert(level.piece_count() == 7);
    assert(level.target_positions.size() == 7);
    assert(level.start_positions.size() == 7);
    assert(level.extra_positions.size() == 3);
    assert((level.target_positions.front() == Vec2i{149, 147}));
    assert((level.start_positions.front() == Vec2i{362, 396}));
    assert((*level.runtime_animation_anchor() == Vec2i{456, 248}));
    assert((level.piece_permutation == std::vector<std::int32_t>{6, 3, 0, 1, 4, 2, 5}));

    assert(level_index(Species::Raptor, Difficulty::Easy) == 0);
    assert(level_index(Species::Triceratops, Difficulty::Easy) == 1);
    assert(level_index(Species::Tyrannosaurus, Difficulty::Easy) == 2);
    assert(level_index(Species::Raptor, Difficulty::Hard) == 6);
    assert(level_index(Species::Tyrannosaurus, Difficulty::Hard) == 8);

    assert(cursor_near_point({100, 100}, {110, 90}, 10));
    assert(!cursor_near_point({100, 100}, {111, 90}, 10));
    assert(cursor_inside_rect({20, 30}, {20, 30}, {40, 50}));
    assert(!cursor_inside_rect({19, 30}, {20, 30}, {40, 50}));
}
