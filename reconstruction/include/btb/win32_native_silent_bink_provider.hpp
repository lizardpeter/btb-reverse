#pragma once
// Windows/DirectDraw7 host for the reconstructed native Bink1 VIDEO codec.
// No RAD DLL. Movies with any compressed-audio tracks are refused; the
// production retail-DLL fallback must handle those until native audio ships.
#if defined(_WIN32)
#include "btb/bink32_native_movie_player.hpp"
#include "btb/bink32_audio_master_clock.hpp"
#include "btb/win32_native_bink_pcm_voice.hpp"
#include "btb/win32_original_game_device.hpp"
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

namespace btb::full_game {
class Win32NativeSilentBinkProvider final : public OriginalBinkProvider {
public:
    Win32NativeSilentBinkProvider(
        Win32OriginalDirectDraw& renderer,
        Win32OriginalDirectSound& audio,
        bool experimental_audio=false)
        :renderer_(renderer),audio_(audio),
         experimental_audio_(experimental_audio) {}
    ~Win32NativeSilentBinkProvider() override;
    void reset_global() noexcept;
    [[nodiscard]] bool open_global(
        const std::filesystem::path& movie,std::string& error) override;
    [[nodiscard]] bool service(
        bool& global_finished,std::string& error) override;
    [[nodiscard]] bool open_walkthrough(
        const std::filesystem::path& movie,
        const std::filesystem::path& help_wav,
        std::string& error) override;
    [[nodiscard]] bool close_walkthrough(std::string& error) override;
    [[nodiscard]] bool render_walkthrough_overlay(
        std::string& error) override;
private:
    void release_walkthrough() noexcept;
    Win32OriginalDirectDraw& renderer_;
    Win32OriginalDirectSound& audio_;
    bool experimental_audio_{};
    std::unique_ptr<Win32NativeBinkPcmVoice> voice_{};
    bool voice_started_{};
    std::optional<bink32_recovered::Bink1AudioMasterClock>
        audio_clock_{};
    std::optional<bink32_recovered::BinkNativeSilentMovie> global_;
    std::optional<bink32_recovered::BinkNativeSilentMovie> walkthrough_;
    IDirectDrawSurface7* walkthrough_surface_{};
};
}
#endif
