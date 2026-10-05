#include "btb/fireworks_layout.hpp"

#include <cassert>
#include <cstdint>
#include <sstream>
#include <string>

using namespace btb::fireworks;

int main() {
    // 18 placement rectangles. The first record intentionally has a
    // nonsensical redundant fourth vertex; retail ignores it.
    std::ostringstream placement_text;
    for (int i = 0; i < 18; ++i) {
        const int l = 100 + i * 10;
        const int t = 20 + i;
        const int r = l + 30;
        const int b = t + 60;
        placement_text
            << l << ' ' << t << '\n'
            << r << ' ' << t << '\n'
            << r << ' ' << b << '\n'
            << (i == 0 ? 7 : l) << ' ' << (i == 0 ? -999 : b) << "\n\n";
    }

    std::ostringstream palette_text;
    for (int i = 0; i < 12; ++i) {
        const int l = 40 + i * 5;
        const int t = 300;
        const int r = l + 50;
        const int b = 390;
        palette_text
            << l << ' ' << t << '\n'
            << r << ' ' << t << '\n'
            << r << ' ' << b << '\n'
            << l << ' ' << b << "\n\n";
    }

    std::istringstream placement(placement_text.str());
    std::istringstream palette(palette_text.str());
    const auto layout = parse_layout(placement, palette);

    assert((layout.placement_source[0] == Recti{100, 20, 130, 80}));
    assert((layout.placement_runtime[0] == Recti{110, 0, 140, 60}));
    assert((layout.palette[0] == Recti{40, 300, 90, 390}));

    assert(layout.interactive_regions.size() == 33);
    assert(layout.interactive_regions[0].action ==
           static_cast<std::int32_t>(EditorAction::PlacementSlot));
    assert(layout.interactive_regions[18].action == 0);
    assert(layout.interactive_regions[29].action == 11);
    assert((layout.interactive_regions[30] ==
            InteractiveRegion{{292, 416, 346, 472},
                              static_cast<std::int32_t>(EditorAction::Play)}));
    assert((layout.interactive_regions[31] ==
            InteractiveRegion{{102, 416, 156, 472},
                              static_cast<std::int32_t>(EditorAction::DeleteAll)}));
    assert((layout.interactive_regions[32] ==
            InteractiveRegion{{483, 416, 537, 472},
                              static_cast<std::int32_t>(EditorAction::DeleteSelected)}));

    RetailGrid saved{};
    saved.fill(-1);
    saved[0] = 0;
    saved[5] = 11;
    saved[17] = 7;
    std::ostringstream binary(std::ios::binary);
    write_retail_grid(binary, saved);
    assert(binary.str().size() == 18 * 4);

    std::istringstream binary_in(binary.str(), std::ios::binary);
    const auto loaded = read_retail_grid(binary_in);
    assert(loaded == saved);
    const auto raw = binary.str();
    assert(static_cast<unsigned char>(raw[0]) == 0);
    assert(static_cast<unsigned char>(raw[1]) == 0);
    assert(static_cast<unsigned char>(raw[2]) == 0);
    assert(static_cast<unsigned char>(raw[3]) == 0);
    assert(static_cast<unsigned char>(raw[4]) == 0xff);
    assert(static_cast<unsigned char>(raw[5]) == 0xff);
    assert(static_cast<unsigned char>(raw[6]) == 0xff);
    assert(static_cast<unsigned char>(raw[7]) == 0xff);

    assert(retail_grid_filename(0) == "firedata1.txt");
    assert(retail_grid_filename(4) == "firedata5.txt");
    assert(retail_grid_filename(5).empty());

    assert(movie_index(FireworkType::RedAirbomb, false) == 0);
    assert(movie_index(FireworkType::RedAirbomb, true) == 12);
    assert(movie_index(FireworkType::BlueAirbomb, true) == 19);
    assert(movie_index(FireworkType::RedCandle, false) == 8);
    assert(movie_index(FireworkType::RedCandle, true) == 8);
    assert(movie_index(FireworkType::BlueCandle, true) == 11);

    static_assert(kTopMiddleMovieIndex == 20);
    static_assert(kCrowdLoopMovieIndex == 21);
    static_assert(kCrowdEndMovieIndex == 22);
}
