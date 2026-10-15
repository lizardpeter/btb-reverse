#include "btb/native_host_input.hpp"
#include "btb/native_input_pulse.hpp"
#include <cassert>
#include <cstdint>
using namespace btb::full_game;
int main() {
 NativeHostInput native{};
 auto initial=native.sample(320,240,0xffffff00U);
 assert(initial.pointer_x==320&&initial.pointer_y==240);
 assert(initial.elapsed_centiseconds==0&&!initial.click_pulse);
 native.key_down(0x25,false);native.key_down(0x26,false);
 native.key_down('A',false);native.character('a');
 native.press_mouse();
 auto frame=native.sample(17,31,0xffffff64U);
 assert(frame.directional_input_bits==(0x01|0x04));
 assert(frame.keyboard_state['A']==0x80);
 assert(frame.pointer_button_held);
 assert(frame.translated_character=='a');
 assert(frame.elapsed_centiseconds==10);
 assert(frame.frame_delta==1);
 auto click=consume_native_click(native.click_pulse);
 assert(click.activity_click&&click.primary_input_click);
 assert(!native.click_pulse);
 assert(native.sample(17,31,0xffffff69U).translated_character==-1);
 native.release_mouse();
 native.key_up(0x25);native.key_down(0x27,false);native.key_down(0x28,false);
 frame=native.sample(17,31,0xffffff70U);
 assert(frame.directional_input_bits==(0x02|0x04|0x08));
 assert(!frame.pointer_button_held);
 // Numeric wrap in timeGetTime() must preserve elapsed time.
 frame=native.sample(17,31,0x00000020U);
 assert(frame.elapsed_centiseconds==28);
 native.press_right_mouse();
 assert(native.right_click_pulse&&native.pointer_held);
 click=consume_native_click(native.right_click_pulse);
 assert(click.activity_click&&click.primary_input_click);
 assert(!native.right_click_pulse);
 native.press_mouse();
 native.release_right_mouse();
 assert(native.pointer_held); // still holding primary
 native.release_mouse();
 assert(!native.pointer_held);
 native.key_down(0x20,false);
 assert(native.click_pulse);
 click=consume_native_click(native.click_pulse);
 assert(click.activity_click&&click.primary_input_click);
 native.key_down(0x20,true);
 assert(!native.click_pulse);
 native.key_up(0x20);
 native.character(0x100);
 assert(native.sample(0,0,0x00000030U).translated_character==-1);
 native.key_down(0x25,false);
 native.press_mouse();
 native.lose_focus();
 frame=native.sample(0,0,0x00000040U);
 assert(frame.directional_input_bits==0);
 assert(!frame.pointer_button_held && !native.click_pulse);
 for(const auto byte:frame.keyboard_state)assert(byte==0);
 return 0;
}
