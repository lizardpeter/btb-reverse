#include "btb/profile_name_entry.hpp"
#include <algorithm>
#include <stdexcept>

namespace btb::profiles {
NameEntryEffects update_enter_name_popup(
    NameEntryState& s,const NameEntryInput& input,ProfileTable& profiles,
    std::array<btb::progress::Record,kProfileCount>& progress) {
    if(s.profile_index>=kProfileCount || s.length<0 ||
       s.length>static_cast<int>(kMaxNameGlyphs) || s.badge < -1 ||
       s.badge>=kBadgeCount)
        throw std::invalid_argument("invalid original name-entry state");
    NameEntryEffects out;
    // Once acceptance starts, the original ignores both keyboard and mouse.
    if(s.awaiting_accept_voice) {
        if(!input.managed_sound_playing) {
            auto& profile=profiles[s.profile_index];
            profile.badge_index=s.badge;
            profile.name_length=s.length;
            for(int i=0;i<s.length;++i)
                profile.name_glyph_codes[static_cast<std::size_t>(i)]=
                    s.glyphs[static_cast<std::size_t>(i)];
            // Unused backing glyphs survive, as in the original.
            progress[s.profile_index].values[0]=1;
            s.awaiting_accept_voice=false;
            s.popup_active=true;
            out.selected_profile=static_cast<int>(s.profile_index);
            out.next_outer_state=0x41;
        }
        return out;
    }
    int scan=-1;
    const auto count=std::min(input.keyboard.size(),std::size_t{0xed});
    for(std::size_t i=0;i<count;++i) {
        if((input.keyboard[i]&0x80)!=0 && !is_shift_scan_code(static_cast<int>(i))) {
            scan=static_cast<int>(i);
            break; // original processes only the first non-shift key
        }
    }
    if(scan!=-1 && scan!=s.previous_scan_code) {
        if(scan==kDirectInputBackspace) {
            out.clear_translated_character=true;
            if(s.length>0)--s.length;
        } else {
            const int glyph=profile_name_glyph_for_input(
                scan,input.translated_character,static_cast<std::size_t>(s.length));
            if(glyph>=0)s.glyphs[static_cast<std::size_t>(s.length++)]=
                static_cast<std::uint8_t>(glyph);
        }
    }
    s.previous_scan_code=scan;
    int hovered=-1;
    for(std::size_t i=0;i<kNameEntryHitRects.size();++i) {
        const auto& r=kNameEntryHitRects[i];
        if(input.pointer_x>r.left && input.pointer_x<r.right &&
           input.pointer_y>r.top && input.pointer_y<r.bottom) {
            hovered=static_cast<int>(i);break;
        }
    }
    if(hovered!=-1) {
        // Arrows do not change the previous-voice latch. Cancel/accept and
        // the two voice-only regions do; repeated frames do not replay them.
        if(hovered!=s.previous_hover && (hovered==2 || hovered==3)) {
            out.voices.push_back({hovered==2?0x1c8:0x1cc});
            s.previous_hover=hovered;
        }
        s.hovered=hovered;
        if(hovered>=4) {
            if(hovered!=s.previous_hover) {
                s.previous_hover=hovered;
                out.voices.push_back({hovered+0x13d});
            }
        } else {
            const auto p=kNameEntryButtonPositions[static_cast<std::size_t>(hovered)];
            out.buttons.push_back({hovered,p[0],p[1],false});
            if(input.button_held)out.buttons.push_back({hovered,p[0],p[1],true});
        }
    } else s.previous_hover=-1;
    if(input.click_pulse) {
        switch(hovered) {
        case 0:s.badge=previous_badge(s.badge);break;
        case 1:s.badge=next_badge(s.badge);break;
        case 2:s.popup_active=false;out.reload_name_signs=true;break;
        case 3:
            // Retail permits badge-only or name-only profiles.
            if(s.length>0 || s.badge>=0) {
                out.voices.push_back({0x140});
                s.awaiting_accept_voice=true;
            }
            break;
        default:break;
        }
    }
    return out;
}
} // namespace btb::profiles
