#include "btb/full_game_runtime.hpp"

#include <algorithm>
#include <utility>
#include <exception>
#include <fstream>

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

void GameRoot::install_profile_screen(
    std::unique_ptr<NativeProfileScreenDriver> driver) {
    profile_screen_driver_=std::move(driver);
    profile_screen_state_={};
    profile_screen_state_.outer_state=
        static_cast<int>(game_flow::State::PlayerProfileAndNameEntry);
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
        initialized_activity_ || name_entry_) {
        return false;
    }
    const auto i = static_cast<std::size_t>(index);
    // 0042E110 clears all five per-player activity files with fopen("w").
    // Otherwise a newly created profile in this slot inherits the deleted
    // player's Park Designer, Fireworks and three compositions.
    if (!profile_directory_.empty()) {
        for (const auto& filename : profiles::activity_files_for_profile(i)) {
            std::ofstream out(profile_directory_ / filename,
                              std::ios::binary | std::ios::trunc);
            if (!out) return false;
            out.close();
            if (!out) return false;
        }
    }
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
    if (state == game_flow::State::StartupMoviesThenProfileSetup) {
        startup_video_index_ = 0;
        startup_video_opened_ = false;
        profile_intro_music_pending_ = false;
    }
}

bool GameRoot::configure_startup_video_sequence(
    std::span<const OriginalVideoSequenceEntry> entries,
    std::string& error) {
    if (entries.empty() || entries.size() > kRetailVideoSequenceCapacity) {
        error = "videoseq.txt must contain between one and ten original movies";
        return false;
    }
    for (const auto& entry : entries) {
        if (entry.path.empty() || entry.path.size() >= 0x104U) {
            error = "videoseq.txt contains an invalid original movie path";
            return false;
        }
    }
    startup_video_sequence_.assign(entries.begin(), entries.end());
    startup_video_index_ = 0;
    startup_video_opened_ = false;
    profile_intro_music_pending_ = false;
    error.clear();
    return true;
}

void GameRoot::skip_startup_video_sequence() noexcept {
    if (globals_.dispatcher.current_state !=
        static_cast<int>(game_flow::State::StartupMoviesThenProfileSetup)) {
        return;
    }
    startup_video_index_ = startup_video_sequence_.size();
    startup_video_opened_ = false;
    profile_intro_music_pending_ = true;
    set_outer_state(game_flow::State::PlayerProfileAndNameEntry);
}

