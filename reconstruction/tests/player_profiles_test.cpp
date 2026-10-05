#include "btb/player_profiles.hpp"

#include <cassert>
#include <sstream>

using namespace btb::profiles;

int main() {
    static_assert(kProfileCount == 5);
    static_assert(kMaxNameGlyphs == 9);
    static_assert(kMaxEnteredNameGlyphs == 8);
    static_assert(kBadgeCount == 6);
    static_assert(kBadgeWidth == 50);
    static_assert(previous_badge(0) == 5);
    static_assert(previous_badge(4) == 3);
    static_assert(next_badge(5) == 0);
    static_assert(next_badge(2) == 3);
    constexpr auto badge0 = badge_source_rect(0);
    static_assert(badge0.left == 0 && badge0.right == 50);
    static_assert(badge0.top == 45 && badge0.bottom == 103);
    constexpr auto badge5 = badge_source_rect(5);
    static_assert(badge5.left == 250 && badge5.right == 300);

    static_assert(hit_test_profile_screen(92, 180) == 0);
    static_assert(hit_test_profile_screen(160, 320) == 1);
    static_assert(hit_test_profile_screen(292, 200) == 2);
    static_assert(hit_test_profile_screen(390, 320) == 3);
    static_assert(hit_test_profile_screen(530, 240) == 4);
    static_assert(hit_test_profile_screen(40, 445) ==
        static_cast<int>(ProfileScreenTarget::Help));
    static_assert(hit_test_profile_screen(320, 445) ==
        static_cast<int>(ProfileScreenTarget::Delete));
    static_assert(hit_test_profile_screen(600, 445) ==
        static_cast<int>(ProfileScreenTarget::Back));

    // Retail uses strict interior comparisons, so boundaries do not hit.
    static_assert(hit_test_profile_screen(42, 180) == -1);
    static_assert(hit_test_profile_screen(92, 140) == -1);
    static_assert(kDirectInputBackspace == 0x0E);
    static_assert(kDirectInputLeftShift == 0x2A);
    static_assert(kDirectInputRightShift == 0x36);

    static_assert(profile_name_glyph_for_input(0x1E, 'A', 0) == ('A' - 0x21));
    static_assert(profile_name_glyph_for_input(0x35, '/', 7) == ('/' - 0x21));
    static_assert(profile_name_glyph_for_input(0x36, 'A', 0) == -1);
    static_assert(profile_name_glyph_for_input(0x0E, 'A', 0) == -1);
    static_assert(profile_name_glyph_for_input(0x1E, 'A', 8) == -1);
    static_assert(profile_name_glyph_for_input(0x1E, 0x21, 0) == -1);
    static_assert(profile_name_glyph_for_input(0x1E, 0x80, 0) == -1);

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

    ProfileInfo badge_only{};
    badge_only.name_length = 0;
    badge_only.badge_index = 3;
    assert(has_profile(badge_only));

    auto mutable_profiles = roundtrip;
    std::array<btb::progress::Record, kProfileCount> progress{};
    mutable_profiles[0].name_glyph_codes[0] = 1234;
    for (auto& value : progress[0].values) {
        value = 7;
    }

    delete_profile_in_memory(mutable_profiles, progress, 0);
    assert(mutable_profiles[0].name_length == 0);
    assert(mutable_profiles[0].badge_index == -1);
    // Retail leaves the fixed 9-int backing slot untouched.
    assert(mutable_profiles[0].name_glyph_codes[0] == 1234);

    assert(progress[0].values[49] == 7);
    for (std::size_t i = 50; i <= 64; ++i) {
        assert(progress[0].values[i] == 0);
    }
    assert(progress[0].values[65] == 7);

    const auto files = activity_files_for_profile(2);
    assert(files[0] == "dypdata3.txt");
    assert(files[1] == "firedata3.txt");
    assert(files[2] == "musicbob3.txt");
    assert(files[3] == "musicwendy3.txt");
    assert(files[4] == "musicfarmer3.txt");
}
