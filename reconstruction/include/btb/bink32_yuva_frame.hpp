#pragma once
// Native Bink1 SJPi alpha/Y/V/U video-frame assembly.
// Each color plane uses the same compressed nine-bundle decoder;
// reference pixels come from a fully decoded previous frame, never
// fabricated from an incomplete decode.
#include "btb/bink32_simple_picture.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>
namespace btb::bink32_recovered {
enum class BinkYuvaPlane : std::size_t {Y=0,U=1,V=2,A=3};
struct BinkNativeYuVaFrame {
    std::size_t width{},height{};
    // Canonical display order Y,U,V,A; compressed SJPi order A,Y,V,U.
    std::array<std::vector<std::uint8_t>,4> planes{};
    std::array<std::size_t,4> plane_end_byte_offsets{};
    std::array<std::array<std::size_t,10>,4> mode_counts{};
    std::size_t video_bytes_consumed{};
};
[[nodiscard]] std::optional<BinkNativeYuVaFrame>
decode_bink_sjpi_yuva_frame(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,
    const BinkNativeYuVaFrame* previous=nullptr);
} // namespace btb::bink32_recovered
