#pragma once
// One complete first-stage COMMAND PASS of original 2002 binkw32.dll
// 0x30011BF0..0x3001202A, not the complete coefficient unpacker.
// Original source has mutable head/end/current command pointers:
// case 1 expands after end, cases 0/2 prepend deferred controls to
// head; case 3 writes one AC; nonactivated controls survive for next pass.
#include "btb/bink32_ac_group4.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
struct AcPendingCommands {
    static constexpr std::size_t capacity=512;
    // Plenty of headroom for bounded prepend as in the original
    // stack layout. Only [head,end) is semantically occupied.
    std::array<std::uint8_t,capacity> data{};
    std::size_t head{256},end{256};
    [[nodiscard]] std::span<const std::uint8_t> pending() const noexcept {
        return std::span<const std::uint8_t>(data).subspan(head,end-head);
    }
};
// Source: 30011B9F..30011BD4 seeds six byte commands:
// [0x10,0x60,0xb0,0x07,0x0b,0x0f].
[[nodiscard]] AcPendingCommands initial_bink_ac_pending() noexcept;
[[nodiscard]] std::optional<AcPendingCommands>
make_bink_ac_pending(std::span<const std::uint8_t> commands) noexcept;

struct AcCommandPassResult {
    std::uint32_t bit_count{},activation_reads{},expanded_commands{};
    std::uint32_t written_coefficients{},deferred_commands{};
    std::size_t remaining_commands{};
};
// Caller supplies the current original x86 EAX bit count and 16-bit
// magnitude-class WORD; original changes these between passes.
// Processes initial [head,end) and any case-1 appended commands;
// deferred case-0/2 commands are *prepended*, to run next pass.
// All state and source cursor update transactionally on full success.
[[nodiscard]] std::optional<AcCommandPassResult> run_bink_ac_command_pass(
    LsbBitReader& compressed,
    AcPendingCommands& controls,
    std::span<std::int16_t> coefficients,
    unsigned low_magnitude_bits,
    std::uint16_t high_magnitude_bit) noexcept;
} // namespace btb::bink32_recovered
