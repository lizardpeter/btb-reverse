#include "btb/full_game_runtime.hpp"
#include <algorithm>
#include <array>
#include <exception>

namespace btb::full_game {
bool GameRoot::begin_profile_name_entry(
    int index,std::span<const Rect> font,std::span<const Rect> badges) {
    if(index<0 || index>=static_cast<int>(profiles::kProfileCount) ||
       initialized_activity_ || name_entry_ || font.size()<95 || font.size()>256 || badges.size()!=6)
        return false;
    for(const auto& r:font)
        if(r.left<0 || r.top<0 || r.right<=r.left || r.bottom<r.top ||
           r.right>65535 || r.bottom>=65535)return false;
    for(std::size_t i=0;i<badges.size();++i) {
        const auto& r=badges[i];
        if(r.left!=static_cast<int>(i)*50 || r.top!=0 ||
           r.right!=r.left+50 || r.bottom<=r.top)return false;
    }
    const auto& metadata=globals_.profile_metadata[static_cast<std::size_t>(index)];
    if(metadata.name_length<0 || metadata.name_length>9 ||
       metadata.badge_index < -1 || metadata.badge_index>=6)return false;
    profiles::NameEntryState staged{.profile_index=static_cast<std::size_t>(index),
        .length=metadata.name_length,.badge=metadata.badge_index};
    for(int i=0;i<staged.length;++i) {
        const auto code=metadata.name_glyph_codes[static_cast<std::size_t>(i)];
        if(code<0 || static_cast<std::size_t>(code)>=font.size())return false;
        staged.glyphs[static_cast<std::size_t>(i)]=static_cast<std::uint8_t>(code);
    }
    name_entry_blue_font_.assign(font.begin(),font.end());
    std::copy(badges.begin(),badges.end(),name_entry_badges_.begin());
    name_entry_=staged;
    name_entry_phase_=1;
    name_entry_blink_=0;
    set_outer_state(game_flow::State::PlayerProfileAndNameEntry);
    return true;
}

GameFrame GameRoot::advance_profile_name_entry(
    const ActivityFrameInput& input,GameFrame result) {
    // Retail state 1 first switches from name_signs.bmp to enter_name.bmp
    // and returns; it neither draws nor accepts input during that frame.
    if(name_entry_phase_==1) {
        name_entry_phase_=2;
        result.kind=FrameKind::FrontEndInitialized;
        return result;
    }
    auto& s=*name_entry_;
    auto& draws=result.effects.draws;
    draws.push_back({"Data/ui/enter_name.bmp",0,0});
    // 0042DB70 DrawEnterNamePopup executes BEFORE 0042DCC0 keyboard writes.
    // -1 is the original empty badge sentinel. Do not invent a negative
    // bitmap rectangle; the native provider has no drawable cell for it.
    if(s.badge>=0)
        draws.push_back({"Data/ui/fonttools.bmp",296,103,
            name_entry_badges_[static_cast<std::size_t>(s.badge)],true});
    int width=0;
    for(int i=0;i<s.length;++i) {
        const auto& r=name_entry_blue_font_[s.glyphs[static_cast<std::size_t>(i)]];
        width+=r.right-r.left;
    }
    int x=322-width/2;
    for(int i=0;i<s.length;++i) {
        auto r=name_entry_blue_font_[s.glyphs[static_cast<std::size_t>(i)]];
        ++r.bottom; // source adds one to the inclusive font bottom
        draws.push_back({"Data/ui/blue_name_boxed_font.bmp",x,200,r,true});
        x+=r.right-r.left;
    }
    if(++name_entry_blink_>=33)name_entry_blink_=0;
    if(name_entry_blink_<=16)
        draws.push_back({"Data/ui/textcursor.bmp",x-3,197,{},true});
    const auto effects=profiles::update_enter_name_popup(s,
        {.keyboard=input.keyboard_state,
         .translated_character=input.translated_character,
         .pointer_x=input.pointer_x,.pointer_y=input.pointer_y,
         .button_held=input.pointer_button_held,.click_pulse=input.click_pulse,
         .managed_sound_playing=input.managed_sound_playing},
        globals_.profile_metadata,globals_.player_progress);
    constexpr std::array<std::array<const char*,2>,4> buttons{{
        {"Data/ui/enternameleftred.bmp","Data/ui/enternameleftdep.bmp"},
        {"Data/ui/enternamerightred.bmp","Data/ui/enternamerightdep.bmp"},
        {"Data/ui/enternamebackred.bmp","Data/ui/enternamebackdep.bmp"},
        {"Data/ui/enternamestartred.bmp","Data/ui/enternamestartdep.bmp"},
    }};
    for(const auto& button:effects.buttons)
        draws.push_back({buttons[button.button][button.pressed?1:0],
            button.x,button.y,{},true});
    for(const auto& voice:effects.voices)
        result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},
            voice.id,voice.priority,voice.arbitration_class});
    result.clear_translated_character=effects.clear_translated_character;
    if(effects.selected_profile) {
        static_cast<void>(select_profile(*effects.selected_profile));
        result.effects.next_outer_state=effects.next_outer_state;
        apply_effects(result.effects);
        result.state_changed=true;
        name_entry_.reset();
    } else if(effects.reload_name_signs) {
        // The next state-1 frame belongs to the original five-sign selector,
        // which remains an explicit missing adapter, not a synthetic menu.
        name_entry_.reset();
    }
    result.kind=FrameKind::FrontEndUpdated;
    return result;
}
} // namespace btb::full_game
