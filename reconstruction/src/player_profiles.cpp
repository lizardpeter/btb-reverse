#include "btb/player_profiles.hpp"

#include <stdexcept>

namespace btb::profiles {

ProfileTable read_playerinfo(std::istream& in) {
    ProfileTable profiles{};

    // Retail first reads all five (length, badge) headers.
    for (auto& profile : profiles) {
        if (!(in >> profile.name_length >> profile.badge_index)) {
            throw std::runtime_error("short playerinfo.txt profile header");
        }
        if (profile.name_length < 0 ||
            profile.name_length > static_cast<std::int32_t>(kMaxNameGlyphs)) {
            throw std::runtime_error("playerinfo.txt name length out of range");
        }
    }

    // Then it reads each profile's glyph codes.
    for (auto& profile : profiles) {
        for (std::int32_t i = 0; i < profile.name_length; ++i) {
            if (!(in >> profile.name_glyph_codes[static_cast<std::size_t>(i)])) {
                throw std::runtime_error("short playerinfo.txt name data");
            }
        }
    }

    return profiles;
}

void write_playerinfo(
    std::ostream& out,
    const ProfileTable& profiles) {

    for (const auto& profile : profiles) {
        out << profile.name_length << ' '
            << profile.badge_index << ' ';
        if (!out) {
            throw std::runtime_error("could not write playerinfo.txt header");
        }
    }

    for (const auto& profile : profiles) {
        for (std::int32_t i = 0; i < profile.name_length; ++i) {
            out << profile.name_glyph_codes[static_cast<std::size_t>(i)] << ' ';
            if (!out) {
                throw std::runtime_error("could not write playerinfo.txt name data");
            }
        }
    }
}

} // namespace btb::profiles
