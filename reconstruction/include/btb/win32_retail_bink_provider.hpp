#pragma once

#if defined(_WIN32)
#include "btb/win32_original_bink_exports.hpp"
#include "btb/win32_original_directdraw.hpp"
#include "btb/win32_original_game_device.hpp"

#include <filesystem>
#include <string>
#include <vector>
#include <cstdint>
#include <utility>

namespace btb::full_game {

// Retail PE32 `0x00428195..0x004281A5` pushes this exact
// walkthrough blit origin and `0x004281D0` initializes the
// independent DD7 playback surface at 282x179.
inline constexpr int kOriginalWalkthroughMovieX=0x3f;
inline constexpr int kOriginalWalkthroughMovieY=0x9d;
inline constexpr int kOriginalWalkthroughMovieWidth=282;
inline constexpr int kOriginalWalkthroughMovieHeight=179;

// Interim retail-DLL-compatible playback: the native Bink
// reconstruction can replace this provider without changing GameRoot.
// The 2002 binkw32.dll is x86. x64 must never LoadLibrary it.
class Win32RetailBinkProvider final : public OriginalBinkProvider {
public:
    Win32RetailBinkProvider(
        Win32OriginalDirectDraw& renderer,
        std::filesystem::path installed_retail_root)
        : renderer_(renderer),root_(std::move(installed_retail_root)) {}
    ~Win32RetailBinkProvider() override;
    [[nodiscard]] bool service(bool& global_finished,
                               std::string& error) override;
    [[nodiscard]] bool open_global(
        const std::filesystem::path& movie,std::string& error) override;
    [[nodiscard]] bool open_walkthrough(
        const std::filesystem::path& movie,
        const std::filesystem::path& help_wav,
        std::string& error) override;
    [[nodiscard]] bool close_walkthrough(std::string& error) override;
    // Fireworks pre-show state 15 uses the same retail Bink disk player.
    // On an activity leave before EOS, cancel it before entering menus.
    void cancel_global_movie() noexcept {close_global();}
    [[nodiscard]] bool render_walkthrough_overlay(
        std::string& error) override;

private:
    [[nodiscard]] bool ensure_original_dll(std::string& error);
    [[nodiscard]] bool decode_walkthrough_frame(std::string& error);
    void close_global() noexcept;
    void release_walkthrough() noexcept;
    Win32OriginalDirectDraw& renderer_;
    std::filesystem::path root_;
    Win32OriginalBinkExports exports_{};
    void* global_{};
    // Tracks actual BinkDoFrame + DirectDraw pixel copy, not just HBINK
    // reaching Frames. Completion is returned on the next host tick.
    bool global_terminal_frame_copied_{};
    // Original 0x4281D0 owns the Bink movie's memory until close,
    // a separate 282x179 DD7 surface, and a looping Bink handle.
    std::vector<std::uint8_t> walkthrough_bytes_{};
    IDirectDrawSurface7* walkthrough_surface_{};
    void* walkthrough_{};
    bool walkthrough_has_frame_{};
    std::uint32_t walkthrough_last_decoded_{};
    std::filesystem::path walkthrough_help_wav_{};

};

} // namespace btb::full_game
#endif
