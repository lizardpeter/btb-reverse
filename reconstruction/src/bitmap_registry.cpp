#include "btb/bitmap_registry.hpp"

#include <algorithm>

namespace btb::bitmap_registry {

BitmapFileOpenPlan bitmap_file_open_plan(
    std::string_view filename,
    std::int32_t requested_width,
    std::int32_t requested_height,
    std::int32_t drive_index) {

    BitmapFileOpenPlan plan;
    plan.direct_path = std::string(filename);
    plan.requested_width = requested_width;
    plan.requested_height = requested_height;

    if (drive_index >= 0) {
        std::string fallback;
        fallback.reserve(filename.size() + 3);
        fallback.push_back(static_cast<char>('a' + drive_index));
        fallback += ":\\";
        fallback.append(filename);
        plan.fallback_path = std::move(fallback);
    }

    return plan;
}

void copy_filename(
    FilenameRecord& destination,
    std::string_view filename) noexcept {

    destination.bytes.fill('\0');
    const auto count =
        std::min(filename.size(), destination.bytes.size() - 1);
    for (std::size_t i = 0; i < count; ++i) {
        destination.bytes[i] = filename[i];
    }
}

RegisterResult register_surface_reference(
    RegistryState& state,
    std::uint32_t surface_ref32,
    std::string_view filename) noexcept {

    RegisterResult result;

    if (state.reload_in_progress) {
        result.skipped_during_reload = true;
        return result;
    }

    std::int32_t slot = 0;
    while (slot < state.high_water_count &&
           state.surface_ref32[static_cast<std::size_t>(slot)] != 0) {
        ++slot;
    }

    if (slot == state.high_water_count) {
        ++state.high_water_count;

        // The retail code performs this peculiar clamp before writing the
        // selected slot. A later full-table high-water rescan can raise the
        // count back to 799 or 800.
        if (state.high_water_count >= 0x31F) {
            state.high_water_count = 0x31E;
        }
    }

    if (slot < 0 ||
        slot >= static_cast<std::int32_t>(kRegistrySlotCount)) {
        result.would_overflow_retail_table = true;
        result.slot = slot;
        return result;
    }

    const auto index = static_cast<std::size_t>(slot);
    state.color_keyed[index] = 0;
    copy_filename(state.filename[index], filename);
    state.surface_ref32[index] = surface_ref32;

    state.high_water_count = recompute_high_water(state);

    result.registered = true;
    result.slot = slot;
    return result;
}

std::vector<ReloadEntry> make_reload_plan(
    RegistryState& state,
    const std::array<bool,kRegistrySlotCount>& referenced_surface_is_nonnull) {

    state.reload_in_progress = true;

    std::vector<ReloadEntry> result;
    result.reserve(
        static_cast<std::size_t>(
            std::max(state.high_water_count, 0)));

    for (std::int32_t i = 0; i < state.high_water_count; ++i) {
        const auto index = static_cast<std::size_t>(i);
        const auto ref = state.surface_ref32[index];
        if (ref == 0 || !referenced_surface_is_nonnull[index]) {
            continue;
        }

        result.push_back({
            i,
            ref,
            true,
            std::string_view(state.filename[index].bytes.data()),
            state.color_keyed[index] == 1,
        });
    }

    return result;
}

} // namespace btb::bitmap_registry
