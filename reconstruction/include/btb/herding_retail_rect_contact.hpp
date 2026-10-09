#pragma once

#include <cstdint>

namespace btb::herding {

struct RetailCornerRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

// Exact helper at 0x004169A0 in the 311296-byte PE32 executable:
// checks four corners of FIRST rectangle against SECOND, with
// inclusive edges; does not check corners of second rectangle.
//
// Do not replace with symmetric AABB overlap. For a large first
// rectangle containing a small second one this can return FALSE,
// even though the rectangles geometrically overlap.
[[nodiscard]] constexpr bool original_herding_rect_contact(
    RetailCornerRect first,
    RetailCornerRect second) noexcept {

    const auto contains=[&](std::int32_t x,std::int32_t y) {
        return x>=second.left && x<=second.right &&
               y>=second.top && y<=second.bottom;
    };
    return contains(first.left,first.top) ||
           contains(first.right,first.top) ||
           contains(first.left,first.bottom) ||
           contains(first.right,first.bottom);
}

} // namespace btb::herding
