#pragma once
// Nine independently compiled original MSVC/Spud Maze/Park Designer
// procedures reconstructed from the PE32 executable's Ghidra output.
// Native .cpp code is linked into the actual Park Designer collision path.
#include "btb/retail_source_batch_20261011.hpp"
namespace btb::original_source9 {
using namespace btb::retail_source_batch;
[[nodiscard]] bool park_no_sample_overlap(I32,I32,I32,I32,Rect32) noexcept;  // 0040DD50
[[nodiscard]] int compare_park_draw_objects(const ParkObject&,const ParkObject&,const ParkSortContext&) noexcept; // 0040D5B0
void update_spud_bob(SpudBobAnimation&) noexcept; // 00422400
void update_pilchard(PilchardAnimation&) noexcept; // 004225E0
char* copy_string_bounded(char*,const char*,std::size_t) noexcept; // 004302A0
[[nodiscard]] int compare_string_bounded(const char*,const char*,std::size_t) noexcept; // 00430E50
[[nodiscard]] U64 divide_unsigned_x86(U32,U32,U32,U32) noexcept; // 00430E90
[[nodiscard]] bool read_original_u32(LegacyReadCursor&,U32&) noexcept; // 00433150
[[nodiscard]] bool read_original_u64(LegacyReadCursor&,U64&) noexcept; // 0043315D
[[nodiscard]] bool verify_9_original_source_functions() noexcept;
}
