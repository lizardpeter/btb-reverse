#pragma once
// Bink1 SJPi original-bitstream to pixel bridge for *simple* luma modes.
// Unlike the block compositor, this owns all nine compressed bundle
// cursors and refills each on the same source-order rows as 0x3000E400.
// Compressed AC-bearing intra/inter DCT uses distinct native 8x8\n// quantization and the original integer inverse transform.
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
// Decode one Bink1 plane from its exact 23-tree header byte offset.
// This supports alpha and both half-resolution chroma planes as well
// as full-resolution Y. All output offsets are absolute packet bytes.
// Unlike the alpha-aware wrapper below, this does not skip any leading
// picture-control DWORD: the caller must supply the tree start.
// When requested, captures the first precise refusal point inside a
// plane (rather than hiding it behind an opaque nullopt). Offsets are
// relative to the original plane entropy-tree start, and line refers
// to the native decoder source revision used for the audit.
struct BinkPlaneDecodeDiagnostic {
    std::size_t row{},column{},bit_offset{};
    unsigned source_line{};
    std::uint8_t mode{255};
};
[[nodiscard]] std::optional<BinkSimplePictureLuma>
decode_bink_picture_plane_at(
    std::span<const std::uint8_t> video_packet,
    std::size_t entropy_tree_byte_offset,
    std::size_t width,std::size_t height,
    std::span<const std::uint8_t> previous_plane={},
    BinkPlaneDecodeDiagnostic* diagnostic=nullptr);

// Decode a complete 8-bit luma plane using Skip, Motion, Fill, Pattern,
// Raw, signed spatial Residue, full Intra/Inter DCT, all sixteen
// source-defined RUN scans, and source-backed
// 16x16 Scaled (Fill/Pattern/Raw/RUN/Intra DCT) modes directly
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