bool GameRoot::configure_completion_movies(std::istream& source,
                                            std::string& error) {
    // 00407870 loads up to twelve 0x100-byte path slots at 0048DAF0.
    // Keep the file order; never infer a filename from an activity name.
    std::vector<std::string> candidate;
    std::string path;
    while(source >> path) {
        if(path.size()>=0x100U || candidate.size()>=12) {
            error="completedmovie.txt exceeds original path slot or table capacity";
            return false;
        }
        candidate.push_back(std::move(path));
    }
    if(source.bad() || !source.eof() || candidate.empty()) {
        error="completedmovie.txt is empty or unreadable";
        return false;
    }
    completion_movies_=std::move(candidate);
    error.clear();
    return true;
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
    const ActivityFrameOutput& output) {

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
        progress_intro_pending_=true;
        progress_back_hover_=false;
        progress_help_hover_=false;
    }
    if (output.open_options_overlay) {
        globals_.dispatcher.options_active = true;
    }
    if (output.open_yes_no_confirmation || output.open_leave_activity_confirmation) {
        modal_background_=output.draws;
        modal_hover_=-1;modal_armed_=-1;
        pending_modal_choice_.reset();
        globals_.dispatcher.generic_yes_no_active=output.open_yes_no_confirmation;
        globals_.dispatcher.leave_activity_active=output.open_leave_activity_confirmation;
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
    GameFrame& result) {

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
    // 0042C08B retains the initialized Fireworks activity across the
    // two replay screens. 0042CA34 chooses internal state 0 or 8,
    // never a new activity initializer. No ends the retained activity.
    if(routed.restart_fireworks_editor||routed.restart_fireworks_view) {
        if(initialized_activity_!=ActivityId::Fireworks||
           !drivers_[static_cast<std::size_t>(ActivityId::Fireworks)]->resume_fireworks_replay(
               routed.restart_fireworks_view,result.effects,result.error)) {
            if(result.error.empty())result.error="Fireworks replay lost its retained activity";
            return;
        }
    }
    if(update==game_flow::State::PlayAgainYesNoUpdate&&
       routed.next_state==game_flow::State::SharedMovieTransition&&
       initialized_activity_==ActivityId::Fireworks) {
        if(!unload_current(result.error))return;
        result.effects.save_and_unload=true;
        if(!profile_directory_.empty()) {
            if(!save_profile_files(profile_directory_,result.error))return;
            result.profile_saved=true;
        }
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
        using S=game_flow::State;
        switch(update) {
        case S::DinoPregameUpdate:
            if(globals_.menu.selected_subgame>=0&&globals_.menu.selected_subgame<3)
                globals_.menu.dino_source_variants[static_cast<std::size_t>(
                    globals_.menu.selected_subgame)]=*choice.difficulty;
            break;
        case S::SpudMazePregameUpdate: globals_.menu.spud_maze_difficulty=*choice.difficulty;break;
        case S::GolfPregameUpdate: globals_.menu.golf_difficulty=*choice.difficulty;break;
        case S::MazePregameUpdate: globals_.menu.maze_difficulty=*choice.difficulty;break;
        case S::SquirrelPregameUpdate: globals_.menu.squirrel_difficulty=*choice.difficulty;break;
        default:break;
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

bool GameRoot::configure_contextual_help(std::istream& source,std::string& error) {
    try {
        help::State candidate{};
        help::load_original_help_info(candidate,source);
        contextual_help_=candidate;
        help_configured_=true;
        help_background_.clear();
        help_hover_=false;help_exit_delay_=0;
        error.clear();return true;
    } catch(const std::exception& e){error=e.what();return false;}
}

GameFrame GameRoot::advance(const ActivityFrameInput& sample) {
    // 00401700 operates on a captured surface and freezes gameplay.
    // Draw-command capture is equivalent for bitmap-only activity scenes.
    // Movie-backed states and dynamic object-specific regions still require
    // their separate native surface/geometry providers.
    if(contextual_help_.active){
        GameFrame result;
        result.kind=FrameKind::Intercept;
        result.dispatch.intercept=game_flow::Intercept::ContextualHelp;
        result.dispatch.current_state=globals_.dispatcher.current_state;
        result.dispatch.generic_screen_mode=globals_.dispatcher.generic_screen_mode;
        result.effects.draws=help_background_;
        if(!sample.pointer_position_valid)return result;
        const int context=contextual_help_.context;
        const auto voice=[&](int id){
            result.effects.audio.push_back({AudioOperation::StopManagedSounds});
            result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},id,50,2});
        };
        if(context>=0&&context<static_cast<int>(contextual_help_.regions.size())){
            const auto count=contextual_help_.counts[context];
            for(int i=0;i<count;++i){
                const auto& region=contextual_help_.regions[context][i];
                if(!help::cursor_inside_region_strict(region,sample.pointer_x,sample.pointer_y))continue;
                if(i==0&&sample.pointer_release_pulse){
                    contextual_help_.active=0;contextual_help_.last_region=0;
                    help_exit_delay_=help::kHelpExitCursorDelay;
                    help_hover_=false;
                    result.effects.audio.push_back({AudioOperation::StopManagedSounds});
                    help_background_.clear();
                    return result;
                }
                if(i==contextual_help_.last_region)continue;
                if(context==static_cast<int>(help::Context::ParkDesigner)&&initialized_activity_&&
                   !drivers_[static_cast<std::size_t>(*initialized_activity_)]->static_help_region_allowed(i))
                    continue;
                voice(region.sound_id);
                contextual_help_.last_region=i;
            }
        }
        result.effects.draws.push_back({"Data/ui/help cursor.bmp",
            sample.pointer_x,sample.pointer_y,{},true});
        return result;
    }
    if(help_exit_delay_>0)--help_exit_delay_;
    ActivityFrameInput input=sample;
    // 004282C0 only enables help with a normal cursor and releases the
    // button before entering. Exclude the movie-backed Spud Skate scene
    // until the native device has a captured-surface overlay provider.
    const auto help_activity=activity_for_state(static_cast<game_flow::State>(globals_.dispatcher.current_state));
    const bool can_help=help_configured_&&initialized_activity_&&help_activity&&
        help_activity->id==*initialized_activity_&&
        *initialized_activity_!=ActivityId::SpudSkate&&
        sample.pointer_position_valid&&help_exit_delay_==0&&
        !globals_.dispatcher.generic_yes_no_active&&!globals_.dispatcher.leave_activity_active&&
        !globals_.dispatcher.options_active&&!globals_.dispatcher.progress_screen_active&&
        drivers_[static_cast<std::size_t>(*initialized_activity_)]->shared_help_available();
    const bool over=can_help&&sample.pointer_x>20&&sample.pointer_x<65&&
        sample.pointer_y>420&&sample.pointer_y<465;
    if(over){input.click_pulse=false;input.pointer_release_pulse=false;}
    auto result=advance_impl(input);
    if(over&&result.kind==FrameKind::ActivityUpdated&&!result.state_changed&&
       !result.effects.open_leave_activity_confirmation&&!result.effects.open_yes_no_confirmation&&
       !result.effects.retain_original_global_movie_pixels&&!result.effects.draws.empty()){
        result.effects.draws.push_back({sample.pointer_button_held?
            "Data/ui/helpreddep.bmp":"Data/ui/helpred.bmp",14,418,{},true});
        if(!help_hover_)result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},455,50,2});
        if(sample.pointer_release_pulse){
            const auto decision=help::enter_decision(globals_.dispatcher.current_state,false,false);
            if(decision.active){
                help_background_=result.effects.draws;
                contextual_help_.active=1;contextual_help_.context=decision.context;
                contextual_help_.last_region=0;
                result.effects.audio.push_back({AudioOperation::StopManagedSounds});
                result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},help::kHelpIntroSoundId,50,2});
            }
        }
    }
    help_hover_=over;
    return result;
}

