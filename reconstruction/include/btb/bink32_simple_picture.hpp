#pragma once
// Bink1 SJPi original-bitstream to pixel bridge for *simple* luma modes.
// Unlike the block compositor, this owns all nine compressed bundle
// cursors and refills each on the same source-order rows as 0x3000E400.
// AC-bearing DCT blocks still require a verified 8x8 inverse transform.
#include "btb/bink32_entropy_commands.hpp"
#include "btb/bink32_block_modes.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
namespace btb::bink32_recovered {
struct BinkSimplePictureLuma {
    BinkModePlane image{};
    std::array<std::size_t,10> mode_counts{};
    std::array<std::size_t,9> source_refills{};
    std::size_t next_plane_byte_offset{};
};
// Decode a complete 8-bit luma plane using Skip, Motion, Fill, Pattern,
// Raw, signed spatial Residue, DC-only Intra/Inter DCT, all sixteen
// source-defined RUN scans, and source-backed
// 16x16 Scaled (Fill/Pattern/Raw/RUN/DC-only Intra) modes directly
// from its compressed SJPi packet.
// Previous luma is mandatory for Skip, Motion and Residue blocks; no previous image is
// required for independent Fill/Pattern/Raw-only planes. Any unsupported
// mode or malformed source returns nullopt with no partial frame.
[[nodiscard]] std::optional<BinkSimplePictureLuma>
decode_bink_simple_picture_luma(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,
    std::span<const std::uint8_t> previous_luma={});
} // namespace btb::bink32_recovered
