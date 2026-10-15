#pragma once
// Shared event-to-retail-frame input bridge for the Windows native host.
// Uses documented Windows virtual-key numeric values without requiring
// windows.h, so its state transitions run in Linux and Windows CI.
#include "btb/full_game_runtime.hpp"
#include <array>
#include <cstdint>
#include <limits>
namespace btb::full_game {
struct NativeHostInput {
 std::array<std::uint8_t,256> keys{};
 bool pointer_held{};
 bool left_held{},right_held{};
 bool click_pulse{},right_click_pulse{};
 int typed_character{-1};
 std::uint32_t initial_tick{};
 bool clock_started{};
 void press_mouse() noexcept {
  left_held=true;pointer_held=true;click_pulse=true;
 }
 void release_mouse() noexcept {
  left_held=false;pointer_held=right_held;
 }
 void press_right_mouse() noexcept {
  right_held=true;pointer_held=true;right_click_pulse=true;
 }
 void release_right_mouse() noexcept {
  right_held=false;pointer_held=left_held;
 }
 void key_down(unsigned key,bool repeated) noexcept {
  if(key>=keys.size())return;
  keys[key]=0x80;
  // Original Golf/manual uses Space as another primary-click pulse.
  // WM_KEYDOWN autorepeat must not generate repeated activations.
  if(key==0x20&&!repeated)click_pulse=true;
 }
 void key_up(unsigned key) noexcept {
  if(key<keys.size())keys[key]=0;
 }
 void character(unsigned unicode) noexcept {
  // The current source game accepts a byte for profile-name text.
  // Preserve visible ASCII only; wide/DBCS conversion is unimplemented.
  if(unicode>=0x20&&unicode<=0x7e)typed_character=static_cast<int>(unicode);
 }
 void lose_focus() noexcept {
  keys.fill(0);pointer_held=left_held=right_held=false;
  click_pulse=right_click_pulse=false;typed_character=-1;
 }
 [[nodiscard]] ActivityFrameInput sample(
    int x,int y,std::uint32_t monotonic_ms) noexcept {
  if(!clock_started){initial_tick=monotonic_ms;clock_started=true;}
  ActivityFrameInput out{};
  out.pointer_x=x;out.pointer_y=y;
  out.keyboard_state=keys;
  out.pointer_button_held=pointer_held;
  out.directional_input_bits=static_cast<std::uint8_t>(
      (keys[0x25]?0x01:0) | (keys[0x27]?0x02:0) |
      (keys[0x26]?0x04:0) | (keys[0x28]?0x08:0));
  const auto elapsed=static_cast<std::uint32_t>(monotonic_ms-initial_tick);
  const auto cs=elapsed/10U;
  out.elapsed_centiseconds=static_cast<int>(cs>static_cast<std::uint32_t>(
    std::numeric_limits<int>::max())?std::numeric_limits<int>::max():cs);
  out.frame_delta=1; // source animation tick, not wall-time simulation steps
  out.translated_character=typed_character;
  typed_character=-1;
  return out;
 }
};
} // namespace btb::full_game
