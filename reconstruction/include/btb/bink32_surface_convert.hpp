#pragma once
// Platform-neutral packed-pixel bridge for decoded native Bink1 SJPi frames.
// The decoded planes remain canonical Y,U,V,A 4:2:0. The caller owns and
// locks its actual display surface; no RAD DLL, FFmpeg or Windows dependency.
#include "btb/bink32_yuva_frame.hpp"
#include <cstddef>
#include <cstdint>
#include <span>

namespace btb::bink32_recovered {
struct BinkPackedSurface {
    std::span<std::uint8_t> bytes{};
    std::size_t row_pitch{}; // Positive byte pitch; top row first.
    std::size_t width{},height{};
    // Little-endian packed RGB. Typical DD7: RGB565, RGB555, XRGB8888.
    std::uint8_t bytes_per_pixel{}; // 2 or 4
    std::uint32_t red_mask{},green_mask{},blue_mask{};
};

enum class BinkSurfaceResult {
    ok,
    invalid_source,
    invalid_surface,
    unsupported_pixel_format,
};

// BT.601 video-range YUV420 to packed RGB. Source A uses straight-alpha
// source-over against the *existing* destination pixel; no keying or alpha
// fake. Preserve non-RGB destination bits. This is a portable presentation
// baseline, not yet a certified match to the 2002 DLL's color conversion.
// All sizes/masks are checked before any destination byte is changed.
[[nodiscard]] BinkSurfaceResult blit_bink_yuva_to_surface(
    const BinkNativeYuVaFrame& source,BinkPackedSurface target) noexcept;
} // namespace btb::bink32_recovered
