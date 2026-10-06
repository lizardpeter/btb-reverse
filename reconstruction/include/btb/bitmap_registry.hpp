#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace btb::bitmap_registry {

inline constexpr std::size_t kRegistrySlotCount = 800;
inline constexpr std::size_t kFilenameBytes = 0x104;
inline constexpr std::uint32_t kMagentaColorKey = 0x00FF00FF;

struct FilenameRecord {
    std::array<char,kFilenameBytes> bytes{};

    [[nodiscard]] constexpr bool empty() const noexcept {
        return bytes[0] == '\0';
    }
};

static_assert(sizeof(FilenameRecord) == 0x104);

// Semantic source model of the three retail global tables:
//   0x0044EB1C: 800 addresses of IDirectDrawSurface7* variables
//   0x0044DE9C: 800 keyed flags
//   0x0044F79C: 800 filename records, stride 0x104
//
// surface_ref32 is the address of a pointer variable, not the surface itself.
struct RegistryState {
    std::array<std::uint32_t,kRegistrySlotCount> surface_ref32{};
    std::array<std::int32_t,kRegistrySlotCount> color_keyed{};
    std::array<FilenameRecord,kRegistrySlotCount> filename{};
    std::int32_t high_water_count{}; // retail global 0x0048241C
    bool reload_in_progress{};       // retail global 0x00482420
};

void copy_filename(
    FilenameRecord& destination,
    std::string_view filename) noexcept;

[[nodiscard]] constexpr std::int32_t recompute_high_water(
    const RegistryState& state) noexcept {
    std::int32_t high = 0;
    for (std::size_t i = 0; i < kRegistrySlotCount; ++i) {
        if (state.surface_ref32[i] != 0) {
            high = static_cast<std::int32_t>(i + 1);
        }
    }
    return high;
}

struct RegisterResult {
    bool registered{};
    bool skipped_during_reload{};
    bool would_overflow_retail_table{};
    std::int32_t slot{-1};
};

// Exact 0x00403770 slot selection/high-water behavior, with one host-safety
// guard: if the shipped logic would choose index 800 after all 800 slots are
// occupied, the reconstruction reports that overflow instead of invoking C++
// out-of-bounds UB.
[[nodiscard]] RegisterResult register_surface_reference(
    RegistryState& state,
    std::uint32_t surface_ref32,
    std::string_view filename) noexcept;

[[nodiscard]] constexpr bool unregister_surface_reference(
    RegistryState& state,
    std::uint32_t surface_ref32) noexcept {

    for (std::int32_t i = 0; i < state.high_water_count; ++i) {
        if (state.surface_ref32[static_cast<std::size_t>(i)] !=
            surface_ref32) {
            continue;
        }

        const auto index = static_cast<std::size_t>(i);
        state.surface_ref32[index] = 0;
        state.color_keyed[index] = 0;
        state.filename[index].bytes[0] = '\0';
        return true;
    }
    return false;
}

[[nodiscard]] constexpr bool mark_surface_color_keyed(
    RegistryState& state,
    std::uint32_t surface_ref32) noexcept {

    for (std::int32_t i = 0; i < state.high_water_count; ++i) {
        if (state.surface_ref32[static_cast<std::size_t>(i)] ==
            surface_ref32) {
            state.color_keyed[static_cast<std::size_t>(i)] = 1;
            return true;
        }
    }
    return false;
}

struct ReloadEntry {
    std::int32_t slot{};
    std::uint32_t surface_ref32{};
    bool release_existing_surface{};
    std::string_view filename{};
    bool reapply_magenta_color_key{};
};

[[nodiscard]] std::vector<ReloadEntry> make_reload_plan(
    RegistryState& state,
    const std::array<bool,kRegistrySlotCount>& referenced_surface_is_nonnull);

constexpr void finish_reload(RegistryState& state) noexcept {
    state.reload_in_progress = false;
}

inline constexpr std::uint32_t kLoadImageFlags = 0x2010;
inline constexpr std::uint32_t kDefaultBitmapSurfaceCaps = 0x40;
inline constexpr std::uint32_t kLargeBitmapSurfaceCaps = 0x800;
inline constexpr std::int32_t kLargeBitmapWidthThreshold = 2000;

struct BitmapLoadSurfacePlan {
    std::uint32_t initial_caps{};
    std::uint32_t retry_caps{kLargeBitmapSurfaceCaps};
    bool retry_with_system_memory_caps_on_create_failure{true};
};

[[nodiscard]] constexpr BitmapLoadSurfacePlan bitmap_surface_plan(
    std::int32_t bitmap_width) noexcept {
    return {
        bitmap_width <= kLargeBitmapWidthThreshold
            ? kDefaultBitmapSurfaceCaps
            : kLargeBitmapSurfaceCaps,
        kLargeBitmapSurfaceCaps,
        true,
    };
}

} // namespace btb::bitmap_registry
