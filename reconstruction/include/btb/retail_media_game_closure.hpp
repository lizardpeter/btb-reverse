#pragma once
// Original Bob Builds a Park, 311296-byte Win32 executable, SHA-256
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Host-neutral reconstruction of source control flow. Real Bink/WinMM/COM
// functions supplied as callbacks. Safety divergence at invalid host inputs.

#include "btb/bink_system.hpp"
#include "btb/retail_shared_runtime.hpp"
#include "btb/retail_system_closure.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>

namespace btb::retail_media {
using I32=std::int32_t;
using U32=std::uint32_t;

// 00409210: no BinkSetVolume when movie is null or paused. Original
// retail threshold of 37, using observed global game-volume conversion.
using SetMovieVolume=void (*)(void*,I32,I32) noexcept;
inline void apply_global_bink_volume(
    void* movie,bool paused,I32 game_volume,SetMovieVolume volume) noexcept {
    if(movie&&!paused&&volume)
        volume(movie,0,btb::bink::bink_volume_from_game_volume(game_volume));
}

using OpenBink=void* (*)(std::string_view,U32) noexcept;
using AbortMissingDisc=void (*)() noexcept;
using DisableInput=void (*)() noexcept;

// 00408E10: direct BinkOpen(path,0x04000000), fallback to CD root only on
// direct-open failure. Always ApplyGlobalBinkVolume even if both attempts fail.
// The original stack-path construction uses a 100-byte buffer; a validated
// string is passed here instead to eliminate buffer overflow on host input.
[[nodiscard]] inline void* open_bink_movie_with_fallback(
    std::string_view filename,std::string_view cd_root,
    OpenBink open,void* global_movie,bool global_paused,
    I32 game_volume,SetMovieVolume volume) noexcept {
    if(!open)return nullptr;
    void* movie=open(filename,0x04000000U);
    if(!movie&&!cd_root.empty()) {
        const std::string path=std::string(cd_root)+std::string(filename);
        movie=open(path,0x04000000U);
    }
    apply_global_bink_volume(global_movie,global_paused,game_volume,volume);
    return movie;
}

// 00408EB0: direct BinkOpen(path,0), retry CD, abort on missing retail CD;
// on success apply global volume and DISABLE input processing.
struct GlobalBink {
    void* movie{};
    bool paused{};
    bool input_enabled{true};
};
[[nodiscard]] inline bool open_global_bink_movie(
    GlobalBink& state,std::string_view filename,
    std::string_view cd_root,OpenBink open,AbortMissingDisc abort,
    DisableInput disable,I32 game_volume,SetMovieVolume volume) noexcept {
    if(!open)return false;
    state.movie=open(filename,0);
    if(!state.movie&&!cd_root.empty()) {
        const std::string path=std::string(cd_root)+std::string(filename);
        state.movie=open(path,0);
    }
    if(!state.movie) {
        if(abort)abort();
        return false;
    }
    apply_global_bink_volume(state.movie,state.paused,game_volume,volume);
    if(disable)disable();
    state.input_enabled=false;
    return true;
}

// 00408D90: clears 0x7C-byte DirectDraw descriptor, sets 640x480 offscreen
// surface flags 7/caps 0x40, binds Bink sound to DirectSound, clears paused.
struct BinkInitialize {
    std::array<U32,31> surface_descriptor{};
    bool paused{true};
};
using MakeOffscreenSurface=void (*)(const std::array<U32,31>&) noexcept;
using BindBinkSoundSystem=void (*)(void*) noexcept;
inline void initialize_bink_playback(
    BinkInitialize& state,MakeOffscreenSurface make_surface,
    BindBinkSoundSystem set_sound_system,void* directsound) noexcept {
    state.surface_descriptor.fill(0);
    state.surface_descriptor[0]=0x7c;
    state.surface_descriptor[1]=7;
    state.surface_descriptor[2]=480; // +0x08 height
    state.surface_descriptor[3]=640; // +0x0c width
    state.surface_descriptor[26]=0x40; // +0x68 DDSCAPS offscreen plain
    if(make_surface)make_surface(state.surface_descriptor);
    if(set_sound_system)set_sound_system(directsound);
    state.paused=false;
}

// 00409070: doFrame, lock surface, if failed return 1; else
// determine BinkDDSurfaceType, copy, unlock, return 0. NO frame advance.
using NoAdvanceDecode=void (*)(void*) noexcept;
using NoAdvanceLock=bool (*)(void*) noexcept; // true iff native lock failed
using NoAdvanceCopy=void (*)(void*) noexcept;
using NoAdvanceUnlock=void (*)(void*) noexcept;
[[nodiscard]] inline I32 decode_bink_frame_without_advance(
    void* movie,void* surface,NoAdvanceDecode decode,NoAdvanceLock lock,
    NoAdvanceCopy copy,NoAdvanceUnlock unlock) noexcept {
    if(decode)decode(movie);
    if(lock && lock(surface))return 1;
    if(copy)copy(movie);
    if(unlock)unlock(surface);
    return 0;
}

// 00428010: stop/delete existing activity music, create WAV from index*50
// in retail table, CSound_Play(0,1), set 0051c368=1 unconditionally.
using StopSound=void (*)(void*) noexcept;
using DeleteSound=void (*)(void*,I32) noexcept;
using CreateMusic=void* (*)(I32) noexcept;
using PlaySound=void (*)(void*,I32,U32) noexcept;
struct ActivityMusic {
    void* current{};
    I32 playback_latch{};
};
inline void play_activity_music_by_index(
    ActivityMusic& state,I32 index,
    StopSound stop,DeleteSound destroy,CreateMusic create,
    PlaySound play) noexcept {
    if(state.current) {
        if(stop)stop(state.current);
        if(destroy)destroy(state.current,1);
        state.current=nullptr;
    }
    if(create)state.current=create(index);
    if(play)play(state.current,0,1);
    state.playback_latch=1;
}

// 004280D0 only cleans a walkthrough movie if its offscreen surface exists:
// CloseBinkMovie, unregister, release surface, then retail scroll reset.
struct Walkthrough {
    void* movie{};
    void* surface{};
    I32 scroll_value{};
};
using VoidCallback=void (*)(void*) noexcept;
using ResetScroll=void (*)(I32) noexcept;
inline void close_walkthrough_movie(
    Walkthrough& w,VoidCallback close_movie,VoidCallback unregister_surface,
    VoidCallback release_surface,ResetScroll reset_scroll) noexcept {
    if(!w.surface)return;
    if(close_movie)close_movie(w.movie);
    if(unregister_surface)unregister_surface(w.surface);
    if(release_surface)release_surface(w.surface);
    w.surface=nullptr;
    if(reset_scroll)reset_scroll(w.scroll_value);
}

// 0042A240 SceneSetMovie. First call opens scene-set; later polling calls
// UpdateGlobalBinkMovie; success only after completed first clip.
using OpenSceneMovie=void (*)(std::string_view) noexcept;
using UpdateMovie=bool (*)() noexcept;
struct SceneSetMovie {
    I32 stage{};
    I32 completed_count{};
    I32 input_gate{};
};
[[nodiscard]] inline bool update_scene_set_movie(
    SceneSetMovie& s,OpenSceneMovie open,UpdateMovie update) noexcept {
    s.input_gate=1;
    if(s.stage==0) {
        if(open)open("Data/movies/sceneset.bik");
        ++s.stage;
    } else if(s.stage==1 && update && update()) {
        s.stage=0;
        ++s.completed_count;
        if(s.completed_count>0) return true;
    }
    return false;
}

// 0042A1A0 Startup movie sequence uses original per-movie input block bit.
// On update completion advances index, and clears playing flag at end.
struct StartupMovies {
    I32 current_index{};
    I32 movie_state{};
    I32 count{};
    bool showing{};
    bool input_gate{};
};
[[nodiscard]] inline bool update_startup_movies(
    StartupMovies& s,std::span<const I32> input_modes,
    std::span<const std::string_view> filenames,
    OpenSceneMovie open,UpdateMovie update) noexcept {
    if(s.current_index<0||
       s.current_index>=static_cast<I32>(input_modes.size())||
       s.current_index>=static_cast<I32>(filenames.size()))
        return false; // host safety
    s.showing=true;
    s.input_gate=input_modes[static_cast<std::size_t>(s.current_index)]!=1;
    if(s.movie_state==0) {
        if(open)open(filenames[static_cast<std::size_t>(s.current_index)]);
        ++s.movie_state;
    } else if(s.movie_state==1 && update && update()) {
        ++s.current_index;
        s.movie_state=0;
        if(s.count<=s.current_index) {
            s.showing=false;
            s.input_gate=true;
            return true;
        }
    }
    return false;
}

// 00428470: Help icon hover uses cursor==default and bit4 pulse; one
// special action displays help bar, with sound IDs 456 or 475 depending
// on global mode. The sound plays only on first pulse after hover latch.
struct HelpHover {
    bool cursor_is_default{};
    bool pointer_pulse_bit4{};
    I32 action{};
    bool alternate_help_surface{};
    I32 mode{};
    bool hover_latched{};
};
struct HelpHoverResult {
    bool draw_help_button{};
    bool alternate_image{};
    I32 voice_id{-1};
};
[[nodiscard]] constexpr HelpHoverResult update_generic_help_hover(
    HelpHover& s) noexcept {
    HelpHoverResult o{};
    if(!s.cursor_is_default)return o;
    if(!s.pointer_pulse_bit4){s.hover_latched=false;return o;}
    if(s.action==12){
        o.draw_help_button=true;
        o.alternate_image=s.alternate_help_surface;
    }
    if(!s.hover_latched){
        if(s.mode==0)o.voice_id=456;
        else if(s.mode==1)o.voice_id=475;
    }
    s.hover_latched=true;
    return o;
}

// 004169A0: detect whether ANY corner of first rectangle lies inclusively
// inside second. Not equivalent to complete rectangle overlap/containment.
using Rect=btb::retail_leaf::Rect32;
[[nodiscard]] constexpr bool any_rect_corner_inside(
    const Rect& a,const Rect& b) noexcept {
    const auto hit=[&](I32 x,I32 y){
        return b.left<=x&&x<=b.right&&b.top<=y&&y<=b.bottom;
    };
    return hit(a.left,a.top)||hit(a.right,a.top)||
           hit(a.left,a.bottom)||hit(a.right,a.bottom);
}

// 00406E10: first fopen(relative,mode), optional CD root retry if failed,
// AbortForRemovedRetailCD only if both attempts fail. Pointer-shaped file
// supplied by real stdio adapter; no synthetic file handle is invented.
using OpenGameFile=void* (*)(std::string_view,std::string_view) noexcept;
[[nodiscard]] inline void* open_game_data_with_cd_fallback(
    std::string_view path,std::string_view mode,
    std::string_view cd_root,OpenGameFile open,AbortMissingDisc abort) noexcept {
    if(!open)return nullptr;
    void* file=open(path,mode);
    if(!file&&!cd_root.empty()) {
        const std::string fallback=std::string(cd_root)+std::string(path);
        file=open(fallback,mode);
    }
    if(!file&&abort)abort();
    return file;
}

// 0041CBE0 and 00423220 share reset of three graph-search globals and
// delegate to the title-specific recursive search. The traversal is a
// callback so an actual algorithm must be provided, not a fabricated path.
struct MazeSearchState {
    I32 count{};
    I32 best_distance{};
    I32 requested_goal{};
};
using RecursiveMazeSearch=std::uint64_t (*)(I32,void*,void*,I32,I32,I32) noexcept;
[[nodiscard]] inline std::uint64_t find_maze_shortest_path(
    MazeSearchState& s,I32 target,void* graph,I32 goal,
    RecursiveMazeSearch search) noexcept {
    s.count=0;s.best_distance=9999999;s.requested_goal=goal;
    return search ? search(target,graph,nullptr,target,0,0) : 0;
}
[[nodiscard]] inline std::uint64_t build_spud_maze_graph_path(
    MazeSearchState& s,I32 target,void* graph,I32 goal,
    RecursiveMazeSearch search) noexcept {
    s.count=0;s.best_distance=9999999;s.requested_goal=goal;
    return search ? search(target,graph,nullptr,target,0,0) : 0;
}

// 00409210 plus 004093C0 global game-volume routing; the existing game
// volume is supplied and original callbacks are invoked in program order.
struct GlobalGameAudio {
    I32 ui_context{};
    std::span<void* const> activity_sounds{};
    void* ui_sound{};
    void* background_music{};
    void* help_sound{};
    void* bink_movie{};
    bool paused{};
    bool music_looping{};
    I32 game_volume{};
};
using SetSoundVolume=void (*)(void*,I32) noexcept;
inline void apply_global_game_volume(
    const GlobalGameAudio& s,StopSound stop,
    SetSoundVolume set_volume,PlaySound play,
    SetMovieVolume set_movie_volume) noexcept {
    if(s.ui_context<12) {
        if(s.ui_sound&&set_volume)set_volume(s.ui_sound,s.game_volume);
    } else if(s.ui_context==12) {
        for(auto* sound:s.activity_sounds) {
            if(!sound)continue;
            if(stop)stop(sound);
            if(set_volume)set_volume(sound,s.game_volume);
        }
    }
    if(s.background_music) {
        if(stop)stop(s.background_music);
        if(set_volume)set_volume(s.background_music,s.game_volume);
        if(play)play(s.background_music,0,s.music_looping?1U:0U);
    }
    apply_global_bink_volume(s.bink_movie,s.paused,
                             s.game_volume,set_movie_volume);
    // Retail calls CSound_SetVolume on global help sound without null guard.
    if(set_volume)set_volume(s.help_sound,s.game_volume);
}

} // namespace btb::retail_media
