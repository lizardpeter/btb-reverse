#include "btb/game_flow.hpp"

#include <cassert>

using namespace btb;
using namespace btb::game_flow;

int main() {
    static_assert(kStateCount == 68);
    static_assert(kHighestState == 0x43);
    static_assert(static_cast<int>(State::StartupMoviesThenProfileSetup) == 0);
    static_assert(static_cast<int>(State::FireworksRun) == 0x25);
    static_assert(static_cast<int>(State::BobsBandRun) == 0x2F);
    static_assert(static_cast<int>(State::SpudMazeRun) == 0x3B);
    static_assert(static_cast<int>(State::FireworksReplayChoiceUpdate) == 0x43);

    static_assert(valid_state_value(0));
    static_assert(valid_state_value(0x43));
    static_assert(!valid_state_value(-1));
    static_assert(!valid_state_value(0x44));

    constexpr auto* fireworks = descriptor(0x25);
    static_assert(fireworks != nullptr);
    static_assert(fireworks->handler_address == 0x0042C08B);
    static_assert(fireworks->kind == StateKind::ActivityRun);
    static_assert(fireworks->module == "fireworks");

    constexpr auto* bobs_band = descriptor(0x2E);
    static_assert(bobs_band != nullptr);
    static_assert(bobs_band->handler_address == 0x0042C6AE);
    static_assert(bobs_band->kind == StateKind::ActivityInit);
    static_assert(bobs_band->module == "bobs_band");

    constexpr auto* no_op = descriptor(0x0A);
    static_assert(no_op != nullptr);
    static_assert(no_op->handler_address == 0x0042CD65);
    static_assert(no_op->kind == StateKind::NoOp);

    // The table is indexed directly by the retail state value.
    for (std::size_t i = 0; i < kStates.size(); ++i) {
        assert(static_cast<std::size_t>(kStates[i].state) == i);
        assert(kStates[i].handler_address >= 0x00400000);
    }

    // Shutdown/credits interception precedes every modal and the jump table.
    constexpr auto shutdown = dispatch_step({
        application::ShutdownPhase::CreditsPlaying,
        true,  // options
        true,  // progress
        true,  // yes/no
        true,  // quit
        true,  // leave
        true,  // play again
        11,
        14,
        true,
        0x25,
        0x04,
    });
    static_assert(shutdown.intercept == Intercept::ShutdownOrCredits);
    static_assert(shutdown.unload_generic_ui);
    static_assert(shutdown.release_transition_surface);
    static_assert(shutdown.call_shutdown_callback);
    static_assert(!shutdown.dispatch);

    // Options beats Progress; Progress beats Yes/No and does not pause Bink.
    constexpr auto options = dispatch_step({
        application::ShutdownPhase::Running,
        true,
        true,
        true,
        true,
        true,
        true,
        0,
        0,
        false,
        0x04,
        0,
    });
    static_assert(options.intercept == Intercept::Options);
    static_assert(options.pause_global_movie);

    constexpr auto progress = dispatch_step({
        application::ShutdownPhase::Running,
        false,
        true,
        true,
        true,
        true,
        true,
        0,
        0,
        false,
        0x04,
        0,
    });
    static_assert(progress.intercept == Intercept::ProgressScreen);
    static_assert(!progress.pause_global_movie);

    constexpr auto yes_no = dispatch_step({
        application::ShutdownPhase::Running,
        false,
        false,
        true,
        true,
        true,
        true,
        0,
        0,
        false,
        0x04,
        0,
    });
    static_assert(yes_no.intercept == Intercept::GenericYesNo);
    static_assert(yes_no.pause_global_movie);

    constexpr auto quit = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,true,true,true,
        0,0,false,0x04,0
    });
    static_assert(quit.intercept == Intercept::WholeGameQuit);

    constexpr auto leave = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,true,true,
        0,0,false,0x04,0
    });
    static_assert(leave.intercept == Intercept::LeaveCurrentActivity);

    constexpr auto play_again_overlay = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,true,
        0,0,false,0x04,0
    });
    static_assert(play_again_overlay.intercept == Intercept::PlayAgainOverlay);
    static_assert(play_again_overlay.pause_global_movie);

    // Positive legacy values intercept one frame through the retail RET hook.
    constexpr auto legacy_low = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,false,
        1,0,false,0x04,0
    });
    static_assert(legacy_low.intercept == Intercept::LegacyNoOpArgument0);
    static_assert(legacy_low.legacy_noop_argument &&
                  *legacy_low.legacy_noop_argument == 0);

    constexpr auto legacy_high = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,false,
        10,0,false,0x04,0
    });
    static_assert(legacy_high.intercept == Intercept::LegacyNoOpArgument1);
    static_assert(legacy_high.legacy_noop_argument &&
                  *legacy_high.legacy_noop_argument == 1);

    // Mode 13: unfinished movie consumes the frame. Completion restores the
    // saved state, sets screen mode 2, clears the shared input pulse, and then
    // dispatches that restored state immediately.
    constexpr auto mode13_wait = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,false,
        0,13,false,0x2F,0x04
    });
    static_assert(mode13_wait.intercept == Intercept::GlobalMovieMode13);
    static_assert(mode13_wait.update_global_movie);
    static_assert(!mode13_wait.dispatch);

    constexpr auto mode13_done = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,false,
        0,13,true,0x2F,0x04
    });
    static_assert(mode13_done.intercept == Intercept::StateTable);
    static_assert(mode13_done.current_state == 0x04);
    static_assert(mode13_done.generic_screen_mode == 2);
    static_assert(mode13_done.clear_input_pulse);
    static_assert(mode13_done.dispatch);
    static_assert(mode13_done.dispatch->state == State::ActivitySelectSetup);

    // Mode 14: completion increments the temporarily-decremented flow state.
    // All normal pregame movie returns set generic screen mode 2.
    constexpr auto mode14_done = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,false,
        0,14,true,0x0B,0
    });
    static_assert(mode14_done.intercept == Intercept::StateTable);
    static_assert(mode14_done.current_state == 0x0C);
    static_assert(mode14_done.generic_screen_mode == 2);
    static_assert(mode14_done.clear_input_pulse);
    static_assert(mode14_done.dispatch);
    static_assert(mode14_done.dispatch->state == State::HerdingPregameSetup);

    // The executable has a special screen-mode result when the post-increment
    // state is 0x23: screen index 7 rather than 2.
    constexpr auto mode14_fireworks_special = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,false,
        0,14,true,0x22,0
    });
    static_assert(mode14_fireworks_special.current_state == 0x23);
    static_assert(mode14_fireworks_special.generic_screen_mode == 7);
    static_assert(mode14_fireworks_special.dispatch);
    static_assert(
        mode14_fireworks_special.dispatch->state ==
        State::FireworksPregameUpdate);

    constexpr auto invalid = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,false,
        0,0,false,0x44,0
    });
    static_assert(invalid.intercept == Intercept::InvalidState);
    static_assert(!invalid.dispatch);

    constexpr auto normal = dispatch_step({
        application::ShutdownPhase::Running,
        false,false,false,false,false,false,
        0,0,false,0x3C,0
    });
    static_assert(normal.intercept == Intercept::StateTable);
    static_assert(normal.dispatch);
    static_assert(normal.dispatch->handler_address == 0x0042CAF4);
    static_assert(normal.dispatch->state == State::PlayAgainYesNoSetup);
}
