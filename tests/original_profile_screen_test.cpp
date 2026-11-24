#include "btb/full_game_runtime.hpp"
#include <algorithm>
#include <cassert>
#include <set>
#include <string>
#include <vector>
using namespace btb::profiles;
struct Provider final : ProfileScreenServices {
    std::vector<std::string> calls;
    std::set<int> playing;
    bool any{},backing{true},stop_removes{};
    unsigned random{1};int random_calls{};
    void draw_background() override {calls.push_back("background");}
    void update_help_button() override {calls.push_back("help");}
    void update_help_hover(bool enabled) override {assert(enabled);calls.push_back("hover_help");}
    void enable_input() override {calls.push_back("enable");}
    bool any_managed_playing() override {return any;}
    bool sound_playing(int id) override {return playing.contains(id);}
    unsigned next_random() override {++random_calls;return random;}
    void play_voice(int id,int priority,int cls) override {
        assert(priority==50 && (cls==1 || cls==2));calls.push_back("voice:"+std::to_string(id));
    }
    void stop_managed() override {calls.push_back("stop");}
    void mark_last_voice_interruptible() override {calls.push_back("interruptible");}
    int glyph_width(int) override {return 7;}
    void draw_glyph(int code,int x,int y) override {
        calls.push_back("glyph:"+std::to_string(code)+":"+std::to_string(x)+":"+std::to_string(y));
    }
    void draw_badge(int badge,int x,int y) override {
        calls.push_back("badge:"+std::to_string(badge)+":"+std::to_string(x)+":"+std::to_string(y));
    }
    void draw_button(int button,bool pressed) override {
        calls.push_back("button:"+std::to_string(button)+(pressed?":down":":hover"));
    }
    void restore_quit_snapshot() override {calls.push_back("restore_quit");}
    void truncate_activity_file(const std::string& name) override {calls.push_back("truncate:"+name);}
    bool backing_present() override {return backing;}
    void stop_backing() override {calls.push_back("stop_backing");if(stop_removes)backing=false;}
    void release_backing() override {calls.push_back("release_backing");backing=false;}
    void open_delete_overlay() override {calls.push_back("delete_overlay");}
    void use_delete_cursor(bool enabled) override {calls.push_back(enabled?"delete_cursor":"default_cursor");}
    bool has(const std::string& value) const {
        return std::find(calls.begin(),calls.end(),value)!=calls.end();
    }
    void clear(){calls.clear();}
};
int main() {
    btb::full_game::GameRoot root;
    ProfileScreenState s;
    Provider native;
    auto step=[&](ProfileScreenInput in={}){root.update_original_profile_screen(s,in,native);};
    auto& profiles=root.globals().profile_metadata;
    auto& progress=root.globals().player_progress;
    s.help_countdown=2;s.variant_globals.fill(17);
    step({.x=92,.y=180});
    assert(s.help_countdown==1 && !s.help_hover_enabled);
    assert(native.calls[0]=="background" && native.calls[1]=="help" &&
           native.calls[2]=="hover_help" && native.calls[3]=="enable");
    for(int v:s.variant_globals)assert(v==0);
    assert(native.has("voice:309"));native.clear();
    step({.x=92,.y=180});assert(!native.has("voice:309"));
    // Empty profile creation, original sound ordering and interruptible mark.
    native.clear();step({.x=92,.y=180,.click=true});
    assert(native.has("stop") && native.has("voice:319") && native.has("interruptible"));
    assert(s.creation_profile==0 && s.name_entry_phase==1 && s.name_entry.badge==-1);
    // Sound-manager introduction latch or any of 303/304/305 block clicks.
    s=ProfileScreenState{};native.clear();
    step({.x=92,.y=180,.click=true,.introduction_gate=true});
    assert(s.creation_profile==-1);
    for(int id:{303,304,305}) {
        native.playing={id};step({.x=92,.y=180,.click=true});assert(s.creation_profile==-1);
    }
    native.playing.clear();
    profiles[2].name_length=2;profiles[2].name_glyph_codes[0]=7;
    profiles[2].name_glyph_codes[1]=9;profiles[2].badge_index=4;
    s=ProfileScreenState{};native.clear();
    step({.x=292,.y=200});
    assert(native.has("glyph:7:283:224") && native.has("glyph:9:290:224"));
    assert(native.has("badge:4:267:164") && native.has("voice:311") && native.random_calls==1);
    // Hover sound suppression must still latch the new occupied sign.
    s.hover_latch=-1;native.playing={310};native.clear();
    step({.x=292,.y=200});assert(s.hover_latch==2 && native.random_calls==1);
    native.playing.clear();native.clear();
    step({.x=292,.y=200,.click=true});
    assert(s.selection_pending==1 && s.selected_profile==2 && native.has("voice:305"));
    native.any=true;native.clear();step();assert(s.outer_state==1 && native.backing);
    s.old_name_length=1;s.old_name[0]=0x121;native.any=false;
    step();assert(s.outer_state==4 && s.selection_pending==-1 && s.creation_profile==-1);
    assert(s.name_entry.glyphs[0]==0x21 && progress[2].values[0]==1);
    assert(root.globals().active_profile==2 && native.has("stop_backing") && native.has("release_backing"));
    // Delete is a toggle followed by a separate confirmed modal transaction.
    s=ProfileScreenState{};native.clear();
    step({.x=320,.y=445,.down=true,.click=true});
    assert(s.delete_mode && native.has("voice:434") && native.has("voice:306"));
    assert(native.has("button:6:down") && native.has("delete_cursor"));
    native.clear();step({.x=92,.y=180,.click=true});assert(!s.delete_overlay_active);
    native.clear();step({.x=292,.y=200,.click=true});
    assert(s.delete_overlay_active && s.delete_overlay_setup && !s.delete_mode && s.deletion_profile==2);
    assert(native.has("delete_overlay") && native.has("voice:816") && native.has("default_cursor"));
    progress[2].values.fill(37);s.deletion_confirmed=true;native.clear();step();
    assert(profiles[2].name_length==0 && profiles[2].badge_index==-1);
    assert(profiles[2].name_glyph_codes[0]==7 && progress[2].values[49]==37 && progress[2].values[65]==37);
    for(int i=50;i<=64;++i)assert(progress[2].values[i]==0);
    for(const auto& file:activity_files_for_profile(2))assert(native.has("truncate:"+file));
    // Quit waits for actual voice completion, restores snapshot, then returns.
    s=ProfileScreenState{};native.clear();step({.x=600,.y=445,.click=true});
    assert(s.quit_voice_pending && s.last_action==7 && native.has("voice:307"));
    native.any=true;native.clear();step();assert(!s.quit_overlay_active);
    native.any=false;native.clear();step();assert(s.quit_overlay_active && native.has("restore_quit"));
    assert(native.calls.back()=="restore_quit");
    // Strict boundary has no hit and clears hover/last-action.
    s=ProfileScreenState{};s.last_action=6;s.hover_latch=6;native.clear();
    step({.x=42,.y=180});assert(s.hover_latch==-1 && s.last_action==-1);
}
