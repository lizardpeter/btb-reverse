#include "btb/player_profiles.hpp"

#include <cassert>
#include <sstream>

using namespace btb::profiles;

int main() {
    static_assert(kProfileCount == 5);
    static_assert(kMaxNameGlyphs == 9);

    ProfileTable profiles{};
    profiles[0].name_length = 3;
    profiles[0].badge_index = 4;
    profiles[0].name_glyph_codes[0] = 1;
    profiles[0].name_glyph_codes[1] = 2;
    profiles[0].name_glyph_codes[2] = 3;

    profiles[1].name_length = 2;
    profiles[1].badge_index = 7;
    profiles[1].name_glyph_codes[0] = 8;
    profiles[1].name_glyph_codes[1] = 9;

    for (std::size_t i = 2; i < kProfileCount; ++i) {
        profiles[i].name_length = 0;
        profiles[i].badge_index = -1;
    }

    std::ostringstream encoded;
    write_playerinfo(encoded, profiles);

    // All five headers come first, followed by variable-length name data.
    const auto text = encoded.str();
    assert(text.rfind("3 4 2 7 0 -1 0 -1 0 -1 ", 0) == 0);

    std::istringstream decoded(text);
    const auto roundtrip = read_playerinfo(decoded);

    assert(roundtrip[0].name_length == 3);
    assert(roundtrip[0].badge_index == 4);
    assert(roundtrip[0].name_glyph_codes[2] == 3);
    assert(roundtrip[1].name_length == 2);
    assert(roundtrip[1].badge_index == 7);
    assert(roundtrip[1].name_glyph_codes[1] == 9);
    assert(has_profile(roundtrip[0]));
    assert(!has_profile(roundtrip[4]));
}
