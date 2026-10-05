#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <ostream>

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
    return profile.name_length > 0;
}

} // namespace btb::profiles
