#include "btb/full_game_spud_skate.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace btb::full_game {
namespace {
constexpr std::array<const char*,3> kOriginalDifficultyFiles{{
    "spuddata1.txt","spuddata2.txt","spuddata3.txt"
}};
constexpr std::array<const char*,5> kOriginalMovies{{
    "bad.bik","normal.bik","ok.bik","good.bik","end.bik"
}};
constexpr const char* kSpudRoot="Data/SubGameSpudSkate/";
}

void SpudSkateDriver::configure_menu_state(
    const RetailMenuState& menu) noexcept {
    if((menu.variant_selection_origin==
            game_flow::State::SpudSkatePregameUpdate ||
        menu.variant_selection_origin==
            game_flow::State::PlayAgainDifficultyUpdate) &&
       menu.source_variant>=0 && menu.source_variant<=2)
        difficulty_=menu.source_variant;
}

bool SpudSkateDriver::initialize(
    int player_index,std::string& error) {
    if(active_ || player_index<0 ||
       player_index>=static_cast<int>(progress::kPlayerCount) ||
       difficulty_<0 || difficulty_>2) {
        error="Spud Skate invalid retail profile, difficulty or replay lifetime";
        return false;
    }
    if(!movies_) {
        // Until the native Bink decoder is genuinely connected,
        // native-host gameplay MUST stop here rather than fabricate
        // decoded frame numbers or silently award the Spud Skate badge.
        error="original five-stream Spud Skate Bink provider is not connected";
        return false;
    }
    const auto folder=root_/kSpudRoot;
    try {
        std::ifstream timing_file(folder/kOriginalDifficultyFiles[
            static_cast<std::size_t>(difficulty_)]);
        std::ifstream sounds_file(folder/"soundinfo.txt");
        if(!timing_file || !sounds_file) {
            error="original Spud Skate timing/sound files unavailable";
            return false;
        }
        const auto timing=spud_skate::parse_timing(timing_file);
        const auto sounds=spud_skate::parse_sound_info(sounds_file);
        // The installed retail files contain exactly eight stunts,
        // two passes with the 44/660 movie synchronization pair.
        if(timing.stunt_count!=static_cast<int>(spud_skate::kStuntCount) ||
           timing.loop_start_frame!=44 ||
           timing.loop_end_frame!=660) {
            error="Spud Skate source file violates original 8-stunt/44-660 contract";
            return false;
        }
        std::array<std::filesystem::path,5> binks{};
        for(std::size_t i=0;i<binks.size();++i) {
            binks[i]=folder/kOriginalMovies[i];
            if(!std::filesystem::is_regular_file(binks[i])) {
                error="missing original Spud Skate Bink: "+binks[i].string();
                return false;
            }
        }
        if(!movies_->open(binks,error))return false;
        timing_=timing;
        sounds_=sounds;
        state_={};
        player_index_=player_index;
        active_=true;
        first_audio_=true;
        error.clear();
        return true;
    } catch(const std::exception& e) {
        error=std::string("Spud Skate source initialization: ")+e.what();
        return false;
    }
}

bool SpudSkateDriver::queue_stunt_sound(
    const spud_skate::StuntSoundRequest& choice,
    ActivityFrameOutput& out,std::string& error) {
    if(choice.stunt>=spud_skate::kStuntCount)return false;
    const auto quality=static_cast<std::size_t>(choice.quality);
    if(quality>=spud_skate::kQualityCount) {
        error="Spud Skate invalid original sound quality";
        return false;
    }
    // Original 0x424690 repeatedly draws rand()%5 until the selected
    // stunt/quality row returns a valid nonnegative ID. Preserve
    // modulo bias and each discarded source random draw.
    bool any=false;
    for(const auto id:sounds_.sound_ids[choice.stunt][quality])
        any|=id>=0;
    if(!any) {
        error="Spud Skate source sound row has no playable retail ID";
        return false;
    }
    for(int tries=0;tries<32768;++tries) {
        const auto slot=static_cast<std::size_t>(draw_original_rand()%5);
        const auto sound=sounds_.sound_ids[choice.stunt][quality][slot];
        if(sound>=0) {
            out.audio.push_back({AudioOperation::ManagedSoundId,{},
                                 sound,50,1});
            return true;
        }
    }
    error="original Spud Skate randomized sound row never resolved";
    return false;
}

bool SpudSkateDriver::queue_result_sounds(
    const spud_skate::ResultPresentation& result,
    ActivityFrameOutput& out,std::string& error) {
    for(std::size_t i=0;i<result.sound_pool_count;++i) {
        const auto& pool=result.sound_pools[i];
        if(pool.count<=0) {
            error="invalid original Spud Skate result voice pool";
            return false;
        }
        out.audio.push_back({AudioOperation::ManagedSoundId,{},
            pool.first_id+draw_original_rand()%pool.count,50,1});
    }
    return true;
}

