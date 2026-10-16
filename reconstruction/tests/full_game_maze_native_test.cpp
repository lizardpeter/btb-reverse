#include "btb/full_game_maze.hpp"
#include "btb/maze_box_layout.hpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"original Maze CTest: %s at %s:%d\\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>

int main() {
    namespace fs=std::filesystem;
    namespace fg=btb::full_game;
    using btb::game_flow::State;
    const auto root=fs::temp_directory_path()/"btb_retail_maze_native_fixture";
    fs::remove_all(root);
    fs::create_directories(root/"loaddata");
    // Original 3-screen grammar and exact middle-screen source node 8,
    // with authentic Y-11 correction. Fixture is synthetic, no retail
    // proprietary bitmap or map copied into the source repository.
    const char* source=R"(header
west0 0 587 188 8 512 -1 -1 -1 -1
NEXT_MiddleScreen
middle8 8 247 278 2 0 -1 9 -1 -1
middle9 9 270 278 8 0 -1 -1 -1 8
NEXT_EastScreen
east0 0 40 180 2 512 -1 -1 -1 -1
END
87 67 0 548 67 0 66 320 0 568 320 0
89 98 8 549 98 9 60 314 8 563 314 9
526 69 0 89 323 0 392 249 0 546 323 0
2.0
1 1 2
5 6 7
240 180 120
1500 1000 200
6
)";
    std::ofstream(root/"loaddata"/"maze_nodes.txt")<<source;
    // Original 0x0041A730 has 1/2/1, 3/3/2 and 4/4/4
    // filled slots; duplicate random indices must be retried.
    for(int difficulty=0;difficulty<3;++difficulty) {
        unsigned sample=0;
        auto allocated=btb::maze::allocate_source_boxes(
            difficulty,[&] {return sample++;});
        assert(allocated.total==4*(difficulty+1));
        assert(allocated.remaining==allocated.total);
        int active=0;
        for(std::size_t screen=0;screen<3;++screen) {
            for(std::size_t i=0;i<4;++i) {
                const auto& slot=allocated.screens[screen][i];
                if(slot.node_id<0)continue;
                ++active;
                assert(slot.status==1);
                for(std::size_t previous=0;previous<i;++previous)
                    assert(allocated.screens[screen][previous].node_id!=slot.node_id);
            }
        }
        assert(active==allocated.total);
    }
    {
        // Explicit repeated source random index must use an additional
        // result rather than silently picking the next free location.
        const unsigned source_rolls[]{0,0,0,1,2};
        std::size_t consumed=0;
        auto allocated=btb::maze::allocate_source_boxes(0,[&] {
            return source_rolls[consumed++];
        });
        assert(consumed==5);
        assert(allocated.screens[0][0].node_id==0);
        assert(allocated.screens[1][0].node_id==0);
        assert(allocated.screens[1][1].node_id==1);
        assert(allocated.screens[2][0].node_id==2);
    }

    fg::GameRoot game;
    assert(game.select_profile(1));
    auto driver=std::make_unique<fg::MazeDriver>(root,0);
    auto* maze=driver.get();
    game.install(fg::ActivityId::Maze,std::move(driver));
    game.set_outer_state(State::MazeInit);
    auto frame=game.advance({});
    assert(frame.kind==fg::FrameKind::ActivityInitialized);
    assert(game.globals().dispatcher.current_state==
           static_cast<int>(State::MazeRun));
    assert(maze->player());
    assert(maze->player()->screen==1);
    assert(maze->player()->node==8);
    assert(maze->player()->x==247.0f&&maze->player()->y==267.0f);
    assert(maze->npc());
    assert(maze->npc()->phase==btb::maze::SpudNpcPhase::SpawnWait);
    assert(maze->npc()->screen==1);
    assert(maze->npc()->spawn_interval==3000);
    assert(maze->npc()->spawn_wait==3000);
    assert(maze->npc()->has_box_target_nodes);
    assert(maze->npc()->package_target_nodes[1][0]==8);
    assert(maze->npc()->package_target_nodes[1][1]==9);
    assert(maze->boxes().total==4);
    assert(maze->boxes().remaining==4);
    assert(maze->boxes().screens[0][0].node_id>=0);
    assert(maze->boxes().screens[0][1].node_id==-1);
    assert(maze->boxes().screens[1][0].node_id>=0);
    assert(maze->boxes().screens[1][1].node_id>=0);
    assert(maze->boxes().screens[1][0].node_id !=
           maze->boxes().screens[1][1].node_id);
    auto intro=game.advance({.elapsed_centiseconds=100});
    assert(intro.kind==fg::FrameKind::ActivityUpdated);
    assert(intro.effects.draws[0].source_asset==
           "Data\\SubGameMaze\\MID_MAZE.bmp");
    assert(intro.effects.draws[1].source_asset==
           "Data\\SubGameMaze\\box.bmp");
    assert(intro.effects.draws[2].source_asset==
           "Data\\SubGameMaze\\box.bmp");
    assert(intro.effects.draws[3].source_asset==
           "Data\\SubGameMaze\\player.bmp");
    assert(intro.effects.draws[3].x==182);
    assert(intro.effects.draws[3].y==203);
    assert(intro.effects.audio.size()==1);
    assert(intro.effects.audio[0].sound_id==0x49);
    assert(maze->remaining_seconds()==240);
    frame=game.advance({.managed_sound_playing=true,
                        .elapsed_centiseconds=200,
                        .directional_input_bits=0x02});
    assert(maze->player()->x==247.0f); // source narrator gate
    frame=game.advance({.elapsed_centiseconds=300});
    assert(maze->player()->x==247.0f);
    frame=game.advance({.elapsed_centiseconds=400,
                        .directional_input_bits=0x02});
    assert(frame.kind==fg::FrameKind::ActivityUpdated);
    assert(!frame.effects.fatal_error);
    assert(maze->player()->x==249.0f);
    assert(maze->remaining_seconds()==239);
    assert(frame.effects.draws[3].x==184);
    assert(frame.effects.draws[4].source_asset==
           "Data\\SubGameSpudMaze\\timer.bmp");
    assert(frame.effects.draws[4].source_rectangle);
    assert(frame.effects.draws[4].source_rectangle->right==101);
    // Retail 0041AF60: moving the cursor while in keyboard mode changes
    // the stored mode but the current frame still takes the keyboard path.
    frame=game.advance({.pointer_x=450,.pointer_y=267,
                        .elapsed_centiseconds=410});
    assert(maze->player()->x==249.0f);
    assert(!maze->player()->mouse_mode);
    // Mouse mode uses the real three-sample direction debounce. The first
    // two stable cursor frames do not move the source character.
    for(int pass=0;pass<2;++pass) {
        frame=game.advance({.pointer_x=450,.pointer_y=267,
                            .elapsed_centiseconds=420+pass*10});
        assert(maze->player()->x==249.0f);
        assert(maze->player()->mouse_mode);
    }
    frame=game.advance({.pointer_x=450,.pointer_y=267,
                        .elapsed_centiseconds=440});
    assert(maze->player()->x==251.0f);
    assert(frame.kind==fg::FrameKind::ActivityUpdated);
    assert(!frame.effects.fatal_error);
    // The same mouse-mode frame that sees keyboard direction writes the
    // future keyboard mode, but uses mouse input for that call.
    frame=game.advance({.pointer_x=450,.pointer_y=267,
                        .elapsed_centiseconds=450,
                        .directional_input_bits=0x02});
    assert(maze->player()->mouse_mode);
    frame=game.advance({.pointer_x=450,.pointer_y=267,
                        .elapsed_centiseconds=460,
                        .directional_input_bits=0x02});
    assert(!maze->player()->mouse_mode);
    // Missing package/NPC simulation must NOT write a fake victory flag.
    assert(frame.effects.progress_writes.empty());
    frame=game.advance({.elapsed_centiseconds=24500});
    assert(maze->remaining_seconds()<0);
    assert(frame.effects.audio.back().sound_id==0x56);
    frame=game.advance({.managed_sound_playing=true,
                        .elapsed_centiseconds=24600});
    assert(!frame.effects.save_and_unload);
    frame=game.advance({.elapsed_centiseconds=24700});
    assert(!frame.effects.save_and_unload);
    frame=game.advance({.elapsed_centiseconds=24800});
    assert(frame.effects.save_and_unload);
    assert(frame.effects.progress_writes.empty());
    assert(game.globals().dispatcher.current_state==
           static_cast<int>(State::PlayAgainYesNoSetup));
    assert(!maze->player());
    // Second native game session: place both middle-screen retail
    // reference boxes at the real initial player location. The original
    // first-match pickup must hide one box, freeze player movement, run
    // the row-3..7 animation, then permit the next box to be collected.
    std::string near_source=source;
    const std::string original_boxes=
        "89 98 8 549 98 9 60 314 8 563 314 9";
    const auto at=near_source.find(original_boxes);
    assert(at!=std::string::npos);
    near_source.replace(at,original_boxes.size(),
        "247 267 8 247 267 9 247 267 8 247 267 9");
    std::ofstream(root/"loaddata"/"maze_nodes.txt")<<near_source;
    fg::GameRoot pickup_game;
    assert(pickup_game.select_profile(1));
    auto pickup_driver=std::make_unique<fg::MazeDriver>(root,0);
    auto* actual=pickup_driver.get();
    pickup_game.install(fg::ActivityId::Maze,std::move(pickup_driver));
    pickup_game.set_outer_state(State::MazeInit);
    assert(pickup_game.advance({}).kind==fg::FrameKind::ActivityInitialized);
    auto pickup=pickup_game.advance({.elapsed_centiseconds=100});
    assert(pickup.effects.audio.size()==1);
    pickup=pickup_game.advance({.elapsed_centiseconds=200});
    assert(actual->boxes().remaining==4);
    pickup=pickup_game.advance({.elapsed_centiseconds=300});
    assert(pickup.kind==fg::FrameKind::ActivityUpdated);
    assert(actual->boxes().remaining==3);
    assert(actual->player()->frozen);
    assert(pickup.effects.draws[0].source_asset==
           "Data\\SubGameMaze\\MID_MAZE.bmp");
    assert(pickup.effects.draws[1].source_asset==
           "Data\\SubGameMaze\\box.bmp");
    assert(pickup.effects.draws[2].source_asset==
           "Data\\SubGameMaze\\player.bmp");
    assert(pickup.effects.draws[2].source_rectangle);
    assert(pickup.effects.draws[2].source_rectangle->left==3*128);
    assert(pickup.effects.draws[2].source_rectangle->top==
           actual->player()->facing*128);
    assert(pickup.effects.progress_writes.empty());
    for(int ticks=0;ticks<81;++ticks) {
        pickup=pickup_game.advance({
            .elapsed_centiseconds=310+ticks,
            .directional_input_bits=0x02});
        assert(pickup.kind==fg::FrameKind::ActivityUpdated);
        assert(actual->player()->x==247.0f);
    }
    assert(!actual->player()->frozen);
    assert(!pickup.effects.audio.empty());
    assert(pickup.effects.audio.back().sound_id==0x4d);
    pickup=pickup_game.advance({.elapsed_centiseconds=420});
    assert(actual->boxes().remaining==2);
    assert(actual->player()->frozen);
    assert(pickup.effects.progress_writes.empty());
    assert(pickup_game.globals().player_progress[1].get(
        btb::progress::Slot::Maze)==0);
    fs::remove_all(root);
}
