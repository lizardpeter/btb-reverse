#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::display {

struct RetailRect32 {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

static_assert(sizeof(RetailRect32) == 0x10);

// Exact 32-bit display wrapper constructed at 0x00402FC0.
// All Win32/COM pointer-shaped fields remain uint32_t for host-independent
// layout fidelity.
struct RetailDisplayManager32 {
    std::uint32_t vtable_ptr32{};              // +0x00
    std::uint32_t direct_draw7_ptr32{};        // +0x04
    std::uint32_t primary_surface_ptr32{};     // +0x08
    std::uint32_t render_surface_ptr32{};      // +0x0C
    std::uint32_t auxiliary_surface_ptr32{};   // +0x10
    std::uint32_t hwnd32{};                    // +0x14
    RetailRect32 destination_rect{};           // +0x18
    std::int32_t windowed{};                   // +0x28
    std::int32_t unused_retail_2c{};           // +0x2C; allocated but never read/written by retail

    [[nodiscard]] constexpr bool is_windowed() const noexcept {
        return windowed != 0;
    }

    [[nodiscard]] constexpr std::int32_t destination_width() const noexcept {
        return destination_rect.right - destination_rect.left;
    }

    [[nodiscard]] constexpr std::int32_t destination_height() const noexcept {
        return destination_rect.bottom - destination_rect.top;
    }
};

static_assert(offsetof(RetailDisplayManager32, vtable_ptr32) == 0x00);
static_assert(offsetof(RetailDisplayManager32, direct_draw7_ptr32) == 0x04);
static_assert(offsetof(RetailDisplayManager32, primary_surface_ptr32) == 0x08);
static_assert(offsetof(RetailDisplayManager32, render_surface_ptr32) == 0x0C);
static_assert(offsetof(RetailDisplayManager32, auxiliary_surface_ptr32) == 0x10);
static_assert(offsetof(RetailDisplayManager32, hwnd32) == 0x14);
static_assert(offsetof(RetailDisplayManager32, destination_rect) == 0x18);
static_assert(offsetof(RetailDisplayManager32, windowed) == 0x28);
static_assert(offsetof(RetailDisplayManager32, unused_retail_2c) == 0x2C);
static_assert(sizeof(RetailDisplayManager32) == 0x30);

inline constexpr std::int32_t kRetailWidth = 640;
inline constexpr std::int32_t kRetailHeight = 480;
inline constexpr std::int32_t kRetailFullscreenBitsPerPixel = 16;

inline constexpr std::uint32_t kDisplayManagerVtable32 = 0x0043B2E0;

inline constexpr std::int32_t kSOk = 0;
inline constexpr std::int32_t kEPointer =
    static_cast<std::int32_t>(0x80004003U);
inline constexpr std::int32_t kEFail =
    static_cast<std::int32_t>(0x80004005U);

inline constexpr std::uint32_t kDdscFullscreenExclusive = 0x11;
inline constexpr std::uint32_t kDdscNormal = 0x08;

inline constexpr std::uint32_t kFullscreenSurfaceFlags = 0x21;
inline constexpr std::uint32_t kFullscreenSurfaceCaps = 0x2218;
inline constexpr std::int32_t kFullscreenBackBufferCount = 1;
inline constexpr std::uint32_t kAttachedBackBufferCaps = 0x04;

inline constexpr std::uint32_t kWindowedPrimarySurfaceFlags = 0x01;
inline constexpr std::uint32_t kWindowedPrimarySurfaceCaps = 0x0200;
inline constexpr std::uint32_t kWindowedRenderSurfaceFlags = 0x07;
inline constexpr std::uint32_t kWindowedRenderSurfaceCaps = 0x2040;

inline constexpr std::uint32_t kDDBltWait = 0x01000000;
inline constexpr std::uint32_t kDDBltColorFill = 0x00000400;

inline constexpr std::int32_t kDdErrSurfaceLost =
    static_cast<std::int32_t>(0x887601C2U);
inline constexpr std::int32_t kDdErrWasStillDrawing =
    static_cast<std::int32_t>(0x8876021CU);

enum class DisplayMode : std::int32_t {
    Fullscreen = 0,
    Windowed = 1,
};

struct SurfaceCreatePlan {
    std::uint32_t descriptor_size{0x7C};
    std::uint32_t descriptor_flags{};
    std::int32_t width{};
    std::int32_t height{};
    std::uint32_t surface_caps{};
    std::int32_t backbuffer_count{};
};

struct FullscreenInitializePlan {
    std::uint32_t cooperative_flags{kDdscFullscreenExclusive};
    std::int32_t width{kRetailWidth};
    std::int32_t height{kRetailHeight};
    std::int32_t bits_per_pixel{kRetailFullscreenBitsPerPixel};
    std::int32_t refresh_rate{};
    std::uint32_t display_mode_flags{};
    SurfaceCreatePlan primary_chain{
        0x7C,
        kFullscreenSurfaceFlags,
        0,
        0,
        kFullscreenSurfaceCaps,
        kFullscreenBackBufferCount,
    };
    std::uint32_t attached_surface_caps{kAttachedBackBufferCaps};
    bool add_ref_attached_backbuffer{true};
};

inline constexpr FullscreenInitializePlan kFullscreenInitializePlan{};

struct WindowedInitializePlan {
    std::uint32_t cooperative_flags{kDdscNormal};
    std::int32_t width{kRetailWidth};
    std::int32_t height{kRetailHeight};
    SurfaceCreatePlan primary{
        0x7C,
        kWindowedPrimarySurfaceFlags,
        0,
        0,
        kWindowedPrimarySurfaceCaps,
        0,
    };
    SurfaceCreatePlan render{
        0x7C,
        kWindowedRenderSurfaceFlags,
        kRetailWidth,
        kRetailHeight,
        kWindowedRenderSurfaceCaps,
        0,
    };
    bool create_clipper{true};
    bool set_clipper_hwnd{true};
    bool attach_clipper_to_primary{true};
    bool release_local_clipper_reference{true};
};

inline constexpr WindowedInitializePlan kWindowedInitializePlan{};

[[nodiscard]] constexpr DisplayMode mode_from_windowed_flag(
    std::int32_t windowed) noexcept {
    return windowed == 0
        ? DisplayMode::Fullscreen
        : DisplayMode::Windowed;
}

constexpr void initialize_retail_object(
    RetailDisplayManager32& display) noexcept {
    display.vtable_ptr32 = kDisplayManagerVtable32;
    display.direct_draw7_ptr32 = 0;
    display.primary_surface_ptr32 = 0;
    display.render_surface_ptr32 = 0;
    display.auxiliary_surface_ptr32 = 0;
}

struct DestroyPlan {
    bool unregister_auxiliary{};
    bool release_auxiliary{};
    bool unregister_render{};
    bool release_render{};
    bool unregister_primary{};
    bool release_primary{};
    bool restore_cooperative_level_normal{};
    bool release_direct_draw{};
};

[[nodiscard]] constexpr DestroyPlan destroy_plan(
    const RetailDisplayManager32& display) noexcept {
    return {
        display.auxiliary_surface_ptr32 != 0,
        display.auxiliary_surface_ptr32 != 0,
        display.render_surface_ptr32 != 0,
        display.render_surface_ptr32 != 0,
        display.primary_surface_ptr32 != 0,
        display.primary_surface_ptr32 != 0,
        display.direct_draw7_ptr32 != 0,
        display.direct_draw7_ptr32 != 0,
    };
}

[[nodiscard]] constexpr RetailRect32 windowed_destination_rect(
    std::int32_t top_left_screen_x,
    std::int32_t top_left_screen_y,
    std::int32_t bottom_right_screen_x,
    std::int32_t bottom_right_screen_y) noexcept {
    return {
        top_left_screen_x,
        top_left_screen_y,
        bottom_right_screen_x,
        bottom_right_screen_y,
    };
}

[[nodiscard]] constexpr RetailRect32 fullscreen_destination_rect(
    std::int32_t screen_width,
    std::int32_t screen_height) noexcept {
    return {0,0,screen_width,screen_height};
}

enum class PresentOperation {
    WindowedBlt,
    FullscreenFlip,
};

[[nodiscard]] constexpr PresentOperation present_operation(
    DisplayMode mode) noexcept {
    return mode == DisplayMode::Windowed
        ? PresentOperation::WindowedBlt
        : PresentOperation::FullscreenFlip;
}

struct PresentAttemptPlan {
    PresentOperation operation{PresentOperation::FullscreenFlip};
    std::uint32_t blt_flags{};
    bool use_destination_rect{};
    bool use_render_surface{};
};

[[nodiscard]] constexpr PresentAttemptPlan present_attempt_plan(
    DisplayMode mode) noexcept {
    if (mode == DisplayMode::Windowed) {
        return {
            PresentOperation::WindowedBlt,
            kDDBltWait,
            true,
            true,
        };
    }
    return {
        PresentOperation::FullscreenFlip,
        0,
        false,
        false,
    };
}

struct PresentResultStep {
    bool retry_same_present{};
    bool restore_primary{};
    bool restore_render{};
    bool restore_all_surfaces{};
    bool set_bitmap_reload_flag{};
    std::int32_t returned_hresult{};
};

[[nodiscard]] constexpr PresentResultStep handle_present_result(
    std::int32_t hresult) noexcept {
    if (hresult == kDdErrWasStillDrawing) {
        return {
            true,
            false,
            false,
            false,
            false,
            hresult,
        };
    }

    if (hresult == kDdErrSurfaceLost) {
        return {
            false,
            true,
            true,
            true,
            true,
            hresult,
        };
    }

    return {
        false,
        false,
        false,
        false,
        false,
        hresult,
    };
}

struct BltFastPlan {
    bool valid_render_surface{};
    std::uint32_t flags{};
};

[[nodiscard]] constexpr BltFastPlan blt_fast_plan(
    const RetailDisplayManager32& display,
    std::uint32_t flags) noexcept {
    return {display.render_surface_ptr32 != 0, flags};
}

struct ClearPlan {
    bool valid_render_surface{};
    std::uint32_t blt_flags{kDDBltColorFill};
    std::uint32_t fill_color{};
    std::uint32_t ddb_lt_fx_size{100};
};

[[nodiscard]] constexpr ClearPlan clear_plan(
    const RetailDisplayManager32& display,
    std::uint32_t fill_color) noexcept {
    return {
        display.render_surface_ptr32 != 0,
        kDDBltColorFill,
        fill_color,
        100,
    };
}

} // namespace btb::display
