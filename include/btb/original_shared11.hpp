#pragma once
// Eleven source-derived native C++ PE32 runtime functions. 32-bit DirectInput,
// DirectSound, Bink and COM are real callbacks and explicit state models;
// original retail binary behavior and Win32 x86 ABI remain unverified.
#include "btb/retail_shared_runtime.hpp"
namespace btb::original_shared11 {
using namespace btb::retail_shared;
// 00407AF0 SetCursorSurface
void set_cursor_surface(CursorSurfaceState&,void*,GetSurfaceDescription) noexcept;
// 00407960 BeginLoadingCursorAnimation
void begin_loading_cursor(BeginLoadingState&,SoundCommand,SoundCommand) noexcept;
// 00407E10 UpdateDirectInputAcquireState
[[nodiscard]] btb::input::AcquireStateStep direct_input_acquire_step(bool,bool) noexcept;
// 004280A0 StopActivityMusic
void stop_activity_music(void*&,SoundCommand,DeleteSound) noexcept;
// 00402CC0 StopSoundSlot
void stop_sound_slot(btb::sound::RetailSoundManager32&,std::size_t,void*,
                     SoundCommand,SoundCommand) noexcept;
// 00402BC0 ReleaseSoundSlot
void release_sound_slot(btb::sound::RetailSoundManager32&,std::size_t,void*,
                        DeleteSound) noexcept;
// 00409260 PauseGlobalBinkMovie
void pause_global_movie(GlobalMovieState&,MoviePause,MovieVolume) noexcept;
// 004092A0 ResumeGlobalBinkMovie
void resume_global_movie(GlobalMovieState&,MoviePause,MovieVolumeRestore) noexcept;
// 00408F50 CloseGlobalBinkMovie
void close_global_movie(GlobalMovieState&,MovieClose) noexcept;
// 00403BC0 CSoundManager_Destructor
void destruct_sound_manager(btb::dsutil::RetailCSoundManager32&,void*,
                            ReleaseCom) noexcept;
// 004043F0 CSound_SetVolume
[[nodiscard]] std::uint32_t sound_set_volume(
    const btb::dsutil::RetailCSound32&,std::span<const SoundVolumeBuffer>,
    std::int32_t) noexcept;
[[nodiscard]] bool verify_11_shared_native_functions() noexcept;
}
