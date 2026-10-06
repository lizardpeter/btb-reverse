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
| `+0x10` | `IDirectDrawSurface7 *` | **dormant legacy surface slot**; constructor zeros it and teardown defensively unregisters/releases it, but no retail path ever populates or reads it |
| `+0x14` | `HWND` | game window |
| `+0x18` | `RECT` | destination/client rectangle in screen coordinates |
| `+0x28` | `BOOL` | 1 = windowed, 0 = exclusive fullscreen |
| `+0x2C` | unused retail dword | allocated as part of the 0x30-byte object but never initialized, read, or written by any retail code path |

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


## Source-level display-manager reconstruction

The DirectDraw wrapper is now represented source-level in
`reconstruction/include/btb/display_manager.hpp`.

The exact initialization contracts are:

### Fullscreen

- `DirectDrawCreateEx(... IID_IDirectDraw7 ...)`
- cooperative flags **0x11**
- display mode **640x480x16**
- primary flip-chain `DDSURFACEDESC2`:
  - `dwSize = 0x7C`
  - `dwFlags = 0x21`
  - raw `ddsCaps.dwCaps = 0x2218`
  - backbuffer count **1**
- attached-surface caps **0x04**
- retail explicitly `AddRef`s the attached backbuffer after retrieval
- wrapper stores HWND, sets windowed flag 0, and refreshes the destination
  rectangle.

### Windowed

- cooperative flags **8**
- primary-surface descriptor:
  - `dwSize = 0x7C`
  - `dwFlags = 1`
  - raw caps **0x200**
- 640x480 offscreen render-surface descriptor:
  - `dwSize = 0x7C`
  - `dwFlags = 7`
  - raw caps **0x2040**
- creates a DirectDraw clipper
- binds it to the HWND
- attaches it to the primary surface
- releases the local clipper reference
- stores HWND, sets windowed flag 1, and computes the screen-space client
  destination rectangle.

### Present and lost-surface recovery

Windowed presentation is:

```text
primary->Blt(&destination_rect, render, nullptr, 0x01000000, nullptr)
```

Fullscreen presentation is:

```text
primary->Flip(nullptr, 0)
```

Retail retries the same present operation while HRESULT is
**0x8876021C / DDERR_WASSTILLDRAWING**.

On **0x887601C2 / DDERR_SURFACELOST**, it:

1. restores the primary surface;
2. restores the render surface;
3. calls `IDirectDraw7::RestoreAllSurfaces`;
4. writes global `0x0051C320 = 1` so the next active frame rebuilds
   registered bitmap surfaces.

The destructor unregisters and releases auxiliary/render/primary surfaces,
restores DirectDraw cooperative level to normal, and releases IDirectDraw7.

## Exact bitmap-surface registry

The surface-loss mechanism is now source-level in
`bitmap_registry.hpp/.cpp`.

Retail maintains three parallel static tables:

| Global | Meaning |
|---|---|
| `0x0044EB1C` | **800 addresses of IDirectDrawSurface7* variables** |
| `0x0044DE9C` | 800 color-key flags |
| `0x0044F79C` | 800 filename records, each **0x104 / 260 bytes** |
| `0x0048241C` | high-water entry count |
| `0x00482420` | reload-in-progress guard |

The registry stores the **address of each surface-pointer variable**, not just
the surface value. This lets reload release the old surface and write the new
pointer directly back into the original activity/global variable.

Registration:

- is ignored while reload is in progress;
- reuses the first zero pointer-reference slot below high-water;
- otherwise appends at the current high-water index;
- copies the filename into that slot;
- clears its color-key flag;
- recomputes high-water by scanning all 800 entries.

The retail append path has a peculiar count clamp around **0x31E/0x31F**.
Because a full-table rescan follows each successful registration, slots 798 and
799 are still usable. If all 800 are occupied, the shipped loop would select
index **800** and write beyond the static arrays. The reconstruction records
that would-overflow condition without invoking host-language UB.

Unregister clears the pointer-reference, keyed flag, and first filename byte;
it does not immediately shrink high-water.

### Reload

`ReloadRegisteredBitmapSurfaces` raises the reload guard and, for every
registered pointer-reference whose current surface is non-null:

1. releases the old DirectDraw surface;
2. writes null through the registered pointer variable;
3. reloads the bitmap using its retained filename;
4. writes the new DirectDraw surface through that same pointer variable;
5. reapplies magenta **0x00FF00FF** when that slot's keyed flag is 1.

### Bitmap load fallback

`LoadBitmapToDirectDrawSurface` uses:

```text
LoadImageA(
  nullptr,
  filename,
  IMAGE_BITMAP,
  requested_width,
  requested_height,
  0x2010 // LR_LOADFROMFILE | LR_CREATEDIBSECTION
)
```

If that fails and the retail drive index is nonnegative, it retries using:

```text
<letter>:\\<filename>
letter = 'a' + drive_index
```

If both attempts fail, retail enters the removed-CD fatal path.

The loaded HBITMAP is queried with `GetObjectA(..., 24, ...)`. Surface caps
are selected from bitmap width:

- width <= **2000**: initial caps **0x40**
- width > **2000**: initial caps **0x800**

If the first DirectDraw `CreateSurface` fails, retail retries with caps
**0x800**. On success it copies the HBITMAP into the surface, deletes the
HBITMAP, and returns the new DirectDraw surface.

A small shipped leak is preserved as evidence: when **both** surface-creation
attempts fail, the function returns without deleting the HBITMAP.


### DisplayManager +0x2C closure

The final dword of the 0x30-byte DisplayManager object is no longer treated as
an unresolved field. A whole-executable reference audit shows:

- `DisplayManagerConstructor` initializes only vtable and members through
  `+0x10`;
- all recovered DisplayManager methods use members only through `+0x28`;
- tracking every load of the global DisplayManager pointer
  `0x0044DE08` finds no dereference at `+0x2C`;
- no external path writes that offset.

The reconstruction therefore names it `unused_retail_2c` and deliberately
leaves it semantically inert.


### DisplayManager +0x10 legacy surface closure

The surface pointer at `+0x10` is also closed. It is not a live auxiliary
render surface in this build.

Evidence from the retail executable:

- constructor `0x00402FC0` writes null to `+0x10`;
- fullscreen and windowed initialization create only `+0x08` primary and
  `+0x0C` render/backbuffer surfaces;
- no DisplayManager method assigns a non-null value to `+0x10`;
- teardown still calls `UnregisterBitmapSurface(&member)`, releases the member
  if non-null, and zeros it;
- all 102 references to global DisplayManager pointer `0x0044DE08` were
  audited. External member dereferences resolve to `+0x04` and `+0x0C`;
  none reaches `+0x10`.

The source model therefore names this member
`unused_legacy_surface_ptr32`. The defensive teardown remains because it is
part of shipped retail behavior.
