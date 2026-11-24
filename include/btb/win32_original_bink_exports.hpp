#pragma once

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <array>
#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>

namespace btb::full_game {

// The original 2002 executable imports Win32 binkw32.dll. A 32-bit game
// DLL cannot be loaded in a 64-bit process: either build the original ABI
// bridge x86 or supply a separately verified compatible modern decoder.
// This loader only validates/imports the native exports. It does not
// guess the original BINK handle struct or claim to decode any frames.
struct OriginalBinkExport {
    std::string_view name{};
    unsigned stdcall_argument_bytes{};
    FARPROC proc{};
};

class Win32OriginalBinkExports final {
public:
    Win32OriginalBinkExports() = default;
    ~Win32OriginalBinkExports();
    Win32OriginalBinkExports(const Win32OriginalBinkExports&) = delete;
    Win32OriginalBinkExports& operator=(
        const Win32OriginalBinkExports&) = delete;

    [[nodiscard]] bool load_exact_original(
        const std::filesystem::path& full_dll_path,
        std::string& error);
    void unload() noexcept;

    [[nodiscard]] FARPROC find(std::string_view symbol) const noexcept;
    [[nodiscard]] bool loaded() const noexcept {
        return module_ != nullptr;
    }

private:
    HMODULE module_{};
    std::array<OriginalBinkExport,12> exports_{};
};

} // namespace btb::full_game
#endif
