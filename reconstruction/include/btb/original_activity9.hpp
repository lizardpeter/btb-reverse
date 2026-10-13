#pragma once
// Nine individually compiled original activity, input, audio and Bink
// algorithms from Bob Builds a Park PE32. Original ABI/Windows providers
// are typed callbacks, never silent synthetic DirectDraw/Bink services.
#include "btb/retail_activity_closure.hpp"
namespace btb::original_activity9 {
using namespace btb::retail_activity;
[[nodiscard]] U32 reset_wave_file(
    btb::dsutil::RetailCWaveFile32&,void*,MmioSeek,
    MmioTask,MmioTask,MmioTask) noexcept; // 004048F0
[[nodiscard]] U32 restore_sound_buffer(
    void*,bool*,GetStatus,Restore,SleepMs,std::size_t=0) noexcept; // 00404270
[[nodiscard]] U32 sound_play(
    void*,bool,U32,U32,I32,ChooseSoundBuffer,RestoreSelected,
    FillSound,ResetSound,PlaySound,SoundVolume) noexcept; // 00404360
[[nodiscard]] I32 hit_test_bobs_band_regions(
    BandHitState&,std::span<const BandRegion>) noexcept; // 0041FB90
void debounce_maze_directions(MazeDebounce&,I32&,I32&) noexcept; // 0041AEB0
void update_squirrel_conveyor(
    SquirrelConveyor&,bool,GenerateConveyorChoices) noexcept; // 00427320
void translate_keyboard_key(U32,std::uint16_t*,GetKeyboardState,
    MapVirtualKey,ToAscii) noexcept; // 00408460
[[nodiscard]] bool seek_decode_bink_exact(
    void*,void*,I32,Keyframe,GotoFrame,MovieFrame,DecodeFrame,
    CopyFrame,DecodeFrame,std::size_t) noexcept; // 00424C80
void draw_park_pond_segments(
    I32,I32,I32,I32,I32,SegmentBlit) noexcept; // 0040D7D0
[[nodiscard]] bool verify_9_original_activity_functions() noexcept;
}
