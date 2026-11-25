#include "btb/native_host_input.hpp"
#include "btb/native_input_pulse.hpp"
#include <cassert>
#include <cstdint>
using namespace btb::full_game;
int main() {
    static_assert(kRetailAccelerators.size()==2);
    static_assert(kRetailAccelerators[0].flags==3&&kRetailAccelerators[0].key==0x1b&&kRetailAccelerators[0].command==0x9c41);
    static_assert(kRetailAccelerators[1].flags==0x13&&kRetailAccelerators[1].key==0x58&&kRetailAccelerators[1].command==0x3e9);
    {
        NativeHostInput name;
        // Multiple WM_CHAR events, including quick repeated physical
        // presses, can arrive before any game frame. Retain order.
        assert(name.queue_name_key('A',0x1e));
        assert(name.queue_name_key('B',0x30));
        assert(name.queue_name_key('B',0x30));
        assert(name.queue_name_key(8,0x0e));
        assert(!name.queue_name_key('B',0x30,true));
        auto frame=name.sample(0,0,0,true);
        assert(frame.translated_character=='A'&&frame.keyboard_state[0x1e]==0x80);
        frame=name.sample(0,0,1,true);
        assert(frame.translated_character=='B'&&frame.keyboard_state[0x30]==0x80);
        frame=name.sample(0,0,2,true);
        assert(frame.translated_character==-1&&frame.keyboard_state[0x30]==0);
        frame=name.sample(0,0,3,true);
        assert(frame.translated_character=='B'&&frame.keyboard_state[0x30]==0x80);
        frame=name.sample(0,0,4,true);
        assert(frame.translated_character==-1&&frame.keyboard_state[0x0e]==0x80);
        assert(name.queue_name_key('C',0x2e));
        name.lose_focus();
        assert(name.sample(0,0,5,true).translated_character==-1);
        assert(name.queue_name_key('D',0x20));
        (void)name.sample(0,0,6,false);
        assert(name.sample(0,0,7,true).translated_character==-1);
    }
    {
        NativeHostInput mouse;
        mouse.press_mouse(22,44);mouse.release_mouse();
        auto frame=mouse.sample(55,66,0);
        assert(frame.menu_release_input&&frame.pointer_release_pulse);
        assert(frame.menu_press_position&&frame.menu_press_position->first==22&&
               frame.menu_press_position->second==44);
        assert(!mouse.sample(55,66,1).menu_press_position);
    }
    {
        btb::full_game::NativeHostInput mouse;
        mouse.press_mouse();mouse.release_mouse();
        const auto pulse=mouse.sample(220,260,0);
        assert(pulse.pointer_release_pulse&&!pulse.pointer_button_held);
        assert(!mouse.sample(220,260,1).pointer_release_pulse);
        mouse.release_mouse();
        assert(!mouse.sample(220,260,2).pointer_release_pulse);
        mouse.press_mouse();mouse.lose_focus();
        assert(!mouse.sample(220,260,3).pointer_release_pulse);
    }
    {
        NativeHostInput mouse;
        mouse.press_mouse(220,260);
        mouse.cancel_mouse();
        auto frame=mouse.sample(300,400,0);
        assert(!frame.pointer_button_held&&!frame.pointer_release_pulse);
        assert(!frame.menu_press_position&&!mouse.click_pulse);
        mouse.release_mouse();
        assert(!mouse.sample(300,400,1).pointer_release_pulse);
        mouse.press_mouse(44,55);
        mouse.release_mouse();
        frame=mouse.sample(44,55,2);
        assert(frame.pointer_release_pulse&&frame.menu_press_position);
    }
    {
        NativeHostInput keyboard;
        keyboard.key_down(0x25,false,0x4b,true);
        keyboard.key_down(0x64,false,0x4b,false);
        auto frame=keyboard.sample(0,0,0);
        assert(frame.keyboard_state[0xcb]==0x80);
        assert(frame.keyboard_state[0x4b]==0x80);
        keyboard.key_up(0x25);
        frame=keyboard.sample(0,0,1);
        assert(frame.keyboard_state[0xcb]==0);
        assert(frame.keyboard_state[0x4b]==0x80);
        keyboard.key_up(0x64);
        assert(keyboard.sample(0,0,2).keyboard_state[0x4b]==0);
    }
 NativeHostInput native{};
 auto initial=native.sample(320,240,0xffffff00U);
 assert(initial.pointer_x==320&&initial.pointer_y==240);
 assert(initial.elapsed_centiseconds==0&&!initial.click_pulse);
 native.key_down(0x25,false);native.key_down(0x26,false);
 native.key_down('A',false);native.character('a');
 native.press_mouse();
 auto frame=native.sample(17,31,0xffffff64U);
 assert(frame.directional_input_bits==(0x01|0x04));
 assert(frame.keyboard_state[0x1e]==0x80); // DIK_A, not VK_A
 assert(frame.keyboard_state['A']==0);
 assert(native_dik_for_vk('A')==0x1e);
 assert(native_dik_for_vk('Z')==0x2c);
 assert(native_dik_for_vk(0x1b)==0x01); // retail Escape scan-code
 // WM_KEYDOWN/VK_ESCAPE is a keyboard event, not a synthesized
 // primary click or request to exit the game.
 NativeHostInput escape_only{};
 escape_only.key_down(0x1b,false);
 assert(!escape_only.click_pulse);
 assert(escape_only.sample(17,31,0xffffff64U).keyboard_state[0x01]==0x80);
 escape_only.key_up(0x1b);
 assert(escape_only.sample(17,31,0xffffff64U).keyboard_state[0x01]==0);
 assert(frame.pointer_button_held);
 assert(frame.translated_character=='a');
 assert(frame.elapsed_centiseconds==10);
 assert(frame.frame_delta==1);
 auto click=consume_native_click(native.click_pulse);
 assert(click.activity_click&&click.primary_input_click);
 assert(!native.click_pulse);
 assert(native.sample(17,31,0xffffff69U).translated_character==-1);
 native.release_mouse();
 native.key_up('A');
 assert(native.sample(17,31,0xffffff69U).keyboard_state[0x1e]==0);
 native.key_down('W',false,0x11);
 assert(native.sample(17,31,0xffffff69U).keyboard_state[0x11]==0x80);
 native.key_up('W');
 assert(native.sample(17,31,0xffffff69U).keyboard_state[0x11]==0);
 native.key_up(0x25);native.key_down(0x27,false);native.key_down(0x28,false);
 frame=native.sample(17,31,0xffffff70U);
 assert(frame.directional_input_bits==(0x02|0x04|0x08));
 assert(!frame.pointer_button_held);
 // Numeric wrap in timeGetTime() must preserve elapsed time.
 frame=native.sample(17,31,0x00000020U);
 assert(frame.elapsed_centiseconds==28);
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
