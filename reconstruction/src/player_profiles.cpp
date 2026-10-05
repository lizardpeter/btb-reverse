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

void delete_profile_in_memory(
    ProfileTable& profiles,
    std::array<btb::progress::Record, kProfileCount>& progress,
    std::size_t profile_index) {

    if (profile_index >= kProfileCount) {
        throw std::runtime_error("profile index out of range");
    }

    // Exact retail delete behavior: metadata is made unreachable, but the
    // stale 9-int name backing slot is not zeroed.
    profiles[profile_index].name_length = 0;
    profiles[profile_index].badge_index = -1;

    // Retail clears exactly the 15 visible progress slots 50..64.
    for (std::size_t i = btb::progress::kRetailProgressFirst;
         i <= btb::progress::kRetailProgressLast;
         ++i) {
        progress[profile_index].values[i] = 0;
    }
}

std::array<std::string, 5> activity_files_for_profile(
    std::size_t profile_index) {

    if (profile_index >= kProfileCount) {
        throw std::runtime_error("profile index out of range");
    }

    const auto n = std::to_string(profile_index + 1);
    return {
        "dypdata" + n + ".txt",
        "firedata" + n + ".txt",
        "musicbob" + n + ".txt",
        "musicwendy" + n + ".txt",
        "musicfarmer" + n + ".txt",
    };
}

} // namespace btb::profiles
