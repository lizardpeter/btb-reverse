#include "btb/full_game_dinosaur.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>

namespace fs=std::filesystem;
using namespace btb::full_game;
namespace {
void bitmap(const fs::path& path,int width=10,int height=10) {
    fs::create_directories(path.parent_path());
    const int stride=(width*3+3)&~3;
    std::vector<unsigned char> bytes(54+stride*height);
    bytes[0]='B';bytes[1]='M';bytes[10]=54;bytes[14]=40;bytes[26]=1;bytes[28]=24;
    const auto put=[&](int at,int value) {
        for (int i=0;i<4;++i) bytes[at+i]=static_cast<unsigned char>(value>>(8*i));
    };
    put(2,static_cast<int>(bytes.size()));put(18,width);put(22,height);
    std::ofstream file(path,std::ios::binary);
    file.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());
}
void fixture(const fs::path& root) {
    for (const char* species:{"Raptor","Tric","Trex"})
        for (const char* difficulty:{"Easy","Medium","Hard"}) {
            const auto path=root/"Data/SubGameDino/Dino"/species/difficulty;
            bitmap(path/"background.bmp");bitmap(path/"piece0.bmp");bitmap(path/"piece1.bmp");
            std::ofstream(path/"dino.txt") <<
                "2\n200 100\n300 200\n10 300\n30 300\n80 300\n-1 -1\n1 0\n";
        }
    for (const char* path:{"Data/SubGameDino/bob.bmp","Data/SubGameDino/ellis.bmp",
            "Data/SubGameDino/vel.bmp","Data/SubGameDino/tri.bmp","Data/SubGameDino/trex.bmp",
            "Data/SubGameFirework/certprint.bmp","Data/SubGameFirework/certprintdep.bmp",
            "Data/ui/printbar.bmp"}) bitmap(root/path);
}
void audit(const GameFrame& frame,const OriginalAssetResolver& assets) {
    assert(frame.kind!=FrameKind::ActivityFailed);
    assert(!frame.effects.fatal_error);
    for (const auto& draw:frame.effects.draws) assert(assets.resolve(draw.source_asset).found());
}
}
int main(int argc,char** argv) {
    const bool retail=argc==2;
    const auto root=retail?fs::path(argv[1]):fs::temp_directory_path()/"btb_dinosaur_native_test";
    if (!retail) { fs::remove_all(root);fixture(root); }
    const OriginalAssetResolver assets(root);
    for (int species=0;species<3;++species) for (int difficulty=0;difficulty<3;++difficulty) {
        GameRoot game;
        assert(game.select_profile(2));
        game.globals().menu.selected_subgame=species;
        game.globals().menu.source_variant=difficulty;
        auto driver=std::make_unique<DinosaurDriver>(root,game.globals().random);
        auto* activity=driver.get();
        game.install(ActivityId::Dinosaur,std::move(driver));
        game.set_outer_state(btb::game_flow::State::DinoInit);
        auto frame=game.advance({});
        assert(frame.kind==FrameKind::ActivityInitialized);
        assert(game.globals().dispatcher.current_state==0x15);
        frame=game.advance({});audit(frame,assets);
        assert(frame.effects.audio.size()==3); // stop music, original music, intro voice
        assert(frame.effects.audio.back().sound_id==188+species);
        assert(frame.effects.audio.back().input_interruptible);
        assert(activity->state().pieces.size()>0);
        const auto count=static_cast<int>(activity->state().pieces.size());
        for (int slot=0;slot<count;++slot) {
            const auto piece=activity->state().pieces[static_cast<std::size_t>(slot)];
            frame=game.advance({.pointer_x=piece.x,.pointer_y=piece.y,.click_pulse=true});
            audit(frame,assets);
            assert(activity->state().selected==slot);
            assert(frame.effects.hide_system_cursor && frame.effects.pointer_warp);
            assert(frame.effects.draws.back().source_asset.ends_with("piece"+std::to_string(slot)+".bmp"));
            if (slot==0) {
                frame=game.advance({.pointer_x=piece.target_x>100?0:600,
                    .pointer_y=piece.target_y>100?0:400,.click_pulse=true});
                audit(frame,assets);
                assert(activity->state().phase==1);
                assert(frame.effects.hide_system_cursor);
                assert(frame.effects.audio.back().sound_id>=170 && frame.effects.audio.back().sound_id<=175);
            }
            frame=game.advance({.pointer_x=piece.target_x,.pointer_y=piece.target_y,.click_pulse=true});
            audit(frame,assets);
            assert(activity->state().phase==2);
            assert(!frame.effects.hide_system_cursor);
            assert(frame.effects.audio.back().sound_id>=164 && frame.effects.audio.back().sound_id<=169);
            frame=game.advance({});audit(frame,assets);
            assert(activity->state().completed==slot+1);
        }
        frame=game.advance({.managed_sound_playing=true});audit(frame,assets);
        assert(activity->state().completion_state==0);
        assert(frame.effects.progress_writes.empty());
        frame=game.advance({});audit(frame,assets);
        assert(activity->state().completion_state==1);
        assert(frame.effects.progress_writes.size()==1);
        assert(game.globals().player_progress[2].values[56+species]==1);
        assert(game.globals().shared_completion_code==1);
        assert(frame.effects.audio[1].sound_id==185+species);
        // Certificate remains interactive during its real completion voice.
        frame=game.advance({.pointer_x=320,.pointer_y=440,.click_pulse=true,
            .managed_sound_playing=true});audit(frame,assets);
        assert(frame.effects.print_frame);
        assert(frame.effects.draws.back().source_asset=="Data/ui/printbar.bmp");
        activity->request_leave();
        frame=game.advance({});
        assert(frame.kind==FrameKind::ActivityUpdated && frame.effects.save_and_unload);
        assert(game.globals().dispatcher.current_state==0x40);
    }
    if (!retail) fs::remove_all(root);
}