void SpudSkateDriver::append_authentic_bitmap_draws(
    ActivityFrameOutput& out) const {
    out.draws.push_back({
        "Data/SubGameSpudSkate/minimised.bmp",0,0,{},false});
    if(const auto overlay=
            spud_skate::overlay_surface_for_quality(state_.quality)) {
        const auto idx=static_cast<int>(*overlay)-
            static_cast<int>(spud_skate::SurfaceKind::NormalOverlay);
        if(idx>=0 && idx<3)out.draws.push_back({
            std::string("Data/SubGameSpudSkate/")+
                static_cast<char>('1'+idx)+".bmp",
            267,415,{},true});
    }
    const auto digits=spud_skate::score_render_plan(state_.score);
    for(std::size_t i=0;i<digits.count;++i) {
        const auto& digit=digits.digits[i];
        out.draws.push_back({"Data/SubGameSpudSkate/skatespudscore.bmp",
            digit.x,digit.y,
            Rect{digit.source_left,digit.source_top,
                 digit.source_right,digit.source_bottom},true});
    }
}

ActivityFrameOutput SpudSkateDriver::advance(
    const ActivityFrameInput& input) {
    ActivityFrameOutput out{};
    if(!active_ || !movies_) {
        out.fatal_error="Spud Skate needs its original initialized Bink streams";
        return out;
    }
    SpudSkateMovieFrame frame{};
    std::string error;
    if(!movies_->observe(frame,error)) {
        out.fatal_error=error.empty()
            ? "original Spud Skate Bink frame observation failed":error;
        return out;
    }
    if(first_audio_) {
        out.audio.push_back({AudioOperation::StartBackingTrack,
            "Data/music/Skateboardrace.wav"});
        out.audio.push_back({AudioOperation::ManagedSoundId,{},758,50,1});
        first_audio_=false;
    }
    if(state_.phase==spud_skate::PlaybackPhase::SynchronizedRun) {
        // No Bink-derived decoded frame -> NO gameplay update, scoring
        // or success. The four retail quality movies advance together.
        if(frame.primary_decoded_frame<0 ||
           !frame.four_quality_streams_synchronized) {
            out.fatal_error="Spud Skate requires synchronized original decoded Bink frames";
            return out;
        }
        const auto step=spud_skate::update_synchronized_run(
            state_,timing_,sounds_,
            {frame.primary_decoded_frame,input.click_pulse,
             frame.quality_stream_reached_end});
        if(step.sound_request &&
           !queue_stunt_sound(*step.sound_request,out,error)) {
            out.fatal_error=error;return out;
        }
        if(!movies_->select_quality(state_.quality,error)) {
            out.fatal_error=error;return out;
        }
        if(step.resync_quality_streams &&
           !movies_->resynchronize_all(step.resync_frame,error)) {
            out.fatal_error=error;return out;
        }
        if(step.start_end_movie &&
           !movies_->begin_end_movie(error)) {
            out.fatal_error=error;return out;
        }
    } else if(state_.phase==spud_skate::PlaybackPhase::EndMovie) {
        // Original UpdateAndDrawSpudSkatePlayback 0x424690 reads
        // record[selected_player].progress[59] before determining
        // whether completion code 2 should be raised. This is the
        // real persisted Spud Maze prerequisite, not an invented
        // per-frame input and not necessarily the current activity.
        const bool maze_completed=
            spud_maze_completed_ &&
            spud_maze_completed_(player_index_);
        const auto end=spud_skate::update_end_movie_phase(state_,{
            frame.end_movie_finished,input.managed_sound_playing,
            maze_completed});
        if(end.stop_all_managed_sounds)
            out.audio.push_back({AudioOperation::StopManagedSounds});
        if(end.result &&
           !queue_result_sounds(*end.result,out,error)) {
            out.fatal_error=error;return out;
        }
        if(end.mark_spud_skate_progress_complete)
            out.progress_writes.push_back({progress::Slot::SpudSkate,1});
        if(end.set_shared_completion_code_2)
            out.shared_completion_code=2;
        if(end.complete_activity) {
            out.audio.push_back({AudioOperation::StopBackingTrack});
            out.next_outer_state=spud_skate::kPlayAgainOuterState;
            out.next_saved_state=spud_skate::kSpudChooserOuterState;
            out.next_ui_context=0x1A;
            out.save_and_unload=true;
        }
    }
    // The native movie provider owns the actual 600x380 Bink surface;
    // these are the original remaining common DirectDraw BMP layers.
    append_authentic_bitmap_draws(out);
    return out;
}

bool SpudSkateDriver::unload(std::string& error) {
    if(active_ && movies_ && !movies_->close(error))return false;
    active_=false;
    first_audio_=false;
    state_={};
    player_index_=-1;
    error.clear();
    return true;
}

} // namespace btb::full_game
