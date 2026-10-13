#pragma once
#include "btb/gameplay_control_batch_10.hpp"
namespace btb::original_control6 {
using namespace btb::gameplay_control_10;
[[nodiscard]] Plan update_fireworks(FireState&,U32 random_roll=0);
void cursor(Plan&,BandEditor&,I32);
[[nodiscard]] Plan begin_band_action(BandEditor&,I32,
 std::optional<btb::bobs_band::Cell> cell={});
[[nodiscard]] Plan update_band_selected(BandEditor&,I32,
 std::optional<btb::bobs_band::Cell> cell={});
[[nodiscard]] Plan update_help_button(Help&);
[[nodiscard]] Plan update_leave_confirmation(Leave&);
[[nodiscard]] Plan update_play_again(PlayAgain&);
[[nodiscard]] bool verify_6_original_gameplay_controls();
}
