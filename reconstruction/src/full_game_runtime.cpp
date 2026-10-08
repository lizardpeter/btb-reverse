#include "btb/full_game_runtime.hpp"

#include <utility>

namespace btb::full_game {

void GameRoot::install(
    ActivityId id, std::unique_ptr<ActivityDriver> driver) {

    const auto i = static_cast<std::size_t>(id);
    if (i >= drivers_.size()) {
        return;
    }
    // Do not invalidate an already running instance underneath the active
    // retail state. Replacing its driver requires explicit unload first.
    if (initialized_activity_ == id) {
        return;
    }
    drivers_[i] = std::move(driver);
}

void GameRoot::install_activity_select(
    std::unique_ptr<FrontEndDriver> driver) {
    activity_select_driver_ = std::move(driver);
    activity_select_initialized_ = false;
}

bool GameRoot::install_generic_front_end_pair(
    game_flow::State setup_state,
    std::unique_ptr<FrontEndDriver> driver) {

    const auto* pair = generic_front_end_pair_for_state(setup_state);
    if (!pair || pair->setup != setup_state || !driver) {
        return false;
    }
    const auto key = static_cast<std::size_t>(setup_state);
    if (generic_front_end_initialized_[key] &&
        globals_.dispatcher.current_state ==
            static_cast<int>(pair->update)) {
        return false; // Cannot replace a currently running native menu.
    }
    generic_front_end_drivers_[key] = std::move(driver);
    generic_front_end_initialized_[key] = false;
    return true;
}

bool GameRoot::install_pregame_front_end_pair(
    game_flow::State setup_state,
    std::unique_ptr<FrontEndDriver> driver) {

    const auto* pair = retail_pregame_for_state(setup_state);
    if (!pair || pair->setup != setup_state || !driver) {
        return false;
    }
    const auto key = static_cast<std::size_t>(setup_state);
    if (generic_front_end_initialized_[key] &&
        globals_.dispatcher.current_state ==
            static_cast<int>(pair->update)) {
        return false; // Do not replace a live original pregame UI.
    }
    generic_front_end_drivers_[key] = std::move(driver);
    generic_front_end_initialized_[key] = false;
    return true;
}

bool GameRoot::select_profile(int index) noexcept {
    if (index < 0 ||
        index >= static_cast<int>(progress::kPlayerCount)) {
        return false;
    }
    if (initialized_activity_ && globals_.active_profile != index) {
        return false; // Never redirect live activity progress to new player.
    }
    if (globals_.active_profile != index) {
        activity_select_initialized_ = false;
        generic_front_end_initialized_.fill(false);
    }
    globals_.active_profile = index;
    return true;
}

bool GameRoot::delete_profile(int index) {
    if (index < 0 ||
        index >= static_cast<int>(progress::kPlayerCount) ||
        initialized_activity_) {
        return false;
    }
    const auto i = static_cast<std::size_t>(index);
    profiles::delete_profile_in_memory(
        globals_.profile_metadata, globals_.player_progress, i);
    if (globals_.active_profile == index) {
        globals_.active_profile.reset();
        globals_.finale_gate = {};
        activity_select_initialized_ = false;
        generic_front_end_initialized_.fill(false);
    }
    return true;
}

void GameRoot::set_outer_state(game_flow::State state) noexcept {
    globals_.dispatcher.current_state = static_cast<std::int32_t>(state);
}

bool GameRoot::unload_current(std::string& error) {
    if (!initialized_activity_) {
        return true;
    }
    const auto index = static_cast<std::size_t>(*initialized_activity_);
    if (!drivers_[index]) {
        error = "active activity lost its registered driver";
        return false;
    }
    if (!drivers_[index]->unload(error)) {
        return false;
    }
    initialized_activity_.reset();
    return true;
}

void GameRoot::apply_effects(
    const ActivityFrameOutput& output) noexcept {

    // The original global progress and movie codes belong to the root, not
    // to individual minigame-owned state objects.
    if (globals_.active_profile) {
        auto& record = globals_.player_progress[
            static_cast<std::size_t>(*globals_.active_profile)];
        for (const auto& write : output.progress_writes) {
            const auto index = static_cast<std::size_t>(write.slot);
            if (index < record.values.size()) {
                record.values[index] = write.value;
            }
        }
    }
    if (output.next_saved_state &&
        game_flow::valid_state_value(*output.next_saved_state)) {
        globals_.dispatcher.saved_state = *output.next_saved_state;
    }
    if (output.next_replay_class) {
        globals_.menu.replay_class = *output.next_replay_class;
    }
    if (output.shared_completion_code) {
        globals_.shared_completion_code = *output.shared_completion_code;
    }
    if (output.next_ui_context) {
        globals_.ui_context = *output.next_ui_context;
    }
    if (output.open_progress_screen) {
        globals_.dispatcher.progress_screen_active = true;
    }
    if (output.open_options_overlay) {
        globals_.dispatcher.options_active = true;
    }
    if (output.open_yes_no_confirmation) {
        globals_.dispatcher.generic_yes_no_active = true;
    }
    if (output.yes_no_context) {
        globals_.ui_context = *output.yes_no_context;
    }
    if (output.next_outer_state) {
        globals_.dispatcher.current_state = *output.next_outer_state;
    }
}

void GameRoot::consume_menu_action(
    game_flow::State update,
    GameFrame& result) noexcept {

    if (!result.effects.negative_ui_action) {
        return;
    }
    const auto routed = route_retail_menu_action(
        update, *result.effects.negative_ui_action,
        globals_.menu, globals_.dispatcher.saved_state);
    if (!routed.recognized) {
        return;
    }
    result.menu_action = routed;
    if (routed.requires_source_variant) {
        result.error = "the original Dino selector source-variant globals "
                       "0x51C344/348/34C are not supplied";
        return;
    }
    apply_retail_menu_action(globals_.menu, update, routed);
    // Native also sets the global 0x51C300 play-again latch to zero.
    if (routed.clear_replay_active_latch) {
        globals_.menu.replay_active_latch = false;
    }
    if (routed.next_state) {
        globals_.dispatcher.current_state =
            static_cast<std::int32_t>(*routed.next_state);
        result.state_changed = true;
    }
}

void GameRoot::consume_pregame_action(
    game_flow::State update,
    GameFrame& result) noexcept {

    if (!result.effects.negative_ui_action) {
        return;
    }
    const auto choice = route_retail_pregame_action(
        update,*result.effects.negative_ui_action);
    if (!choice.recognized) {
        return;
    }
    result.pregame_action = choice;
    if (choice.difficulty) {
        globals_.menu.source_variant = *choice.difficulty;
        globals_.menu.variant_selection_origin = update;
        if (choice.persistent_herding_difficulty) {
            globals_.retained_herding_difficulty = *choice.difficulty;
        }
    }
    if (choice.skate_immediate_start) {
        globals_.spud_skate_start_latch = true;
    }
    if (choice.next_state) {
        globals_.dispatcher.current_state =
            static_cast<std::int32_t>(*choice.next_state);
        result.state_changed = true;
    }
}

GameFrame GameRoot::advance(const ActivityFrameInput& input) {
    GameFrame result;
    const auto dispatch = game_flow::dispatch_step(globals_.dispatcher);
    result.dispatch = dispatch;

    // Native global movie modes 13/14 mutate the state/working UI mode and
    // clear the shared pulse before executing the restored state on this very
    // frame. Preserve that ordering even when that state is not yet hosted.
    globals_.dispatcher.current_state = dispatch.current_state;
    globals_.dispatcher.generic_screen_mode =
        dispatch.generic_screen_mode;
    if (dispatch.clear_input_pulse) {
        globals_.input_pulse = false;
        result.clear_input_pulse = true;
    }

    if (dispatch.intercept == game_flow::Intercept::InvalidState) {
        result.kind = FrameKind::InvalidState;
        return result;
    }
    if (!dispatch.dispatch) {
        result.kind = FrameKind::Intercept;
        return result;
    }

    const auto& state = *dispatch.dispatch;
    if (state.kind == game_flow::StateKind::NoOp) {
        result.kind = FrameKind::StateNoOp;
        return result;
    }

    if (state.state == game_flow::State::ActivitySelectSetup ||
        state.state == game_flow::State::ActivitySelectUpdate) {

        if (!activity_select_driver_) {
            result.kind = FrameKind::FrontEndRequiresAdapter;
            return result;
        }
        if (!globals_.active_profile) {
            result.kind = FrameKind::InactiveProfile;
            return result;
        }
        if (state.state == game_flow::State::ActivitySelectSetup) {
            activity_select_initialized_ = false;
            auto& record = globals_.player_progress[
                static_cast<std::size_t>(*globals_.active_profile)];
            if (!activity_select_driver_->initialize(
                    record, globals_.finale_gate, result.error)) {
                result.kind = FrameKind::FrontEndFailed;
                return result;
            }
            activity_select_initialized_ = true;
            globals_.dispatcher.current_state =
                static_cast<int>(game_flow::State::ActivitySelectUpdate);
            result.kind = FrameKind::FrontEndInitialized;
            result.state_changed = true;
            return result;
        }

        if (!activity_select_initialized_) {
            result.kind = FrameKind::FrontEndFailed;
            result.error = "Activity Select state 0x05 entered without setup 0x04";
            return result;
        }
        const auto& record = globals_.player_progress[
            static_cast<std::size_t>(*globals_.active_profile)];
        globals_.finale_gate = progress::update_finale_gate_from_progress(
            globals_.finale_gate, record);
        activity_select_driver_->synchronize_finale_gate(
            globals_.finale_gate);
        result.effects = activity_select_driver_->advance(input);
        apply_effects(result.effects);
        result.state_changed = result.effects.next_outer_state.has_value();
        consume_menu_action(state.state, result);
        result.kind = FrameKind::FrontEndUpdated;
        return result;
    }

    // The original same generic UI implementation serves seven chooser/
    // replay pairs AND ten pregame/instruction pairs. Their negative actions
    // must be interpreted by the matching native outer state, never as
    // untyped negative 68-entry jump-table offsets.
    const auto* menu = generic_front_end_pair_for_state(state.state);
    const auto* pregame = retail_pregame_for_state(state.state);
    if (menu || pregame) {
        const auto setup_state = menu ? menu->setup : pregame->setup;
        const auto update_state = menu ? menu->update : pregame->update;
        const auto key = static_cast<std::size_t>(setup_state);
        auto* driver = generic_front_end_drivers_[key].get();
        if (!driver) {
            result.kind = FrameKind::FrontEndRequiresAdapter;
            return result;
        }
        if (!globals_.active_profile) {
            result.kind = FrameKind::InactiveProfile;
            return result;
        }
        if (state.state == setup_state) {
            generic_front_end_initialized_[key] = false;
            auto& record = globals_.player_progress[
                static_cast<std::size_t>(*globals_.active_profile)];
            if (!driver->initialize(
                    record,globals_.finale_gate,result.error)) {
                result.kind = FrameKind::FrontEndFailed;
                return result;
            }
            generic_front_end_initialized_[key] = true;
            globals_.dispatcher.current_state =
                static_cast<int>(update_state);
            if (pregame) {
                // Native setup calls OpenWalkthroughMovie @ 0x4281D0 and
                // registers original instruction surfaces. The real Bink
                // device must be connected before claiming presentation.
                result.requires_original_walkthrough_host = true;
                if (pregame->setup ==
                    game_flow::State::HerdingPregameSetup) {
                    globals_.menu.source_variant =
                        globals_.retained_herding_difficulty;
                }
            }
            result.kind = FrameKind::FrontEndInitialized;
            result.state_changed = true;
            return result;
        }
        if (!generic_front_end_initialized_[key]) {
            result.kind = FrameKind::FrontEndFailed;
            result.error = "generic UI update entered before original setup";
            return result;
        }
        driver->synchronize_finale_gate(globals_.finale_gate);
        result.effects = driver->advance(input);
        apply_effects(result.effects);
        result.state_changed =
            result.effects.next_outer_state.has_value();
        if (pregame) {
            consume_pregame_action(state.state,result);
            result.requires_original_walkthrough_host = true;
        } else {
            consume_menu_action(state.state,result);
        }
        if (result.menu_action &&
            result.menu_action->requires_source_variant) {
            // No fabricated 0x51C344/348/34C dino variant values.
            // Retail initializes these in its pregame bookkeeping; the
            // host must supply the actual values before selecting species.
            result.kind = FrameKind::FrontEndFailed;
            if (result.error.empty()) {
                result.error = "native Dino chooser source-variant globals are not initialized";
            }
            return result;
        }
        result.kind = FrameKind::FrontEndUpdated;
        return result;
    }

    if (state.kind != game_flow::StateKind::ActivityInit &&
        state.kind != game_flow::StateKind::ActivityRun) {
        // Front-end/pregame movies, chooser, and replay states need their
        // authentic frontend adapters; never jump over them to start a game.
        result.kind = FrameKind::FrontEndRequiresAdapter;
        return result;
    }

    const auto activity = activity_for_state(state.state);
    if (!activity) {
        result.kind = FrameKind::ActivityFailed;
        result.error = "activity state has no native 68-state registry owner";
        return result;
    }
    result.activity = activity->id;
    const auto index = static_cast<std::size_t>(activity->id);
    if (!drivers_[index]) {
        result.kind = FrameKind::ActivityRequiresAdapter;
        return result;
    }
    if (!globals_.active_profile) {
        result.kind = FrameKind::InactiveProfile;
        return result;
    }

    if (state.kind == game_flow::StateKind::ActivityInit) {
        if (initialized_activity_ && initialized_activity_ != activity->id) {
            if (!unload_current(result.error)) {
                result.kind = FrameKind::ActivityFailed;
                return result;
            }
        }
        // The retail pregame/chooser writes selected variant and conductor
        // globals before the corresponding original initializer executes.
        // Supply those choices to the activity driver before loading data.
        if (initialized_activity_ != activity->id) {
            drivers_[index]->configure_menu_state(globals_.menu);
        }
        if (initialized_activity_ != activity->id &&
            !drivers_[index]->initialize(*globals_.active_profile,
                                         result.error)) {
            result.kind = FrameKind::ActivityFailed;
            return result;
        }
        initialized_activity_ = activity->id;
        globals_.dispatcher.current_state =
            static_cast<std::int32_t>(activity->run);
        result.state_changed = true;
        result.kind = FrameKind::ActivityInitialized;
        return result;
    }

    if (initialized_activity_ != activity->id) {
        result.kind = FrameKind::ActivityFailed;
        result.error = "native run state entered without successful initializer";
        return result;
    }

    result.effects = drivers_[index]->advance(input);
    result.kind = FrameKind::ActivityUpdated;

    if (result.effects.save_and_unload &&
        result.effects.next_outer_state ==
            static_cast<int>(game_flow::State::PlayAgainYesNoSetup)) {
        // Every native activity exiting via PreparePlayAgainTransition
        // executes the same sound/latch setup. Preserve the original 573
        // managed voice, rather than replaying a made-up sound on screen 9.
        const auto plan = prepare_retail_play_again_transition();
        result.replay_preparation = plan;
        globals_.menu.replay_active_latch =
            plan.set_replay_active_latch;
        if (plan.stop_all_managed_sounds) {
            result.effects.audio.push_back({
                AudioOperation::StopManagedSounds
            });
        }
        result.effects.audio.push_back({
            AudioOperation::ManagedSoundId, {},
            plan.voice_sound_id,
            plan.voice_priority,
            plan.voice_arbitration_class,
            plan.voice_input_interruptible
        });
    }

    if (result.effects.save_and_unload) {
        // Native state-10 teardown persists the activity before the outer
        // dispatcher switches to Play Again. Do not commit the next state if
        // the activity's persistence failed.
        if (!unload_current(result.error)) {
            result.kind = FrameKind::ActivityFailed;
            return result;
        }
    }
    apply_effects(result.effects);
    result.state_changed = result.effects.next_outer_state.has_value();

    if (result.effects.save_and_unload &&
        !profile_directory_.empty()) {
        if (!save_profile_files(profile_directory_, result.error)) {
            // Filesystem errors are not swallowed; the host can surface or
            // retry the save. The native activity was already unloaded and
            // its transition remains visible, rather than being re-run.
            result.kind = FrameKind::ActivityFailed;
        } else {
            result.profile_saved = true;
        }
    }
    return result;
}

} // namespace btb::full_game
