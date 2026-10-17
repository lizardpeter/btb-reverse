#include "btb/full_game_runtime.hpp"

#include <cassert>
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

    game.globals().dispatcher.current_state = 0x44;
    assert(game.advance({}).kind == FrameKind::InvalidState);
}
