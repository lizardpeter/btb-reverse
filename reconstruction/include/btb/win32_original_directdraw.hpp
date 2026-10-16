#pragma once

// This component is the actual Windows/DirectDraw7 implementation boundary,
// not another browser/HTML preview. It is deliberately a separate source
// unit until the complete original-game executable build is enabled.
//
// Requires the Win32 SDK, ddraw.h, ddraw.lib/dxguid.lib and a real HWND.
// The original software-bitmaps are loaded from installed/CD paths, never
// replaced with generated artwork. All COM releases are owned here.
#if defined(_WIN32)

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <ddraw.h>

#include "btb/full_game_resources.hpp"
#include "btb/display_manager.hpp"
#include "btb/win32_native_bitmap_registry.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace btb::full_game {

class Win32OriginalDirectDraw final {
public:
    Win32OriginalDirectDraw() = default;
    ~Win32OriginalDirectDraw();
    Win32OriginalDirectDraw(const Win32OriginalDirectDraw&) = delete;
    Win32OriginalDirectDraw& operator=(
        const Win32OriginalDirectDraw&) = delete;

    [[nodiscard]] bool initialize(
        HWND hwnd,display::DisplayMode mode,std::string& error);
    [[nodiscard]] bool render_ordered(
        const std::vector<ResolvedDraw>& draw_stream,
        std::string& error);
    [[nodiscard]] bool present(std::string& error);
    [[nodiscard]] bool print_frame(
        const std::filesystem::path& bitmap,std::string& error);
    // Copy already decoded retail Bink BGRA video pixels into the actual
    // DirectDraw7 backbuffer between ordered draw and present.
    [[nodiscard]] bool blit_decoded_bink32(
        const void* pixels,unsigned width,unsigned height,int x,int y,
        std::string& error);
    // Direct original 00402440 cooperative test and loss recovery.
    [[nodiscard]] HRESULT cooperative_status() const noexcept;
    [[nodiscard]] bool restore_for_frame(std::string& error);
    [[nodiscard]] HRESULT clear_retail_backbuffer(DWORD color) noexcept;
    void shutdown() noexcept;

    [[nodiscard]] bool initialized() const noexcept {
        return draw_ != nullptr && primary_ != nullptr &&
               backbuffer_ != nullptr;
    }

private:
    struct OriginalSurface {
        IDirectDrawSurface7* surface{};
        int width{};
        int height{};
    };
    [[nodiscard]] bool load_bitmap(
        const std::filesystem::path& file,
        OriginalSurface& out,std::string& error);
    [[nodiscard]] bool blit(
        const ResolvedDraw& draw,std::string& error);
    [[nodiscard]] bool restore_lost_surfaces(std::string& error);
    void release_cached_bitmaps() noexcept;

    HWND hwnd_{};
    display::DisplayMode mode_{display::DisplayMode::Windowed};
    IDirectDraw7* draw_{};
    IDirectDrawSurface7* primary_{};
    IDirectDrawSurface7* backbuffer_{};
    IDirectDrawClipper* clipper_{};
    bool extra_back_reference_{};
    std::unordered_map<std::wstring,OriginalSurface> images_{};
    std::unique_ptr<original_win32::NativeBitmapRegistry> registry_{
        std::make_unique<original_win32::NativeBitmapRegistry>()};
    std::vector<ResolvedDraw> previous_frame_{};
    bool surface_lost_during_draw_{};
};

} // namespace btb::full_game
#endif
