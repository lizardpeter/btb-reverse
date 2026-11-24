#include "btb/application_state.hpp"

#include <cassert>

using namespace btb::application;

int main() {
    static_assert(static_cast<int>(ShutdownPhase::Running) == 0);
    static_assert(static_cast<int>(ShutdownPhase::BeginCredits) == 1);
    static_assert(static_cast<int>(ShutdownPhase::CreditsPlaying) == 2);
    static_assert(static_cast<int>(ShutdownPhase::FinalTeardown) == 3);

    static_assert(static_cast<int>(DisplayMode::Fullscreen) == 0);
    static_assert(static_cast<int>(DisplayMode::Windowed) == 1);

    static_assert(frame_active_for_size_state(0));
    static_assert(!frame_active_for_size_state(1));
    static_assert(frame_active_for_size_state(2));
    static_assert(frame_active_for_size_state(3));
    static_assert(!frame_active_for_size_state(4));

    static_assert(install_system_key_hook(DisplayMode::Fullscreen));
    static_assert(!install_system_key_hook(DisplayMode::Windowed));

    static_assert(is_running(ShutdownPhase::Running));
    static_assert(!is_running(ShutdownPhase::BeginCredits));

    static_assert(credits_are_active(ShutdownPhase::BeginCredits));
    static_assert(credits_are_active(ShutdownPhase::CreditsPlaying));
    static_assert(!credits_are_active(ShutdownPhase::Running));
    static_assert(!credits_are_active(ShutdownPhase::FinalTeardown));

    static_assert(!should_final_teardown(ShutdownPhase::CreditsPlaying));
    static_assert(should_final_teardown(ShutdownPhase::FinalTeardown));

    static_assert(should_run_active_frame(true, ShutdownPhase::Running));
    static_assert(should_run_active_frame(true, ShutdownPhase::CreditsPlaying));
    static_assert(!should_run_active_frame(false, ShutdownPhase::Running));
    static_assert(!should_run_active_frame(true, ShutdownPhase::FinalTeardown));

    static_assert(kRetailWidth == 640);
    static_assert(kRetailHeight == 480);
    static_assert(kRetailFullscreenBitsPerPixel == 16);
    static_assert(kCdValidationFrameInterval == 100);
}
