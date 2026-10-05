#include "btb/fireworks_layout.hpp"

#include <cassert>
#include <sstream>

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
    assert((layout.placement_runtime[0] == Recti{80, 20, 110, 80}));
    assert((layout.palette[0] == Recti{40, 300, 90, 390}));

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
