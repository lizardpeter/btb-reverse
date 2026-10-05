#pragma once

#include <cstdint>

namespace btb::fireworks {

inline constexpr std::int32_t kCertificateNameCenterX = 491;
inline constexpr std::int32_t kCertificateNameY = 135;

inline constexpr std::int32_t kCertificateBadgeX = 471;
inline constexpr std::int32_t kCertificateBadgeY = 156;
inline constexpr std::int32_t kCertificateBadgeCellWidth = 50;
inline constexpr std::int32_t kCertificateBadgeHeight = 45;

inline constexpr std::int32_t kCertificatePrintLeft = 296;
inline constexpr std::int32_t kCertificatePrintTop = 418;
inline constexpr std::int32_t kCertificatePrintRight = 348;
inline constexpr std::int32_t kCertificatePrintBottom = 466;

inline constexpr std::int32_t kCertificatePrintHoverSoundId = 143;

[[nodiscard]] constexpr bool certificate_print_hit(
    std::int32_t x,
    std::int32_t y) noexcept {

    // Retail uses strict comparisons.
    return x > kCertificatePrintLeft &&
           x < kCertificatePrintRight &&
           y > kCertificatePrintTop &&
           y < kCertificatePrintBottom;
}

struct BadgeSourceRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

[[nodiscard]] constexpr BadgeSourceRect certificate_badge_source_rect(
    std::int32_t badge_index) noexcept {

    if (badge_index < 0) {
        return {};
    }

    const auto left = badge_index * kCertificateBadgeCellWidth;
    return {
        left,
        0,
        left + kCertificateBadgeCellWidth,
        kCertificateBadgeHeight,
    };
}

} // namespace btb::fireworks
