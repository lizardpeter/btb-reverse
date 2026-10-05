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
}
