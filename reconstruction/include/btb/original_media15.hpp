#pragma once
// Original 15-function Bink/game-media module. Definitions live in C++
// translation units and are linked into the native game executable.
// Recovered from PE32 Ghidra functions. The real Bink/COM providers remain
// injected; this is not a fabricated decoder or original x86 ABI parity.
#include "btb/retail_media_game_closure.hpp"
namespace btb::original_media15 {
using namespace btb::retail_media;
void apply_global_bink_volume(void*,bool,I32,SetMovieVolume) noexcept; // 00409210
[[nodiscard]] void* open_bink_movie_with_fallback(
    std::string_view,std::string_view,OpenBink,void*,bool,I32,
    SetMovieVolume) noexcept; // 00408E10
[[nodiscard]] bool open_global_bink_movie(
    GlobalBink&,std::string_view,std::string_view,OpenBink,
    AbortMissingDisc,DisableInput,I32,SetMovieVolume) noexcept; // 00408EB0
void initialize_bink_playback(
    BinkInitialize&,MakeOffscreenSurface,BindBinkSoundSystem,void*) noexcept; // 00408D90
[[nodiscard]] I32 decode_bink_frame_without_advance(
    void*,void*,NoAdvanceDecode,NoAdvanceLock,NoAdvanceCopy,NoAdvanceUnlock) noexcept; // 00409070
void play_activity_music_by_index(ActivityMusic&,I32,
    StopSound,DeleteSound,CreateMusic,PlaySound) noexcept; // 00428010
void close_walkthrough_movie(Walkthrough&,VoidCallback,VoidCallback,
    VoidCallback,ResetScroll) noexcept; // 004280D0
[[nodiscard]] bool update_scene_set_movie(SceneSetMovie&,
    OpenSceneMovie,UpdateMovie) noexcept; // 0042A240
[[nodiscard]] bool update_startup_movies(StartupMovies&,
    std::span<const I32>,std::span<const std::string_view>,
    OpenSceneMovie,UpdateMovie) noexcept; // 0042A1A0
[[nodiscard]] HelpHoverResult update_generic_help_hover(HelpHover&) noexcept; // 00428470
[[nodiscard]] bool any_rect_corner_inside(const Rect&,const Rect&) noexcept; // 004169A0
[[nodiscard]] void* open_game_data_with_cd_fallback(
    std::string_view,std::string_view,std::string_view,
    OpenGameFile,AbortMissingDisc) noexcept; // 00406E10
[[nodiscard]] std::uint64_t find_maze_shortest_path(
    MazeSearchState&,I32,void*,I32,RecursiveMazeSearch) noexcept; // 0041CBE0
[[nodiscard]] std::uint64_t build_spud_maze_graph_path(
    MazeSearchState&,I32,void*,I32,RecursiveMazeSearch) noexcept; // 00423220
void apply_global_game_volume(
    const GlobalGameAudio&,StopSound,SetSoundVolume,PlaySound,SetMovieVolume) noexcept; // 004093C0
[[nodiscard]] bool verify_15_original_media_functions() noexcept;
}
