#include "btb/full_game_runtime.hpp"

#include <cassert>
#include <sstream>
#include <memory>
#include <string>
#include <utility>

using namespace btb::full_game;
using btb::game_flow::State;

namespace {
struct ScriptedFireworks final : ActivityDriver {
    int starts{};
    bool fail_initialize{};
    [[nodiscard]] bool initialize(int, std::string& error) override {
        ++starts;
        if(fail_initialize) {
            error="original Fireworks graph/movies are unavailable";
            return false;
        }
        error.clear();
        return true;
    }
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput&) override {
        return {};
    }
    [[nodiscard]] bool unload(std::string& error) override {
        error.clear();
        return true;
    }
};

struct LeaveProbe final : ActivityDriver {
    int updates{},unloads{};
    bool allow_back{true},completed{};
    bool initialize(int,std::string& error) override {error.clear();return true;}
    ActivityFrameOutput advance(const ActivityFrameInput& input) override {
        ++updates;
        assert(!input.click_pulse || !allow_back);
        ActivityFrameOutput out;
        out.draws.push_back({"fixture.bmp",0,0});
        return out;
    }
    bool unload(std::string& error) override {++unloads;error.clear();return true;}
    bool shared_back_available() const noexcept override {return allow_back;}
    bool shared_back_starts_replay() const noexcept override {return completed;}
};

struct BadgeActivity final : ActivityDriver {
    btb::progress::Slot slot;
    explicit BadgeActivity(btb::progress::Slot earned):slot(earned){}
    bool initialize(int,std::string& error) override {
        error.clear();return true;
    }
    ActivityFrameOutput advance(const ActivityFrameInput&) override {
        ActivityFrameOutput out;
        out.progress_writes.push_back({slot,1});
        return out;
    }
    bool unload(std::string& error) override {error.clear();return true;}
};

struct ScriptedDino final : ActivityDriver {
    int starts{};
    int frames{};
    int unloads{};
    int profile{-1};

    [[nodiscard]] bool initialize(int p, std::string& error) override {
        ++starts;
        profile = p;
        error.clear();
        return true;
    }
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput&) override {
        ++frames;
        ActivityFrameOutput out;
        out.progress_writes.push_back({
            btb::progress::Slot::DinoRaptor, 1
        });
        out.shared_completion_code = 1;
        if (frames == 2) {
            out.next_outer_state =
                static_cast<int>(State::PlayAgainYesNoSetup);
            out.next_ui_context = 0x14;
            out.save_and_unload = true;
        }
        return out;
    }
    [[nodiscard]] bool unload(std::string& error) override {
        ++unloads;
        error.clear();
        return true;
    }
};
}

