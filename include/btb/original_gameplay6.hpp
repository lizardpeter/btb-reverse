#pragma once
// Original game-specific PE32 implementations, compiled out of line.
// The typed original resource effects are supplied to the native game
// rather than pretending source-only models are decoded Bink/DDraw calls.
#include "btb/native_gameplay_batch_6.hpp"
namespace btb::original_gameplay6 {
using namespace btb::native_gameplay_6;
[[nodiscard]] Plan initialize_golf_activity(GolfInit&,const btb::golf::Data&); // 004145F0
[[nodiscard]] Plan initialize_spud_skate(SkateInit&,
 const btb::spud_skate::TimingData&,const std::array<std::string,5>&,
 const std::function<std::optional<std::vector<U8>>(const std::string&,std::size_t)>&); // 00424360
[[nodiscard]] Plan update_grand_opening_toolbar(BandToolbar&,U32=0); // 0041F820
[[nodiscard]] Plan play_draw_grand_opening(BandPlayback&); // 0041FC90
[[nodiscard]] Plan update_fireworks_editor_controls(FireControls&); // 00413B60
[[nodiscard]] Plan draw_grand_opening_machines(BandDraw&); // 0041F090
[[nodiscard]] bool verify_6_original_gameplay_functions() noexcept;
}
