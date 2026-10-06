#include "btb/bitmap_registry.hpp"

#include <cassert>
#include <string>

using namespace btb::bitmap_registry;

int main() {
    static_assert(kRegistrySlotCount == 800);
    static_assert(kFilenameBytes == 0x104);
    static_assert(sizeof(FilenameRecord) == 0x104);
    static_assert(kMagentaColorKey == 0x00FF00FF);

    RegistryState state;

    auto added = register_surface_reference(
        state, 0x1000, "Data\\SubGameOpen\\music_01.bmp");
    assert(added.registered);
    assert(added.slot == 0);
    assert(state.high_water_count == 1);
    assert(state.surface_ref32[0] == 0x1000);
    assert(state.color_keyed[0] == 0);
    assert(std::string(state.filename[0].bytes.data()) ==
           "Data\\SubGameOpen\\music_01.bmp");

    added = register_surface_reference(
        state, 0x2000, "Data\\SubGameOpen\\toolbar.bmp");
    assert(added.registered);
    assert(added.slot == 1);
    assert(state.high_water_count == 2);

    assert(mark_surface_color_keyed(state, 0x2000));
    assert(state.color_keyed[1] == 1);
    assert(!mark_surface_color_keyed(state, 0xDEAD));

    // Unregister clears the pointer reference, color-key flag, and only the
    // first filename byte. It does not lower the high-water count.
    assert(unregister_surface_reference(state, 0x1000));
    assert(state.surface_ref32[0] == 0);
    assert(state.color_keyed[0] == 0);
    assert(state.filename[0].bytes[0] == '\0');
    assert(state.high_water_count == 2);
    assert(!unregister_surface_reference(state, 0xDEAD));

    // The first hole below high-water is reused.
    added = register_surface_reference(
        state, 0x3000, "replacement.bmp");
    assert(added.registered);
    assert(added.slot == 0);
    assert(state.high_water_count == 2);

    // Registration is completely skipped while ReloadRegisteredBitmapSurfaces
    // has raised the retail guard.
    state.reload_in_progress = true;
    added = register_surface_reference(
        state, 0x4000, "blocked.bmp");
    assert(!added.registered);
    assert(added.skipped_during_reload);
    assert(state.high_water_count == 2);
    state.reload_in_progress = false;

    // High-water is recomputed from all 800 surface-reference slots after a
    // successful registration.
    state.surface_ref32[10] = 0xAAAA;
    assert(recompute_high_water(state) == 11);
    added = register_surface_reference(
        state, 0x5000, "slot2.bmp");
    assert(added.registered);
    assert(added.slot == 2);
    assert(state.high_water_count == 11);

    // Exact peculiar retail clamp: extending a fully occupied count of 798
    // selects slot 798, temporarily clamps count to 798, then the full-table
    // rescan raises it to 799.
    RegistryState near_limit;
    near_limit.high_water_count = 798;
    for (std::size_t i = 0; i < 798; ++i) {
        near_limit.surface_ref32[i] =
            static_cast<std::uint32_t>(0x10000 + i * 4);
    }

    added = register_surface_reference(
        near_limit, 0x77770000, "slot798.bmp");
    assert(added.registered);
    assert(added.slot == 798);
    assert(near_limit.high_water_count == 799);

    // The next full extension selects slot 799 and the rescan yields 800.
    added = register_surface_reference(
        near_limit, 0x77770004, "slot799.bmp");
    assert(added.registered);
    assert(added.slot == 799);
    assert(near_limit.high_water_count == 800);

    // If all 800 entries are occupied, the shipped loop would choose index
    // 800 and write beyond its static arrays. The reconstruction reports that
    // exact would-overflow condition instead of invoking C++ UB.
    added = register_surface_reference(
        near_limit, 0x77770008, "overflow.bmp");
    assert(!added.registered);
    assert(added.would_overflow_retail_table);
    assert(added.slot == 800);

    // Reload plans only include registered references whose referenced surface
    // currently exists. The guard is raised before scanning.
    RegistryState reload;
    auto r0 = register_surface_reference(
        reload, 0x1111, "opaque.bmp");
    auto r1 = register_surface_reference(
        reload, 0x2222, "keyed.bmp");
    auto r2 = register_surface_reference(
        reload, 0x3333, "already-null.bmp");
    assert(r0.slot == 0 && r1.slot == 1 && r2.slot == 2);
    assert(mark_surface_color_keyed(reload, 0x2222));

    std::array<bool,kRegistrySlotCount> nonnull{};
    nonnull[0] = true;
    nonnull[1] = true;
    nonnull[2] = false;

    const auto plan = make_reload_plan(reload, nonnull);
    assert(reload.reload_in_progress);
    assert(plan.size() == 2);

    assert(plan[0].slot == 0);
    assert(plan[0].surface_ref32 == 0x1111);
    assert(plan[0].release_existing_surface);
    assert(plan[0].filename == "opaque.bmp");
    assert(!plan[0].reapply_magenta_color_key);

    assert(plan[1].slot == 1);
    assert(plan[1].surface_ref32 == 0x2222);
    assert(plan[1].filename == "keyed.bmp");
    assert(plan[1].reapply_magenta_color_key);

    finish_reload(reload);
    assert(!reload.reload_in_progress);

    static_assert(kLoadImageFlags == 0x2010);
    static_assert(kLargeBitmapWidthThreshold == 2000);

    const auto file_plan = bitmap_file_open_plan(
        "Data\\SubGameOpen\\music_01.bmp", 0, 0, 4);
    assert(file_plan.direct_path ==
           "Data\\SubGameOpen\\music_01.bmp");
    assert(file_plan.fallback_path);
    assert(*file_plan.fallback_path ==
           "e:\\Data\\SubGameOpen\\music_01.bmp");
    assert(file_plan.requested_width == 0);
    assert(file_plan.requested_height == 0);
    assert(file_plan.load_image_flags == 0x2010);
    assert(file_plan.abort_for_removed_cd_if_both_fail);

    const auto no_cd = bitmap_file_open_plan(
        "Data\\x.bmp", 32, 64, -1);
    assert(!no_cd.fallback_path);
    assert(no_cd.requested_width == 32);
    assert(no_cd.requested_height == 64);

    static_assert(kBitmapLoadLifecycle.get_bitmap_object_info);
    static_assert(kBitmapLoadLifecycle.bitmap_object_info_bytes == 24);
    static_assert(kBitmapLoadLifecycle.surface_descriptor_size == 0x7C);
    static_assert(kBitmapLoadLifecycle.surface_descriptor_flags == 0x07);
    static_assert(
        kBitmapLoadLifecycle.copy_bitmap_to_surface_after_create);
    static_assert(kBitmapLoadLifecycle.delete_hbitmap_after_success);
    static_assert(
        kBitmapLoadLifecycle.retail_leaks_hbitmap_if_both_surface_creates_fail);

    constexpr auto normal_surface =
        bitmap_surface_plan(640);
    static_assert(normal_surface.initial_caps == 0x40);
    static_assert(normal_surface.retry_caps == 0x800);
    static_assert(
        normal_surface.retry_with_system_memory_caps_on_create_failure);

    constexpr auto huge_surface =
        bitmap_surface_plan(2001);
    static_assert(huge_surface.initial_caps == 0x800);
    static_assert(huge_surface.retry_caps == 0x800);
}