int main() {
    static_assert(kActivities.size() == 10);
    for (const auto& a : kActivities) {
        const auto* init = btb::game_flow::descriptor(
            static_cast<int>(a.init));
        const auto* run = btb::game_flow::descriptor(
            static_cast<int>(a.run));
        assert(init && run);
        assert(init->kind == btb::game_flow::StateKind::ActivityInit);
        assert(run->kind == btb::game_flow::StateKind::ActivityRun);
        assert(init->module == a.module);
        assert(run->module == a.module);
        assert(activity_for_state(a.pregame_setup) == &a);
        assert(activity_for_state(a.pregame_update) == &a);
        assert(activity_for_state(a.init) == &a);
        assert(activity_for_state(a.run) == &a);
    }

    GameRoot game;
    assert(!game.select_profile(-1));
    assert(!game.select_profile(5));
    assert(game.select_profile(2));

    // The frontend is not silently bypassed into a minigame.
    game.set_outer_state(State::ActivitySelectSetup);
    assert(game.advance({}).kind == FrameKind::FrontEndRequiresAdapter);

    game.set_outer_state(State::BobsBandInit);
    auto no_band = game.advance({});
    assert(no_band.kind == FrameKind::ActivityRequiresAdapter);

    auto driver = std::make_unique<ScriptedDino>();
    auto* original = driver.get();
    game.install(ActivityId::Dinosaur, std::move(driver));
    game.set_outer_state(State::DinoInit);

    // The retail modal precedence remains in control before the activity
    // initializer runs. Options defeats movie, chooser and activity states.
    std::istringstream options_fixture("0 0 250 417 299 462 340 417 389 466 208 222 410 242 122 86 480 350");
    std::string options_error;
    assert(game.configure_options(options_fixture,options_error));
    game.globals().dispatcher.options_active = true;
    game.globals().dispatcher.generic_screen_mode = 14;
    game.globals().dispatcher.global_movie_finished = true;
    auto intercepted = game.advance({});
    assert(intercepted.kind == FrameKind::Intercept);
    assert(intercepted.dispatch.pause_global_movie);
    assert(original->starts == 0);
    game.globals().dispatcher.options_active = false;
    game.globals().dispatcher.generic_screen_mode = 0;

    const auto initialize = game.advance({});
    assert(initialize.kind == FrameKind::ActivityInitialized);
    assert(initialize.state_changed);
    assert(original->starts == 1 && original->profile == 2);
    assert(game.globals().dispatcher.current_state ==
           static_cast<int>(State::DinoRun));

    const auto first = game.advance({});
    assert(first.kind == FrameKind::ActivityUpdated);
    assert(original->frames == 1);
    assert(game.globals().player_progress[2].get(
        btb::progress::Slot::DinoRaptor) == 1);
    assert(game.globals().shared_completion_code == 1);

    const auto second = game.advance({});
    assert(second.kind == FrameKind::ActivityUpdated);
    assert(second.effects.save_and_unload);
    assert(original->unloads == 1);
    assert(game.globals().dispatcher.current_state ==
           static_cast<int>(State::PlayAgainYesNoSetup));
    assert(game.globals().ui_context == 0x14);

    // Replays require the game's front-end adapter and must not re-run the
    // old activity behind the user's back.
    assert(game.advance({}).kind == FrameKind::FrontEndRequiresAdapter);
    game.set_outer_state(State::DinoRun);
    assert(game.advance({}).kind == FrameKind::ActivityFailed);

    // Movie mode 13 restores saved state and clears shared input on the
    // same dispatcher invocation, before handing control to it.
    game.globals().shared_completion_code=-1;
    game.globals().input_pulse = true;
    game.globals().dispatcher.current_state =
        static_cast<int>(State::DinoInit);
    game.globals().dispatcher.generic_screen_mode = 13;
    game.globals().dispatcher.saved_state =
        static_cast<int>(State::ActivitySelectSetup);
    game.globals().dispatcher.global_movie_finished = true;
    const auto movie = game.advance({});
    assert(movie.clear_input_pulse);
    assert(!game.globals().input_pulse);
    assert(game.globals().dispatcher.generic_screen_mode == 2);
    assert(game.globals().dispatcher.current_state ==
           static_cast<int>(State::ActivitySelectSetup));

    // The locked Finale overlay must use the selected player's stars
    // and preserve Activity Select on Back release.
    {
        auto root=std::make_unique<GameRoot>();
        assert(root->select_profile(1));
        root->set_outer_state(State::ActivitySelectUpdate);
        root->globals().dispatcher.progress_screen_active=true;
        root->globals().player_progress[0].set(btb::progress::Slot::Golf,1);
        root->globals().player_progress[1].set(btb::progress::Slot::DinoRaptor,1);
        root->globals().player_progress[1].set(btb::progress::Slot::PetsCorner,1);
        auto frame=root->advance({});
        assert(frame.kind==FrameKind::Intercept);
        assert(frame.dispatch.intercept==btb::game_flow::Intercept::ProgressScreen);
        assert(frame.effects.draws.size()==3);
        assert(frame.effects.draws[0].source_asset=="Data/ui/Progress-screen.bmp");
        assert(frame.effects.draws[1].source_asset=="Data/ui/star.bmp");
        assert(frame.effects.draws[1].x==282&&frame.effects.draws[1].y==307);
        assert(frame.effects.draws[2].x==282&&frame.effects.draws[2].y==184);
        ActivityFrameInput pointer{};
        pointer.pointer_position_valid=true;
        pointer.pointer_x=572;pointer.pointer_y=445;
        pointer.pointer_release_pulse=true;
        frame=root->advance(pointer);
        assert(root->globals().dispatcher.progress_screen_active);
        pointer.pointer_x=600;
        pointer.pointer_release_pulse=false;pointer.pointer_button_held=true;
        frame=root->advance(pointer);
        assert(root->globals().dispatcher.progress_screen_active);
        pointer.pointer_button_held=false;pointer.pointer_release_pulse=true;
        frame=root->advance(pointer);
        assert(!root->globals().dispatcher.progress_screen_active);
        assert(root->globals().dispatcher.current_state==static_cast<int>(State::ActivitySelectUpdate));
        assert(root->globals().player_progress[1].get(btb::progress::Slot::FireworkFinaleStarted)==0);
    }

    // Direct 00427C30 hover rectangles, missing-star narration, and
    // demand-driven shared CRT RNG. Leaving a star does not reset its latch.
    {
        auto root=std::make_unique<GameRoot>();
        assert(root->select_profile(0));
        root->set_outer_state(State::ActivitySelectUpdate);
        root->globals().dispatcher.progress_screen_active=true;
        auto& record=root->globals().player_progress[0];
        record.set(btb::progress::Slot::PetsCorner,1);
        btb::retail::OriginalRetailRandom rng(1),oracle(1);
        ActivityFrameInput in{};
        in.pointer_position_valid=true;in.original_random=&rng;
        in.pointer_x=312;in.pointer_y=337; // inclusive Pets Corner edge
        auto frame=root->advance(in);
        assert(frame.effects.audio.size()==1);
        assert(frame.effects.audio.back().sound_id==955+(oracle.next_rand()&1));
        assert(rng.state()==oracle.state());
        in.pointer_x=0;in.pointer_y=0;
        assert(root->advance(in).effects.audio.empty());
        in.pointer_x=282;in.pointer_y=307;
        assert(root->advance(in).effects.audio.empty());
        in.pointer_x=282;in.pointer_y=266; // unearned three-part Band
        frame=root->advance(in);
        assert(frame.effects.audio.size()==1&&frame.effects.audio.back().sound_id==959);
        assert(rng.state()==oracle.state());
        in.pointer_x=282;in.pointer_y=307;
        frame=root->advance(in);
        assert(frame.effects.audio.back().sound_id==955+(oracle.next_rand()&1));
        assert(rng.state()==oracle.state());
    }

    // 00429800 draws the preceding slider position before applying
    // movement; only held input changes volume and OK waits for narration.
    {
        auto root=std::make_unique<GameRoot>();
        std::istringstream options("0 0 250 417 299 462 340 417 389 466 208 222 410 242 122 86 480 350");
        std::string error;
        assert(root->configure_options(options,error));
        root->globals().dispatcher.options_active=true;
        ActivityFrameInput in{};
        in.pointer_position_valid=true;in.pointer_x=309;in.pointer_y=160;
        auto frame=root->advance(in);
        assert(!frame.effects.original_volume_percent);
        assert(frame.effects.draws.back().x==369);
        in.pointer_button_held=true;
        frame=root->advance(in);
        assert(frame.effects.original_volume_percent==50);
        assert(frame.effects.draws.back().x==369);
        frame=root->advance(in);
        assert(!frame.effects.original_volume_percent&&frame.effects.draws.back().x==309);
        in.pointer_x=208;
        assert(root->advance(in).effects.original_volume_percent==0);
        in.pointer_x=410;
        assert(root->advance(in).effects.original_volume_percent==100);
        in.pointer_button_held=false;in.pointer_release_pulse=true;
        in.pointer_x=300;in.pointer_y=280;
        frame=root->advance(in);
        assert(frame.effects.audio.back().sound_id==862);
        assert(frame.effects.audio.back().input_interruptible);
        in.pointer_release_pulse=false;in.managed_sound_playing=true;
        frame=root->advance(in);
        assert(root->globals().dispatcher.options_active);
        in.managed_sound_playing=false;
        frame=root->advance(in);
        assert(!root->globals().dispatcher.options_active);
    }

    // Quit cancellation and confirmation do not mutate or delete profiles.
    // Credits advance on clicks or the retail >4-second integer threshold.
    {
        auto root=std::make_unique<GameRoot>();
        std::istringstream credits("credits1.bmp credits2.bmp credits3.bmp");
        std::string error;
        assert(root->configure_credits(credits,error));
        root->set_outer_state(State::PlayerProfileAndNameEntry);
        root->globals().dispatcher.whole_game_quit_active=true;
        ActivityFrameInput in{};
        in.pointer_position_valid=true;in.pointer_x=350;in.pointer_y=275;
        in.pointer_release_pulse=true;
        auto frame=root->advance(in);
        assert(!root->globals().dispatcher.whole_game_quit_active);
        assert(frame.effects.draws.front().source_asset=="Data/ui/main/quitmain.bmp");
        root->globals().dispatcher.whole_game_quit_active=true;
        in.pointer_x=240;in.random_value=3;
        frame=root->advance(in);
        assert(frame.effects.audio.back().sound_id==629);
        in.pointer_release_pulse=false;in.managed_sound_playing=true;
        root->advance(in);
        assert(root->globals().dispatcher.shutdown_phase==btb::application::ShutdownPhase::Running);
        in.managed_sound_playing=false;
        root->advance(in);
        assert(root->globals().dispatcher.shutdown_phase==btb::application::ShutdownPhase::BeginCredits);
        frame=root->advance(in);
        assert(frame.effects.draws.front().source_asset=="credits1.bmp");
        in.elapsed_centiseconds=499;
        assert(root->advance(in).effects.draws.front().source_asset=="credits1.bmp");
        in.elapsed_centiseconds=500;
        assert(root->advance(in).effects.draws.front().source_asset=="credits2.bmp");
        in.click_pulse=true;
        assert(root->advance(in).effects.draws.front().source_asset=="credits3.bmp");
        root->advance(in);
        assert(root->globals().dispatcher.shutdown_phase==btb::application::ShutdownPhase::FinalTeardown);
    }

    // Original InitializeFireworksActivity writes slot 63 BEFORE the
    // authored show and certificate. This applies to the active profile
    // only and never creates a nonexistent slot-64 completion flag.
    GameRoot finale;
    assert(finale.select_profile(3));
    auto fireworks=std::make_unique<ScriptedFireworks>();
    auto* live_fireworks=fireworks.get();
    finale.install(ActivityId::Fireworks,std::move(fireworks));
    finale.set_outer_state(State::FireworksInit);
    const auto started=finale.advance({});
    assert(started.kind==FrameKind::ActivityInitialized);
    assert(live_fireworks->starts==1);
    assert(finale.globals().player_progress[3].get(
        btb::progress::Slot::FireworkFinaleStarted)==1);
    assert(finale.globals().player_progress[3].get(
        btb::progress::Slot::ReservedUnused)==0);
    for(int p=0;p<5;++p)
        if(p!=3)
            assert(finale.globals().player_progress[p].get(
                btb::progress::Slot::FireworkFinaleStarted)==0);
    const auto show=finale.advance({});
    assert(show.kind==FrameKind::ActivityUpdated);
    assert(show.effects.progress_writes.empty());
    finale.set_outer_state(State::FireworksInit);
    assert(finale.advance({}).kind==FrameKind::ActivityInitialized);
    assert(live_fireworks->starts==1); // same session, no double init

    GameRoot failed_finale;
    assert(failed_finale.select_profile(4));
    auto broken_fireworks=std::make_unique<ScriptedFireworks>();
    broken_fireworks->fail_initialize=true;
    failed_finale.install(ActivityId::Fireworks,
                          std::move(broken_fireworks));
    failed_finale.set_outer_state(State::FireworksInit);
    const auto unavailable=failed_finale.advance({});
    assert(unavailable.kind==FrameKind::ActivityFailed);
    assert(failed_finale.globals().player_progress[4].get(
        btb::progress::Slot::FireworkFinaleStarted)==0);
    assert(failed_finale.globals().player_progress[4].get(
        btb::progress::Slot::ReservedUnused)==0);

    // Host WM_DESTROY now unloads the active real minigame BEFORE saving
    // global player-progress files. This must work on an activity that is
    // still running (not only when the Play Again route already unloaded).
    GameRoot closing;
    assert(closing.select_profile(0));
    auto live=std::make_unique<ScriptedDino>();
    auto* live_source=live.get();
    closing.install(ActivityId::Dinosaur,std::move(live));
    closing.set_outer_state(State::DinoInit);
    assert(closing.advance({}).kind == FrameKind::ActivityInitialized);
    assert(live_source->starts==1);
    std::string close_error;
    assert(closing.unload_current(close_error));
    assert(live_source->unloads==1);
    assert(closing.unload_current(close_error)); // no duplicate save/unload
    assert(live_source->unloads==1);
    closing.set_outer_state(State::DinoRun);
    assert(closing.advance({}).kind == FrameKind::ActivityFailed);

    // Original paired completion movies are scoped to the active profile.
    using Slot=btb::progress::Slot;
    struct PairCase {ActivityId id; State init; Slot award,other; int code;};
    for(const auto& pair:std::array<PairCase,3>{{
        {ActivityId::Golf,State::GolfInit,Slot::Golf,Slot::Maze,3},
        {ActivityId::Maze,State::MazeInit,Slot::Maze,Slot::Golf,3},
        {ActivityId::SpudMaze,State::SpudMazeInit,Slot::SpudMaze,Slot::SpudSkate,2}
    }}) {
        for(bool paired:{false,true}) {
            auto root=std::make_unique<GameRoot>();
            assert(root->select_profile(1));
            root->globals().player_progress[0].values[
                static_cast<std::size_t>(pair.other)]=1;
            if(paired)root->globals().player_progress[1].values[
                static_cast<std::size_t>(pair.other)]=1;
            root->install(pair.id,std::make_unique<BadgeActivity>(pair.award));
            root->set_outer_state(pair.init);
            assert(root->advance({}).kind==FrameKind::ActivityInitialized);
            const auto result=root->advance({});
            assert(result.kind==FrameKind::ActivityUpdated);
            assert(root->globals().player_progress[1].get(pair.award)==1);
            assert(root->globals().shared_completion_code==(paired?pair.code:-1));
        }
    }

    // Every immediate-unload source handler is exercised with physical
    // Back/No and Back/Yes pulses, including an outstanding badge movie.
    struct LeaveCase{ActivityId id;State init;State destination;};
    for(const auto& c:std::array<LeaveCase,9>{{
        {ActivityId::Herding,State::HerdingInit,State::ActivitySelectSetup},
        {ActivityId::Dinosaur,State::DinoInit,State::DinoChooserSetup},
        {ActivityId::SpudSkate,State::SpudSkateInit,State::SpudChooserSetup},
        {ActivityId::Maze,State::MazeInit,State::AdventureChooserSetup},
        {ActivityId::Fireworks,State::FireworksInit,State::ActivitySelectSetup},
        {ActivityId::Squirrel,State::SquirrelInit,State::ActivitySelectSetup},
        {ActivityId::BobsBand,State::BobsBandInit,State::MusicChooserSetup},
        {ActivityId::Golf,State::GolfInit,State::AdventureChooserSetup},
        {ActivityId::SpudMaze,State::SpudMazeInit,State::SpudChooserSetup}
    }})for(bool movie:{false,true}){
        auto root=std::make_unique<GameRoot>();
        assert(root->select_profile(0));
        auto provider=std::make_unique<LeaveProbe>();auto* live=provider.get();
        root->install(c.id,std::move(provider));root->set_outer_state(c.init);
        auto step=[&](int x,int y,bool down=false,bool up=false){
            ActivityFrameInput input{};input.pointer_position_valid=true;
            input.pointer_x=x;input.pointer_y=y;input.click_pulse=down;
            input.pointer_release_pulse=up;input.pointer_button_held=down;
            return root->advance(input);
        };
        assert(step(0,0).kind==FrameKind::ActivityInitialized);
        live->allow_back=false;
        assert(!step(590,440,true).effects.open_leave_activity_confirmation);
        live->allow_back=true;
        assert(!step(572,440).effects.open_leave_activity_confirmation);
        assert(step(590,440,true).effects.open_leave_activity_confirmation);
        auto before=live->updates;
        assert(step(370,280,true).kind==FrameKind::Intercept);
        assert(step(370,280,false,true).kind==FrameKind::Intercept);
        assert(live->updates==before&&live->unloads==0);
        assert(!root->globals().dispatcher.leave_activity_active);
        assert(step(590,440,true).effects.open_leave_activity_confirmation);
        if(movie)root->globals().shared_completion_code=3;
        step(250,280,true);step(250,280,false,true);
        auto exited=step(0,0);
        assert(exited.kind==FrameKind::ActivityUpdated&&exited.effects.save_and_unload);
        assert(exited.effects.progress_writes.empty());
        assert(live->unloads==1&&live->updates==before+1);
        assert(root->globals().shared_movie_return_state==static_cast<int>(c.destination));
        const auto expected=movie&&c.id!=ActivityId::Fireworks?State::SharedMovieTransition:c.destination;
        assert(root->globals().dispatcher.current_state==static_cast<int>(expected));
        if(movie&&c.id==ActivityId::Fireworks){
            assert(step(0,0).kind==FrameKind::FrontEndUpdated);
            assert(root->globals().dispatcher.current_state==static_cast<int>(State::SharedMovieTransition));
        }
    }
    // Completed Dinosaur uses Play Again without interrupting its badge.
    {
        auto root=std::make_unique<GameRoot>();assert(root->select_profile(0));
        auto provider=std::make_unique<LeaveProbe>();auto* live=provider.get();live->completed=true;
        root->install(ActivityId::Dinosaur,std::move(provider));root->set_outer_state(State::DinoInit);
        assert(root->advance({}).kind==FrameKind::ActivityInitialized);
        auto exit=root->advance({.pointer_x=590,.pointer_y=440,.click_pulse=true,.pointer_position_valid=true});
        assert(exit.effects.save_and_unload&&live->unloads==1);
        assert(exit.effects.next_outer_state==static_cast<int>(State::PlayAgainYesNoSetup));
        assert(!root->globals().dispatcher.leave_activity_active&&root->globals().shared_movie_return_state==0x10);
    }

    game.globals().dispatcher.current_state = 0x44;
    assert(game.advance({}).kind == FrameKind::InvalidState);
}
