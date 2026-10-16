#include "btb/full_game_maze.hpp"

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
    auto intro=game.advance({.elapsed_centiseconds=100});
    assert(intro.kind==fg::FrameKind::ActivityUpdated);
    assert(intro.effects.draws[0].source_asset==
           "Data\\SubGameMaze\\MID_MAZE.bmp");
    assert(intro.effects.draws[1].source_asset==
           "Data\\SubGameMaze\\player.bmp");
    assert(intro.effects.draws[1].x==182);
    assert(intro.effects.draws[1].y==203);
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
    assert(frame.effects.draws[1].x==184);
    assert(frame.effects.draws[2].source_asset==
           "Data\\SubGameSpudMaze\\timer.bmp");
    assert(frame.effects.draws[2].source_rectangle);
    assert(frame.effects.draws[2].source_rectangle->right==101);
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
    fs::remove_all(root);
}
