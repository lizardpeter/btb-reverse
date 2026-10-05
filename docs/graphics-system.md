# Graphics / DirectDraw architecture

The game's generic renderer is a compact DirectDraw 7 wrapper plus a global bitmap-surface registry. It is conventional early-2000s 2D DirectX code and is now largely mapped.

## Global display object

The heap object referenced through global `0x0044DE08` is allocated as **0x30 bytes** by `0x00401FA0 CreateOrResetDisplayManager`.

Recovered layout:

| Offset | Type | Meaning |
|---:|---|---|
| `+0x00` | vtable | one virtual cleanup method at `0x00402FF0` |
| `+0x04` | `IDirectDraw7 *` | DirectDraw 7 interface |
| `+0x08` | `IDirectDrawSurface7 *` | primary/front surface |
| `+0x0C` | `IDirectDrawSurface7 *` | fullscreen backbuffer or windowed offscreen render surface |
| `+0x10` | `IDirectDrawSurface7 *` | optional/legacy auxiliary surface; initialized/released but no creation path identified yet |
| `+0x14` | `HWND` | game window |
| `+0x18` | `RECT` | destination/client rectangle in screen coordinates |
| `+0x28` | `BOOL` | 1 = windowed, 0 = exclusive fullscreen |
| `+0x2C` | unknown/reserved | not yet required by recovered paths |

The IID passed to `DirectDrawCreateEx` at `0x0043B568` decodes to:

`15e65ec0-3b9c-11d2-b92f-00609797ea5b`

which is `IID_IDirectDraw7`.

## Display creation

### `0x00401FA0 CreateOrResetDisplayManager`

This is the high-level wrapper used by the application. It destroys any existing display object, allocates 0x30 bytes, constructs it, and selects one of two paths:

- `InitializeWindowedDisplay(hwnd, 640, 480)`
- `InitializeFullscreenDisplay(hwnd, 640, 480, 16)`

It then clears the render surface.

### `0x00403080 InitializeFullscreenDisplay`

The fullscreen path:

1. calls `DirectDrawCreateEx(... IID_IDirectDraw7 ...)`
2. calls `IDirectDraw7::SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE)`
3. calls `IDirectDraw7::SetDisplayMode(640, 480, 16, 0, 0)`
4. creates a complex/flipping primary surface chain
5. obtains the attached backbuffer through `GetAttachedSurface`
6. stores the HWND
7. marks the wrapper as fullscreen
8. caches the fullscreen destination rectangle

### `0x004031E0 InitializeWindowedDisplay`

The windowed path:

1. creates `IDirectDraw7`
2. uses `DDSCL_NORMAL`
3. adjusts the Win32 window style/position around the requested 640x480 client area
4. creates the primary surface
5. creates a 640x480 offscreen rendering surface
6. creates an `IDirectDrawClipper`
7. binds the clipper to the HWND
8. attaches it to the primary surface
9. stores the HWND and windowed flag
10. caches the client rectangle in screen coordinates

The window style work uses `GetWindowLongA`, `SetWindowLongA`, `AdjustWindowRectEx`, `SetWindowPos`, and menu metrics.

## Present path

### `0x00403480 PresentDisplay`

Windowed mode:

```
primary->Blt(&destination_rect, offscreen, NULL, DDBLT_WAIT, NULL)
```

Fullscreen mode:

```
primary->Flip(NULL, 0)
```

The function explicitly recognizes DirectDraw surface/mode-loss errors. On a lost-surface path it:

1. restores the primary surface
2. restores the back/offscreen surface
3. calls `IDirectDraw7::RestoreAllSurfaces`
4. raises the game's global surface-reload flag

That flag works together with `ReloadRegisteredBitmapSurfaces`, which recreates the game's loaded bitmap surfaces from their original filenames.

## Drawing helpers

### `0x00403500 BltFastToBackBuffer`

Thin wrapper around the managed render surface's `IDirectDrawSurface7::BltFast`.

### `0x00403540 ClearBackBuffer`

Builds a `DDBLTFX`, sets its fill-color field, and calls:

`IDirectDrawSurface7::Blt(..., DDBLT_COLORFILL, ...)`

against the managed render surface.

### `0x00403590 UpdateDisplayDestinationRect`

Windowed mode uses:

- `GetClientRect`
- two `ClientToScreen` calls

to convert the client rectangle to screen coordinates.

Fullscreen mode uses `GetSystemMetrics(SM_CXSCREEN/SM_CYSCREEN)` and `SetRect`.

### `0x004035F0 CopyBitmapToSurface`

This is the GDI bridge used by the bitmap loader. It restores the target DirectDraw surface if needed, creates a compatible GDI DC, selects the HBITMAP, obtains the target surface DC, copies via `BitBlt(SRCCOPY)`, and releases both DCs.

## Bitmap surface registry

The existing registry functions complete the resource-lifetime picture:

- `0x004036E0 MarkRegisteredSurfaceColorKeyed`
- `0x00403720 UnregisterBitmapSurface`
- `0x00403770 RegisterBitmapSurface`
- `0x00403830 ReloadRegisteredBitmapSurfaces`
- `0x004038D0 LoadBitmapToDirectDrawSurface`
- `0x00403B70 SetSurfaceTransparencyColorKey`

The registry can track up to roughly 798 active entries. For each surface it retains the original filename and whether the magenta transparency key `0x00FF00FF` must be re-applied after reload.

## Architectural consequence

At this point the generic rendering technology is no longer a major unknown. The game renders its activities as 2D bitmap/surface composition into one managed render surface, then either flips it in fullscreen mode or blits it into the window in windowed mode. The remaining graphics reverse engineering is primarily **activity-specific composition/layout/animation logic**, not an unknown renderer.


## DirectX version context

The renderer is specifically **DirectDraw 7**, even though the executable otherwise uses DirectX 8-era APIs. The same EXE imports `DirectInput8Create` and `DirectSoundCreate8`, so the overall application should be thought of as a **DirectX 8-era title with the final DirectDraw7 rendering interface**.
