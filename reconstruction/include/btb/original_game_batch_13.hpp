#pragma once
// Thirteen original game and subsystem routines, independent non-inline
// C++ function bodies. Reconstructed from the identified 2002 PE32
// pseudocode at analysis/ghidra_main_exe/functions/<address>.c.
// Host global references are explicit until game state rebinding is complete.
// Compilable native source != proved x86 ABI or retail differential parity.
#include "btb/display_manager.hpp"
#include "btb/dsutil.hpp"
#include "btb/printing_system.hpp"
#include "btb/retail_shared_runtime.hpp"
#include <cstdint>
#include <span>

namespace btb::original_game13 {
using I32=std::int32_t;
using Rect=btb::display::RetailRect32;
using btb::retail_shared::BandstandRecord;
using btb::retail_shared::PondRecord;
using btb::retail_shared::PondGlobals;
using btb::retail_shared::HerdingSortEntry;

void construct_display_manager(btb::display::RetailDisplayManager32&) noexcept; // 00402FC0
void construct_wave_file(btb::dsutil::RetailCWaveFile32&) noexcept; // 00404510
void construct_bitmap_printer(btb::printing::RetailBitmapPrinter32&) noexcept; // 00404DD0
void construct_sound_manager(btb::dsutil::RetailCSoundManager32&) noexcept; // 00403BB0
[[nodiscard]] Rect squirrel_frame_rectangle(I32 frame) noexcept; // 00425980
[[nodiscard]] I32 compare_park_sort_layers(
    I32 type_a,I32 layer_a,I32 type_b,I32 layer_b) noexcept; // 0040D780
[[nodiscard]] bool can_place_grand_opening(
    std::span<const I32> grid,I32 row,I32 column,I32 length) noexcept; // 0041F3A0
[[nodiscard]] bool can_place_firework(
    std::span<const I32> grid,I32 row,I32 column,I32 length) noexcept; // 00410FD0
[[nodiscard]] I32 remove_firework(
    std::span<I32> grid,std::span<const I32> lengths,I32 row,I32 column) noexcept; // 00411080
[[nodiscard]] I32 hit_test_park_toolbar(
    std::span<const Rect> table,I32 x,I32 y) noexcept; // 0040D420
void clear_bandstand(std::span<BandstandRecord>,I32 selector) noexcept; // 0040DCE0
void clear_pond(std::span<PondRecord>,I32 selector,PondGlobals&) noexcept; // 0040DC90
[[nodiscard]] I32 compare_herding_depth(
    const HerdingSortEntry&,const HerdingSortEntry&) noexcept; // 00416310
[[nodiscard]] bool verify_13_original_game_calls() noexcept;
}
