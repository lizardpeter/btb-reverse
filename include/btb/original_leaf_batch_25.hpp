#pragma once
// 25 separately compiled original PE32 leaf implementations.
// Evidence: analysis/ghidra_main_exe/functions/<address>.c
// Source PE sha256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
//
// Unlike header-only analysis models, these are independently emitted
// C++ object-code functions linked into btb_recompiled_core and btb_game.
// Game-global operands are explicit references until original global layout
// and x86 ABI integration is complete; retail differential parity is NOT
// established by this compilation gate.
#include "btb/input_runtime.hpp"
#include "btb/dsutil.hpp"
#include "btb/printing_system.hpp"
#include "btb/display_manager.hpp"
#include <array>
#include <cstdint>
#include <string_view>

namespace btb::original_leaf {
using I32 = std::int32_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;
using Rect = btb::display::RetailRect32;

// 00402AF0
void no_op_legacy_hook() noexcept;
// 00407AC0
void reset_mouse_bounds(btb::input::MouseBounds& bounds) noexcept;
// 00408440 / 00408450 / 00408430
void enable_input_processing(I32& original_flag_0043ed14) noexcept;
void disable_input_processing(I32& original_flag_0043ed14) noexcept;
void clear_004fbe54(I32& original_flag_004fbe54) noexcept;
// 00407A70 / 00407A90 / 004079C0
void set_cursor_hotspot(btb::input::CursorState& state,I32 x,I32 y) noexcept;
void clear_cursor_hotspot(btb::input::CursorState& state) noexcept;
struct LoadingCursorGlobals {
    void* current_surface{};
    void* default_surface{};
    I32 completion_004fbfc4{};
    I32 completion_0051c33c{};
};
void end_loading_cursor_animation(LoadingCursorGlobals& g) noexcept;
// 00409CA0 / 00409C60 / 0040DD20 / 004294D0
[[nodiscard]] bool is_dino_cursor_inside_rect(
    const btb::input::CursorState& cursor,const Rect& r) noexcept;
[[nodiscard]] bool is_dino_cursor_near_point(
    const btb::input::CursorState& cursor,I32 x,I32 y,I32 tolerance) noexcept;
[[nodiscard]] bool point_inside_rect_exclusive(I32 x,I32 y,const Rect& r) noexcept;
[[nodiscard]] bool point_inside_rect_inclusive(I32 x,I32 y,const Rect& r) noexcept;
// 00413420 / 0041FC60 / 0041A700
[[nodiscard]] I32 filetime_delta_milliseconds(I32 old_low,I32 new_low) noexcept;
[[nodiscard]] I32 filetime_delta_centiseconds(I32 old_low,I32 new_low) noexcept;
[[nodiscard]] U64 unsigned_filetime_delta_seconds(
    std::array<U32,2> old_time,std::array<U32,2> new_time) noexcept;
// 00412B50 / 0041F3F0
struct Cell {I32 row{},column{};};
[[nodiscard]] Cell decode_fireworks_cell(I32 selected_0050a5c0) noexcept;
[[nodiscard]] Cell decode_bobs_band_cell(I32 selected_0050a5c0) noexcept;
// 00423610
struct PilchardGlobals { I32 value_005109cc{},delay_005107e8{}; };
void reset_pilchard_spawn_delay(PilchardGlobals& state) noexcept;
// 004048E0
[[nodiscard]] U32 wave_file_get_size(
    const btb::dsutil::RetailCWaveFile32& file) noexcept;
// 00405330 / 00405350 / 00406970 / 00406980 / 004069B0
void printer_set_document_name(
    btb::printing::RetailBitmapPrinter32& p,std::string_view name) noexcept;
void printer_set_target_rect(
    btb::printing::RetailBitmapPrinter32& p,const Rect& rect) noexcept;
[[nodiscard]] const char* printer_get_document_name(
    const btb::printing::RetailBitmapPrinter32& p) noexcept;
[[nodiscard]] Rect printer_get_target_rect(
    const btb::printing::RetailBitmapPrinter32& p) noexcept;
[[nodiscard]] I32 printer_get_scale_mode(
    const btb::printing::RetailBitmapPrinter32& p) noexcept;
// 00415D30. Long-double is host approximation; x87 parity not certified.
[[nodiscard]] long double distance_between_integer_points(
    I32 x1,I32 y1,I32 x2,I32 y2) noexcept;

// Forces all 25 out-of-line bodies to be both linker-reachable and actually
// called by the real executable's --recompiled-smoke pathway.
[[nodiscard]] bool verify_25_original_leaf_calls() noexcept;
} // namespace btb::original_leaf
