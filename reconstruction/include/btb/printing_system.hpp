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

inline constexpr std::int32_t kDocInfoSize = 20;
inline constexpr std::int32_t kRasterCapsRejectedPaletteBit = 0x100;
inline constexpr std::int32_t kEscapeQuerySupport = 8;
inline constexpr std::int32_t kEscapeNextBand = 3;
inline constexpr std::int32_t kStretchMode = 3;
inline constexpr std::uint32_t kStretchRopSrcCopy = 0x00CC0020;
inline constexpr std::uint32_t kDibRgbColors = 0;

enum class PrintValidationError {
    None,
    NoBitmapDefined,
    InvalidPrinterDc,
    InvalidTargetRectangle,
    UnsupportedPalettePrinter,
    MissingDibInfo,
    StretchDibFailed,
};

struct DocumentPrintPlan {
    PrintValidationError error{PrintValidationError::None};
    bool call_start_doc{};
    bool call_start_page{};
    bool call_render_to_dc{};
    bool call_end_page{};
    bool call_end_doc{};
};

[[nodiscard]] constexpr DocumentPrintPlan document_print_plan(
    bool has_bitmap,
    bool valid_printer_dc,
    std::int32_t start_doc_result,
    std::int32_t start_page_result) noexcept {

    if (!has_bitmap) {
        return {PrintValidationError::NoBitmapDefined};
    }
    if (!valid_printer_dc) {
        return {PrintValidationError::InvalidPrinterDc};
    }

    DocumentPrintPlan out;
    out.call_start_doc = true;

    if (start_doc_result <= 0) {
        return out;
    }

    out.call_start_page = true;
    out.call_end_doc = true;

    if (start_page_result <= 0) {
        return out;
    }

    out.call_render_to_dc = true;
    out.call_end_page = true;
    return out;
}

struct StretchDibPlan {
    PrintValidationError error{PrintValidationError::None};
    bool call_escape_query_support{};
    std::int32_t escape_query_code{kEscapeQuerySupport};
    std::int32_t escape_requested_operation{kEscapeNextBand};
    bool call_get_version_ex{};
    bool call_set_stretch_mode{};
    std::int32_t stretch_mode{kStretchMode};
    bool call_stretch_dibits{};
    std::uint32_t dib_usage{kDibRgbColors};
    std::uint32_t raster_op{kStretchRopSrcCopy};
};

[[nodiscard]] constexpr StretchDibPlan stretch_dib_plan(
    const display::RetailRect32& target_rect,
    std::int32_t raster_caps,
    bool has_dib_allocation,
    bool has_dib_bits) noexcept {

    if (target_rect.right <= target_rect.left ||
        target_rect.bottom <= target_rect.top) {
        return {PrintValidationError::InvalidTargetRectangle};
    }

    if ((raster_caps & kRasterCapsRejectedPaletteBit) != 0) {
        return {PrintValidationError::UnsupportedPalettePrinter};
    }

    if (!has_dib_allocation || !has_dib_bits) {
        return {PrintValidationError::MissingDibInfo};
    }

    StretchDibPlan out;
    out.call_escape_query_support = true;
    out.call_get_version_ex = true;
    out.call_set_stretch_mode = true;
    out.call_stretch_dibits = true;
    return out;
}

[[nodiscard]] constexpr PrintValidationError stretch_result_error(
    std::int32_t stretch_dibits_result) noexcept {
    return stretch_dibits_result == -1
        ? PrintValidationError::StretchDibFailed
        : PrintValidationError::None;
}

struct ChannelMaskInfo {
    std::uint32_t mask{};
    std::int32_t trailing_zero_bits{};
    std::int32_t contiguous_one_bits{};
    std::int32_t left_shift_to_u8{};
};

[[nodiscard]] constexpr ChannelMaskInfo analyze_channel_mask(
    std::uint32_t mask) noexcept {

    ChannelMaskInfo out;
    out.mask = mask;

    if (mask == 0) {
        out.left_shift_to_u8 = 8;
        return out;
    }

    auto shifted = mask;
    while ((shifted & 1U) == 0U) {
        shifted >>= 1U;
        ++out.trailing_zero_bits;
    }

    while ((shifted & 1U) != 0U) {
        shifted >>= 1U;
        ++out.contiguous_one_bits;
    }

    out.left_shift_to_u8 = 8 - out.contiguous_one_bits;
    return out;
}

[[nodiscard]] constexpr std::uint8_t extract_channel_u8(
    std::uint16_t pixel,
    const ChannelMaskInfo& channel) noexcept {

    if (channel.contiguous_one_bits <= 0) {
        return 0;
    }

    auto value =
        static_cast<std::uint32_t>(pixel) >>
        static_cast<std::uint32_t>(channel.trailing_zero_bits);

    if (channel.left_shift_to_u8 >= 0) {
        value <<= static_cast<std::uint32_t>(
            channel.left_shift_to_u8);
    } else {
        value >>= static_cast<std::uint32_t>(
            -channel.left_shift_to_u8);
    }

    return static_cast<std::uint8_t>(value & 0xFFU);
}

struct ExportedPixelBytes {
    std::uint8_t byte0{};
    std::uint8_t byte1{};
    std::uint8_t byte2{};
};

// Exact 0x00409460 write order. Despite constructing a 24-bit BMP, retail
// writes channels in G,R,B order from the DDSURFACEDESC2 RGB masks.
[[nodiscard]] constexpr ExportedPixelBytes export_pixel_bytes(
    std::uint16_t pixel,
    std::uint32_t red_mask,
    std::uint32_t green_mask,
    std::uint32_t blue_mask) noexcept {

    const auto red = analyze_channel_mask(red_mask);
    const auto green = analyze_channel_mask(green_mask);
    const auto blue = analyze_channel_mask(blue_mask);

    return {
        extract_channel_u8(pixel, green),
        extract_channel_u8(pixel, red),
        extract_channel_u8(pixel, blue),
    };
}

struct BackBufferExportPlan {
    std::int32_t width{};
    std::int32_t height{};
    std::int32_t source_pitch{};
    std::size_t output_pixel_bytes{};
    bool starts_from_last_source_row{true};
    bool walks_source_rows_upward{true};
    bool source_pixels_are_16_bit{true};
    bool output_pixels_are_24_bit{true};
};

[[nodiscard]] constexpr BackBufferExportPlan backbuffer_export_plan(
    std::int32_t width,
    std::int32_t height,
    std::int32_t source_pitch) noexcept {

    return {
        width,
        height,
        source_pitch,
        static_cast<std::size_t>(width) *
            static_cast<std::size_t>(height) * 3U,
        true,
        true,
        true,
        true,
    };
}

inline constexpr std::int32_t kGamePrintScaleMode =
    static_cast<std::int32_t>(ScaleMode::RetailScreenScaled);
inline constexpr std::string_view kPrintBitmapFilename = "PrintMe.bmp";
inline constexpr std::string_view kPrintDocumentTitle =
    "Bob the Builder - Bob Builds a Park";

} // namespace btb::printing
