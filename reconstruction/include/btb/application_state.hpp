#pragma once

#include <cstdint>

namespace btb::application {

// Global 0x0044DDB0. This is an exit/credits phase, not a normal game-flow
// state. Normal activity routing lives separately at 0x0044DE14.
enum class ShutdownPhase : std::int32_t {
    Running = 0,
    BeginCredits = 1,
    CreditsPlaying = 2,
    FinalTeardown = 3,
};

// Global 0x0044DE0C, passed directly to CreateOrResetDisplayManager.
// Zero selects 640x480x16 exclusive fullscreen; nonzero selects windowed.
enum class DisplayMode : std::int32_t {
    Fullscreen = 0,
    Windowed = 1,
};

// Global 0x0044DE10. WM_SIZE clears this for SIZE_MINIMIZED (1) and
// SIZE_MAXHIDE (4); all other observed size states enable active frames.
[[nodiscard]] constexpr bool frame_active_for_size_state(
    std::uint32_t size_state) noexcept {
    return size_state != 1U && size_state != 4U;
}

[[nodiscard]] constexpr bool install_system_key_hook(
    DisplayMode mode) noexcept {
    return mode == DisplayMode::Fullscreen;
}

[[nodiscard]] constexpr bool is_running(
    ShutdownPhase phase) noexcept {
    return phase == ShutdownPhase::Running;
}

[[nodiscard]] constexpr bool credits_are_active(
    ShutdownPhase phase) noexcept {
    return phase == ShutdownPhase::BeginCredits ||
           phase == ShutdownPhase::CreditsPlaying;
}

[[nodiscard]] constexpr bool should_final_teardown(
    ShutdownPhase phase) noexcept {
    return static_cast<std::int32_t>(phase) >=
           static_cast<std::int32_t>(ShutdownPhase::FinalTeardown);
}

// The main loop uses WaitMessage instead of running/presenting a frame while
// the window is minimized/hidden.
[[nodiscard]] constexpr bool should_run_active_frame(
    bool frame_active,
    ShutdownPhase phase) noexcept {
    return frame_active && !should_final_teardown(phase);
}

inline constexpr std::uint32_t kRetailWidth = 640;
inline constexpr std::uint32_t kRetailHeight = 480;
inline constexpr std::uint32_t kRetailFullscreenBitsPerPixel = 16;
inline constexpr std::uint32_t kCdValidationFrameInterval = 100;

} // namespace btb::application
