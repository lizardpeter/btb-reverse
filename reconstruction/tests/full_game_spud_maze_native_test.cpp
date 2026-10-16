#include "btb/full_game_spud_maze.hpp"

#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"native Spud Maze: %s at %s:%d\\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

int main() {
    namespace fs=std::filesystem;
    namespace fg=btb::full_game;
    using btb::game_flow::State;
    const auto root=fs::temp_directory_path()/
        "btb_spud_maze_native_source_fixture";
    fs::remove_all(root);
    fs::create_directories(root/"loaddata");
    {
        std::ofstream f(root/"loaddata"/"spudmaze_nodes.txt");
        f<<"WestScreen_NODE_NUM_X_Y_DIR_TYPE_LINKS\n"
         <<"start 2 76 386 3 0 -1 -1 -1 -1\n"
         <<"NEXT_2nd Screen\n"
         <<"middle 0 606 300 8 256 -1 -1 -1 -1\n"
         <<"NEXT_3rdScreen\n"
         <<"third 0 606 300 8 256 -1 -1 -1 -1\n"
         <<"NEXT_4thScreen\n"
         <<"fourth 0 606 300 8 256 -1 -1 -1 -1\n"
         <<"NEXT_LastRightScreen\n"
         <<"last 0 434 136 2 0 -1 -1 -1 -1\n"
         <<"END\n";
        for(int screen=0;screen<5;++screen)
            for(int location=0;location<4;++location)
                f<<100+location*20<<" "<<100+screen*25<<" 0 ";
        f<<"\n2.0\n1 1 2\n1 1 2\n500 400 300\n-1 1000 200\n6\n";
        constexpr int point_counts[]{8,5,4,6,9};
        for(int screen=0;screen<5;++screen)
            for(int index=0;index<point_counts[screen];++index)
                f<<100+index*25<<" "<<120+screen*30<<"\n";
    }
    for(int difficulty=0;difficulty<3;++difficulty) {
        fg::GameRoot game;
        assert(game.select_profile(0));
        auto driver=std::make_unique<fg::SpudMazeDriver>(root,difficulty);
        auto* live=driver.get();
        game.install(fg::ActivityId::SpudMaze,std::move(driver));
        game.set_outer_state(State::SpudMazeInit);
        auto step=game.advance({});
        assert(step.kind==fg::FrameKind::ActivityInitialized);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::SpudMazeRun));
        assert(live->bob());
        assert(live->bob()->screen==0);
        assert(live->bob()->node==2);
        assert(live->bob()->x==76.f);
        assert(live->bob()->y==375.f);
        assert(live->bob()->speed==4.f); // source overwrites file's 2.0
        assert(live->bob()->repair_tool_engaged);
        assert(live->remaining_repairs()>=4);
        assert(live->remaining_repairs()<=11);
        assert(live->timer()==500-difficulty*100);
        step=game.advance({.elapsed_centiseconds=100});
        assert(step.kind==fg::FrameKind::ActivityUpdated);
        assert(!step.effects.fatal_error);
        assert(!step.effects.draws.empty());
        assert(step.effects.draws[0].source_asset==
               "Data\\SubGameSpudMaze\\screen1.bmp");
        bool drew_bob=false,drew_border=false,drew_repair=false;
        for(const auto& draw:step.effects.draws) {
            drew_bob|=draw.source_asset==
                "Data\\SubGameSpudMaze\\Repair.bmp";
            drew_border|=draw.source_asset==
                "Data\\SubGameSpudMaze\\screenborder.bmp";
            drew_repair|=draw.source_asset.find("-")!=std::string::npos;
        }
        assert(drew_bob&&drew_border&&drew_repair);
        assert(!step.effects.audio.empty());
        assert(step.effects.audio[0].sound_id==
               btb::spud_maze::kStartupVoiceSoundId);
        assert(step.effects.progress_writes.empty());

        step=game.advance({.elapsed_centiseconds=10100});
        assert(live->timer()==400-difficulty*100);
        assert(live->remaining_repairs()>=4);
        assert(step.effects.progress_writes.empty());
        step=game.advance({.elapsed_centiseconds=100000});
        assert(live->timer()<0);
        assert(!step.effects.save_and_unload);
        step=game.advance({.elapsed_centiseconds=100001});
        assert(!step.effects.save_and_unload);
        step=game.advance({.elapsed_centiseconds=100002});
        assert(step.effects.save_and_unload);
        assert(step.effects.progress_writes.empty());
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::PlayAgainYesNoSetup));
        assert(!live->bob());
        assert(game.globals().player_progress[0].get(
            btb::progress::Slot::SpudMaze)==0);
    }
    fs::remove_all(root);
}
