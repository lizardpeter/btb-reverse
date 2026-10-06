#include "btb/display_manager.hpp"

#include <cassert>
#include <cstddef>

using namespace btb::display;

int main() {
    static_assert(sizeof(RetailRect32) == 0x10);
    static_assert(sizeof(RetailDisplayManager32) == 0x30);

    static_assert(offsetof(RetailDisplayManager32, direct_draw7_ptr32) == 0x04);
    static_assert(offsetof(RetailDisplayManager32, primary_surface_ptr32) == 0x08);
    static_assert(offsetof(RetailDisplayManager32, render_surface_ptr32) == 0x0C);
    static_assert(offsetof(RetailDisplayManager32, destination_rect) == 0x18);
    static_assert(offsetof(RetailDisplayManager32, windowed) == 0x28);

    static_assert(kRetailWidth == 640);
    static_assert(kRetailHeight == 480);
    static_assert(kRetailFullscreenBitsPerPixel == 16);

    RetailDisplayManager32 display{};
    display.destination_rect = {10, 20, 650, 500};
    display.windowed = 1;

    assert(display.is_windowed());
    assert(display.destination_width() == 640);
    assert(display.destination_height() == 480);

    display.windowed = 0;
    assert(!display.is_windowed());

    static_assert(offsetof(RetailDisplayManager32, unused_retail_2c) == 0x2C);
    static_assert(sizeof(RetailDisplayManager32) == 0x30);

    static_assert(kDisplayManagerVtable32 == 0x0043B2E0);
    static_assert(kDdscFullscreenExclusive == 0x11);
    static_assert(kDdscNormal == 0x08);
    static_assert(kFullscreenSurfaceFlags == 0x21);
    static_assert(kFullscreenSurfaceCaps == 0x2218);
    static_assert(kFullscreenBackBufferCount == 1);
    static_assert(kAttachedBackBufferCaps == 0x04);
    static_assert(kWindowedPrimarySurfaceFlags == 0x01);
    static_assert(kWindowedPrimarySurfaceCaps == 0x0200);
    static_assert(kWindowedRenderSurfaceFlags == 0x07);
    static_assert(kWindowedRenderSurfaceCaps == 0x2040);

    static_assert(
        kFullscreenInitializePlan.cooperative_flags == 0x11);
    static_assert(kFullscreenInitializePlan.width == 640);
    static_assert(kFullscreenInitializePlan.height == 480);
    static_assert(kFullscreenInitializePlan.bits_per_pixel == 16);
    static_assert(
        kFullscreenInitializePlan.primary_chain.surface_caps == 0x2218);
    static_assert(
        kFullscreenInitializePlan.primary_chain.backbuffer_count == 1);
    static_assert(
        kFullscreenInitializePlan.attached_surface_caps == 4);
    static_assert(
        kFullscreenInitializePlan.add_ref_attached_backbuffer);

    static_assert(kWindowedInitializePlan.cooperative_flags == 8);
    static_assert(
        kWindowedInitializePlan.primary.surface_caps == 0x200);
    static_assert(kWindowedInitializePlan.render.width == 640);
    static_assert(kWindowedInitializePlan.render.height == 480);
    static_assert(
        kWindowedInitializePlan.render.surface_caps == 0x2040);
    static_assert(kWindowedInitializePlan.create_clipper);
    static_assert(kWindowedInitializePlan.set_clipper_hwnd);
    static_assert(kWindowedInitializePlan.attach_clipper_to_primary);
    static_assert(
        kWindowedInitializePlan.release_local_clipper_reference);

    static_assert(
        mode_from_windowed_flag(0) == DisplayMode::Fullscreen);
    static_assert(
        mode_from_windowed_flag(1) == DisplayMode::Windowed);
    static_assert(
        mode_from_windowed_flag(7) == DisplayMode::Windowed);

    RetailDisplayManager32 constructed{};
    constructed.hwnd32 = 0x1234;
    constructed.destination_rect = {1,2,3,4};
    constructed.windowed = 99;
    initialize_retail_object(constructed);
    assert(constructed.vtable_ptr32 == kDisplayManagerVtable32);
    assert(constructed.direct_draw7_ptr32 == 0);
    assert(constructed.primary_surface_ptr32 == 0);
    assert(constructed.render_surface_ptr32 == 0);
    assert(constructed.auxiliary_surface_ptr32 == 0);
    // The original constructor only initializes vtable + four COM pointers.
    assert(constructed.hwnd32 == 0x1234);
    assert(constructed.destination_rect.left == 1);
    assert(constructed.windowed == 99);

    RetailDisplayManager32 destroy_state{};
    destroy_state.direct_draw7_ptr32 = 1;
    destroy_state.primary_surface_ptr32 = 2;
    destroy_state.render_surface_ptr32 = 3;
    destroy_state.auxiliary_surface_ptr32 = 4;
    constexpr auto empty_destroy =
        destroy_plan(RetailDisplayManager32{});
    static_assert(!empty_destroy.unregister_auxiliary);
    static_assert(!empty_destroy.release_direct_draw);

    const auto full_destroy = destroy_plan(destroy_state);
    assert(full_destroy.unregister_auxiliary);
    assert(full_destroy.release_auxiliary);
    assert(full_destroy.unregister_render);
    assert(full_destroy.release_render);
    assert(full_destroy.unregister_primary);
    assert(full_destroy.release_primary);
    assert(full_destroy.restore_cooperative_level_normal);
    assert(full_destroy.release_direct_draw);

    constexpr auto window_rect =
        windowed_destination_rect(100,200,740,680);
    static_assert(window_rect.left == 100);
    static_assert(window_rect.top == 200);
    static_assert(window_rect.right == 740);
    static_assert(window_rect.bottom == 680);

    constexpr auto fullscreen_rect =
        fullscreen_destination_rect(1920,1080);
    static_assert(fullscreen_rect.left == 0);
    static_assert(fullscreen_rect.top == 0);
    static_assert(fullscreen_rect.right == 1920);
    static_assert(fullscreen_rect.bottom == 1080);

    constexpr auto window_present =
        present_attempt_plan(DisplayMode::Windowed);
    static_assert(
        window_present.operation == PresentOperation::WindowedBlt);
    static_assert(window_present.blt_flags == 0x01000000);
    static_assert(window_present.use_destination_rect);
    static_assert(window_present.use_render_surface);

    constexpr auto fullscreen_present =
        present_attempt_plan(DisplayMode::Fullscreen);
    static_assert(
        fullscreen_present.operation == PresentOperation::FullscreenFlip);
    static_assert(fullscreen_present.blt_flags == 0);
    static_assert(!fullscreen_present.use_destination_rect);
    static_assert(!fullscreen_present.use_render_surface);

    static_assert(kDdErrSurfaceLost ==
                  static_cast<std::int32_t>(0x887601C2U));
    static_assert(kDdErrWasStillDrawing ==
                  static_cast<std::int32_t>(0x8876021CU));

    constexpr auto busy =
        handle_present_result(kDdErrWasStillDrawing);
    static_assert(busy.retry_same_present);
    static_assert(!busy.restore_primary);
    static_assert(!busy.set_bitmap_reload_flag);

    constexpr auto lost =
        handle_present_result(kDdErrSurfaceLost);
    static_assert(!lost.retry_same_present);
    static_assert(lost.restore_primary);
    static_assert(lost.restore_render);
    static_assert(lost.restore_all_surfaces);
    static_assert(lost.set_bitmap_reload_flag);

    constexpr auto okay = handle_present_result(kSOk);
    static_assert(!okay.retry_same_present);
    static_assert(!okay.restore_primary);
    static_assert(okay.returned_hresult == 0);

    RetailDisplayManager32 render_state{};
    auto blt = blt_fast_plan(render_state, 7);
    assert(!blt.valid_render_surface);
    assert(blt.flags == 7);

    render_state.render_surface_ptr32 = 0x1234;
    blt = blt_fast_plan(render_state, 0x42);
    assert(blt.valid_render_surface);
    assert(blt.flags == 0x42);

    const auto clear = clear_plan(render_state, 0xBEEF);
    assert(clear.valid_render_surface);
    assert(clear.blt_flags == 0x400);
    assert(clear.fill_color == 0xBEEF);
    assert(clear.ddb_lt_fx_size == 100);
}
