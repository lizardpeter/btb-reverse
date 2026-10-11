#pragma once
// Evidence-backed, host-neutral closures of low-complexity original functions.
// Address references are to Bob Builds a Park retail Windows PE32 SHA-256
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
// Shared global storage is passed in explicitly; hardware calls are callbacks.
// This is NOT a new game runtime or a complete retail WinMain replacement.

#include "btb/bink_system.hpp"
#include "btb/dino_level.hpp"
#include "btb/dsutil.hpp"
#include "btb/input_runtime.hpp"
#include "btb/printing_system.hpp"
#include "btb/retail_point_in_polygon.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace btb::retail_leaf {

// 00402AF0 (retail body is one RET instruction).
constexpr void no_op_legacy_hook() noexcept {}

// 00407AC0: resets four globals in the order encoded by the original.
constexpr void reset_mouse_bounds(btb::input::MouseBounds& bounds) noexcept {
    bounds.max_x = 637;
    bounds.min_y = 2;
    bounds.min_x = 2;
    bounds.max_y = 477;
}

// 00408440 / 00408450: 32-bit input gate at original 0043ED14.
constexpr void enable_input_processing(std::int32_t& flag) noexcept {
    flag = 1;
}
constexpr void disable_input_processing(std::int32_t& flag) noexcept {
    flag = 0;
}
// 00408430: unlabelled original function only clears global 004FBE54.
// Keep unknown meaning explicit rather than invent a semantic name.
constexpr void clear_original_flag_004fbe54(std::int32_t& flag) noexcept {
    flag = 0;
}

// 00407A70 / 00407A90: original cursor position and hotspot globals.
constexpr void set_cursor_hotspot(btb::input::CursorState& state,
                                  std::int32_t x, std::int32_t y) noexcept {
    btb::input::set_hotspot(state, x, y);
}
constexpr void clear_cursor_hotspot_and_clamp_nonnegative(
    btb::input::CursorState& state) noexcept {
    btb::input::clear_hotspot_and_clamp_nonnegative(state);
}

