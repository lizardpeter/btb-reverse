#pragma once
// x64 uses only the reconstructed Bink1 video/audio implementation.
// x86 retains the retail decoder for comparison and optional native trials.
// No fake track muting, movie-completion timers or synthetic frames.
#if defined(_WIN32)
#include "btb/win32_native_silent_bink_provider.hpp"
#include "btb/win32_retail_bink_provider.hpp"
#include <filesystem>
#include <string>
#include <utility>

namespace btb::full_game {
class Win32HybridBinkProvider final : public OriginalBinkProvider {
public:
    Win32HybridBinkProvider(Win32OriginalDirectDraw& renderer,
                            Win32OriginalDirectSound& audio,
                            std::filesystem::path root,
                            bool experimental_audio=false,
                            bool experimental_video=false)
        : native_(renderer,audio,sizeof(void*)==8||experimental_audio),
          retail_(renderer,std::move(root)),
          native_enabled_(sizeof(void*)==8||experimental_video||experimental_audio) {}
    void cancel_global_movie() noexcept;
    [[nodiscard]] bool close_global(std::string& error) override;
    [[nodiscard]] bool open_global(
        const std::filesystem::path& movie,
        std::string& error) override;
    [[nodiscard]] bool service(
        bool& global_finished,std::string& error) override;
    [[nodiscard]] bool open_walkthrough(
        const std::filesystem::path& movie,
        std::string& error) override;
    [[nodiscard]] bool close_walkthrough(
        std::string& error) override;
    [[nodiscard]] bool render_walkthrough_overlay(
        std::string& error) override;
private:
    enum class Backend { none,native,retail };
    Win32NativeSilentBinkProvider native_;
    Win32RetailBinkProvider retail_;
    Backend global_{Backend::none},walkthrough_{Backend::none};
    bool native_enabled_{};
};
}
#endif