GameFrame GameRoot::advance_impl(const ActivityFrameInput& sample) {
    GameFrame result;
    ActivityFrameInput input=sample;
    if(pending_modal_choice_){
        input.confirmation_resolved=true;input.confirmation_yes=*pending_modal_choice_;
        input.click_pulse=false;input.pointer_release_pulse=false;
        pending_modal_choice_.reset();
    }
    // The original shared Yes/No screen owns the modal while unresolved.
    // Once the physical UI reports a completed choice, release that overlay
    // before 68-state dispatch and deliver the choice to the activity on
    // this same frame. Otherwise every subsequent frame is intercepted and
    // the activity never receives its confirmed result.
    if (input.confirmation_resolved) {
        globals_.dispatcher.generic_yes_no_active = false;
    }
    const auto dispatch = game_flow::dispatch_step(globals_.dispatcher);
    result.dispatch = dispatch;

    // Native global movie modes 13/14 mutate the state/working UI mode and
    // clear the shared pulse before executing the restored state on this very
    // frame. Preserve that ordering even when that state is not yet hosted.
    globals_.dispatcher.current_state = dispatch.current_state;
    globals_.dispatcher.generic_screen_mode =
        dispatch.generic_screen_mode;
    ActivityFrameInput effective_input = input;
    if (dispatch.clear_input_pulse) {
        globals_.input_pulse = false;
        result.clear_input_pulse = true;
        // When native global Bink mode 13/14 finishes, retail clears
        // 0x4FBE54 BEFORE the restored state executes on this same frame.
        // Suppress the host's matching click event as well; otherwise a
        // movie-dismissal click can incorrectly activate a menu choice.
        effective_input.click_pulse = false;
    }

    if (dispatch.intercept == game_flow::Intercept::InvalidState) {
        result.kind = FrameKind::InvalidState;
        return result;
    }
    if (!dispatch.dispatch) {
        result.kind = FrameKind::Intercept;
        // Original 00429C90/00429FE0 run before the outer state table.
        // Preserve explicit fixture-provided confirmation input; the live
        // Win32 path supplies physical pointer-down/release samples.
        // Original 00427C30, UpdateProgressScreen. This overlay does not
        // change the outer state or unload the Activity Select driver.
        if(dispatch.intercept==game_flow::Intercept::ProgressScreen){
            if(!globals_.active_profile){
                result.kind=FrameKind::InactiveProfile;
                result.error="original progress screen requires an active profile";
                return result;
            }
            if(progress_intro_pending_&&!input.managed_sound_playing){
                progress_intro_pending_=false;
                result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},954,50,2,true});
            }
            result.effects.draws.push_back({"Data/ui/Progress-screen.bmp",0,0,{},false});
            const auto& record=globals_.player_progress[
                static_cast<std::size_t>(*globals_.active_profile)];
            for(std::size_t slot=progress::kRetailProgressFirst;
                slot<=progress::kRetailProgressLast;++slot){
                if(record.values[slot]<=0)continue;
                const auto pos=progress::progress_star_position(slot);
                result.effects.draws.push_back({"Data/ui/star.bmp",pos.x,pos.y,{},true});
            }
            if(!input.pointer_position_valid)return result;
            // Reuse recovered shared Back/Help hit regions. Star-hover
            // rectangles are not yet recovered; do not invent them.
            const bool back=input.pointer_x>572&&input.pointer_x<617&&
                input.pointer_y>422&&input.pointer_y<467;
            const bool help_button=input.pointer_x>20&&input.pointer_x<65&&
                input.pointer_y>420&&input.pointer_y<465;
            if(back){
                result.effects.draws.push_back({input.pointer_button_held?
                    "Data/ui/backreddep.bmp":"Data/ui/backred.bmp",574,418,{},true});
                if(!progress_back_hover_)
                    result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},456,50,2});
                if(input.pointer_release_pulse){
                    globals_.dispatcher.progress_screen_active=false;
                    progress_intro_pending_=false;
                }
            }else if(help_button){
                result.effects.draws.push_back({input.pointer_button_held?
                    "Data/ui/helpreddep.bmp":"Data/ui/helpred.bmp",14,418,{},true});
                if(!progress_help_hover_)
                    result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},455,50,2});
                if(help_configured_&&help_exit_delay_==0&&input.pointer_release_pulse){
                    const auto decision=help::enter_decision(
                        globals_.dispatcher.current_state,false,true);
                    if(decision.active){
                        help_background_=result.effects.draws;
                        contextual_help_.active=1;
                        contextual_help_.context=decision.context;
                        contextual_help_.last_region=0;
                        result.effects.audio.push_back({AudioOperation::StopManagedSounds});
                        result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},help::kHelpIntroSoundId,50,2});
                    }
                }
            }
            progress_back_hover_=back;
            progress_help_hover_=help_button;
            return result;
        }
        const bool generic=dispatch.intercept==game_flow::Intercept::GenericYesNo;
        const bool leave=dispatch.intercept==game_flow::Intercept::LeaveCurrentActivity;
        if((generic||leave)&&input.pointer_position_valid){
            result.effects.draws=modal_background_;
            if(generic&&initialized_activity_==ActivityId::ParkDesigner){
                ActivityFrameOutput scene{};
                std::string error;
                auto& driver=drivers_[static_cast<std::size_t>(*initialized_activity_)];
                if(driver->redraw_confirmation_background(scene,error))result.effects.draws=std::move(scene.draws);
                else if(!error.empty()){result.kind=FrameKind::ActivityFailed;result.error=std::move(error);return result;}
            }
            const char* backdrop=nullptr;
            if(leave)backdrop="Data/ui/main/quitmain.bmp";
            // Adapter context numbers are not unique. Select by activity.
            else if(initialized_activity_==ActivityId::ParkDesigner)backdrop="Data/ui/Deleteallobjects.bmp";
            else if(initialized_activity_==ActivityId::BobsBand)backdrop="Data/ui/Deletebricks.bmp";
            else if(initialized_activity_==ActivityId::Fireworks)backdrop="Data/ui/DeleteFireworks.bmp";
            else if(globals_.dispatcher.current_state==1)backdrop="Data/ui/deletegame.bmp";
            if(!backdrop){result.kind=FrameKind::ActivityFailed;result.error="unmapped original confirmation context";return result;}
            result.effects.draws.push_back({backdrop,0,0,{},true});
            const int x=input.pointer_x,y=input.pointer_y;
            int hit=-1;
            if(leave){
                if(y>260&&y<316){
                    if(x>220&&x<305)hit=0;
                    if(x>334&&x<420)hit=1;
                }
            }else if(y>=260&&y<=312){
                if(x>=220&&x<=305)hit=0;
                if(x>=334&&x<=417)hit=1;
            }
            if(input.click_pulse)modal_armed_=hit;
            if(hit>=0){
                const bool pressed=input.pointer_button_held&&modal_armed_==hit;
                const char* asset=hit==0?(pressed?"Data/ui/playagain/payesdep.bmp":"Data/ui/playagain/payessel.bmp"):
                    (pressed?"Data/ui/playagain/panodep.bmp":"Data/ui/playagain/panosel.bmp");
                result.effects.draws.push_back({asset,hit==0?220:334,260,{},true});
                if(leave&&hit!=modal_hover_)
                    result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},453+hit,50,2});
            }
            if(input.pointer_release_pulse){
                const bool choice=hit>=0&&(leave||modal_armed_==hit);
                modal_armed_=-1;
                if(choice){
                    if(generic){
                        pending_modal_choice_=(hit==0);
                    }else{
                        globals_.dispatcher.leave_activity_active=false;
                        if(hit==0&&initialized_activity_){
                            auto& driver=drivers_[static_cast<std::size_t>(*initialized_activity_)];
                            if(!driver->request_confirmed_leave())pending_confirmed_leave_=true;
                        }
                    }
                }
            }
            modal_hover_=hit;
        }
        return result;
    }

    const auto& state = *dispatch.dispatch;
    if (state.state == game_flow::State::StartupMoviesThenProfileSetup) {
        if (startup_video_sequence_.empty()) {
            result.kind = FrameKind::FrontEndFailed;
            result.error = "original loaddata/videoseq.txt was not configured";
            return result;
        }
        if (startup_video_index_ >= startup_video_sequence_.size()) {
            result.kind = FrameKind::FrontEndFailed;
            result.error = "original startup movie sequence index is invalid";
            return result;
        }
        const auto& current_movie =
            startup_video_sequence_[startup_video_index_];
        const bool movie_skip_requested =
            effective_input.original_global_movie_skip_pulse ||
            effective_input.click_pulse;
        if (!startup_video_opened_) {
            result.original_startup_sequence_movie_path =
                current_movie.path;
            startup_video_opened_ = true;
        } else if (effective_input.original_global_movie_finished ||
                   (movie_skip_requested && current_movie.input_mode != 1)) {
            if (!effective_input.original_global_movie_finished &&
                movie_skip_requested && current_movie.input_mode != 1) {
                result.close_original_global_movie = true;
            }
            ++startup_video_index_;
            startup_video_opened_ = false;
            if (startup_video_index_ >= startup_video_sequence_.size()) {
                // Retail case 0 loads name signs and starts entername.wav
                // immediately after UpdateStartupVideoSequence completes.
                set_outer_state(game_flow::State::PlayerProfileAndNameEntry);
                result.effects.audio.push_back({
                    AudioOperation::StartBackingTrack,
                    "Data/music/entername.wav"
                });
                result.state_changed = true;
            }
        }
        // Global Bink owns the display surface for every startup sequence
        // frame. Never clear it with an empty profile/UI draw list.
        result.effects.retain_original_global_movie_pixels = true;
        result.kind = FrameKind::FrontEndUpdated;
        return result;
    }
    if (state.state == game_flow::State::SharedMovieTransition) {
        // RunMainGameFlow case 0x40 (0042CCEA). The movie index and
        // return state are NOT the saved replay initializer at 0051B418.
        const int code=globals_.shared_completion_code;
        const int destination=globals_.shared_movie_return_state==-1
            ? static_cast<int>(game_flow::State::ActivitySelectSetup)
            : globals_.shared_movie_return_state;
        if(!game_flow::valid_state_value(destination) ||
           destination==static_cast<int>(game_flow::State::SharedMovieTransition)) {
            result.kind=FrameKind::FrontEndFailed;
            result.error="invalid original shared-movie return state";
            return result;
        }
        if(code < -2 || (code==-2 && !shared_movie_opened_)) {
            result.kind=FrameKind::FrontEndFailed;
            result.error="invalid original shared-movie lifecycle";
            return result;
        }
        const auto finish=[&] {
            globals_.shared_completion_code=-1;
            globals_.shared_movie_return_state=-1;
            shared_movie_opened_=false;
            set_outer_state(static_cast<game_flow::State>(destination));
            result.state_changed=true;
        };
        if(code==-1) {
            finish();
        } else if(effective_input.skip_shared_completion_video) {
            result.close_original_global_movie=shared_movie_opened_;
            finish();
        } else if(code>=0) {
            if(static_cast<std::size_t>(code)>=completion_movies_.size() ||
               completion_movies_[static_cast<std::size_t>(code)]=="NULL") {
                result.kind=FrameKind::FrontEndFailed;
                result.error="original completedmovie.txt index unavailable";
                return result;
            }
            result.original_completion_movie_path=
                completion_movies_[static_cast<std::size_t>(code)];
            globals_.shared_completion_code=-2;
            shared_movie_opened_=true;
        } else if(effective_input.original_global_movie_finished) {
            finish();
        }
        // The original handler submits no ordinary bitmap draw here.
        result.effects.retain_original_global_movie_pixels=true;
        result.kind=FrameKind::FrontEndUpdated;
        return result;
    }
    if (state.state == game_flow::State::SceneSetMovieTransition) {
        // Original 0x0042A708 calls UpdateSceneSetMovie (0x0042A240).
        // Its first frame opens the authentic Data/movies/sceneset.bik.
        // Subsequent frames remain in state 0x41 until the real decoder
        // reports completion; then the outer state advances to 0x04.
        if (!globals_.active_profile) {
            result.kind=FrameKind::InactiveProfile;
            result.error="scene-setting movie requires an active profile";
            return result;
        }
        if (!scene_set_movie_opened_) {
            result.original_scene_set_movie_path="Data/movies/sceneset.bik";
            scene_set_movie_opened_=true;
        } else if (effective_input.original_global_movie_finished ||
                   effective_input.original_global_movie_skip_pulse ||
                   effective_input.click_pulse) {
            if (!effective_input.original_global_movie_finished)
                result.close_original_global_movie=true;
            scene_set_movie_opened_=false;
            set_outer_state(game_flow::State::ActivitySelectSetup);
            result.state_changed=true;
        }
        // The Bink player owns the DD7 backbuffer until the next real
        // activity-select frame. Drawing an empty frame erases its pixels.
        result.effects.retain_original_global_movie_pixels=true;
        result.kind=FrameKind::FrontEndUpdated;
        return result;
    }
    if (state.state == game_flow::State::PlayerProfileAndNameEntry &&
        profile_intro_music_pending_) {
        result.effects.audio.push_back({
            AudioOperation::StartBackingTrack,
            "Data/music/entername.wav"
        });
        profile_intro_music_pending_ = false;
    }
    if (state.state == game_flow::State::PlayerProfileAndNameEntry && name_entry_) {
        return advance_profile_name_entry(effective_input,std::move(result));
    }
    if (state.state == game_flow::State::PlayerProfileAndNameEntry) {
        if (!profile_screen_driver_) {
            result.kind=FrameKind::FrontEndRequiresAdapter;
            return result;
        }
        if(profile_screen_state_.outer_state!=static_cast<int>(state.state))
            profile_screen_state_={};
        if (!profile_screen_driver_->begin_frame(
                effective_input,result.effects,result.error)) {
            result.kind=FrameKind::FrontEndFailed;
            return result;
        }
        // The source 0x0042E110 deletes the selected player's progress
        // only after the real shared Yes/No confirmation has resolved.
        if (effective_input.confirmation_resolved &&
            profile_screen_state_.delete_overlay_active) {
            profile_screen_state_.deletion_confirmed=
                effective_input.confirmation_yes;
            profile_screen_state_.delete_overlay_active=false;
            profile_screen_state_.delete_overlay_setup=false;
        }
        const auto old_state=globals_.dispatcher.current_state;
        const bool confirmed_profile_delete=
            profile_screen_state_.deletion_confirmed;
        try {
            update_original_profile_screen(
                profile_screen_state_,
                {.x=effective_input.pointer_x,
                 .y=effective_input.pointer_y,
                 .down=effective_input.pointer_button_held,
                 .click=effective_input.click_pulse,
                 .introduction_gate=false},
                *profile_screen_driver_);
        } catch (const std::exception& e) {
            result.kind=FrameKind::FrontEndFailed;
            result.error=e.what();
            return result;
        }
        if (!profile_screen_driver_->end_frame(result.error)) {
            result.kind=FrameKind::FrontEndFailed;
            return result;
        }
        // Retail profile deletion truncates the activity files through
        // 0042E110; the shared metadata and visible progress must also be
        // flushed immediately, not left only in memory until a later game.
        if(confirmed_profile_delete&&globals_.active_profile==profile_screen_state_.deletion_profile){
            globals_.active_profile.reset();
            globals_.finale_gate={};
            activity_select_initialized_=false;
            generic_front_end_initialized_.fill(false);
        }
        if ((confirmed_profile_delete || globals_.dispatcher.current_state!=old_state) &&
            !profile_directory_.empty()) {
            if (!save_profile_files(profile_directory_,result.error)) {
                result.kind=FrameKind::FrontEndFailed;
                return result;
            }
            result.profile_saved=true;
        }
        if (profile_screen_state_.creation_profile>=0 &&
            profile_screen_state_.name_entry_phase==1) {
            const int created=profile_screen_state_.creation_profile;
            if (!begin_profile_name_entry(
                    created,profile_screen_driver_->blue_font_rectangles(),
                    profile_screen_driver_->badge_rectangles())) {
                result.kind=FrameKind::FrontEndFailed;
                result.error="original profile name popup cannot use "
                             "installed retail font/badge geometry";
                return result;
            }
            profile_screen_state_.creation_profile=-1;
            profile_screen_state_.name_entry_phase=0;
        }
        // The selector's source routine mutates the same live GameRoot
        // profile record. No temporary UI copy or fake profile is used.
        apply_effects(result.effects);
        result.state_changed=globals_.dispatcher.current_state!=old_state;
        result.kind=FrameKind::FrontEndUpdated;
        return result;
    }
    if (state.kind == game_flow::StateKind::NoOp) {
        result.kind = FrameKind::StateNoOp;
        return result;
    }

    // 0042A7E7 checks the pending completion movie before loading the
    // activity-select background. Park Designer and Fireworks return to 4.
    if(state.state==game_flow::State::ActivitySelectSetup&&
       globals_.shared_completion_code!=-1){
        set_outer_state(game_flow::State::SharedMovieTransition);
        result.kind=FrameKind::FrontEndUpdated;
        result.state_changed=true;
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
        result.effects = activity_select_driver_->advance(effective_input);
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
            if (pregame) {
                // Restore each activity's original retained difficulty
                // BEFORE configuring the highlight and eventual game driver.
                using S=game_flow::State;
                auto& menu=globals_.menu;
                switch(pregame->setup) {
                case S::HerdingPregameSetup: menu.source_variant=globals_.retained_herding_difficulty;break;
                case S::DinoPregameSetup:
                    if(menu.selected_subgame>=0&&menu.selected_subgame<3) {
                        const auto value=menu.dino_source_variants[static_cast<std::size_t>(menu.selected_subgame)];
                        if(value)menu.source_variant=*value;
                    }
                    break;
                case S::SpudMazePregameSetup: menu.source_variant=menu.spud_maze_difficulty;break;
                case S::GolfPregameSetup: menu.source_variant=menu.golf_difficulty;break;
                case S::MazePregameSetup: menu.source_variant=menu.maze_difficulty;break;
                case S::SquirrelPregameSetup: menu.source_variant=menu.squirrel_difficulty;break;
                default:break;
                }
                if(pregame->selected_difficulty)
                    menu.variant_selection_origin=pregame->update;
                driver->configure_menu_state(menu);
            }
            if (!driver->initialize(
                    record,globals_.finale_gate,result.error)) {
                result.kind = FrameKind::FrontEndFailed;
                return result;
            }
            generic_front_end_initialized_[key] = true;
            globals_.dispatcher.current_state =
                static_cast<int>(update_state);
            if (pregame) {
                // Native setup loads the instructed generic screen, then
                // opens the independent walkthrough via 0x4281D0.
                result.requires_original_walkthrough_host = true;
                result.original_walkthrough_index = retail_walkthrough_index(
                    pregame->setup,globals_.menu.selected_subgame);
                result.original_pregame_backdrop_slot =
                    retail_pregame_backdrop_index(
                        pregame->setup,globals_.menu.selected_subgame);
                if (!result.original_walkthrough_index ||
                    !result.original_pregame_backdrop_slot) {
                    result.kind = FrameKind::FrontEndFailed;
                    result.error = "original walkthrough/background selection "
                                   "requires the valid retail subgame index";
                    generic_front_end_initialized_[key] = false;
                    return result;
                }
                globals_.dispatcher.generic_screen_mode =
                    static_cast<int>(pregame->screen);
                // These exact setup cases clear 00446F38 in 0042A2C0.
                switch(pregame->setup) {
                case game_flow::State::DinoPregameSetup:
                case game_flow::State::SpudSkatePregameSetup:
                case game_flow::State::MazePregameSetup:
                case game_flow::State::SquirrelPregameSetup:
                case game_flow::State::BobsBandPregameSetup:
                case game_flow::State::GolfPregameSetup:
                case game_flow::State::SpudMazePregameSetup:
                    globals_.shared_completion_code=-1;
                    break;
                default: break;
                }
                if (pregame->starts_global_movie_mode14) {
                    result.original_global_intro_movie_index =
                        retail_pregame_global_intro_index(pregame->setup);
                    // Herding, Fireworks, Squirrel and Park Designer open
                    // an additional global Bink at 0x408EB0, DECREMENT
                    // the state back to setup, then enter screen mode 14.
                    // The original dispatcher awaits a real host-completed
                    // Bink and only then increments to update (in the same
                    // frame) and clears the input pulse. Do not skip it.
                    globals_.dispatcher.current_state =
                        static_cast<int>(setup_state);
                    globals_.dispatcher.generic_screen_mode = 14;
                    globals_.dispatcher.global_movie_finished = false;
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
        if(pregame)driver->configure_menu_state(globals_.menu);
        result.effects = driver->advance(effective_input);
        apply_effects(result.effects);
        result.state_changed =
            result.effects.next_outer_state.has_value();
        if (pregame) {
            consume_pregame_action(state.state,result);
            result.requires_original_walkthrough_host = true;
        } else {
            consume_menu_action(state.state,result);
        }
        if (!result.error.empty()) {
            result.kind=FrameKind::FrontEndFailed;
            return result;
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
            const bool leaving_maze=initialized_activity_==ActivityId::Maze;
            if (!unload_current(result.error)) {
                result.kind = FrameKind::ActivityFailed;
                return result;
            }
            if(leaving_maze)result.effects.audio.push_back({AudioOperation::StopActivityLoop});
        }
        // The retail pregame/chooser writes selected variant and conductor
        // globals before the corresponding original initializer executes.
        // Supply those choices to the activity driver before loading data.
        if (initialized_activity_ != activity->id) {
            drivers_[index]->configure_menu_state(globals_.menu);
            drivers_[index]->configure_display_mode(effective_input.original_windowed_display);
        }
        if (initialized_activity_ != activity->id &&
            !drivers_[index]->initialize(*globals_.active_profile,
                                         result.error)) {
            result.kind = FrameKind::ActivityFailed;
            return result;
        }
        initialized_activity_ = activity->id;
        shared_back_hover_=false;
        pending_confirmed_leave_=false;
        // 00411C80 and 0040B7E0 initialize the completion return to 4.
        if(activity->id==ActivityId::Fireworks ||
           activity->id==ActivityId::ParkDesigner) {
            globals_.shared_movie_return_state=4;
            globals_.shared_completion_code=-1;
        }
        // Retail InitializeFireworksActivity (0x00411CE0) sets slot 63
        // in the *selected* player's 400-byte progress record at init,
        // not after a show, certificate, or teardown. This is the sole
        // source Fireworks write; slot 64 has no game-code writer.
        // Never grant it if the activity initializer returned failure.
        if(activity->id == ActivityId::Fireworks) {
            auto& profile=globals_.player_progress[
                static_cast<std::size_t>(*globals_.active_profile)];
            progress::mark_firework_finale_started(profile);
        }
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

    // Original activity handlers test 0051C2E8 before any update on the
    // next ordinary frame. Their resource-specific unload implementations
    // are used here; no gameplay/progress mutation is performed on exit.
    if(pending_confirmed_leave_){
        pending_confirmed_leave_=false;
        using S=game_flow::State;
        S destination=S::ActivitySelectSetup;
        switch(activity->id){
        case ActivityId::Dinosaur: destination=S::DinoChooserSetup;break;
        case ActivityId::SpudSkate:
        case ActivityId::SpudMaze: destination=S::SpudChooserSetup;break;
        case ActivityId::Maze:
        case ActivityId::Golf: destination=S::AdventureChooserSetup;break;
        case ActivityId::BobsBand: destination=S::MusicChooserSetup;break;
        default: break;
        }
        if(!unload_current(result.error)){
            pending_confirmed_leave_=true;
            result.kind=FrameKind::ActivityFailed;
            return result;
        }
        result.effects.save_and_unload=true;
        result.effects.audio.push_back({AudioOperation::StopManagedSounds});
        result.effects.audio.push_back({AudioOperation::StopBackingTrack});
        if(activity->id==ActivityId::Maze)
            result.effects.audio.push_back({AudioOperation::StopActivityLoop});
        globals_.shared_movie_return_state=static_cast<int>(destination);
        // 00413F10 returns to 4 even with a pending movie. State 4
        // will perform that transition on the following dispatch.
        if(activity->id!=ActivityId::Fireworks&&globals_.shared_completion_code!=-1)
            destination=S::SharedMovieTransition;
        result.effects.next_outer_state=static_cast<int>(destination);
        apply_effects(result.effects);
        result.kind=FrameKind::ActivityUpdated;
        result.state_changed=true;
        if(!profile_directory_.empty()&&!save_profile_files(profile_directory_,result.error))
            result.kind=FrameKind::ActivityFailed;
        return result;
    }

    const bool shared_back=activity->id!=ActivityId::ParkDesigner&&
        effective_input.pointer_position_valid&&drivers_[index]->shared_back_available()&&
        effective_input.pointer_x>572&&effective_input.pointer_x<617&&
        effective_input.pointer_y>422&&effective_input.pointer_y<467;
    const bool shared_back_click=shared_back&&(effective_input.click_pulse||effective_input.pointer_release_pulse);
    // The shared controller consumes the back-button action, not the
    // activity's own placement/shot interaction.
    if(shared_back_click)effective_input.click_pulse=false;
    result.effects = drivers_[index]->advance(effective_input);
    if(!result.effects.fatal_error&&!result.effects.save_and_unload&&
       !result.effects.next_outer_state&&shared_back){
        if(!result.effects.retain_original_global_movie_pixels) {
            const bool post_movie=std::any_of(result.effects.draws.begin(),
                result.effects.draws.end(),[](const Draw& d){return d.after_activity_movie;});
            result.effects.draws.push_back({input.pointer_button_held?
                "Data/ui/backreddep.bmp":"Data/ui/backred.bmp",574,418,{},true,{},post_movie});
        }
        if(!shared_back_hover_)
            result.effects.audio.push_back({AudioOperation::ManagedSoundId,{},456,50,2});
        if(shared_back_click&&!result.effects.open_yes_no_confirmation){
            if(activity->id==ActivityId::Fireworks&&drivers_[index]->shared_back_starts_replay()) {
                result.effects.next_saved_state=static_cast<int>(game_flow::State::FireworksRun);
                result.effects.next_replay_class=2;
                result.effects.next_outer_state=static_cast<int>(game_flow::State::PlayAgainYesNoSetup);
            }else if(activity->id==ActivityId::Dinosaur&&drivers_[index]->shared_back_starts_replay()){
                // 0042B0B8 completed dinosaur bypasses leave confirmation.
                result.effects.save_and_unload=true;
                result.effects.next_saved_state=static_cast<int>(game_flow::State::DinoInit);
                result.effects.next_replay_class=1;
                result.effects.next_outer_state=static_cast<int>(game_flow::State::PlayAgainYesNoSetup);
            }else{
                result.effects.open_leave_activity_confirmation=true;
                if(activity->id==ActivityId::Herding||activity->id==ActivityId::Dinosaur||
                   activity->id==ActivityId::BobsBand||activity->id==ActivityId::Fireworks)
                    result.effects.audio.push_back({AudioOperation::StopManagedSounds});
                if(activity->id==ActivityId::Fireworks)
                    result.effects.audio.push_back({AudioOperation::StopBackingTrack});
            }
        }
    }
    shared_back_hover_=shared_back;
    // A provider can have a valid registered ActivityDriver yet still lack a
    // required original subsystem (for example the Fireworks Bink bank).
    // Fail BEFORE applying progress, transitions, audio or save/unload.
    // Otherwise a partially reconstructed frame is exposed as successful.
    if (result.effects.fatal_error) {
        result.kind = FrameKind::ActivityFailed;
        result.error = *result.effects.fatal_error;
        return result;
    }
    result.kind = FrameKind::ActivityUpdated;

    if ((result.effects.save_and_unload||activity->id==ActivityId::Fireworks) &&
        result.effects.next_outer_state ==
            static_cast<int>(game_flow::State::PlayAgainYesNoSetup)) {
        // Every native activity exiting via PreparePlayAgainTransition
        // executes the same sound/latch setup. Preserve the original 573
        // managed voice, rather than replaying a made-up sound on screen 9.
        const auto plan = prepare_retail_play_again_transition();
        result.replay_preparation = plan;
        globals_.menu.replay_active_latch =
            plan.set_replay_active_latch;
        if(plan.stop_and_release_activity_music) {
            bool already_stopped=false;
            for(const auto& a:result.effects.audio)
                if(a.operation==AudioOperation::StopBackingTrack)already_stopped=true;
            if(!already_stopped)result.effects.audio.push_back({
                AudioOperation::StopBackingTrack});
        }
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
        if(activity->id==ActivityId::Maze)
            result.effects.audio.push_back({AudioOperation::StopActivityLoop});
        // Native state-10 teardown persists the activity before the outer
        // dispatcher switches to Play Again. Do not commit the next state if
        // the activity's persistence failed.
        if (!unload_current(result.error)) {
            result.kind = FrameKind::ActivityFailed;
            return result;
        }
    }
    // Paired badge movie codes are conditional on the actual active
    // player's other completed activity, not the current score alone.
    // 00415830 (Golf), 0041D4D0 (Maze), 00423F40 (Spud Maze).
    if(globals_.active_profile) {
        const auto& progress_record=globals_.player_progress[
            static_cast<std::size_t>(*globals_.active_profile)];
        const auto earned=[&](progress::Slot slot) {
            for(const auto& write:result.effects.progress_writes)
                if(write.slot==slot && write.value==1)return true;
            return false;
        };
        if(activity->id==ActivityId::Golf && earned(progress::Slot::Golf) &&
           progress_record.get(progress::Slot::Maze)==1)
            result.effects.shared_completion_code=3;
        if(activity->id==ActivityId::Maze && earned(progress::Slot::Maze))
            result.effects.shared_completion_code=
                progress_record.get(progress::Slot::Golf)==1?3:-1;
        if(activity->id==ActivityId::SpudMaze &&
           earned(progress::Slot::SpudMaze) &&
           progress_record.get(progress::Slot::SpudSkate)==1)
            result.effects.shared_completion_code=2;
    }
    if(result.effects.save_and_unload &&
       result.effects.next_outer_state==
           static_cast<int>(game_flow::State::PlayAgainYesNoSetup)) {
        // Distinct return globals from original activity exit handlers:
        // 00409E00, 00424D10, 00423F40, 0041D4D0, 00415830,
        // 004182B0 and 004279F0. Do not overwrite with replay init state.
        switch(activity->id) {
        case ActivityId::Dinosaur: globals_.shared_movie_return_state=0x10; break;
        case ActivityId::SpudSkate:
        case ActivityId::SpudMaze: globals_.shared_movie_return_state=0x16; break;
        case ActivityId::Maze:
        case ActivityId::Golf: globals_.shared_movie_return_state=0x1c; break;
        case ActivityId::Herding:
        case ActivityId::Squirrel: globals_.shared_movie_return_state=-1; break;
        default: break;
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
