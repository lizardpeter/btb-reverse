#pragma once
// Native C++ reconstructions of Bink32 timing/audio state derived from actual
// x86 code at 0x30003CB0, 0x30006DB0 and 0x3000668A.
// Codec and platform callbacks remain explicit, not fabricated.
#include <cstdint>
#include <span>

namespace btb::bink32_recovered {

using SoundOpenCallback=std::uintptr_t(*)(std::uintptr_t argument,void* user);
struct SoundSystemState {
    // Original global state: 30054050 factory, 30054054 handle,
    // 30054058 locked flag.
    std::uintptr_t factory_identity{}, opened_handle{};
    bool locked{};
};
[[nodiscard]] bool set_sound_system(
    SoundSystemState& state,std::uintptr_t factory_identity,
    std::uintptr_t argument,SoundOpenCallback callback,void* user);

struct PauseTelemetry {
    // Addresses: +0xC current frame; +0xFC paused; +0x270 audio clock;
    // +0x27C pause tick; +0x280 clock origin; +0x28C interval tick;
    // +0x290/298 longest/second-longest intervals; +0x294/29C frames;
    // +0x2B4 paused-tick accumulator; +0x2F0 track count;
    // +0x330 lag accumulator.
    std::uint32_t current_frame{}, paused{}, audio_clock{};
    std::uint32_t pause_tick{}, clock_origin{}, interval_tick{};
    std::uint32_t largest_interval{}, largest_frame{};
    std::uint32_t second_interval{}, second_frame{};
    std::uint32_t accumulated_paused_ticks{}, lag_ticks{};
    std::uint32_t track_count{};
};
struct TrackPauseCallback {
    void (*invoke)(void* user,std::uint32_t paused){};
    void* user{};
};
struct PauseRuntime {
    std::span<const TrackPauseCallback> tracks{};
    void (*service_audio)(void* user){};
    void* user{};
};
// Source model of original _BinkPause@8, tracking the real clocks,
// interval ranking, pause state and per-track callback invocations.
// Original 0x30004E00 service provider is caller-supplied.
[[nodiscard]] std::uint32_t pause(
    PauseTelemetry* movie,std::uint32_t requested_paused,
    std::uint32_t clock_now,const PauseRuntime& runtime);

// Last section of _BinkNextFrame@4 at 0x3000668A..0x300066A1,
// extracted as a separately testable component. Audio services and frame
// binding are still needed for the full original exported routine.
[[nodiscard]] std::uint32_t next_frame_number(
    std::uint32_t current,std::uint32_t frame_count) noexcept;

struct SeekPreflight {
    std::uint32_t requested_frame{}, first_decode_frame{};
    bool use_direct_seek{}, already_at_frame{};
};
// Original _BinkGoto@12 prologue at 0x300067A0..0x30006846.
// This is ONLY its clamp/keyframe/pre-roll decision, not decode iteration.
[[nodiscard]] SeekPreflight plan_seek(
    std::uint32_t total_frames,std::uint32_t current_frame,
    std::uint32_t target_frame,std::uint32_t flags,
    std::uint32_t audio_tracks,std::uint32_t fps_numerator,
    std::uint32_t fps_denominator,std::uint32_t previous_key_frame) noexcept;
} // namespace btb::bink32_recovered
