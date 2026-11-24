#pragma once
#include "btb/profile_name_entry.hpp"

namespace btb::profiles {
struct ProfileScreenState {
    int help_countdown{};                 // 0051C31C
    bool help_hover_enabled{};            // 0051C370
    std::array<int,10> variant_globals{};  // 0051C340..364
    bool quit_voice_pending{};            // 0051C338
    bool quit_overlay_active{};           // 0051C2C0
    bool delete_mode{};                   // 0051C3A0
    bool deletion_confirmed{};            // 0051C2DC
    int deletion_profile{-1};             // 00447004
    int hover_latch{-1};                  // 00447008
    int last_action{-1};                  // 00447010
    int selection_pending{-1};            // 0044700C
    int selected_profile{-1};             // 00519934
    int creation_profile{-1};             // 0051B3A0, may be -1
    int old_name_length{};                // 0051B400
    std::array<std::uint32_t,9> old_name{}; // 0051B3F8 backing words
    NameEntryState name_entry{};
    int name_entry_phase{};               // 0051C2F4, 1=setup, 2=active
    bool profile_help_mode{};             // 0051C310
    bool delete_overlay_active{};         // 0051C2D0
    bool delete_overlay_setup{};          // 0051C2D4
    int outer_state{1};                   // 0044DE14
};
struct ProfileScreenInput {
    int x{},y{};
    bool down{},click{};
    bool introduction_gate{}; // original sound-manager byte +0x7FB
};
// Platform operations are required dependencies, not optional callbacks
// interpreted as success. A native provider must implement the existing
// shared bitmap, sound, help, cursor and filesystem services.
class ProfileScreenServices {
public:
    virtual ~ProfileScreenServices()=default;
    virtual void draw_background()=0;
    virtual void update_help_button()=0;
    virtual void update_help_hover(bool enabled)=0;
    virtual void enable_input()=0;
    virtual bool any_managed_playing()=0;
    virtual bool sound_playing(int id)=0;
    virtual unsigned next_random()=0;
    virtual void play_voice(int id,int priority,int arbitration_class)=0;
    virtual void stop_managed()=0;
    virtual void mark_last_voice_interruptible()=0;
    virtual int glyph_width(int code)=0;
    virtual void draw_glyph(int code,int x,int y)=0;
    virtual void draw_badge(int badge,int x,int y)=0;
    virtual void draw_button(int button,bool pressed)=0;
    virtual void restore_quit_snapshot()=0;
    virtual void truncate_activity_file(const std::string&)=0;
    virtual bool backing_present()=0;
    virtual void stop_backing()=0;
    virtual void release_backing()=0;
    // Load/register 0051C294 deletegame.bmp, magenta key, original overlay.
    virtual void open_delete_overlay()=0;
    virtual void use_delete_cursor(bool)=0;
};
// Original 0042E110. Compiled out of line; preserves all source branches.
void update_player_profile_screen(ProfileScreenState&,const ProfileScreenInput&,
    ProfileTable&,std::array<btb::progress::Record,kProfileCount>&,
    ProfileScreenServices&);
} // namespace btb::profiles
