#include "btb/full_game_native_session.hpp"

#include <utility>

namespace btb::full_game {

bool NativeGameSession::fail(
    NativeSessionFrame& frame,
    PhysicalPresentationStage stage,
    std::string message) {

    failed_ = true;
    frame.failure = PhysicalPresentationError{stage,std::move(message)};
    return false;
}

NativeSessionFrame NativeGameSession::tick(
    ActivityFrameInput input,
    bool primary_input_pulse,
    bool secondary_input_pulse,
    bool defer_present) {

    NativeSessionFrame frame{};
    if(pending_present_) {
        fail(frame,PhysicalPresentationStage::Present,
             "prior source frame was never presented or discarded");
        return frame;
    }
    if (failed_) {
        frame.failure = PhysicalPresentationError{
            PhysicalPresentationStage::GameAdvance,
            "native session was previously interrupted by provider failure"};
        return frame;
    }

    PhysicalFrameObservation physical{};
    std::string error;
    // 0042A2C0 intercepts call 00409260 BEFORE the global movie
    // update. A stopped movie must not report decoder EOS behind a modal.
    const bool pause=!skip_video_&&game_flow::dispatch_step(
        root_.globals().dispatcher).pause_global_movie;
    input.physical_help_snapshot_available=device_.supports_captured_underlays();
    if(pause!=global_movie_paused_){
        // Restore the last submitted movie pixels before the decoder
        // advances or waits; fullscreen Flip may have swapped buffers.
        if(!pause&&global_snapshot_valid_&&
           !device_.draw_captured_underlay(CapturedUnderlay::GlobalMovie,{},error)){
            fail(frame,PhysicalPresentationStage::SubmitDraw,error);return frame;
        }
        if(!device_.set_global_movie_paused(pause,error)){
            fail(frame,PhysicalPresentationStage::ObserveDevice,error);return frame;
        }
        global_movie_paused_=pause;
    }
    if (!device_.observe(physical,error)) {
        fail(frame,PhysicalPresentationStage::ObserveDevice,error);
        return frame;
    }

    input.managed_sound_playing = physical.any_managed_voice_playing;
    // Profile selection uses IsSoundIdPlaying on particular IDs, not the
    // aggregate any-playing test. This keeps unrelated hover/narration
    // voices from preventing a new player from being selected.
    for(std::size_t i=0;i<kProfileGateSoundIds.size();++i) {
        input.profile_sound_playing[i]=sound::is_sound_id_playing(
            effects_.audio().manager(),kProfileGateSoundIds[i],
            physical.managed.playing);
    }
    for(std::size_t i=0;i<input.herding_animal_sound_playing.size();++i) {
        input.herding_animal_sound_playing[i]=sound::is_sound_id_playing(
            effects_.audio().manager(),0x331+static_cast<int>(i),
            physical.managed.playing);
    }
    input.maze_intro_voice_playing.emplace();
    for(std::size_t i=0;i<input.maze_intro_voice_playing->size();++i)
        (*input.maze_intro_voice_playing)[i]=sound::is_sound_id_playing(
            effects_.audio().manager(),0x49+static_cast<int>(i),physical.managed.playing);
    input.squirrel_lofty_voice_playing.emplace();
    for(std::size_t i=0;i<input.squirrel_lofty_voice_playing->size();++i)
        (*input.squirrel_lofty_voice_playing)[i]=sound::is_sound_id_playing(
            effects_.audio().manager(),633+static_cast<int>(i),physical.managed.playing);
    if(global_movie_paused_)physical.global_movie_finished=false;
    input.original_global_movie_finished = physical.global_movie_finished;
    input.original_global_movie_skip_pulse =
        input.original_global_movie_skip_pulse || input.click_pulse ||
        primary_input_pulse || secondary_input_pulse;
    input.fireworks_crowd_end_sound_playing=
        sound::is_sound_id_playing(
            effects_.audio().manager(),349,
            physical.managed.playing);
    // Explicit non-retail --skip-bink routing. Retail 0x41 plays the
    // scene-set clip after a newly created profile and then enters 0x04.
    // State 0x40 owns its distinct 00446CE8 return-state global.
    // Explicit video-free mode follows that state without inventing EOS.
    input.skip_shared_completion_video=skip_video_;
    if (skip_video_ &&
        root_.globals().dispatcher.current_state ==
            static_cast<int>(game_flow::State::StartupMoviesThenProfileSetup)) {
        root_.skip_startup_video_sequence();
        input.click_pulse=false;
    }
    if (skip_video_ &&
        root_.globals().dispatcher.current_state ==
            static_cast<int>(game_flow::State::SceneSetMovieTransition)) {
        root_.set_outer_state(game_flow::State::ActivitySelectSetup);
        root_.globals().dispatcher.generic_screen_mode=0;
        input.click_pulse=false; // a movie-dismissal click cannot select UI
    }
    // Normal operation uses ONLY original decoder end-of-stream signaling.
    // An explicitly opted-in video-free session may advance modes 13/14
    // without claiming a Bink frame was ever decoded or presented.
    const auto movie_mode=root_.globals().dispatcher.generic_screen_mode;
    root_.globals().dispatcher.global_movie_finished =
        (skip_video_ && (movie_mode == 13 || movie_mode == 14))
            ? true : physical.global_movie_finished;

    const auto entry_state=static_cast<game_flow::State>(
        root_.globals().dispatcher.current_state);
    using S=game_flow::State;
    const bool replay_screen=entry_state==S::PlayAgainYesNoSetup||
        entry_state==S::PlayAgainYesNoUpdate||
        entry_state==S::PlayAgainDifficultySetup||
        entry_state==S::PlayAgainDifficultyUpdate||
        entry_state==S::FireworksReplayChoiceSetup||
        entry_state==S::FireworksReplayChoiceUpdate;
    const bool replay_underlay=replay_screen&&root_.globals().menu.replay_active_latch;
    if(entry_state==S::SpudSkateInit)skate_snapshot_valid_=false;

    auto game = root_.advance(input);
    frame.prepared.game = game;

    if (game.kind == FrameKind::FrontEndFailed ||
        game.kind == FrameKind::ActivityFailed ||
        game.kind == FrameKind::InvalidState ||
        game.kind == FrameKind::FrontEndRequiresAdapter ||
        game.kind == FrameKind::ActivityRequiresAdapter ||
        game.kind == FrameKind::InactiveProfile) {
        fail(frame,PhysicalPresentationStage::GameAdvance,
             game.error.empty()
                 ? "native game state cannot run without its original "
                   "screen/activity/profile adapter"
                 : game.error);
        return frame;
    }

    const auto resolve_movie = [&](const std::string& original)
        -> std::optional<std::filesystem::path> {
        auto resolved=effects_.assets().resolve(original);
        if (!resolved.found()) {
            error="retail movie asset could not be resolved: "+original+
                " ("+resolved.error+")";
            return std::nullopt;
        }
        return resolved.absolute_path;
    };

    if (game.close_original_global_movie) {
        if (!device_.close_global_movie(error)) {
            fail(frame,PhysicalPresentationStage::MovieClose,error);
            return frame;
        }
    }

    if (!skip_video_ && game.original_walkthrough_index) {
        const auto* entry=walkthroughs_.entry(
            *game.original_walkthrough_index);
        if (!entry) {
            fail(frame,PhysicalPresentationStage::MovieResolve,
                 "required original binkwalk.txt index unavailable");
            return frame;
        }
        auto movie=resolve_movie(entry->movie);
        // 004281D0 reads only the BIK. 0042A2C0 dispatches narration
        // separately through the numbered managed-sound catalog.
        if (!movie) {
            fail(frame,PhysicalPresentationStage::MovieResolve,error);
            return frame;
        }
        if (walkthrough_open_ &&
            !device_.close_walkthrough_movie(error)) {
            fail(frame,PhysicalPresentationStage::MovieClose,error);
            return frame;
        }
        if (walkthrough_open_) {
            frame.walkthrough_closed = true;
            walkthrough_open_ = false;
        }
        if (!device_.open_walkthrough_movie(*movie,error)) {
            fail(frame,PhysicalPresentationStage::MovieOpen,error);
            return frame;
        }
        walkthrough_open_ = true;
        frame.walkthrough_opened = true;
    }

    if (!skip_video_ && game.original_global_intro_movie_index) {
        const auto* title=startups_.movie(
            *game.original_global_intro_movie_index);
        if (!title) {
            fail(frame,PhysicalPresentationStage::MovieResolve,
                 "required original startupmovie.txt index unavailable");
            return frame;
        }
        auto file=resolve_movie(*title);
        if (!file) {
            fail(frame,PhysicalPresentationStage::MovieResolve,error);
            return frame;
        }
        if (!device_.open_global_movie(*file,error)) {
            fail(frame,PhysicalPresentationStage::MovieOpen,error);
            return frame;
        }
        frame.global_movie_opened = true;
    }

    // State 0x00 consumes the ordered original videoseq.txt records.
    // The decoder's physical end-of-stream observation advances the index.
    if (!skip_video_ && game.original_startup_sequence_movie_path) {
        auto movie=resolve_movie(*game.original_startup_sequence_movie_path);
        if (!movie) {
            fail(frame,PhysicalPresentationStage::MovieResolve,error);
            return frame;
        }
        if (!device_.open_global_movie(*movie,error)) {
            fail(frame,PhysicalPresentationStage::MovieOpen,error);
            return frame;
        }
        frame.global_movie_opened=true;
    }

    // State 0x40 reads the separate original completedmovie.txt table.
    if (!skip_video_ && game.original_completion_movie_path) {
        auto movie=resolve_movie(*game.original_completion_movie_path);
        if (!movie) {
            fail(frame,PhysicalPresentationStage::MovieResolve,error);
            return frame;
        }
        if (!device_.open_global_movie(*movie,error)) {
            fail(frame,PhysicalPresentationStage::MovieOpen,error);
            return frame;
        }
        frame.global_movie_opened=true;
    }

    // State 0x41 uses the dedicated UpdateSceneSetMovie source path, not
    // a startupmovie.txt index. Open the genuine Bink once and let its
    // physical end-of-stream observation drive the next state.
    if (!skip_video_ && game.original_scene_set_movie_path) {
        auto movie=resolve_movie(*game.original_scene_set_movie_path);
        if (!movie) {
            fail(frame,PhysicalPresentationStage::MovieResolve,error);
            return frame;
        }
        if (!device_.open_global_movie(*movie,error)) {
            fail(frame,PhysicalPresentationStage::MovieOpen,error);
            return frame;
        }
        frame.global_movie_opened=true;
    }

    // The retail instruction walkthrough is torn down when leaving its
    // pregame state for Back or Start. Difficulty/Help actions remain.
    if (walkthrough_open_ &&
        game.pregame_action &&
        game.pregame_action->next_state) {
        if (!device_.close_walkthrough_movie(error)) {
            fail(frame,PhysicalPresentationStage::MovieClose,error);
            return frame;
        }
        walkthrough_open_ = false;
        frame.walkthrough_closed = true;
    }

    frame.prepared = effects_.prepare(
        std::move(game),physical.managed,
        primary_input_pulse,secondary_input_pulse);

    // A missing original asset is not permission to render synthetic
    // replacement artwork. Fail the whole physical frame before submission.
    if (!frame.prepared.resources.ready_for_submission() ||
        frame.prepared.missing_catalog_ids ||
        frame.prepared.audio_policy.invalid_requests) {
        // Report the actual request, not a generic fatal dialog. This
        // remains fail-closed; no unresolved draw/audio is submitted.
        const auto& resources=frame.prepared.resources;
        const auto& audio=frame.prepared.audio_policy;
        std::string detail="original frame asset resolution failed";
        for(const auto& draw:resources.draws) {
            if(!draw.source.found()) {
                detail += "\nBitmap: "+draw.original.source_asset+
                          " ("+draw.source.error+")";
                break;
            }
        }
        for(const auto& sound:resources.sound) {
            if(sound.source && !sound.source->found()) {
                detail += "\nWAV: "+sound.original.filename+
                          " ("+sound.source->error+")";
                break;
            }
        }
        if(!audio.missing_catalog_sound_ids.empty()) {
            detail += "\nMissing sound ID: "+
                std::to_string(audio.missing_catalog_sound_ids.front());
        }
        if(audio.invalid_requests)
            detail += "\nInvalid audio requests: "+
                      std::to_string(audio.invalid_requests);
        detail += "\nUnresolved bitmap/WAV counts: "+
                  std::to_string(resources.unresolved_draws)+"/"+
                  std::to_string(resources.unresolved_audio_assets);
        fail(frame,PhysicalPresentationStage::AssetResolve,std::move(detail));
        return frame;
    }

    if(frame.prepared.game.effects.original_volume_percent&&
       !device_.set_original_volume(*frame.prepared.game.effects.original_volume_percent,error)){
        fail(frame,PhysicalPresentationStage::SubmitAudio,error);return frame;
    }
    for (const auto& command : frame.prepared.resources.sound) {
        if (!device_.apply_sound(command,error)) {
            fail(frame,PhysicalPresentationStage::SubmitAudio,error);
            return frame;
        }
    }

    // Global Bink modes 13/14 decode directly into the common display
    // surface during device.observe(). If the original dispatcher is
    // intercepted waiting for that movie, do not clear the Bink pixels
    // with a synthetic empty gameplay/UI draw list before present.
    const auto intercept=frame.prepared.game.dispatch.intercept;
    const bool global_movie_frame =
        !skip_video_ &&
        (frame.global_movie_opened ||
         (frame.prepared.game.kind == FrameKind::Intercept &&
          (intercept==game_flow::Intercept::GlobalMovieMode13 ||
           intercept==game_flow::Intercept::GlobalMovieMode14)) ||
         frame.prepared.game.effects.retain_original_global_movie_pixels);
    const bool preparing_replay=frame.prepared.game.replay_preparation&&
        frame.prepared.game.replay_preparation->draw_replay_underlay;
    if (!global_movie_frame) {
        const bool skate_replay=preparing_replay&&entry_state==S::SpudSkateRun&&
                                skate_snapshot_valid_;
        const auto& draws=frame.prepared.resources.draws;
        // For an activity whose source has already unloaded the Bink
        // bank, do not replace its final visible scene with BMP-only art.
        bool drawn=false;
        const auto& effects=frame.prepared.game.effects;
        const auto render_snapshot=[&](CapturedUnderlay kind,std::size_t prefix){
            if(prefix>draws.size()){error="invalid captured overlay draw prefix";return false;}
            return device_.draw_captured_underlay(kind,
                std::vector<ResolvedDraw>(draws.begin()+prefix,draws.end()),error);
        };
        if(input.physical_help_snapshot_available&&effects.original_help_background_count)
            drawn=render_snapshot(CapturedUnderlay::Help,*effects.original_help_background_count);
        else if(global_movie_paused_&&global_snapshot_valid_&&effects.original_modal_background_count)
            drawn=render_snapshot(CapturedUnderlay::GlobalMovie,*effects.original_modal_background_count);
        else if(skate_replay)drawn=device_.draw_replay_ordered({},error);
        else if(replay_underlay)drawn=device_.draw_replay_ordered(draws,error);
        else if(preparing_replay&&draws.empty())drawn=true; // capture existing pixels
        else drawn=device_.draw_ordered(draws,error);
        if(!drawn) {
            fail(frame,PhysicalPresentationStage::SubmitDraw,error);
            return frame;
        }
        if((preparing_replay&&!skate_replay)||
           (entry_state==S::SpudSkateRun&&!preparing_replay&&
            !frame.prepared.game.effects.save_and_unload)) {
            if(!device_.capture_replay_underlay(error)) {
                fail(frame,PhysicalPresentationStage::SubmitDraw,error);
                return frame;
            }
            if(entry_state==S::SpudSkateRun)skate_snapshot_valid_=true;
        }
    }
    if(input.physical_help_snapshot_available){
        if(frame.prepared.game.effects.capture_original_help&&
           !device_.capture_underlay(CapturedUnderlay::Help,error)){
            fail(frame,PhysicalPresentationStage::SubmitDraw,error);return frame;
        }
        if(global_movie_frame){
            if(!device_.capture_underlay(CapturedUnderlay::GlobalMovie,error)){
                fail(frame,PhysicalPresentationStage::SubmitDraw,error);return frame;
            }
            global_snapshot_valid_=true;
        }else if(!global_movie_paused_)global_snapshot_valid_=false;
    }
    if(frame.prepared.game.effects.request_original_frame_print &&
       !device_.print_current_frame(error)) {
        fail(frame,PhysicalPresentationStage::Print,error);
        return frame;
    }
    // The original activity unloaders release their bitmap groups.
    // Do this only after the final composed image (and any replay
    // snapshot/print) exists. A lost-surface retry can recreate source
    // bitmaps from the saved draw stream; the underlay is independent.
    if(frame.prepared.game.effects.save_and_unload ||
       frame.prepared.game.kind==FrameKind::ActivityInitialized)
        device_.release_activity_bitmaps();
    if(defer_present) {
        pending_present_=true;
        frame.ready_for_present=true;
        return frame;
    }
    if (!device_.present(error)) {
        fail(frame,PhysicalPresentationStage::Present,error);
        return frame;
    }

    frame.submitted = true;
    return frame;
}

bool NativeGameSession::present_prepared(std::string& error) {
    if(failed_ || !pending_present_) {
        error="source frame has no valid deferred presentation";
        return false;
    }
    pending_present_=false;
    if(!device_.present(error)) {
        failed_=true;
        return false;
    }
    error.clear();
    return true;
}

} // namespace btb::full_game
