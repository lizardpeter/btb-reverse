#pragma once
// PE32 00401FA0 and 00401F80. Live DirectDraw7 display lifetime and
// original fullscreen/windowed game-window initialization.
// Ghidra: analysis/ghidra_main_exe/functions/00401FA0.c,
//         analysis/ghidra_main_exe/functions/00401F80.c.
// Win32 API/resource effects are real; the object is an owning modern
// adapter rather than a byte-for-byte reinterpretation of PE32 heap data.
#if defined(_WIN32)
#include "btb/win32_original_directdraw.hpp"
#include <string>
namespace btb::original_win32 {
[[nodiscard]] HRESULT create_or_reset_display_manager(
    HWND window,display::DisplayMode mode,
    full_game::Win32OriginalDirectDraw& graphics,
    std::string& error);
[[nodiscard]] HRESULT initialize_display_for_game_window(
    HWND window,display::DisplayMode mode,
    full_game::Win32OriginalDirectDraw& graphics,
    std::string& error);
}
#endif
