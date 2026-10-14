#include "btb/profile_name_entry.hpp"
#include <cassert>
#include <stdexcept>
#include <sstream>
using namespace btb::profiles;

int main() {
    ProfileTable profiles{};
    std::array<btb::progress::Record,kProfileCount> progress{};
    NameEntryState s{.profile_index=3};
    std::array<std::uint8_t,256> keys{};
    NameEntryInput in{.keyboard=keys};
    auto update=[&]{return update_enter_name_popup(s,in,profiles,progress);};
    // Shift is ignored and the first physical non-shift key wins.
    keys[0x2a]=0x80; keys[0x1e]=0x80; keys[0x30]=0x80;
    in.translated_character='A';
    update();assert(s.length==1 && s.glyphs[0]=='A'-0x21);
    update();assert(s.length==1); // held key cannot repeat
    keys.fill(0);update();keys[0x1e]=0x80;
    update();assert(s.length==2);
    keys.fill(0);update();keys[0x0e]=0x80;
    auto back=update();assert(back.clear_translated_character && s.length==1);
    update();assert(s.length==1);
    keys.fill(0);update();
    // Capacity is eight input glyphs, though the retail backing slot has nine.
    for(int i=0;i<12;++i) {
        keys[0x1e]=0x80;update();keys.fill(0);update();
    }
    assert(s.length==8);
    keys[0xed]=0x80;update();assert(s.previous_scan_code==-1);
    keys.fill(0);
    in.pointer_x=252;in.pointer_y=120;in.click_pulse=true;
    update();assert(s.badge==-1); // strict boundary
    in.pointer_x=260;update();assert(s.badge==5);
    in.pointer_x=370;update();assert(s.badge==0);
    s.badge=5;update();assert(s.badge==0);
    // Voice-only regions can hover but cannot accept/cancel.
    in.pointer_x=200;in.pointer_y=200;
    auto voice=update();assert(voice.voices.size()==1 && voice.voices[0].id==0x141);
    assert(voice.buttons.empty() && !s.awaiting_accept_voice);
    assert(update().voices.empty());
    in.pointer_x=250;in.pointer_y=100;
    voice=update();assert(voice.voices.size()==1 && voice.voices[0].id==0x142);
    // Accept draws both normal and pressed surfaces, then waits for all
    // managed voices; held keyboard input is ignored throughout the wait.
    profiles[3].name_glyph_codes[8]=99;
    progress[3].values[51]=7;
    in.pointer_x=300;in.pointer_y=320;in.button_held=true;
    voice=update();assert(voice.voices.size()==2);
    assert(voice.voices[0].id==0x1cc && voice.voices[1].id==0x140);
    assert(voice.buttons.size()==2 && !voice.buttons[0].pressed && voice.buttons[1].pressed);
    assert(s.awaiting_accept_voice && profiles[3].name_length==0);
    in.managed_sound_playing=true;keys[0x0e]=0x80;
    assert(!update().selected_profile && s.length==8);
    in.managed_sound_playing=false;
    auto committed=update();assert(committed.selected_profile==3 && committed.next_outer_state==0x41);
    assert(profiles[3].name_length==8 && profiles[3].badge_index==0);
    assert(profiles[3].name_glyph_codes[8]==99);
    assert(progress[3].values[0]==1 && progress[3].values[51]==7);
    // Empty acceptance has no effect; badge-only creation is allowed.
    s=NameEntryState{.profile_index=1};keys.fill(0);in.button_held=false;
    voice=update();assert(!s.awaiting_accept_voice && voice.voices.size()==1);
    s.badge=2;update();assert(s.awaiting_accept_voice);
    committed=update();assert(committed.selected_profile==1 && profiles[1].name_length==0);
    // Cancel requests the real registered names bitmap and leaves saves alone.
    s=NameEntryState{.profile_index=2};in.pointer_x=510;in.pointer_y=330;
    auto cancel=update();assert(cancel.reload_name_signs && !s.popup_active);
    assert(cancel.voices.size()==1 && cancel.voices[0].id==0x1c8);
    assert(profiles[2].badge_index==-1);
    s.profile_index=5;bool rejected=false;
    try{update();}catch(const std::invalid_argument&){rejected=true;}
    assert(rejected);
    // Save validation must fail before output and cannot read a tenth glyph.
    profiles[0].name_length=10;
    std::ostringstream output;rejected=false;
    try{write_playerinfo(output,profiles);}catch(const std::runtime_error&){rejected=true;}
    assert(rejected && output.str().empty());
}
