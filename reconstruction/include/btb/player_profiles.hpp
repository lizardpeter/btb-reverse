#pragma once

#include "btb/player_progress.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

namespace btb::profiles {

inline constexpr std::size_t kProfileCount = 5;
inline constexpr std::size_t kMaxNameGlyphs = 9;

struct ProfileInfo {
    std::int32_t name_length{};
    std::int32_t badge_index{-1};
    std::array<std::int32_t, kMaxNameGlyphs> name_glyph_codes{};
};

using ProfileTable = std::array<ProfileInfo, kProfileCount>;

ProfileTable read_playerinfo(std::istream& in);
void write_playerinfo(std::ostream& out, const ProfileTable& profiles);

[[nodiscard]] constexpr bool has_profile(
    const ProfileInfo& profile) noexcept {
    return profile.name_length > 0 || profile.badge_index >= 0;
}

void delete_profile_in_memory(
    ProfileTable& profiles,
    std::array<btb::progress::Record, kProfileCount>& progress,
    std::size_t profile_index);

[[nodiscard]] std::array<std::string, 5> activity_files_for_profile(
    std::size_t profile_index);

} // namespace btb::profiles
