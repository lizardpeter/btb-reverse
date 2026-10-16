#pragma once
// Shared event-to-retail-frame input bridge for the Windows native host.
// Uses documented Windows virtual-key numeric values without requiring
// windows.h, so its state transitions run in Linux and Windows CI.
#include "btb/full_game_runtime.hpp"
#include <array>
#include <cstdint>
#include <limits>
namespace btb::full_game {
// Original game keyboard input is indexed by DirectInput scan codes,
// not by Win32 virtual keys (VK_A 0x41 is DIK_A 0x1E).
// The window proc supplies the actual low-byte hardware scan code from
// WM_KEYDOWN lParam. The fallback supports portable unit tests.
[[nodiscard]] constexpr unsigned native_dik_for_vk(unsigned key) noexcept {
 if(key>='A' && key<='Z') {
  constexpr std::array<unsigned,26> alpha{{
   0x1e,0x30,0x2e,0x20,0x12,0x21,0x22,0x23,0x17,
   0x24,0x25,0x26,0x32,0x31,0x18,0x19,0x10,0x13,
   0x1f,0x14,0x16,0x2f,0x11,0x2d,0x15,0x2c
  }};
  return alpha[key-'A'];
 }
 if(key>='1' && key<='9')return 0x02U+(key-'1');
 if(key=='0')return 0x0b;
 switch(key) {
 case 0x1b:return 0x01;  // Escape / DIK_ESCAPE
 case 0x08:return 0x0e;  // Backspace
 case 0x10:case 0xa0:return 0x2a; // Shift / LShift
 case 0xa1:return 0x36;  // RShift
 case 0x20:return 0x39;  // Space
 case 0x25:return 0xcb;  // Left arrow extended
 case 0x26:return 0xc8;  // Up arrow extended
 case 0x27:return 0xcd;  // Right arrow extended
 case 0x28:return 0xd0;  // Down arrow extended
 default:return 0;
 }
}
struct NativeHostInput {
 std::array<std::uint8_t,256> keys{};
 std::array<std::uint8_t,256> scan_keys{};
 std::array<unsigned,256> held_scan_by_vk{};
 bool pointer_held{};
 bool click_pulse{};
 int typed_character{-1};
 std::uint32_t initial_tick{};
 bool clock_started{};
 void press_mouse() noexcept {pointer_held=true;click_pulse=true;}
 void release_mouse() noexcept {pointer_held=false;}
 void key_down(unsigned key,bool repeated,unsigned scan=0) noexcept {
  if(key>=keys.size())return;
  keys[key]=0x80;
  const auto dik=scan!=0 ? (scan&0xffU) : native_dik_for_vk(key);
  held_scan_by_vk[key]=dik;
  if(dik<scan_keys.size() && dik!=0)scan_keys[dik]=0x80;
  // Original Golf/manual uses Space as another primary-click pulse.
  // WM_KEYDOWN autorepeat must not generate repeated activations.
  if(key==0x20&&!repeated)click_pulse=true;
 }
 void key_up(unsigned key) noexcept {
  if(key>=keys.size())return;
  keys[key]=0;
  const auto dik=held_scan_by_vk[key];
  if(dik<scan_keys.size() && dik!=0)scan_keys[dik]=0;
  held_scan_by_vk[key]=0;
 }
 void character(unsigned unicode) noexcept {
  // The current source game accepts a byte for profile-name text.
  // Preserve visible ASCII only; wide/DBCS conversion is unimplemented.
  if(unicode>=0x20&&unicode<=0x7e)typed_character=static_cast<int>(unicode);
 }
 void lose_focus() noexcept {
  keys.fill(0);scan_keys.fill(0);held_scan_by_vk.fill(0);
  pointer_held=false;click_pulse=false;typed_character=-1;
 }
 [[nodiscard]] ActivityFrameInput sample(
    int x,int y,std::uint32_t monotonic_ms) noexcept {
  if(!clock_started){initial_tick=monotonic_ms;clock_started=true;}
  ActivityFrameInput out{};
  out.pointer_x=x;out.pointer_y=y;
  out.keyboard_state=scan_keys;
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
