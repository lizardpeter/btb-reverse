#pragma once
// Native Bink1 8x8 signed spatial residue entropy decoder.
// The source layout is 7-bit mask event quota, 3-bit starting mask
// exponent, then adaptive four-coefficient subdivision/refinement.
// This is pixel-domain signed residue, NOT DCT AC coefficient parsing.
#include "btb/bink32_dynamic_huffman.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
struct BinkDecodedResidue {
    std::array<std::int16_t,64> coefficients{};
    std::size_t consumed_bits{};
    std::size_t coefficient_events{};
    std::size_t refinement_events{};
};
// Read the original 7-bit event quota AND the complete residue coding.
// Transactional: malformed/truncated input leaves bits unmodified.
[[nodiscard]] std::optional<BinkDecodedResidue>
decode_bink_residue_packet(LsbBitReader& bits) noexcept;

// Separately decode the variable-length residual coefficient tree when
// the original caller already supplied its 7-bit event quota.
[[nodiscard]] std::optional<BinkDecodedResidue>
decode_bink_residue_coefficients(
    LsbBitReader& bits,unsigned event_quota) noexcept;
} // namespace btb::bink32_recovered
