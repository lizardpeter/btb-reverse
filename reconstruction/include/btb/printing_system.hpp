#pragma once

#include "btb/display_manager.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace btb::printing {

inline constexpr std::size_t kBitmapPrinterObjectBytes = 0x124;
inline constexpr std::size_t kDocumentNameBytes = 0x104;
inline constexpr std::uint32_t kBitmapPrinterVtable32 = 0x0043B2F4;

enum class ScaleMode : std::int32_t {
    FitPrintableWidth = 0,
    FitPrintableHeight = 1,
    RetailScreenScaled = 2,
    ExplicitTargetRect = 3,
};

// Exact 32-bit retail object built by 0x00404DD0.
//
// Pointer-shaped values remain uint32_t so this source-level reconstruction
// keeps the original x86 layout on a 64-bit host.
struct RetailBitmapPrinter32 {
    std::uint32_t vtable_ptr32{};       // +0x000
    std::uint32_t dib_allocation_ptr32{}; // +0x004
    std::uint32_t dib_bits_ptr32{};     // +0x008
    std::array<char, kDocumentNameBytes> document_name{}; // +0x00C
    display::RetailRect32 target_rect{}; // +0x110
    std::int32_t scale_mode{};           // +0x120

    constexpr void initialize_retail_defaults() noexcept {
        vtable_ptr32 = kBitmapPrinterVtable32;
        dib_allocation_ptr32 = 0;
        dib_bits_ptr32 = 0;
        document_name.fill('\0');
        target_rect = {};
        scale_mode =
            static_cast<std::int32_t>(ScaleMode::ExplicitTargetRect);
    }

    [[nodiscard]] constexpr ScaleMode mode() const noexcept {
        return static_cast<ScaleMode>(scale_mode);
    }
};

static_assert(offsetof(RetailBitmapPrinter32, vtable_ptr32) == 0x000);
static_assert(offsetof(RetailBitmapPrinter32, dib_allocation_ptr32) == 0x004);
static_assert(offsetof(RetailBitmapPrinter32, dib_bits_ptr32) == 0x008);
static_assert(offsetof(RetailBitmapPrinter32, document_name) == 0x00C);
static_assert(offsetof(RetailBitmapPrinter32, target_rect) == 0x110);
static_assert(offsetof(RetailBitmapPrinter32, scale_mode) == 0x120);
static_assert(sizeof(RetailBitmapPrinter32) == kBitmapPrinterObjectBytes);

[[nodiscard]] constexpr bool valid_scale_mode(
    std::int32_t mode) noexcept {
    return mode >= 0 && mode <= 3;
}

[[nodiscard]] constexpr bool set_scale_mode(
    RetailBitmapPrinter32& printer,
    std::int32_t mode) noexcept {
    if (!valid_scale_mode(mode)) {
        return false;
    }
    printer.scale_mode = mode;
    return true;
}

// 0x00405350 copies all four dwords and forces mode 3.
constexpr void set_target_rect(
    RetailBitmapPrinter32& printer,
    const display::RetailRect32& rect) noexcept {
    printer.target_rect = rect;
    printer.scale_mode =
        static_cast<std::int32_t>(ScaleMode::ExplicitTargetRect);
}

// 0x00405330 is lstrcpynA(object+0x0C, name, 0x104).
void set_document_name(
    RetailBitmapPrinter32& printer,
    std::string_view name) noexcept;

struct BitmapMetrics {
    std::int32_t width{};
    std::int32_t height{};
};

struct DeviceMetrics {
    std::int32_t horizontal_resolution{};
    std::int32_t vertical_resolution{};
    std::int32_t dpi_x{};
    std::int32_t dpi_y{};
};

struct ScreenMetrics {
    std::int32_t dpi_x{};
    std::int32_t dpi_y{};
};

struct TargetRectStep {
    display::RetailRect32 rect{};
    bool changed{};
    bool acquired_window_dc{};
    // The retail mode-2 branch obtains GetDC(owner_hwnd) and does not issue
    // ReleaseDC before returning. Keep that observable quirk explicit.
    bool retail_leaks_window_dc{};
};

[[nodiscard]] TargetRectStep compute_target_rect(
    ScaleMode mode,
    const BitmapMetrics& bitmap,
    const DeviceMetrics& printer,
    const ScreenMetrics& screen = {}) noexcept;

[[nodiscard]] inline TargetRectStep compute_target_rect(
    const RetailBitmapPrinter32& object,
    const BitmapMetrics& bitmap,
    const DeviceMetrics& printer,
    const ScreenMetrics& screen = {}) noexcept {
    return compute_target_rect(object.mode(), bitmap, printer, screen);
}

struct ClipMapStep {
    bool intersects{};
    display::RetailRect32 clipped_destination{};
    display::RetailRect32 mapped_source{};
};

// Exact 0x004056C0 behavior. Retail first IntersectRect()s the requested
// destination against object.target_rect, then maps the clipped offset and
// extent proportionally into a zero-based source rectangle. Positive values
// use +0.5 then x87 truncate-toward-zero rounding.
[[nodiscard]] ClipMapStep map_clipped_print_rect_to_source(
    const display::RetailRect32& target_rect,
    const display::RetailRect32& requested_destination,
    const display::RetailRect32& source_rect) noexcept;

inline constexpr std::int32_t kGamePrintScaleMode =
    static_cast<std::int32_t>(ScaleMode::RetailScreenScaled);
inline constexpr std::string_view kPrintBitmapFilename = "PrintMe.bmp";
inline constexpr std::string_view kPrintDocumentTitle =
    "Bob the Builder - Bob Builds a Park";

} // namespace btb::printing
