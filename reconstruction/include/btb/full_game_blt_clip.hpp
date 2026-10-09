#pragma once

#include "btb/display_manager.hpp"
#include "btb/full_game_runtime.hpp"

#include <algorithm>
#include <cstdint>

namespace btb::full_game {

// Win32 DirectDraw blits used by the 640x480 original game may have a
// negative destination coordinate or extend past the viewport. Clip
// destination and source *together*; never stretch, mirror, or wrap a
// cropped original sprite into a different image region.
//
// This is a platform-independent geometry helper so the exact same
// crop policy can be checked without compiling the Windows COM host.
struct NativeClippedBlt {
    Rect source{};
    Rect destination{};
    bool visible{};
};

[[nodiscard]] constexpr NativeClippedBlt retail_clip_blt_to_rect(
    Rect original_source,
    int dest_x,
    int dest_y,
    Rect destination_viewport) noexcept {

    using std::int64_t;
    const int64_t width =
        static_cast<int64_t>(original_source.right)-original_source.left;
    const int64_t height =
        static_cast<int64_t>(original_source.bottom)-original_source.top;
    if (width<=0 || height<=0) {
        return {};
    }

    const int64_t left=dest_x;
    const int64_t top=dest_y;
    const int64_t right=left+width;
    const int64_t bottom=top+height;

    const int64_t min_x=std::clamp<int64_t>(
        destination_viewport.left,0,display::kRetailWidth);
    const int64_t min_y=std::clamp<int64_t>(
        destination_viewport.top,0,display::kRetailHeight);
    const int64_t max_x=std::clamp<int64_t>(
        destination_viewport.right,0,display::kRetailWidth);
    const int64_t max_y=std::clamp<int64_t>(
        destination_viewport.bottom,0,display::kRetailHeight);
    const int64_t clipped_left=std::max<int64_t>(left,min_x);
    const int64_t clipped_top=std::max<int64_t>(top,min_y);
    const int64_t clipped_right=std::min<int64_t>(right,max_x);
    const int64_t clipped_bottom=std::min<int64_t>(bottom,max_y);

    if (clipped_left>=clipped_right ||
        clipped_top>=clipped_bottom) {
        return {};
    }
    return {
        Rect{
            static_cast<int>(
                static_cast<int64_t>(original_source.left)+
                clipped_left-left),
            static_cast<int>(
                static_cast<int64_t>(original_source.top)+
                clipped_top-top),
            static_cast<int>(
                static_cast<int64_t>(original_source.right)-
                (right-clipped_right)),
            static_cast<int>(
                static_cast<int64_t>(original_source.bottom)-
                (bottom-clipped_bottom)),
        },
        Rect{
            static_cast<int>(clipped_left),
            static_cast<int>(clipped_top),
            static_cast<int>(clipped_right),
            static_cast<int>(clipped_bottom)
        },
        true,
    };
}

[[nodiscard]] constexpr NativeClippedBlt retail_clip_blt(
    Rect original_source, int dest_x, int dest_y) noexcept {
    return retail_clip_blt_to_rect(
        original_source,dest_x,dest_y,
        Rect{0,0,display::kRetailWidth,display::kRetailHeight});
}

} // namespace btb::full_game
