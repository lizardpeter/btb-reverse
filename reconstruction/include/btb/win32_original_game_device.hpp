#pragma once

#if defined(_WIN32)
#include "btb/full_game_native_session.hpp"
#include "btb/win32_original_directdraw.hpp"
#include "btb/win32_original_directsound.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace btb::full_game {

// The original imported binkw32.dll decoder still needs the exact binary
// ABI/pixel-buffer implementation. This is deliberately a required service,
// not a no-op "movie completed" shim.
//
// service() must decode global frames into the actual DirectDraw render
// surface before global-mode presents. For instruction walkthroughs,
// render_walkthrough_overlay() must blit the looping walkthrough AFTER the
// instruction background and UI layers, not before draw_ordered clears them.
class OriginalBinkProvider {
public:
    virtual ~OriginalBinkProvider() = default;
    [[nodiscard]] virtual bool service(
        bool& global_finished,std::string& error) = 0;
    [[nodiscard]] virtual bool open_global(
        const std::filesystem::path& movie,std::string& error) = 0;
    [[nodiscard]] virtual bool open_walkthrough(
        const std::filesystem::path& movie,
        const std::filesystem::path& help_wav,
        std::string& error) = 0;
    [[nodiscard]] virtual bool close_walkthrough(
        std::string& error) = 0;
    [[nodiscard]] virtual bool render_walkthrough_overlay(
        std::string& error) = 0;
};

// Concrete Win32 composition: original UI/gameplay bitmap surfaces through
// DirectDraw7, exact managed sound observations/commands through DirectSound8,
// with a mandatory full Bink provider. A missing Bink provider is not replaced
// by an elapsed-time counter or skipped video.
class Win32OriginalGameDevice final : public NativeGameDevice {
public:
    Win32OriginalGameDevice(
        Win32OriginalDirectDraw& draw,
        Win32OriginalDirectSound& audio,
        OriginalBinkProvider& bink)
        : draw_(draw),audio_(audio),bink_(bink) {}

    [[nodiscard]] bool observe(
        PhysicalFrameObservation& out,std::string& error) override;
    [[nodiscard]] bool open_global_movie(
        const std::filesystem::path& movie,std::string& error) override;
    [[nodiscard]] bool open_walkthrough_movie(
        const std::filesystem::path& movie,
        const std::filesystem::path& spoken_help_wav,
        std::string& error) override;
    [[nodiscard]] bool close_walkthrough_movie(
        std::string& error) override;
    [[nodiscard]] bool apply_sound(
        const ResolvedSoundEffect& effect,
        std::string& error) override;
    [[nodiscard]] bool draw_ordered(
        const std::vector<ResolvedDraw>& draws,
        std::string& error) override;
    [[nodiscard]] bool present(std::string& error) override;

private:
    Win32OriginalDirectDraw& draw_;
    Win32OriginalDirectSound& audio_;
    OriginalBinkProvider& bink_;
};

} // namespace btb::full_game
#endif
