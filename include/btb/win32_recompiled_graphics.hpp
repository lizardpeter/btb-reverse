#pragma once
// Direct, executable Win32 reconstructions of 00403500, 00403540, 00403590,
// 00403A70, 00403B70, 00402620, 00402600 and 00403480. They call the real DirectDraw7/GDI interfaces.
// Original PE32 calling conventions, registry side effects and retail
// differential parity are not certified by these host-friendly signatures.
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <ddraw.h>

namespace btb::original_win32 {
[[nodiscard]] HRESULT blt_fast_to_backbuffer(
    IDirectDrawSurface7* backbuffer,DWORD x,DWORD y,
    IDirectDrawSurface7* source,const RECT* source_rect,DWORD flags) noexcept;
[[nodiscard]] HRESULT clear_backbuffer(
    IDirectDrawSurface7* backbuffer,DWORD color) noexcept;
[[nodiscard]] bool update_display_destination_rect(
    HWND window,bool windowed,RECT& destination) noexcept;
[[nodiscard]] DWORD colorref_to_surface_pixel(
    IDirectDrawSurface7* surface,COLORREF color) noexcept;
[[nodiscard]] HRESULT set_surface_transparency_color_key(
    IDirectDrawSurface7* surface,COLORREF color) noexcept;
// PE32 00402620: original DirectDraw7::RestoreAllSurfaces wrapper.
// Returns zero for SUCCEEDED or original HRESULT for failures.
[[nodiscard]] HRESULT restore_all_directdraw_surfaces(
    IDirectDraw7* draw) noexcept;

// PE32 00403480: DirectDraw7 Flip/Blt and still-drawing retry with
// lost-surface restore sequence. True non-stub native COM method dispatch.
// The source's global reload latch is passed as an explicit reference.
[[nodiscard]] HRESULT present_display_retail(
    IDirectDrawSurface7* primary,IDirectDrawSurface7* render,
    IDirectDraw7* draw,const RECT* destination,
    bool windowed,bool& bitmap_reload) noexcept;

// PE32 00402600: game-frame wrapper normalizes successful HRESULT to S_OK.
// Both functions are wired into Win32OriginalDirectDraw::present.
[[nodiscard]] HRESULT present_game_frame_retail(
    IDirectDrawSurface7* primary,IDirectDrawSurface7* render,
    IDirectDraw7* draw,const RECT* destination,
    bool windowed,bool& bitmap_reload) noexcept;
} // namespace btb::original_win32
#endif
