#include "btb/full_game_spud_maze.hpp"

#include <cmath>
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
    fs::create_directories(root/"Data/SubGameSpudMaze");
    // Metadata-only synthetic repair surfaces, no shipped artwork.
    for(int screen=0;screen<5;++screen){
        for(std::size_t i=0;i<btb::spud_maze::kRepairPointCounts[screen];++i){
            std::array<unsigned char,26> bmp{};bmp[0]='B';bmp[1]='M';
            bmp[14]=40;bmp[18]=2;bmp[22]=20;
            std::ofstream image(root/"Data/SubGameSpudMaze"/
                (std::to_string(screen+1)+"-"+std::to_string(i+1)+".bmp"),std::ios::binary);
            image.write(reinterpret_cast<const char*>(bmp.data()),bmp.size());
        }
    }
    {
        std::ofstream f(root/"loaddata"/"spudmaze_nodes.txt");
        f<<"WestScreen_NODE_NUM_X_Y_DIR_TYPE_LINKS\n"
         <<"start 2 76 386 7 0 4 -1 -1 -1\n"
         <<"upper 4 76 300 4 0 -1 -1 2 -1\n"
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
                f<<(screen==0?76:100+index*25)<<" "
                 <<(screen==0?335:120+screen*30)<<"\n";
    }
    for(int difficulty=0;difficulty<3;++difficulty) {
        fg::GameRoot game;
        assert(game.select_profile(0));
        btb::retail::OriginalRetailRandom rng{7};
        btb::retail::OriginalRetailRandom reference{7};
        static_cast<void>(reference.next_rand());
        auto driver=std::make_unique<fg::SpudMazeDriver>(root,difficulty,&rng);
        auto* live=driver.get();
        game.install(fg::ActivityId::SpudMaze,std::move(driver));
        game.set_outer_state(State::SpudMazeInit);
        auto step=game.advance({});
        assert(step.kind==fg::FrameKind::ActivityInitialized);
        assert(rng.state()==reference.state());
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::SpudMazeRun));
        assert(live->bob());
        assert(live->bob()->screen==0);
        assert(live->bob()->node==2);
        assert(live->bob()->x==76.f);
        assert(live->bob()->y==375.f);
        assert(live->bob()->speed==4.f); // source overwrites file's 2.0
        assert(live->bob()->repair_tool_engaged);
        assert(live->bob()->repairs[0][0].width==2&&live->bob()->repairs[0][0].height==20);
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
        for(const auto& d:step.effects.draws)
            assert(d.source_asset!="Data\\SubGameSpudMaze\\toolbarhammer.bmp");
        for(const auto& sprite:step.effects.draws)
            if(sprite.source_asset=="Data\\SubGameSpudMaze\\Repair.bmp") {
                assert(sprite.source_rectangle);
                assert(sprite.source_rectangle->left==30*83);
                assert(sprite.source_rectangle->right==31*83);
                assert(sprite.source_rectangle->top==0);
                assert(sprite.source_rectangle->bottom==83);
            }
        assert(!step.effects.audio.empty());
        assert(step.effects.audio[0].operation==fg::AudioOperation::StartBackingTrack);
        assert(step.effects.audio[0].source_asset=="Data/music/Skateboardfix.wav");
        assert(step.effects.audio[1].sound_id==btb::spud_maze::kStartupVoiceSoundId);
        assert(step.effects.audio[1].input_interruptible);
        assert(step.effects.progress_writes.empty());
        step=game.advance({.elapsed_centiseconds=105,
                           .directional_input_bits=0x04});
        assert(step.kind==fg::FrameKind::ActivityUpdated);
        assert(std::abs(live->bob()->y-371.f)<0.02f); // original Up scan code
        step=game.advance({.elapsed_centiseconds=110,
                           .directional_input_bits=0x08});
        // The recovered integer heading and float stores introduce subpixel drift.
        assert(std::abs(live->bob()->y-375.f)<0.2f&&std::abs(live->bob()->x-76.f)<1.0f); // original Down scan code

        step=game.advance({.elapsed_centiseconds=10100});
        assert(live->timer()==400-difficulty*100);
        assert(live->remaining_repairs()>=4);
        assert(step.effects.progress_writes.empty());
        step=game.advance({.elapsed_centiseconds=100000});
        assert(live->timer()<0);
        assert(!step.effects.save_and_unload);
        std::vector<int> narration;
        bool bentley_drawn=false;
        for(int tick=1;tick<3000&&!step.effects.save_and_unload;++tick) {
            step=game.advance({.elapsed_centiseconds=100000+tick});
            assert(!step.effects.fatal_error);
            for(const auto& voice:step.effects.audio)
                if(voice.sound_id>=713&&voice.sound_id<=727)
                    narration.push_back(voice.sound_id);
            for(const auto& image:step.effects.draws)
                if(image.source_asset=="Data\\SubGameSpudMaze\\bentley.bmp") {
                    bentley_drawn=true;
                    assert(image.source_rectangle&&image.source_rectangle->right<=4256);
                }
        }
        // The first timeout frame starts the entrance animation and does
        // not queue a voice until Bentley enters the visible playfield.
        assert(bentley_drawn);
        assert(narration.size()==3&&narration[0]==713&&narration[1]==714);
        assert(narration[2]==716||narration[2]==717);
        assert(step.effects.save_and_unload);
        assert(step.effects.progress_writes.empty());
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::PlayAgainYesNoSetup));
        assert(!live->bob());
        assert(game.globals().player_progress[0].get(
            btb::progress::Slot::SpudMaze)==0);
    }
    {
        // Actual normal GameRoot mouse click starts a source repair
        // beneath Bob. 0x00422400 animates 9..14, wraps across five
        // source passes at one frame every four ticks, and ONLY then
        // decrements the outstanding-track count.
        fg::GameRoot game;
        assert(game.select_profile(1));
        auto driver=std::make_unique<fg::SpudMazeDriver>(root,0);
        auto* live=driver.get();
        game.install(fg::ActivityId::SpudMaze,std::move(driver));
        game.set_outer_state(State::SpudMazeInit);
        assert(game.advance({}).kind==fg::FrameKind::ActivityInitialized);
        auto frame=game.advance({.elapsed_centiseconds=0});
        assert(live->remaining_repairs()==4);
        frame=game.advance({.click_pulse=true,.elapsed_centiseconds=1});
        assert(frame.kind==fg::FrameKind::ActivityUpdated);
        assert(live->bob()->hazard_lock);
        assert(live->bob()->animation==9);
        assert(live->remaining_repairs()==4);
        bool repair_voice=false;
        for(const auto& audio:frame.effects.audio)
            repair_voice|=audio.sound_id>=702&&audio.sound_id<=708;
        assert(repair_voice);
        for(int ticks=1;ticks<=119;++ticks) {
            frame=game.advance({.elapsed_centiseconds=ticks+1});
            assert(frame.kind==fg::FrameKind::ActivityUpdated);
            assert(live->remaining_repairs()==4);
        }
        frame=game.advance({.elapsed_centiseconds=121});
        assert(live->remaining_repairs()==3);
        assert(!live->bob()->hazard_lock);
        assert(live->bob()->animation==0);
        assert(frame.effects.progress_writes.empty());
    }
    fs::remove_all(root);
}
