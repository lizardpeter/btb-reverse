#include "btb/printing_system.hpp"

#include <algorithm>
#include <cmath>

namespace btb::printing {
namespace {

[[nodiscard]] constexpr std::int32_t abs_i32(
    std::int32_t value) noexcept {
    return value < 0 ? -value : value;
}

[[nodiscard]] constexpr std::int32_t round_positive_half_up(
    double value) noexcept {
    return static_cast<std::int32_t>(value + 0.5);
}

[[nodiscard]] constexpr bool nonempty(
    const display::RetailRect32& rect) noexcept {
    return rect.right > rect.left && rect.bottom > rect.top;
}

[[nodiscard]] constexpr display::RetailRect32 intersect_rect(
    const display::RetailRect32& a,
    const display::RetailRect32& b) noexcept {
    return {
        std::max(a.left, b.left),
        std::max(a.top, b.top),
        std::min(a.right, b.right),
        std::min(a.bottom, b.bottom),
    };
}

} // namespace

void set_document_name(
    RetailBitmapPrinter32& printer,
    std::string_view name) noexcept {

    printer.document_name.fill('\0');
    const auto count =
        std::min(name.size(), printer.document_name.size() - 1);
    for (std::size_t i = 0; i < count; ++i) {
        printer.document_name[i] = name[i];
    }
}

TargetRectStep compute_target_rect(
    ScaleMode mode,
    const BitmapMetrics& bitmap,
    const DeviceMetrics& printer,
    const ScreenMetrics& screen) noexcept {

    TargetRectStep result;

    if (bitmap.width == 0 ||
        bitmap.height == 0 ||
        printer.dpi_x == 0 ||
        printer.dpi_y == 0) {
        return result;
    }

    const auto bitmap_height = abs_i32(bitmap.height);

    switch (mode) {
    case ScaleMode::FitPrintableWidth: {
        // left/top = 0, right = HORZRES.
        //
        // bottom = round(
        //   abs(bitmap_height) *
        //   HORZRES / bitmap_width *
        //   printer_dpi_y / printer_dpi_x)
        const double height =
            static_cast<double>(bitmap_height) *
            static_cast<double>(printer.horizontal_resolution) /
            static_cast<double>(bitmap.width) *
            static_cast<double>(printer.dpi_y) /
            static_cast<double>(printer.dpi_x);

        result.rect = {
            0,
            0,
            printer.horizontal_resolution,
            round_positive_half_up(height),
        };
        result.changed = true;
        return result;
    }

    case ScaleMode::FitPrintableHeight: {
        // left/top = 0, bottom = VERTRES.
        //
        // right = round(
        //   VERTRES / abs(bitmap_height) *
        //   printer_dpi_x / printer_dpi_y *
        //   bitmap_width)
        const double width =
            static_cast<double>(printer.vertical_resolution) /
            static_cast<double>(bitmap_height) *
            static_cast<double>(printer.dpi_x) /
            static_cast<double>(printer.dpi_y) *
            static_cast<double>(bitmap.width);

        result.rect = {
            0,
            0,
            round_positive_half_up(width),
            printer.vertical_resolution,
        };
        result.changed = true;
        return result;
    }

    case ScaleMode::RetailScreenScaled: {
        if (screen.dpi_x == 0 || screen.dpi_y == 0) {
            return result;
        }

        // The retail function obtains GetDC(owner_hwnd), samples screen DPI,
        // then samples printer DPI again. Both ratios are explicitly forced
        // >=1 by dividing the larger by the smaller. The Y ratio receives a
        // further hard-coded 1.5 multiplier.
        const float screen_x = static_cast<float>(screen.dpi_x);
        const float screen_y = static_cast<float>(screen.dpi_y);
        const float printer_x = static_cast<float>(printer.dpi_x);
        const float printer_y = static_cast<float>(printer.dpi_y);

        const float ratio_x =
            screen_x > printer_x
                ? screen_x / printer_x
                : printer_x / screen_x;

        const float ratio_y =
            (screen_y > printer_y
                ? screen_y / printer_y
                : printer_y / screen_y) * 1.5F;

        // Preserve the original x87 truncate-toward-zero conversions and its
        // unusual raw RECT writes. The function stores the centered left/top
        // coordinates but stores the scaled width/height directly into
        // right/bottom rather than adding left/top.
        const auto scaled_width = static_cast<std::int32_t>(
            static_cast<float>(640.0F) * ratio_x);
        const auto scaled_height = static_cast<std::int32_t>(
            static_cast<float>(480.0F) * ratio_y);

        result.rect = {
            printer.horizontal_resolution / 2 - scaled_width / 2,
            printer.vertical_resolution / 2 - scaled_height / 2,
            scaled_width,
            scaled_height,
        };
        result.changed = true;
        result.acquired_window_dc = true;
        result.retail_leaks_window_dc = true;
        return result;
    }

    case ScaleMode::ExplicitTargetRect:
        // Mode 3 leaves object+0x110 untouched.
        return result;
    }

    return result;
}

ClipMapStep map_clipped_print_rect_to_source(
    const display::RetailRect32& target_rect,
    const display::RetailRect32& requested_destination,
    const display::RetailRect32& source_rect) noexcept {

    ClipMapStep result;
    result.clipped_destination =
        intersect_rect(requested_destination, target_rect);

    if (!nonempty(result.clipped_destination)) {
        return result;
    }

    const auto target_width =
        target_rect.right - target_rect.left;
    const auto target_height =
        target_rect.bottom - target_rect.top;
    const auto source_width =
        source_rect.right - source_rect.left;
    const auto source_height =
        source_rect.bottom - source_rect.top;

    if (target_width == 0 ||
        target_height == 0 ||
        source_width == 0 ||
        source_height == 0) {
        return result;
    }

    // The executable uses ratios target/source and then divides clipped
    // destination offsets/extents by those ratios. It does not add the
    // original source left/top back in; callers pass a zero-based source RECT.
    const double horizontal_ratio =
        static_cast<double>(target_width) /
        static_cast<double>(source_width);
    const double vertical_ratio =
        static_cast<double>(target_height) /
        static_cast<double>(source_height);

    const auto x_offset =
        result.clipped_destination.left - target_rect.left;
    const auto y_offset =
        result.clipped_destination.top - target_rect.top;
    const auto clipped_width =
        result.clipped_destination.right -
        result.clipped_destination.left;
    const auto clipped_height =
        result.clipped_destination.bottom -
        result.clipped_destination.top;

    const auto source_left = round_positive_half_up(
        static_cast<double>(x_offset) / horizontal_ratio);
    const auto source_top = round_positive_half_up(
        static_cast<double>(y_offset) / vertical_ratio);
    const auto mapped_width = round_positive_half_up(
        static_cast<double>(clipped_width) / horizontal_ratio);
    const auto mapped_height = round_positive_half_up(
        static_cast<double>(clipped_height) / vertical_ratio);

    result.intersects = true;
    result.mapped_source = {
        source_left,
        source_top,
        source_left + mapped_width,
        source_top + mapped_height,
    };
    return result;
}

} // namespace btb::printing