struct Rect32 {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

// 004079C0/004079E0/00407A40. Retail cursor uses 6 100x100 frames;
// loading cursor is drawn at 0x118,0xBE with blit flag 1.
struct LoadingCursorState {
    void* current_surface{};
    void* default_surface{};
    void* loading_surface{};
    std::int32_t current_frame{};
    std::int32_t completion_latch_004fbfc4{};
    std::int32_t completion_latch_0051c33c{};
    Rect32 source_rectangle{};
};
constexpr void end_loading_cursor_animation(LoadingCursorState& s) noexcept {
    s.current_surface = s.default_surface;
    s.completion_latch_004fbfc4 = 0;
    s.completion_latch_0051c33c = 0;
}
inline void update_loading_cursor_animation(
    LoadingCursorState& s, void (*poll_direct_input)()) {
    if (s.current_surface != s.loading_surface) return;
    // 004079E0 polls the input system BEFORE advancing the sprite frame.
    if (poll_direct_input) poll_direct_input();
    ++s.current_frame;
    if (s.current_frame > 5) s.current_frame = 0;
    s.source_rectangle = {s.current_frame * 100, 0,
                          s.current_frame * 100 + 100, 100};
}
using LoadingCursorBlit = void (*)(void*,std::int32_t,std::int32_t,
                                   void*,const Rect32&,std::int32_t);
inline void draw_loading_cursor_animation(
    void* destination, const LoadingCursorState& s,
    LoadingCursorBlit blit) {
    if (blit) blit(destination, 0x118, 0xBE,
                   s.current_surface, s.source_rectangle, 1);
}

// 00409CA0: both rectangle endpoints inclusive.
[[nodiscard]] constexpr bool dino_cursor_inside_rect(
    const btb::input::CursorState& cursor,
    const Rect32& bounds) noexcept {
    return bounds.left <= cursor.x && cursor.x <= bounds.right &&
           bounds.top <= cursor.y && cursor.y <= bounds.bottom;
}

// 00409C60: signed 32-bit x86 subtraction/abs has wrapping behavior.
// Return signed compares, including unusual INT_MIN edge, without C++ UB.
[[nodiscard]] constexpr std::int32_t x86_signed_abs_diff(
    std::int32_t a,std::int32_t b) noexcept {
    const auto diff = static_cast<std::uint32_t>(a) -
                      static_cast<std::uint32_t>(b);
    const std::uint32_t mask = (diff & 0x80000000U) ? 0xFFFFFFFFU : 0U;
    return std::bit_cast<std::int32_t>((diff ^ mask) - mask);
}
[[nodiscard]] constexpr bool dino_cursor_near_point(
    const btb::input::CursorState& cursor,
    std::int32_t x,std::int32_t y,std::int32_t tolerance) noexcept {
    return x86_signed_abs_diff(cursor.x,x) <= tolerance &&
           x86_signed_abs_diff(cursor.y,y) <= tolerance;
}

// 0040DD20 strict rectangle tests; 004294D0 inclusive rectangle tests.
[[nodiscard]] constexpr bool point_inside_rect_exclusive(
    std::int32_t x,std::int32_t y,const Rect32& r) noexcept {
    return r.left < x && x < r.right && r.top < y && y < r.bottom;
}
[[nodiscard]] constexpr bool point_in_rect_inclusive(
    std::int32_t x,std::int32_t y,const Rect32& r) noexcept {
    return r.left <= x && x <= r.right && r.top <= y && y <= r.bottom;
}

// 00413420 / 0041FC60 use ONLY the low 32 bits of FILETIME and signed
// subtraction, not full 64-bit Windows FILETIME arithmetic.
[[nodiscard]] constexpr std::int32_t low_dword_delta_divided(
    std::int32_t earlier,std::int32_t later,std::int32_t divisor) noexcept {
    const auto delta = std::bit_cast<std::int32_t>(
        static_cast<std::uint32_t>(later) -
        static_cast<std::uint32_t>(earlier));
    return delta / divisor;
}
[[nodiscard]] constexpr std::int32_t filetime_delta_milliseconds(
    std::int32_t low_earlier,std::int32_t low_later) noexcept {
    return low_dword_delta_divided(low_earlier,low_later,10000);
}
[[nodiscard]] constexpr std::int32_t filetime_delta_centiseconds(
    std::int32_t low_earlier,std::int32_t low_later) noexcept {
    return low_dword_delta_divided(low_earlier,low_later,100000);
}
// 0041A700 reconstructs an unsigned 64-bit subtract with borrow, then
// uses __aulldiv(..., 10,000,000).
[[nodiscard]] constexpr std::uint64_t filetime_delta_seconds(
    std::array<std::uint32_t,2> earlier,
    std::array<std::uint32_t,2> later) noexcept {
    const auto first = (static_cast<std::uint64_t>(earlier[1])<<32) | earlier[0];
    const auto last = (static_cast<std::uint64_t>(later[1])<<32) | later[0];
    return (last-first)/10000000ULL;
}

struct GridCell {
    std::int32_t row{};
    std::int32_t column{};
};
// 00412B50 divides by six; 0041F3F0 divides by 24.
// C++ signed / and % truncate toward zero as original x86 IDIV does.
[[nodiscard]] constexpr GridCell decode_fireworks_cell(std::int32_t index) noexcept {
    return {index/6,index%6};
}
[[nodiscard]] constexpr GridCell decode_bobs_band_cell(std::int32_t index) noexcept {
    return {index/24,index%24};
}
// 00423610: exact two writes, no random reseed.
struct PilchardSpawnState {
    std::int32_t value_005109cc{};
    std::int32_t delay_005107e8{};
};
constexpr void reset_pilchard_spawn_delay(PilchardSpawnState& s) noexcept {
    s.value_005109cc = 0;
    s.delay_005107e8 = 500;
}

// 004048E0 is the native CWaveFile m_dwSize getter.
[[nodiscard]] constexpr std::uint32_t wave_file_get_size(
    const btb::dsutil::RetailCWaveFile32& file) noexcept {
    return file.m_dwSize;
}

// 00405330/00405350 and 00406970/00406980/004069B0 are operations on
// the exact 0x124-byte printer struct. Reuse the proven native layout.
inline void printer_set_document_name(
    btb::printing::RetailBitmapPrinter32& p,std::string_view name) noexcept {
    btb::printing::set_document_name(p,name);
}
constexpr void printer_set_target_rect(
    btb::printing::RetailBitmapPrinter32& p,
    const btb::display::RetailRect32& rect) noexcept {
    btb::printing::set_target_rect(p,rect);
}
[[nodiscard]] constexpr const char* printer_get_document_name(
    const btb::printing::RetailBitmapPrinter32& p) noexcept {
    return p.document_name.data();
}
[[nodiscard]] constexpr btb::display::RetailRect32 printer_get_target_rect(
    const btb::printing::RetailBitmapPrinter32& p) noexcept {
    return p.target_rect;
}
[[nodiscard]] constexpr std::int32_t printer_get_scale_mode(
    const btb::printing::RetailBitmapPrinter32& p) noexcept {
    return p.scale_mode;
}

// 00408D80 calls BinkGoto(movie,1,0); 00408EA0 calls BinkClose(movie).
// Real Bink providers are supplied by the host; no synthetic movie data.
using BinkGoto = void (*)(void*,std::int32_t,std::int32_t);
using BinkClose = void (*)(void*);
inline void restart_bink_movie(void* movie,BinkGoto goto_frame) {
    if (goto_frame) goto_frame(movie,1,0);
}
inline void close_bink_movie(void* movie,BinkClose close) {
    if (close) close(movie);
}

// 00415D30 computes the Euclidean length using x87 intermediates.
// long double is the closest host-neutral model, not guaranteed bit-level
// x87 transcendental parity on platforms whose long double is binary64.
[[nodiscard]] inline long double distance_between_integer_points(
    std::int32_t x1,std::int32_t y1,std::int32_t x2,std::int32_t y2) noexcept {
    const auto dx=static_cast<long double>(x86_signed_abs_diff(x1,x2));
    const auto dy=static_cast<long double>(x86_signed_abs_diff(y1,y2));
    return std::sqrt(dx*dx+dy*dy);
}

} // namespace btb::retail_leaf
