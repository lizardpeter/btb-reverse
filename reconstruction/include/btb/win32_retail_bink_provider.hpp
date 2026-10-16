#pragma once

#if defined(_WIN32)
#include "btb/win32_original_bink_exports.hpp"
#include "btb/win32_original_directdraw.hpp"
#include "btb/win32_original_game_device.hpp"

#include <filesystem>
#include <string>
#include <utility>

namespace btb::full_game {

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
    [[nodiscard]] bool render_walkthrough_overlay(
        std::string& error) override;

private:
    [[nodiscard]] bool ensure_original_dll(std::string& error);
    void close_global() noexcept;
    Win32OriginalDirectDraw& renderer_;
    std::filesystem::path root_;
    Win32OriginalBinkExports exports_{};
    void* global_{};
};

} // namespace btb::full_game
#endif
