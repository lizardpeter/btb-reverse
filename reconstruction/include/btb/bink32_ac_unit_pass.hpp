#pragma once
// Original Bink32 2002 0x30012030..0x300122E7.
// After magnitude-class passes, this final pass writes +/-1 from
// one sign bit for each active coefficient; zero keeps its old value.
// This is a source-derived stage of 0x30011B10, NOT full frame decode.
#include "btb/bink32_ac_command_pass.hpp"
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
struct AcUnitPassResult {
    std::uint32_t consumed_bits{},activations{},written_coefficients{};
    std::uint32_t deferred_commands{},expanded_commands{};
    std::size_t remaining_commands{};
};
// Original 0x3001242C is a second 4-entry branch table. Case 0 rewrites
// source code to (base+0x11); case 1 appends three commands; case 2
// processes four positions with skip/one-bit sign; case 3 writes one
// signed unit coefficient. Mutations commit atomically on complete input.
[[nodiscard]] std::optional<AcUnitPassResult> run_bink_ac_unit_pass(
    LsbBitReader& compressed,
    AcPendingCommands& controls,
    std::span<std::int16_t> coefficients) noexcept;
} // namespace btb::bink32_recovered
