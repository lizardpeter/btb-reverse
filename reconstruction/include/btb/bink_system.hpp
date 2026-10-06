#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace btb::bink {

inline constexpr std::int32_t kRetailWidth = 640;
inline constexpr std::int32_t kRetailHeight = 480;

inline constexpr std::uint32_t kSurfaceDescSize = 0x7C;
inline constexpr std::uint32_t kSurfaceDescFlags = 0x00000007;
inline constexpr std::uint32_t kOffscreenPlainCaps = 0x00000040;

inline constexpr std::uint32_t kGenericOpenFlags = 0x04000000;
inline constexpr std::uint32_t kGlobalOpenFlags = 0x00000000;
inline constexpr std::size_t kRetailFallbackPathBufferBytes = 100;

struct PlaybackSurfaceSpec {
    std::uint32_t descriptor_size{kSurfaceDescSize};
    std::uint32_t descriptor_flags{kSurfaceDescFlags};
    std::int32_t height{kRetailHeight};
    std::int32_t width{kRetailWidth};
    std::uint32_t surface_caps{kOffscreenPlainCaps};
};

inline constexpr PlaybackSurfaceSpec kSharedPlaybackSurface{};

enum class FrameMode : std::int32_t {
    // Return complete at end-of-stream or on any of the three shared input
    // pulses. This is the ordinary skippable one-shot path.
    SkippableOneShot = 0,

    // Do not terminate on end/input inside DecodeAndBlitBinkFrame. Retail calls
    // BinkNextFrame on every successful update, including the first.
    Looping = 1,

    // Ignore skip input, but return complete when FrameNum == Frames.
    UnskippableOneShot = 2,
};

struct InputPulses {
    bool primary{};
    bool secondary{};
    bool tertiary{};

    [[nodiscard]] constexpr bool any() const noexcept {
        return primary || secondary || tertiary;
    }
};

struct OpenPlan {
    std::string direct_path{};
    std::optional<std::string> fallback_path{};
    std::uint32_t flags{};
    bool apply_global_volume_after_attempt{};
    bool abort_if_all_attempts_fail{};
    bool disable_input_after_success{};
};

[[nodiscard]] std::optional<std::string> cd_fallback_path(
    std::string_view path,
    std::int32_t drive_index);

[[nodiscard]] OpenPlan generic_open_plan(
    std::string_view path,
    std::int32_t drive_index);

[[nodiscard]] OpenPlan global_open_plan(
    std::string_view path,
    std::int32_t drive_index);

struct GenericFrameInput {
    FrameMode mode{FrameMode::SkippableOneShot};
    std::int32_t frame_number{};
    std::int32_t frame_count{};
    InputPulses input{};
    bool frame_advance_latch{};
    bool surface_lock_failed{};
};

struct GenericFrameStep {
    bool decode_frame{true};
    bool lock_surface{true};
    bool copy_to_surface{};
    bool unlock_surface{};
    bool complete{};
    bool advance_frame{};
    bool wait_for_bink{};
    bool set_frame_advance_latch{};
    bool blit_to_backbuffer{};
};

// Source-level control contract of 0x00408F70 DecodeAndBlitBinkFrame after the
// BinkDoFrame call. Surface lock failure returns 1 immediately. Otherwise
// terminal checks occur after decode/copy/unlock and before BinkNextFrame.
[[nodiscard]] constexpr GenericFrameStep generic_frame_step(
    const GenericFrameInput& input) noexcept {

    GenericFrameStep step;

    if (input.surface_lock_failed) {
        step.complete = true;
        return step;
    }

    step.copy_to_surface = true;
    step.unlock_surface = true;

    const bool at_end = input.frame_number == input.frame_count;

    if (input.mode == FrameMode::SkippableOneShot &&
        (at_end || input.input.any())) {
        step.complete = true;
        return step;
    }

    if (input.mode == FrameMode::UnskippableOneShot && at_end) {
        step.complete = true;
        return step;
    }

    if (input.frame_advance_latch ||
        input.mode == FrameMode::Looping) {
        step.advance_frame = true;
        step.wait_for_bink = true;
    }

    step.set_frame_advance_latch = true;
    step.blit_to_backbuffer = true;
    return step;
}

struct GlobalFrameInput {
    std::int32_t frame_number{};
    std::int32_t frame_count{};
    InputPulses input{};
    bool input_skip_enabled{};
    bool surface_lock_failed{};
};

struct GlobalFrameStep {
    bool decode_frame{true};
    bool lock_surface{true};
    bool copy_to_surface{};
    bool unlock_surface{};
    bool close_movie{};
    bool complete{};
    bool advance_frame{};
    bool wait_for_bink{};
    bool blit_shared_surface_at_origin{};
};

// 0x004090D0 UpdateGlobalBinkMovie. Unlike the generic helper, the shared
// movie always advances/waits after a nonterminal decoded frame and then blits
// its 640x480 shared surface at (0,0).
[[nodiscard]] constexpr GlobalFrameStep global_frame_step(
    const GlobalFrameInput& input) noexcept {

    GlobalFrameStep step;

    if (input.surface_lock_failed) {
        step.complete = true;
        return step;
    }

    step.copy_to_surface = true;
    step.unlock_surface = true;

    if (input.frame_number == input.frame_count ||
        (input.input_skip_enabled && input.input.any())) {
        step.close_movie = true;
        step.complete = true;
        return step;
    }

    step.advance_frame = true;
    step.wait_for_bink = true;
    step.blit_shared_surface_at_origin = true;
    return step;
}

inline constexpr std::int32_t kMinimumBinkVolume = 37;
inline constexpr double kBinkVolumeScaleA = 31000.0;
inline constexpr double kBinkVolumeScaleB = 0.01;

// Exact 0x00409210 conversion. Values below 5 are forced directly to 37.
// Otherwise retail truncates game_volume * 31000 * 0.01 toward zero and then
// clamps the result to at least 37.
[[nodiscard]] constexpr std::int32_t bink_volume_from_game_volume(
    std::int32_t game_volume) noexcept {

    if (game_volume < 5) {
        return kMinimumBinkVolume;
    }

    auto value = static_cast<std::int32_t>(
        static_cast<double>(game_volume) *
        kBinkVolumeScaleA *
        kBinkVolumeScaleB);

    if (value < kMinimumBinkVolume) {
        value = kMinimumBinkVolume;
    }
    return value;
}

struct GlobalPlaybackState {
    bool has_movie{};
    bool paused{};
    bool input_processing_enabled{true};
};

struct PauseResumeStep {
    bool call_bink_pause{};
    bool pause_value{};
    bool set_volume{};
    std::int32_t volume{};
    bool input_processing_enabled{};
};

[[nodiscard]] constexpr PauseResumeStep pause_global_movie(
    GlobalPlaybackState& state) noexcept {

    if (!state.has_movie) {
        return {};
    }

    state.paused = true;
    state.input_processing_enabled = false;
    return {
        true,
        true,
        true,
        0,
        false,
    };
}

[[nodiscard]] constexpr PauseResumeStep resume_global_movie(
    GlobalPlaybackState& state,
    std::int32_t game_volume) noexcept {

    if (!state.has_movie) {
        return {};
    }

    state.paused = false;
    state.input_processing_enabled = true;
    return {
        true,
        false,
        true,
        bink_volume_from_game_volume(game_volume),
        true,
    };
}

[[nodiscard]] constexpr bool should_apply_global_volume(
    const GlobalPlaybackState& state) noexcept {
    return state.has_movie && !state.paused;
}

} // namespace btb::bink
