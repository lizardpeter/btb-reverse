#pragma once
// Exact Bink1 intra/inter 8x8 coefficient dequantization + pixel stage.
// Uses original signed integer 8x8 transform from 2002 Bink DLL,
// with the distinct intra/inter scan-ordered quantization tables.
#include "btb/bink32_dct_coefficients.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
namespace btb::bink32_recovered {
enum class BinkDctQuantFamily : std::uint8_t {Intra=0,Inter=1};
[[nodiscard]] std::optional<std::span<const std::int32_t,64>>
bink_dct_quant_matrix(BinkDctQuantFamily family,
                      std::uint8_t selector) noexcept;
[[nodiscard]] std::optional<std::array<std::uint8_t,64>>
reconstruct_bink_dct_pixels(
    const BinkDctCoefficients& block,
    BinkDctQuantFamily family) noexcept;
} // namespace btb::bink32_recovered
