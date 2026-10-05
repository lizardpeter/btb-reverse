#include "btb/bobs_band_data.hpp"

#include <array>
#include <cassert>
#include <sstream>

using namespace btb::bobs_band;

int main() {
    std::istringstream in(R"(
0 138
0 140
98 117
100 120
219 1
228 25
349 113
352 123
410 70
406 81
168 157
216 166
174 128
194 144
142 216
169 191
73 95
63 78
105 136
121 139
525 141
519 145
524 142
56 71
56 68
52 72
59 59 64
259 264 319 290
206 246 258 289
323 248 378 276
264 224 318 262
353 219 397 247
320 196 351 245
413 193 445 232
354 183 409 217
493 200 525 248
448 193 491 252
)");

    const auto data = parse_machine_data(in);

    assert((data.machine_positions[0] == Vec2i{0,138}));
    assert((data.machine_positions[9] == Vec2i{406,81}));
    assert((data.machine_sizes[0] == Vec2i{168,157}));
    assert((data.machine_sizes[9] == Vec2i{121,139}));

    assert((data.source_conductor_positions[0] == Vec2i{525,141}));
    assert((data.source_conductor_positions[2] == Vec2i{524,142}));
    assert((data.source_conductor_sizes[0] == Vec2i{56,71}));
    assert((data.source_conductor_sizes[2] == Vec2i{52,72}));

    assert((data.conductor_frame_counts ==
        std::array<std::int32_t,3>{59,59,64}));

    assert((data.sound_regions[0] == Box4i{259,264,319,290}));
    assert((data.sound_regions[9] == Box4i{448,193,491,252}));
}
