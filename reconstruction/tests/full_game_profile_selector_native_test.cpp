#include "btb/full_game_profile_screen_native.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
  std::fprintf(stderr,"retail profile selector: %s at %s:%d\\n", \
      #expr,__FILE__,__LINE__);std::abort();} } while(false)
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace {
namespace fs=std::filesystem;
using namespace btb::full_game;
using btb::game_flow::State;

void create_file(const fs::path& path) {
    fs::create_directories(path.parent_path());
    std::ofstream(path,std::ios::binary).put('x');
}
void create_font(const fs::path& path) {
    fs::create_directories(path.parent_path());
    std::ofstream file(path);
    for(int i=0;i<191;++i) {
        const int x=(i%20)*12,y=(i/20)*22;
        file<<x<<" "<<y<<" "<<x+8<<" "<<y+18<<"\n";
    }
}
fs::path setup_assets() {
    const auto root=fs::temp_directory_path()/
        "btb_profile_selector_retail_native_regression";
    fs::remove_all(root);
    create_font(root/"loaddata"/"fontdatabrown.txt");
    create_font(root/"loaddata"/"fontdatablue.txt");
    for(const auto name:{
        "name signs.bmp","enter name boxed font.bmp",
        "blue name boxed font.bmp","fonttools.bmp","enter name.bmp",
        "textcursor.bmp","deletegame.bmp",
        "helpred.bmp","backred.bmp","enternamedeletered.bmp"})
        create_file(root/"Data"/"ui"/name);
    return root;
}
bool drawn(const ActivityFrameOutput& output,std::string_view file) {
    for(const auto& layer:output.draws)
        if(layer.source_asset==file)return true;
    return false;
}
std::unique_ptr<NativeProfileScreenAdapter> adapter(
    const fs::path& root) {
    auto value=std::make_unique<NativeProfileScreenAdapter>(
        root,root/"profiles");
    std::string error;
    assert(value->initialize(error));
    assert(value->blue_font_rectangles().size()==191);
    assert(value->badge_rectangles().size()==6);
    return value;
}
}

int main() {
    const auto root=setup_assets();
    {
        // Original profile action gates are per-sound ID. Playing an
        // unrelated hover/voice sample must not make every gate busy.
        auto screen=adapter(root);
        ActivityFrameOutput output;
        std::string error;
        ActivityFrameInput input{};
        input.managed_sound_playing=true;
        assert(screen->begin_frame(input,output,error));
        assert(screen->any_managed_playing());
        for(const int id:kProfileGateSoundIds)
            assert(!screen->sound_playing(id));
        assert(screen->end_frame(error));

        input.profile_sound_playing={false,true,false,false,true};
        assert(screen->begin_frame(input,output,error));
        assert(!screen->sound_playing(0x12f));
        assert(screen->sound_playing(0x130));
        assert(!screen->sound_playing(0x131));
        assert(!screen->sound_playing(0x136));
        assert(screen->sound_playing(0x137));
        assert(screen->end_frame(error));
    }
    {
        GameRoot game;
        game.globals().profile_metadata[2].name_length=1;
        game.globals().profile_metadata[2].badge_index=0;
        game.globals().profile_metadata[2].name_glyph_codes[0]=32;
        game.install_profile_screen(adapter(root));
        game.set_outer_state(State::PlayerProfileAndNameEntry);
        auto frame=game.advance({});
        assert(frame.kind==FrameKind::FrontEndUpdated);
        assert(drawn(frame.effects,"Data/ui/name signs.bmp"));
        assert(drawn(frame.effects,"Data/ui/enter name boxed font.bmp"));
        assert(drawn(frame.effects,"Data/ui/fonttools.bmp"));
        assert(!game.globals().active_profile);
        frame=game.advance({.pointer_x=280,.pointer_y=195,
                            .click_pulse=true});
        assert(frame.kind==FrameKind::FrontEndUpdated);
        assert(!frame.state_changed);
        assert(!game.globals().active_profile);
        frame=game.advance({});
        assert(frame.kind==FrameKind::FrontEndUpdated);
        assert(frame.state_changed);
        assert(game.globals().active_profile==2);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::ActivitySelectSetup));
        assert(game.globals().player_progress[2].values[0]==1);
    }
    {
        GameRoot game;
        game.install_profile_screen(adapter(root));
        game.set_outer_state(State::PlayerProfileAndNameEntry);
        auto frame=game.advance({.pointer_x=80,.pointer_y=175,
                                 .click_pulse=true});
        assert(frame.kind==FrameKind::FrontEndUpdated);
        assert(!game.globals().active_profile);
        frame=game.advance({});
        assert(frame.kind==FrameKind::FrontEndInitialized);
        frame=game.advance({});
        assert(frame.kind==FrameKind::FrontEndUpdated);
        assert(drawn(frame.effects,"Data/ui/enter name.bmp"));
        btb::full_game::ActivityFrameInput typed;
        typed.keyboard_state[0x1e]=0x80;
        typed.translated_character='A';
        frame=game.advance(typed);
        assert(frame.kind==FrameKind::FrontEndUpdated);
        frame=game.advance({});
        assert(drawn(frame.effects,
            "Data/ui/blue name boxed font.bmp"));
    }
    {
        // Confirmed delete must be durable even if the user quits without
        // finishing another activity; original per-player data is truncated.
        GameRoot game;
        const auto profile_dir=root/"profiles";
        fs::create_directories(profile_dir);
        std::string error;
        assert(game.load_profile_files(profile_dir,error));
        game.globals().profile_metadata[1].name_length=1;
        game.globals().profile_metadata[1].badge_index=2;
        game.globals().profile_metadata[1].name_glyph_codes[0]=32;
        game.globals().player_progress[1].values[0]=1;
        assert(game.save_profile_files(profile_dir,error));
        for(const auto& file:btb::profiles::activity_files_for_profile(1))
            create_file(profile_dir/file);
        game.install_profile_screen(adapter(root));
        game.set_outer_state(State::PlayerProfileAndNameEntry);
        auto frame=game.advance({.pointer_x=310,.pointer_y=445,
                                 .click_pulse=true});
        assert(frame.kind==FrameKind::FrontEndUpdated);
        frame=game.advance({.pointer_x=130,.pointer_y=310,
                            .click_pulse=true});
        assert(frame.kind==FrameKind::FrontEndUpdated);
        assert(frame.effects.open_yes_no_confirmation);
        frame=game.advance({.confirmation_resolved=true,
                            .confirmation_yes=true});
        assert(frame.kind==FrameKind::FrontEndUpdated);
        assert(frame.profile_saved);
        for(const auto& file:btb::profiles::activity_files_for_profile(1))
            assert(fs::file_size(profile_dir/file)==0);
        GameRoot restarted;
        assert(restarted.load_profile_files(profile_dir,error));
        assert(restarted.globals().profile_metadata[1].name_length==0);
        assert(restarted.globals().profile_metadata[1].badge_index==-1);
    }
    {
        const auto missing=root/"loaddata"/"fontdatablue.txt";
        fs::remove(missing);
        NativeProfileScreenAdapter driver(root,root/"profiles");
        std::string error;
        assert(!driver.initialize(error));
        assert(error.find("original profile font")!=std::string::npos);
    }
    fs::remove_all(root);
}
