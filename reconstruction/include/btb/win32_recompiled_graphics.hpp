#pragma once
// Direct, executable Win32 reconstructions of 00403500, 00403540, 00403590,
// 00403A70 and 00403B70. They call the real DirectDraw7/GDI interfaces.
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
} // namespace btb::original_win32
#endif
