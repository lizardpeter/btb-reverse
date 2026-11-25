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
// RT_ACCELERATOR (9), resource 103, language 1033, original SHA-256
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
// Disk records: 03001b00419c0000 93005800e9030000.
// The on-disk final-entry bit 0x80 is not a Win32 ACCEL.fVirt flag.
struct RetailAccelerator {std::uint8_t flags;std::uint16_t key,command;};
inline constexpr std::array<RetailAccelerator,2> kRetailAccelerators{{
    {0x03,0x1b,0x9c41}, // Escape: Options
    {0x13,0x58,0x03e9}, // Alt+X: credits
}};
struct NativeHostInput {
 std::array<std::uint8_t,256> keys{};
 std::array<std::uint8_t,256> scan_keys{};
 std::array<unsigned,256> held_scan_by_vk{};
 bool pointer_held{};
 bool click_pulse{};
 bool release_pulse{};
 std::optional<std::pair<int,int>> press_position{};
 int typed_character{-1};
 struct NameKey {unsigned scan{};int character{-1};};
 std::array<NameKey,32> name_queue{};
 std::size_t name_head{},name_size{};
 unsigned previous_name_scan{};
 // The original popup processes one non-shift scan per frame. Win32
 // WM_CHAR can arrive and its key be released between two host ticks.
 // Queue the translated event with its actual scan, preserving order.
 // Bounded compatibility adapter; do not synthesize held gameplay input.
 [[nodiscard]] bool queue_name_key(unsigned unicode,unsigned scan,bool repeat=false) noexcept {
  if(repeat||scan==0||scan>0x35||scan==0x2a||
     (unicode!=8&&(unicode<0x20||unicode>0x7e))||
     (unicode==8&&scan!=0x0e)||name_size==name_queue.size())return false;
  name_queue[(name_head+name_size)%name_queue.size()]={scan,unicode==8?-1:static_cast<int>(unicode)};
  ++name_size;return true;
 }
 void clear_name_queue() noexcept {name_head=name_size=0;previous_name_scan=0;}

 std::uint32_t initial_tick{};
 bool clock_started{};
 void press_mouse() noexcept {
  if(!pointer_held){pointer_held=true;click_pulse=true;press_position.reset();}
 }
 void press_mouse(int x,int y) noexcept {
  if(!pointer_held){press_mouse();press_position=std::pair{x,y};}
 }
 void release_mouse() noexcept {if(pointer_held)release_pulse=true;pointer_held=false;}
 // Capture loss is cancellation, never a synthetic release/activation.
 void cancel_mouse() noexcept {
  pointer_held=false;click_pulse=false;release_pulse=false;press_position.reset();
 }
 void key_down(unsigned key,bool repeated,unsigned scan=0,bool extended=false) noexcept {
  if(key>=keys.size())return;
  keys[key]=0x80;
  // Win32 supplies an E0 indicator separately from the scan byte.
  // DirectInput encodes E0 keys with bit 7 (e.g. Left = 0xcb,
  // keypad 4 = 0x4b). Do not conflate the two.
  const auto dik=scan!=0 ? ((scan&0xffU)|(extended?0x80U:0U)) : native_dik_for_vk(key);
  held_scan_by_vk[key]=dik;
  if(dik<scan_keys.size() && dik!=0)scan_keys[dik]=0x80;
  // Original Golf/manual uses Space as another primary-click pulse.
  // WM_KEYDOWN autorepeat must not generate repeated activations.
  if(key==0x20&&!repeated){click_pulse=true;release_pulse=true;}
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
  pointer_held=false;click_pulse=false;release_pulse=false;press_position.reset();typed_character=-1;
  clear_name_queue();
 }
 [[nodiscard]] ActivityFrameInput sample(
    int x,int y,std::uint32_t monotonic_ms,bool name_entry=false) noexcept {
  if(!clock_started){initial_tick=monotonic_ms;clock_started=true;}
  ActivityFrameInput out{};
  out.pointer_x=x;out.pointer_y=y;
  out.keyboard_state=scan_keys;
  out.pointer_button_held=pointer_held;
  out.pointer_release_pulse=release_pulse;release_pulse=false;
  out.menu_release_input=true;
  out.menu_press_position=press_position;press_position.reset();
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
  if(name_entry){
   out.keyboard_state.fill(0);out.translated_character=-1;
   if(name_size&&name_queue[name_head].scan!=previous_name_scan){
    const auto event=name_queue[name_head];
    name_head=(name_head+1)%name_queue.size();--name_size;
    out.keyboard_state[event.scan]=0x80;
    out.translated_character=event.character;
    previous_name_scan=event.scan;
   }else{
    // Two distinct presses of the same scan require one release tick
    // for the original previous-scan latch to accept the second event.
    previous_name_scan=0;
   }
  }else clear_name_queue();
  return out;
 }
};
} // namespace btb::full_game
