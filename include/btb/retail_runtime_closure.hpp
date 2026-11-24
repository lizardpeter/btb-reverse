#pragma once
// Bob Builds a Park / retail PE32 SHA-256:
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Host-neutral source closures for Ghidra-identified gameplay and DirectDraw.
// Hardware callbacks dispatch real native calls; none are fabricated.
// Original addresses are included below. Modern bounds checks are marked.

#include "btb/retail_shared_runtime.hpp"
#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <span>

namespace btb::retail_closure {

// 00411010: placement validates consecutive slots (even across the 6-wide
// row boundary), then writes first-cell type and type+14 continuation cells.
// param_4==0 is a pure validation path, but still checks every cell.
[[nodiscard]] inline bool place_firework(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t row,std::int32_t col,std::int32_t type,
    bool commit) noexcept {
    if(type<0 || type>=static_cast<std::int32_t>(type_lengths.size()))
        return false; // host safety
    const auto count=type_lengths[static_cast<std::size_t>(type)];
    if(!btb::retail_shared::can_place_firework(grid,row,col,count))
        return false;
    if(!commit) return true;
    const std::int64_t start=static_cast<std::int64_t>(row)*6+col;
    if(start<0 || start>=static_cast<std::int64_t>(grid.size()))
        return false;
    grid[static_cast<std::size_t>(start)]=type;
    for(std::int32_t i=1;i<count;++i)
        grid[static_cast<std::size_t>(start+i)]=type+14;
    return true;
}

// 00412B80: action 12 alone can delete. Ghidra derives row/column from
// the same selected flat cell global (00412B50), and refuses continuation
// marker values >=12. Delete an item through the original removal helper.
[[nodiscard]] inline bool delete_selected_firework(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t action,std::int32_t selected_cell) noexcept {
    if(action!=12 || selected_cell<0 ||
       selected_cell>=static_cast<std::int32_t>(grid.size()))
        return false;
    const auto type=grid[static_cast<std::size_t>(selected_cell)];
    if(type<0 || type>=12) return false;
    return btb::retail_shared::remove_firework(
        grid,type_lengths,selected_cell/6,selected_cell%6)>=0;
}

// 0041F430: the Grand Opening 24-wide grid stores continuation markers
// (>9). Removal scans BACK from selected tail cell to its head (<=9), then
// erases the entire object using its source-declared length.
// Original invalid head/overrun cases could fault; we safely refuse them.
[[nodiscard]] inline std::int32_t remove_grand_opening_event(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t row,std::int32_t col) noexcept {
    if(!btb::retail_shared::can_place_grand_opening(
           grid,row,col,type_lengths.empty()?0:type_lengths[0]))
        {
            const auto begin=static_cast<std::int64_t>(row)*24;
            std::int64_t idx=begin+col;
            if(col<0 || col>=24 || begin<0 ||
               idx>=static_cast<std::int64_t>(grid.size())) return -1;
            while(idx>begin && grid[static_cast<std::size_t>(idx)]>9) --idx;
            const auto type=grid[static_cast<std::size_t>(idx)];
            if(type<0 || type>=static_cast<std::int32_t>(type_lengths.size()))
                return -1;
            const auto length=type_lengths[static_cast<std::size_t>(type)];
            if(length<1 || idx+length>static_cast<std::int64_t>(grid.size()))
                return -1;
            for(std::int32_t i=0;i<length;++i)
                grid[static_cast<std::size_t>(idx+i)]=-1;
            return type;
        }
    return -1;
}

// 00403540: creates a 100-byte DirectDraw DDBLTFX with dwSize=100 and
// dwFillColor at DWORD index20. Calls the source method 0x14 Blt with
// DDBLT_COLORFILL (0x400) and null rectangles.
using BltFill = std::uint32_t (*)(
    void* back_buffer,const std::array<std::uint32_t,25>& effect,
    std::uint32_t flags) noexcept;
[[nodiscard]] inline std::uint32_t clear_back_buffer(
    void* back_buffer,std::uint32_t fill_color,BltFill blt) noexcept {
    if(!back_buffer) return static_cast<std::uint32_t>(btb::display::kEPointer);
    if(!blt) return static_cast<std::uint32_t>(btb::display::kEPointer);
    std::array<std::uint32_t,25> effect{};
    effect[0]=100;
    effect[20]=fill_color;
    return blt(back_buffer,effect,btb::display::kDDBltColorFill);
}

// 00403500: direct virtual call to render surface method 0x1C BltFast.
// The null-surface path returns E_POINTER rather than reporting success.
using BltFast = std::uint32_t (*)(
    void* back_buffer,std::uint32_t x,std::uint32_t y,
    void* source,const void* rect,std::uint32_t flags) noexcept;
[[nodiscard]] inline std::uint32_t blt_fast_to_back_buffer(
    void* back_buffer,std::uint32_t x,std::uint32_t y,
    void* source,const void* rect,std::uint32_t flags,
    BltFast blt) noexcept {
    if(!back_buffer || !blt)
        return static_cast<std::uint32_t>(btb::display::kEPointer);
    return blt(back_buffer,x,y,source,rect,flags);
}

// 00402620: original global DirectDraw RestoreAllSurfaces HRESULT
// returns 0 on SUCCEEDED, preserves FAILED HRESULT bits unmodified.
[[nodiscard]] constexpr std::uint32_t restore_surfaces_result(
    std::uint32_t hresult) noexcept {
    return std::bit_cast<std::int32_t>(hresult)<0 ? hresult : 0U;
}
using RestoreAll = std::uint32_t (*)(void*) noexcept;
[[nodiscard]] inline std::uint32_t restore_all_surfaces(
    void* directdraw,RestoreAll restore) noexcept {
    if(!directdraw||!restore)
        return static_cast<std::uint32_t>(btb::display::kEPointer);
    return restore_surfaces_result(restore(directdraw));
}

// 00403590: window mode first GetClientRect, then transforms its
// top-left AND bottom-right through ClientToScreen. Fullscreen uses
// GetSystemMetrics(0/1) and a zero-origin rect. Native callback order matters.
using GetClientRect = bool (*)(std::uintptr_t hwnd,
                              btb::display::RetailRect32&) noexcept;
using ClientToScreen = void (*)(std::uintptr_t hwnd,
                               std::int32_t&,std::int32_t&) noexcept;
using ScreenSize = std::int32_t (*)(std::int32_t index) noexcept;
inline void update_display_destination_rect(
    btb::display::RetailDisplayManager32& display,GetClientRect get_client,
    ClientToScreen client_to_screen,ScreenSize screen_size) noexcept {
    if(display.windowed) {
        if(get_client) (void)get_client(display.hwnd32,display.destination_rect);
        if(client_to_screen) {
            auto& r=display.destination_rect;
            client_to_screen(display.hwnd32,r.left,r.top);
            client_to_screen(display.hwnd32,r.right,r.bottom);
        }
    } else if(screen_size) {
        display.destination_rect={0,0,screen_size(0),screen_size(1)};
    }
}

// 00414A30 uses a global x87 radians-per-degree multiplier. Supplying the
// original global's float value keeps this a source-level model rather than
// claiming transcendental bit-for-bit parity on all C++ hosts.
struct Vec2f { float x{};float y{}; };
[[nodiscard]] inline Vec2f vector_from_degrees_magnitude(
    std::int32_t angle,std::int32_t magnitude,float radians_per_degree) noexcept {
    const long double radians=static_cast<long double>(angle)*
        static_cast<long double>(radians_per_degree);
    return {static_cast<float>(std::cos(radians) * magnitude),
            static_cast<float>(std::sin(radians) * magnitude)};
}

// 0041AE90: compares float input to ORIGINAL GLOBAL float threshold,
// not necessarily zero; returns negated value only when less than threshold.
// Native execution returns x87 extended precision, model uses long double.
[[nodiscard]] constexpr long double float_abs_with_retail_threshold(
    float value,float threshold) noexcept {
    return value<threshold ? -static_cast<long double>(value)
                           : static_cast<long double>(value);
}

} // namespace btb::retail_closure
