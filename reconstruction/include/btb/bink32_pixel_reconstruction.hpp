#pragma once
// Reconstructed eight-bit residual/prediction stage from original Bink32 DLL
// 0x30012940..0x30012Bxx. 0x30012440 calculates 8x8 residuals; this
// module reproduces the original *post-transform* byte addition.
// Does not implement inverse transform, coefficients, or full-frame decode.
#include <cstddef>
#include <cstdint>
#include <span>

namespace btb::bink32_recovered {
constexpr std::size_t kBinkBlockSide=8;
constexpr std::size_t kBinkBlockPixels=64;
// Result is 64 actual reconstructed single-channel pixels. Original x86
// ADD of two bytes deliberately wraps mod 256 instead of saturating.
// Supports in-place reference/destination; rejects invalid dimensions
// without modifying output.
[[nodiscard]] bool compose_bink_interframe_block8(
    std::span<const std::uint8_t> reference,std::size_t reference_stride,
    std::span<const std::int8_t> signed_residuals,
    std::span<std::uint8_t> output,std::size_t output_stride) noexcept;
// Original 0x30012440 intra path contributes a complete decoded 8x8
// block directly. Copy without reinterpretation, supporting padded rows.
[[nodiscard]] bool store_bink_intra_block8(
    std::span<const std::uint8_t> decoded_pixels,
    std::span<std::uint8_t> output,std::size_t output_stride) noexcept;
} // namespace btb::bink32_recovered
