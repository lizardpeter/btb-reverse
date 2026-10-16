#include "btb/full_game_maze.hpp"
#include "btb/maze_box_layout.hpp"
#include "btb/maze_controller.hpp"

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

// Isolate GameRoot's real Maze-win progress/movie and disk contracts
// without forging package collision inside the production MazeDriver.
class MazeVictoryEffectFixture final : public btb::full_game::ActivityDriver {
public:
    bool initialize(int,std::string& error) override {
        error.clear();return true;
    }
    btb::full_game::ActivityFrameOutput advance(
        const btb::full_game::ActivityFrameInput&) override {
        btb::full_game::ActivityFrameOutput out;
        out.progress_writes.push_back({btb::progress::Slot::Maze,1});
        out.save_and_unload=true;
        out.next_outer_state=static_cast<int>(
            btb::game_flow::State::PlayAgainYesNoSetup);
        out.next_ui_context=0x20;
        return out;
    }
    bool unload(std::string& error) override {
        error.clear();return true;
    }
};

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

    // Exact 0x0043B460 is IEEE754 0x42380000 = 46.0f in the genuine
    // SHA-matched retail EXE. Distance ==46 must NEVER collect a package.
    assert(btb::maze::kRetailPackagePickupRadius==46);
    {
        auto parsed=btb::maze::parse_data(source);
        unsigned next=0;
        auto boxes=btb::maze::allocate_source_boxes(0,[&] {
            return next++;
        });
        assert(boxes.remaining==4);
        for(int screen=0;screen<3;++screen) {
            for(auto& slot:boxes.screens[static_cast<std::size_t>(screen)]) {
                if(slot.node_id<0 || slot.status!=1)continue;
                const auto target=parsed.box_locations[
                    static_cast<std::size_t>(screen)][
                    static_cast<std::size_t>(slot.node_id)].position;
                const auto too_far=btb::maze::pickup_source_packages(
                    boxes,parsed,screen,static_cast<float>(target.x+46),
                    static_cast<float>(target.y));
                assert(too_far.collected==0);
                assert(slot.status==1);
                const auto accepted=btb::maze::pickup_source_packages(
                    boxes,parsed,screen,static_cast<float>(target.x+45),
                    static_cast<float>(target.y));
                assert(accepted.collected==1);
                assert(slot.status==0);
                assert(accepted.begin_player_collection_animation);
                const auto again=btb::maze::pickup_source_packages(
                    boxes,parsed,screen,static_cast<float>(target.x),
                    static_cast<float>(target.y));
                assert(again.collected==0);
            }
        }
        assert(boxes.remaining==0);
        assert(btb::maze::classify_end_condition(
            boxes.remaining,180,0)==btb::maze::EndCondition::Success);
        const auto early=btb::maze::begin_outcome_feedback(
            btb::maze::EndCondition::Success,180);
        const auto late=btb::maze::begin_outcome_feedback(
            btb::maze::EndCondition::Success,15);
        const auto timed=btb::maze::begin_outcome_feedback(
            btb::maze::EndCondition::Timeout,-1);
        assert(early && early->sound_id==0x54);
        assert(late && late->sound_id==0x55);
        assert(timed && timed->sound_id==0x56);
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
    {
        // Source output from a true completed Maze must pair correctly with
        // the adjacent already-finished Golf progress slot and persist.
        fg::GameRoot pair;
        assert(pair.select_profile(0));
        pair.globals().player_progress[0].set(btb::progress::Slot::Golf,1);
        const auto saves=root/"victory_profiles";
        pair.configure_profile_directory(saves);
        pair.install(fg::ActivityId::Maze,
                     std::make_unique<MazeVictoryEffectFixture>());
        pair.set_outer_state(State::MazeInit);
        assert(pair.advance({}).kind==fg::FrameKind::ActivityInitialized);
        const auto won=pair.advance({});
        assert(won.kind==fg::FrameKind::ActivityUpdated);
        assert(won.effects.progress_writes.size()==1);
        assert(won.profile_saved);
        assert(pair.globals().shared_completion_code==3);
        assert(pair.globals().ui_context==0x20);
        fg::GameRoot reopened;
        std::string save_error;
        assert(reopened.load_profile_files(saves,save_error));
        assert(reopened.globals().player_progress[0].get(
            btb::progress::Slot::Maze)==1);
        assert(reopened.globals().player_progress[0].get(
            btb::progress::Slot::Golf)==1);
    }
    {
        fg::GameRoot solo;
        assert(solo.select_profile(0));
        solo.install(fg::ActivityId::Maze,
                     std::make_unique<MazeVictoryEffectFixture>());
        solo.set_outer_state(State::MazeInit);
        assert(solo.advance({}).kind==fg::FrameKind::ActivityInitialized);
        assert(solo.advance({}).kind==fg::FrameKind::ActivityUpdated);
        assert(solo.globals().shared_completion_code==-1);
    }
    {
        // A compact source-format THREE-SCREEN graph exercises actual
        // movement/portal capture, package collection and the normal
        // 0x0041D4D0 success end gate. This is intentionally a synthetic
        // test map, not a claim of a retail-level playthrough.
        const auto short_map=root/"short_map";
        fs::create_directories(short_map/"loaddata");
        std::ofstream(short_map/"loaddata"/"maze_nodes.txt") << R"(header
west0 0 100 111 2 512 -1 -1 -1 -1
NEXT_MiddleScreen
mid8 8 247 278 10 0 -1 9 -1 10
midWest 9 255 278 2 128 -1 -1 -1 -1
midEast 10 239 278 8 256 -1 -1 -1 -1
NEXT_EastScreen
east0 0 100 111 2 0 -1 -1 -1 -1
END
100 100 0 100 100 0 100 100 0 100 100 0
247 267 8 247 267 8 247 267 8 247 267 8
100 100 0 100 100 0 100 100 0 100 100 0
2.0
1 1 2
5 6 7
240 180 120
1500 1000 200
6
)";
        fg::GameRoot source_game;
        assert(source_game.select_profile(0));
        source_game.globals().player_progress[0].set(
            btb::progress::Slot::Golf,1);
        const auto saves=short_map/"profiles";
        source_game.configure_profile_directory(saves);
        auto actual=std::make_unique<fg::MazeDriver>(short_map,0);
        auto* driver=actual.get();
        source_game.install(fg::ActivityId::Maze,std::move(actual));
        source_game.set_outer_state(State::MazeInit);
        assert(source_game.advance({}).kind==fg::FrameKind::ActivityInitialized);
        assert(driver->boxes().total==4);
        assert(source_game.advance({}).effects.audio.back().sound_id==0x49);
        assert(source_game.advance({.managed_sound_playing=true}).kind==
               fg::FrameKind::ActivityUpdated);
        assert(source_game.advance({}).kind==fg::FrameKind::ActivityUpdated);
        // Move from the center to the West portal, then return to middle,
        // then use the East portal. Source node proximity and authored
        // layout place 1,2,1 packages exactly at those three entrances.
        auto move=[&](std::uint8_t bits) {
            fg::ActivityFrameInput input;
            input.directional_input_bits=bits;
            auto result=source_game.advance(input);
            assert(result.kind==fg::FrameKind::ActivityUpdated);
            assert(!result.effects.fatal_error);
        };
        // Original current-node capture is <=4, but linked-node
        // capture is strict <4; do not teleport after only two steps.
        for(int step=0;step<3;++step)move(0x02);
        assert(driver->player()->screen==1);
        move(0x02); // linked node9 captured -> West; first box
        assert(driver->player()->screen==0);
        assert(driver->boxes().remaining==3);
        move(0x02); // West node0 -> middle node8; two center boxes
        assert(driver->player()->screen==1);
        assert(driver->boxes().remaining==1);
        for(int step=0;step<3;++step)move(0x01);
        assert(driver->player()->screen==1);
        move(0x01); // linked node10 captured -> East; final box
        assert(driver->player()->screen==2);
        assert(driver->boxes().remaining==0);
        auto feedback=source_game.advance({});
        assert(feedback.kind==fg::FrameKind::ActivityUpdated);
        assert(!feedback.effects.audio.empty());
        assert(feedback.effects.audio.back().sound_id==0x54);
        assert(!feedback.effects.save_and_unload);
        assert(source_game.advance({.managed_sound_playing=true}).kind==
               fg::FrameKind::ActivityUpdated);
        assert(source_game.advance({}).kind==fg::FrameKind::ActivityUpdated);
        auto won=source_game.advance({});
        assert(won.kind==fg::FrameKind::ActivityUpdated);
        assert(won.effects.save_and_unload);
        assert(won.profile_saved);
        assert(won.effects.progress_writes.size()==1);
        assert(won.effects.progress_writes[0].slot==
               btb::progress::Slot::Maze);
        assert(source_game.globals().shared_completion_code==3);
        assert(source_game.globals().dispatcher.current_state==
               static_cast<int>(State::PlayAgainYesNoSetup));
        fg::GameRoot restart;
        std::string load_error;
        assert(restart.load_profile_files(saves,load_error));
        assert(restart.globals().player_progress[0].get(
            btb::progress::Slot::Maze)==1);
        assert(restart.globals().player_progress[0].get(
            btb::progress::Slot::Golf)==1);
        assert(!driver->player()); // original activity has been unloaded
    }
    fs::remove_all(root);
}
