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
inline constexpr std::size_t kMaxEnteredNameGlyphs = 8;
inline constexpr std::int32_t kBadgeCount = 6;
inline constexpr std::int32_t kBadgeWidth = 50;

[[nodiscard]] constexpr std::int32_t previous_badge(
    std::int32_t badge) noexcept {
    return badge <= 0 ? kBadgeCount - 1 : badge - 1;
}

[[nodiscard]] constexpr std::int32_t next_badge(
    std::int32_t badge) noexcept {
    return badge >= kBadgeCount - 1 ? 0 : badge + 1;
}

struct BadgeSourceRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

enum class ProfileScreenTarget : std::int32_t {
    Profile0 = 0,
    Profile1 = 1,
    Profile2 = 2,
    Profile3 = 3,
    Profile4 = 4,
    Help = 5,
    Delete = 6,
    Back = 7,
};

struct ProfileHitRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

inline constexpr std::array<ProfileHitRect, 8> kProfileHitRects{{
    {42, 140, 142, 223},
    {90, 261, 230, 386},
    {232, 156, 352, 247},
    {321, 261, 454, 372},
    {471, 195, 597, 291},
    {17, 425, 61, 467},
    {293, 421, 345, 472},
    {576, 424, 618, 467},
}};

[[nodiscard]] constexpr std::int32_t hit_test_profile_screen(
    std::int32_t x,
    std::int32_t y) noexcept {
    for (std::size_t i = 0; i < kProfileHitRects.size(); ++i) {
        const auto& r = kProfileHitRects[i];
        if (x > r.left && x < r.right &&
            y > r.top && y < r.bottom) {
            return static_cast<std::int32_t>(i);
        }
    }
    return -1;
}

[[nodiscard]] constexpr BadgeSourceRect badge_source_rect(
    std::int32_t badge) noexcept {
    const auto left = badge * kBadgeWidth;
    return {left, 45, left + kBadgeWidth, 103};
}

inline constexpr std::int32_t kDirectInputBackspace = 0x0E;
inline constexpr std::int32_t kDirectInputLeftShift = 0x2A;
inline constexpr std::int32_t kDirectInputRightShift = 0x36;

[[nodiscard]] constexpr bool is_shift_scan_code(
    std::int32_t scan_code) noexcept {
    return scan_code == kDirectInputLeftShift ||
           scan_code == kDirectInputRightShift;
}

// Exact live name-entry acceptance gate. The input subsystem has already
// translated the physical key into an ASCII-like character code at this point.
// Retail stores translated_code - 0x21 as the blue-font glyph index.
[[nodiscard]] constexpr std::int32_t profile_name_glyph_for_input(
    std::int32_t direct_input_scan_code,
    std::int32_t translated_code,
    std::size_t current_length) noexcept {

    if (is_shift_scan_code(direct_input_scan_code) ||
        direct_input_scan_code == kDirectInputBackspace ||
        direct_input_scan_code > 0x35 ||
        current_length >= kMaxEnteredNameGlyphs ||
        translated_code > 0x7F ||
        translated_code <= 0x21) {
        return -1;
    }

    return translated_code - 0x21;
}

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
