#include "btb/original_profile_screen.hpp"
#include <stdexcept>

namespace btb::profiles {
void update_player_profile_screen(ProfileScreenState& s,const ProfileScreenInput& in,
    ProfileTable& profiles,std::array<btb::progress::Record,kProfileCount>& progress,
    ProfileScreenServices& native) {
    for(const auto& p:profiles)
        if(p.name_length<0 || p.name_length>9)
            throw std::invalid_argument("invalid original profile name length");
    native.draw_background();
    if(s.help_countdown>0)--s.help_countdown;
    native.update_help_button();
    s.help_hover_enabled=true;
    native.update_help_hover(s.help_hover_enabled);
    s.help_hover_enabled=false;
    native.enable_input();
    s.variant_globals.fill(0);
    if(s.quit_voice_pending && !native.any_managed_playing()) {
        s.quit_voice_pending=false;
        native.restore_quit_snapshot();
        s.quit_overlay_active=true;
        s.delete_mode=false;
        return;
    }
    if(s.deletion_confirmed) {
        if(s.deletion_profile<0 || s.deletion_profile>=5)
            throw std::invalid_argument("invalid confirmed deletion profile");
        s.deletion_confirmed=false;
        const auto index=static_cast<std::size_t>(s.deletion_profile);
        delete_profile_in_memory(profiles,progress,index);
        for(const auto& path:activity_files_for_profile(index))
            native.truncate_activity_file(path);
    }
    constexpr std::array<int,5> name_x{92,167,290,388,537};
    constexpr std::array<int,5> name_y{199,336,224,328,263};
    constexpr std::array<std::array<int,2>,5> badge_positions{{
        {66,140},{138,269},{267,164},{363,265},{517,200}}};
    for(std::size_t i=0;i<5;++i) {
        const auto& p=profiles[i];
        int width=0;
        for(int j=0;j<p.name_length;++j)
            width+=native.glyph_width(p.name_glyph_codes[static_cast<std::size_t>(j)]);
        int x=name_x[i]-width/2;
        for(int j=0;j<p.name_length;++j) {
            const int glyph=p.name_glyph_codes[static_cast<std::size_t>(j)];
            native.draw_glyph(glyph,x,name_y[i]);
            x+=native.glyph_width(glyph);
        }
    }
    for(std::size_t i=0;i<5;++i)
        if(profiles[i].badge_index!=-1)
            native.draw_badge(profiles[i].badge_index,
                badge_positions[i][0],badge_positions[i][1]);
    if(s.selection_pending>=0) {
        if(!native.any_managed_playing()) {
            if(s.selected_profile<0 || s.selected_profile>=5 ||
               s.old_name_length<0 || s.old_name_length>9)
                throw std::invalid_argument("invalid selected original profile");
            s.selection_pending=-1;
            s.creation_profile=-1;
            s.name_entry.length=s.old_name_length;
            for(int j=0;j<s.old_name_length;++j)
                s.name_entry.glyphs[static_cast<std::size_t>(j)]=
                    static_cast<std::uint8_t>(s.old_name[static_cast<std::size_t>(j)]);
            // -1 refers to the source creation-index global, distinct from
            // the already selected player. No create popup is opened here.
            s.name_entry_phase=1;
            s.outer_state=4;
            progress[static_cast<std::size_t>(s.selected_profile)].values[0]=1;
            if(native.backing_present()) {
                native.stop_backing();
                if(native.backing_present())native.release_backing();
            }
        }
        return;
    }
    int next_action=s.last_action;
    if(!s.quit_voice_pending) {
        const int hit=hit_test_profile_screen(in.x,in.y);
        if(hit>=0) {
            if(hit<5) {
                const auto& p=profiles[static_cast<std::size_t>(hit)];
                if(p.name_length==0 && p.badge_index==-1) {
                    if(s.hover_latch!=hit && !s.delete_mode)
                        native.play_voice(0x135,50,2);
                    s.hover_latch=hit;
                } else if(s.hover_latch!=hit) {
                    s.hover_latch=hit;
                    if(!native.sound_playing(0x136) &&
                       !native.sound_playing(0x137) && !s.delete_mode)
                        native.play_voice(0x136+static_cast<int>(native.next_random()%2),50,2);
                }
            } else {
                if(hit==6 && s.hover_latch!=6)native.play_voice(0x1b2,50,2);
                s.hover_latch=hit;
                native.draw_button(hit,false);
            }
        } else {
            s.hover_latch=-1;
            s.last_action=-1;
            next_action=-1;
        }
        if(in.down && hit>4)native.draw_button(hit,true);
        if(in.click && !in.introduction_gate &&
           !native.sound_playing(0x12f) && !native.sound_playing(0x130) &&
           !native.sound_playing(0x131)) {
            if(hit>=0 && hit<5) {
                const auto index=static_cast<std::size_t>(hit);
                const auto& p=profiles[index];
                if(!s.delete_mode) {
                    if(p.name_length<1 && p.badge_index<0) {
                        native.stop_managed();
                        native.play_voice(0x13f,50,1);
                        native.mark_last_voice_interruptible();
                        s.name_entry.profile_index=index;
                        s.creation_profile=hit;
                        s.name_entry.length=p.name_length;
                        s.name_entry.badge=p.badge_index;
                        for(int j=0;j<p.name_length;++j)
                            s.name_entry.glyphs[static_cast<std::size_t>(j)]=
                                static_cast<std::uint8_t>(p.name_glyph_codes[static_cast<std::size_t>(j)]);
                        s.name_entry_phase=1;
                    } else {
                        native.play_voice(0x131,50,2);
                        s.selection_pending=1;
                        s.selected_profile=hit;
                    }
                } else if(p.name_length>0 || p.badge_index>=0) {
                    s.profile_help_mode=false;
                    s.delete_overlay_active=true;
                    native.open_delete_overlay();
                    native.stop_managed();
                    native.play_voice(0x330,50,2);
                    s.delete_overlay_setup=true;
                    s.delete_mode=false;
                    s.deletion_profile=hit;
                    native.use_delete_cursor(false);
                    return;
                }
            }
            if(hit==7) {
                s.quit_voice_pending=true;
                native.play_voice(0x133,50,2);
                s.last_action=7;
                return;
            }
            next_action=hit;
            if(hit==6) {
                if(!s.delete_mode)native.play_voice(0x132,50,2);
                s.delete_mode=!s.delete_mode;
                native.use_delete_cursor(s.delete_mode);
            }
        }
    }
    s.last_action=next_action;
}
} // namespace btb::profiles
