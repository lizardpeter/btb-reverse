#pragma once
// Reconstructed executable functions 004035F0, 004038D0 and 00415E50.
// Source: analysis/ghidra_main_exe/functions/{004035F0,004038D0,00415E50}.c
// These functions issue actual Win32 GDI and DirectDraw7 operations.
// The retail CD fallback and global 0x4439A8 error dispatcher remain
// external dependencies; they must not be mistaken for verified x86 parity.
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <ddraw.h>
#include <filesystem>

namespace btb::original_win32 {

// 004035F0 CopyBitmapToSurface.
// Copies the actual HBITMAP into the destination DirectDraw surface;
// caller owns the HBITMAP and destination COM reference.
[[nodiscard]] HRESULT copy_bitmap_to_surface(
    IDirectDrawSurface7* destination,HBITMAP source) noexcept;

struct LoadedBitmap {
    IDirectDrawSurface7* surface{};
    LONG width{};
    LONG height{};
};

// 004038D0 LoadBitmapToDirectDrawSurface.
// The caller first resolves the original installed/CD filename to a file.
// Honors retail offscreen/system-memory retry, consumes no fabricated data,
// and delegates actual bitmap upload to 004035F0.
[[nodiscard]] HRESULT load_bitmap_to_directdraw_surface(
    IDirectDraw7* directdraw,const std::filesystem::path& source,
    LoadedBitmap& out) noexcept;

// 00415E50 BlitColorKeyedSurfaceClipped.
// Applies original screen-space camera offsets and 640x480 cropping;
// issues the actual DirectDraw keyed Blt operation (DDBLT_KEYSRC|WAIT).
[[nodiscard]] HRESULT blit_color_keyed_surface_clipped(
    IDirectDrawSurface7* destination,IDirectDrawSurface7* source,
    int screen_x,int screen_y,const RECT* source_rectangle,
    int camera_x,int camera_y) noexcept;

} // namespace btb::original_win32
#endif
