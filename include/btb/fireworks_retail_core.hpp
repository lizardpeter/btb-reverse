#pragma once

#include "btb/fireworks_layout.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::fireworks {

// These 12 exact little-endian uint32_t entries were read from the
// SHA-256-pinned retail executable's .data section at VA 0x00442734.
// The executable is NOT bundled with the reconstructed project.
// Original functions: 0x00410FD0, 0x00411010, 0x00411080.
inline constexpr std::array<std::int32_t, 12> kOriginalFireworkLengths{{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
}};
inline constexpr std::int32_t kOriginalFireworkEmpty = -1;

[[nodiscard]] constexpr bool original_firework_type_valid(
    std::int32_t id) noexcept {
    return id >= 0 &&
           id < static_cast<std::int32_t>(kOriginalFireworkLengths.size());
}

// A single source-owned 18*4-byte integer grid, rather than unrelated
// editor and file-save representations. Source operations are deliberately
// bounded at the edges; the retail x86 version could read outside the array
// when invoked with corrupt row, column or type parameters.
class OriginalFireworkGrid {
public:
    OriginalFireworkGrid() noexcept { clear(); }

    [[nodiscard]] constexpr const RetailGrid& raw() const noexcept {
        return cells_;
    }

    [[nodiscard]] constexpr bool in_bounds(
        std::size_t row, std::size_t column) const noexcept {
        return row < 3 && column < 6;
    }

    [[nodiscard]] constexpr std::optional<FireworkType> at(
        std::size_t row, std::size_t column) const noexcept {
        if (!in_bounds(row, column)) return std::nullopt;
        const auto id = cells_[row * 6 + column];
        if (!original_firework_type_valid(id)) return std::nullopt;
        return static_cast<FireworkType>(id);
    }

    [[nodiscard]] constexpr bool empty(
        std::size_t row, std::size_t column) const noexcept {
        return in_bounds(row, column) &&
               cells_[row * 6 + column] == kOriginalFireworkEmpty;
    }

    // Source 0x00410FD0 uses row*6+column and scans the exact type
    // length table. The 12 retail types all have length 1.
    [[nodiscard]] constexpr bool can_place(
        std::size_t row, std::size_t column, FireworkType type) const noexcept {
        const auto id = static_cast<std::int32_t>(type);
        if (!in_bounds(row, column) || !original_firework_type_valid(id))
            return false;
        const auto index = row * 6 + column;
        const auto length = static_cast<std::size_t>(
            kOriginalFireworkLengths[static_cast<std::size_t>(id)]);
        if (index + length > cells_.size()) return false;
        for (std::size_t i = 0; i < length; ++i) {
            if (cells_[index + i] != kOriginalFireworkEmpty) return false;
        }
        return true;
    }

    // Source 0x00411010: dry run does not mutate and commit writes the
    // original head type followed by id+14 continuation cells (dormant
    // with the verified one-cell retail table).
    [[nodiscard]] constexpr bool place(
        std::size_t row, std::size_t column, FireworkType type,
        bool commit) noexcept {
        if (!can_place(row, column, type)) return false;
        if (!commit) return true;
        const auto id = static_cast<std::int32_t>(type);
        const auto start = row * 6 + column;
        const auto length = static_cast<std::size_t>(
            kOriginalFireworkLengths[static_cast<std::size_t>(id)]);
        cells_[start] = id;
        for (std::size_t i = 1; i < length; ++i)
            cells_[start + i] = id + 14;
        return true;
    }

    // Source 0x00411080 removes the original head, or returns no type
    // if empty. This source table has no continuation cells in retail.
    [[nodiscard]] constexpr std::optional<FireworkType> remove(
        std::size_t row, std::size_t column) noexcept {
        const auto value = at(row, column);
        if (!value) return std::nullopt;
        const auto id = static_cast<std::int32_t>(*value);
        const auto start = row * 6 + column;
        const auto length = static_cast<std::size_t>(
            kOriginalFireworkLengths[static_cast<std::size_t>(id)]);
        for (std::size_t i = 0; i < length; ++i)
            cells_[start + i] = kOriginalFireworkEmpty;
        return value;
    }

    // The retail editor deletes an existing item before placing its
    // replacement, not after finding an empty slot.
    [[nodiscard]] constexpr bool replace(
        std::size_t row, std::size_t column, FireworkType type) noexcept {
        if (!in_bounds(row, column) ||
            !original_firework_type_valid(static_cast<std::int32_t>(type)))
            return false;
        (void)remove(row, column);
        return place(row, column, type, true);
    }

    constexpr void clear() noexcept {
        cells_.fill(kOriginalFireworkEmpty);
    }

    [[nodiscard]] constexpr std::size_t occupied_count() const noexcept {
        std::size_t count = 0;
        for (const auto value : cells_)
            if (value != kOriginalFireworkEmpty) ++count;
        return count;
    }

    // File-backed 18x int32 source grid: accept ONLY -1 and head IDs 0..11.
    // Invalid files are rejected atomically; this is an intentional
    // memory-safety/validation improvement, not a claim of retail behavior.
    [[nodiscard]] constexpr bool load(const RetailGrid& data) noexcept {
        for (const auto value : data)
            if (value != kOriginalFireworkEmpty &&
                !original_firework_type_valid(value))
                return false;
        cells_ = data;
        return true;
    }

private:
    RetailGrid cells_{};
};

static_assert(kOriginalFireworkLengths.size() == 12);
static_assert(kRetailGridSlotCount == 18);
static_assert(sizeof(RetailGrid) == 18 * sizeof(std::int32_t));

} // namespace btb::fireworks
