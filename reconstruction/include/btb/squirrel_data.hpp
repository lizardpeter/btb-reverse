#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <vector>

namespace btb::squirrel {

inline constexpr std::size_t kPhaseColumnCount = 7;
inline constexpr std::size_t kPhaseRowCount = 8;
inline constexpr std::size_t kRetailLoadedIntegerCount = 71;

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

// These tables are indexed dynamically by the retail animation/placement code.
// Their exact higher-level labels are not yet closed, so the reconstruction
// deliberately preserves neutral names rather than inventing puzzle semantics.
struct Data {
    std::array<std::int32_t, kPhaseColumnCount> phase_header{};
    std::array<
        std::array<std::int32_t, kPhaseColumnCount>,
        kPhaseRowCount> phase_table{};

    std::int32_t animation_step_delay{}; // shipped value 7

    // Retail reads these two integers into a temporary and never stores them.
    std::array<std::int32_t, 2> discarded_scalars{};

    // Stored in four globals at 0x51505C, 0x515060, 0x515064, 0x514F04.
    std::array<std::int32_t, 4> tuning_tuple{};

    // Stored at 0x515084 and used in initial placement alignment.
    std::int32_t alignment_offset{};
};

Data parse_retail_data(std::istream& in);

// The shipped file contains five extra coordinate pairs after the exact retail
// read boundary. The 2002 executable closes the file before reading them.
// This helper exists only to inventory/preserve those legacy bytes in tooling.
std::vector<Vec2i> read_unconsumed_legacy_pairs(std::istream& in);

} // namespace btb::squirrel
