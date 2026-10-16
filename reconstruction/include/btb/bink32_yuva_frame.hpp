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
    bool source_has_alpha{true};
    // Canonical display order Y,U,V,A; compressed SJPi order A,Y,V,U.
    std::array<std::vector<std::uint8_t>,4> planes{};
    std::array<std::size_t,4> plane_end_byte_offsets{};
    std::array<std::array<std::size_t,10>,4> mode_counts{};
    std::size_t video_bytes_consumed{};
};
// Decode either valid SJPi video layout. Alpha sources begin with
// the recorded alpha section, then a picture DWORD; opaque sources
// have only the picture DWORD. Output is always canonical Y,U,V,A
// with 255-filled A for a source that has no alpha.
[[nodiscard]] std::optional<BinkNativeYuVaFrame>
decode_bink_sjpi_video_frame(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,bool has_alpha,
    const BinkNativeYuVaFrame* previous=nullptr);

[[nodiscard]] std::optional<BinkNativeYuVaFrame>
decode_bink_sjpi_yuva_frame(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,
    const BinkNativeYuVaFrame* previous=nullptr);

// Byte-accurate comparison against externally decoded packed yuva420p
// raw video (plane byte order: Y, U, V, A). This does not assume or
// manufacture native parity. Any wrong-size reference is rejected.
struct BinkPlaneParity {
    std::size_t compared_bytes{};
    std::size_t different_bytes{};
    std::size_t first_different_index{};
    std::uint8_t first_native{},first_reference{};
    std::uint8_t maximum_absolute_difference{};
    std::uint64_t sum_absolute_differences{};
};
struct BinkFrameParity {
    std::array<BinkPlaneParity,4> planes{};
    std::size_t total_different_bytes{};
    bool identical{};
};
[[nodiscard]] std::optional<BinkFrameParity>
compare_bink_yuva_reference(
    const BinkNativeYuVaFrame& native,
    std::span<const std::uint8_t> packed_yuva420p) noexcept;

} // namespace btb::bink32_recovered
