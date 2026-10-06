#include "btb/printing_system.hpp"

#include <cassert>
#include <string>

using namespace btb::printing;

int main() {
    static_assert(kBitmapPrinterObjectBytes == 0x124);
    static_assert(kDocumentNameBytes == 0x104);
    static_assert(sizeof(RetailBitmapPrinter32) == 0x124);
    static_assert(offsetof(RetailBitmapPrinter32, document_name) == 0x0C);
    static_assert(offsetof(RetailBitmapPrinter32, target_rect) == 0x110);
    static_assert(offsetof(RetailBitmapPrinter32, scale_mode) == 0x120);
    static_assert(kBitmapPrinterVtable32 == 0x0043B2F4);

    RetailBitmapPrinter32 printer;
    printer.initialize_retail_defaults();
    assert(printer.vtable_ptr32 == kBitmapPrinterVtable32);
    assert(printer.dib_allocation_ptr32 == 0);
    assert(printer.dib_bits_ptr32 == 0);
    assert(printer.scale_mode == 3);
    assert(printer.target_rect.left == 0);
    assert(printer.target_rect.top == 0);
    assert(printer.target_rect.right == 0);
    assert(printer.target_rect.bottom == 0);
    for (const auto ch : printer.document_name) {
        assert(ch == '\0');
    }

    static_assert(valid_scale_mode(0));
    static_assert(valid_scale_mode(1));
    static_assert(valid_scale_mode(2));
    static_assert(valid_scale_mode(3));
    static_assert(!valid_scale_mode(-1));
    static_assert(!valid_scale_mode(4));

    assert(set_scale_mode(printer, 2));
    assert(printer.mode() == ScaleMode::RetailScreenScaled);
    assert(!set_scale_mode(printer, 4));
    assert(printer.mode() == ScaleMode::RetailScreenScaled);

    set_target_rect(printer, {10,20,300,400});
    assert(printer.target_rect.left == 10);
    assert(printer.target_rect.top == 20);
    assert(printer.target_rect.right == 300);
    assert(printer.target_rect.bottom == 400);
    assert(printer.mode() == ScaleMode::ExplicitTargetRect);

    set_document_name(printer, "Bob the Builder - Bob Builds a Park");
    assert(std::string(printer.document_name.data()) ==
           "Bob the Builder - Bob Builds a Park");

    // lstrcpynA count 0x104 means at most 259 source characters survive,
    // followed by the terminating NUL.
    std::string oversized(400, 'X');
    set_document_name(printer, oversized);
    assert(printer.document_name[258] == 'X');
    assert(printer.document_name[259] == '\0');

    // Mode 0 fits printable width with printer-DPI physical aspect correction.
    // 640x480 at square DPI into 2400px width -> 1800px tall.
    auto target = compute_target_rect(
        ScaleMode::FitPrintableWidth,
        {640,480},
        {2400,3000,300,300});
    assert(target.changed);
    assert(!target.acquired_window_dc);
    assert(target.rect.left == 0);
    assert(target.rect.top == 0);
    assert(target.rect.right == 2400);
    assert(target.rect.bottom == 1800);

    // Non-square printer DPI is part of the retail equation.
    target = compute_target_rect(
        ScaleMode::FitPrintableWidth,
        {640,480},
        {2400,3000,300,600});
    assert(target.rect.right == 2400);
    assert(target.rect.bottom == 3600);

    // Mode 1 fits printable height.
    target = compute_target_rect(
        ScaleMode::FitPrintableHeight,
        {640,480},
        {2400,3000,300,300});
    assert(target.changed);
    assert(target.rect.left == 0);
    assert(target.rect.top == 0);
    assert(target.rect.right == 4000);
    assert(target.rect.bottom == 3000);

    target = compute_target_rect(
        ScaleMode::FitPrintableHeight,
        {640,-480},
        {2400,3000,300,300});
    assert(target.rect.right == 4000);
    assert(target.rect.bottom == 3000);

    // Retail mode 2 samples the owner-window DC and intentionally does not
    // release it before returning. With 96dpi screen and 300dpi printer:
    // ratioX=3.125, ratioY=3.125*1.5=4.6875 => W=2000,H=2250.
    target = compute_target_rect(
        ScaleMode::RetailScreenScaled,
        {640,480},
        {2550,3300,300,300},
        {96,96});
    assert(target.changed);
    assert(target.acquired_window_dc);
    assert(target.retail_leaks_window_dc);
    assert(target.rect.left == 275);
    assert(target.rect.top == 525);
    // Preserve the shipped code's unusual raw writes: right/bottom are the
    // scaled width/height, not left+width/top+height.
    assert(target.rect.right == 2000);
    assert(target.rect.bottom == 2250);

    target = compute_target_rect(
        ScaleMode::ExplicitTargetRect,
        {640,480},
        {2550,3300,300,300},
        {96,96});
    assert(!target.changed);

    // Full target maps to the full zero-based source.
    auto clip = map_clipped_print_rect_to_source(
        {100,200,500,600},
        {100,200,500,600},
        {0,0,640,480});
    assert(clip.intersects);
    assert(clip.clipped_destination.left == 100);
    assert(clip.clipped_destination.top == 200);
    assert(clip.clipped_destination.right == 500);
    assert(clip.clipped_destination.bottom == 600);
    assert(clip.mapped_source.left == 0);
    assert(clip.mapped_source.top == 0);
    assert(clip.mapped_source.right == 640);
    assert(clip.mapped_source.bottom == 480);

    // Clip the left half away. Target is 400px wide and source is 640px, so
    // x=200 target pixels maps to source x=320.
    clip = map_clipped_print_rect_to_source(
        {100,200,500,600},
        {300,200,700,600},
        {0,0,640,480});
    assert(clip.intersects);
    assert(clip.clipped_destination.left == 300);
    assert(clip.clipped_destination.right == 500);
    assert(clip.mapped_source.left == 320);
    assert(clip.mapped_source.right == 640);
    assert(clip.mapped_source.top == 0);
    assert(clip.mapped_source.bottom == 480);

    // Vertical and horizontal clipping use +0.5 then truncate.
    clip = map_clipped_print_rect_to_source(
        {0,0,300,300},
        {75,75,225,225},
        {0,0,101,101});
    assert(clip.intersects);
    assert(clip.mapped_source.left == 25);
    assert(clip.mapped_source.top == 25);
    assert(clip.mapped_source.right == 76);
    assert(clip.mapped_source.bottom == 76);

    clip = map_clipped_print_rect_to_source(
        {0,0,100,100},
        {200,200,300,300},
        {0,0,640,480});
    assert(!clip.intersects);

    constexpr auto red565 = analyze_channel_mask(0xF800);
    static_assert(red565.trailing_zero_bits == 11);
    static_assert(red565.contiguous_one_bits == 5);
    static_assert(red565.left_shift_to_u8 == 3);

    constexpr auto green565 = analyze_channel_mask(0x07E0);
    static_assert(green565.trailing_zero_bits == 5);
    static_assert(green565.contiguous_one_bits == 6);
    static_assert(green565.left_shift_to_u8 == 2);

    constexpr auto blue565 = analyze_channel_mask(0x001F);
    static_assert(blue565.trailing_zero_bits == 0);
    static_assert(blue565.contiguous_one_bits == 5);
    static_assert(blue565.left_shift_to_u8 == 3);

    // Retail scales a 5/6-bit channel by shifting into the high bits; it does
    // not replicate low bits to reach 255.
    static_assert(extract_channel_u8(0xF800, red565) == 0xF8);
    static_assert(extract_channel_u8(0x07E0, green565) == 0xFC);
    static_assert(extract_channel_u8(0x001F, blue565) == 0xF8);

    constexpr auto pure_red =
        export_pixel_bytes(0xF800, 0xF800, 0x07E0, 0x001F);
    static_assert(pure_red.byte0 == 0x00); // green
    static_assert(pure_red.byte1 == 0xF8); // red
    static_assert(pure_red.byte2 == 0x00); // blue

    constexpr auto pure_green =
        export_pixel_bytes(0x07E0, 0xF800, 0x07E0, 0x001F);
    static_assert(pure_green.byte0 == 0xFC);
    static_assert(pure_green.byte1 == 0x00);
    static_assert(pure_green.byte2 == 0x00);

    constexpr auto pure_blue =
        export_pixel_bytes(0x001F, 0xF800, 0x07E0, 0x001F);
    static_assert(pure_blue.byte0 == 0x00);
    static_assert(pure_blue.byte1 == 0x00);
    static_assert(pure_blue.byte2 == 0xF8);

    constexpr auto export_plan =
        backbuffer_export_plan(640, 480, 1280);
    static_assert(export_plan.output_pixel_bytes == 640U * 480U * 3U);
    static_assert(export_plan.starts_from_last_source_row);
    static_assert(export_plan.walks_source_rows_upward);
    static_assert(export_plan.source_pixels_are_16_bit);
    static_assert(export_plan.output_pixels_are_24_bit);

    static_assert(kGamePrintScaleMode == 2);
    static_assert(kPrintBitmapFilename == "PrintMe.bmp");
    static_assert(
        kPrintDocumentTitle ==
        "Bob the Builder - Bob Builds a Park");
}
