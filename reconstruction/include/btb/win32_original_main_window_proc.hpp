#pragma once
// PE32 004020D0: original Bob Builds a Park MainWindowProc.
// Physical Win32 message effects; optional callbacks make missing DirectInput,
// cursor and contextual help providers explicit rather than fabricating them.
#if defined(_WIN32)
#include "btb/win32_original_display_lifecycle.hpp"
#include <optional>
#include <string>
#include <cstdint>
namespace btb::original_win32 {
struct RetailWindowState {
 bool windowed{true}; bool accepting_input{true};
 std::uint32_t outer_state{}; std::uint32_t help_active{};
 std::uint32_t help_pending{}; std::uint32_t help_phase{};
 std::uint32_t last_input_ms{};
 RECT previous_window_rect{};RECT destination{};
};
using WindowService=void (*)(void*) noexcept;
struct WindowServices {
 WindowService acquire_input{},advance_loading_cursor{},
               draw_loading_cursor{},contextual_help{};
 void* context{};
};
[[nodiscard]] std::optional<LRESULT> main_window_proc_recompiled(
 HWND hwnd,UINT msg,WPARAM wp,LPARAM lp,RetailWindowState& state,
 full_game::Win32OriginalDirectDraw& graphics,std::string& error,
 WindowServices services={});
}
#endif
