#pragma once
// Bink1 2002 packet DCT coefficient decoding, for both ordinary intra
// and inter 8x8 blocks. Original DC is supplied from the separate
// unsigned intra/signed inter bundle: the AC tree must NOT consume it.
// Coefficients and quantization selector are parsed, but no unverified
// 8x8 iDCT or matrix is substituted for the original pixel transform.
#include "btb/bink32_dynamic_huffman.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
namespace btb::bink32_recovered {
struct BinkDctCoefficients {
    std::array<std::int32_t,64> values{};
    std::array<std::uint8_t,64> changed_scan_indices{};
    std::size_t changed_count{};
    std::uint8_t quant_selector{};
    std::uint8_t magnitude_planes{};
    std::size_t consumed_bits{};
};
// Source-compatible: 4-bit magnitude-plane count, adaptive 8x8
// coefficient group tree, signed coefficient leaves, then 4-bit
// quantization selector when no external selector is provided.
// On malformed/truncated input neither reader nor caller-owned data
// changes. A selector override is only for source call sites with
// an externally supplied matrix selection; ordinary Bink1 DCT uses
// the four-bit in-band selector.
[[nodiscard]] std::optional<BinkDctCoefficients>
decode_bink_dct_coefficients(
    LsbBitReader& bits,std::int16_t dc,
    std::optional<std::uint8_t> selector_override=std::nullopt) noexcept;
} // namespace btb::bink32_recovered
