# Known symbol map

This file records names that can already be assigned before full decompilation. The machine-readable source is `ghidra/known_symbols.csv`.

## Static CRT routines

The executable statically links a Visual C++ 6-era CRT, which means a nontrivial part of the anonymous tail of `.text` is library code rather than game logic.

| Address | Name | Confidence | Basis |
|---|---|---|---|
| `0x00430144` | `crt_sprintf` | high | varargs formatter wrapper with destination buffer semantics |
| `0x00430A3F` | `crt_fopen` | high | thin wrapper around lower-level stream open using sharing flag `0x40` |
| `0x0042FB62` | `crt_fclose` | high | operates directly on MSVC `FILE` internals and closes/frees the stream |
| `0x0042FBB8` | `crt_fscanf` | high | constructs a varargs pointer and enters the CRT formatted-input engine |

Recognizing these four functions immediately makes many of the data-table loaders readable.

## Game file loader

### `0x00406E10 OpenGameDataFileWithCDFallback`

This routine receives a path plus mode, calls the CRT file-open routine, and, when the direct open fails, constructs an alternate path using the game/CD root information before retrying.

A large portion of the game's configuration loading flows through this one routine.

## Bitmap / DirectDraw resource registry

The game has a central bitmap-surface registry rather than each minigame managing every surface independently.

### `0x004038D0 LoadBitmapToDirectDrawSurface`

Verified behavior:

1. calls `LoadImageA(..., IMAGE_BITMAP, ..., LR_LOADFROMFILE | ...)`
2. retries using the alternate game/CD-root path when necessary
3. obtains the bitmap dimensions with `GetObjectA`
4. creates a DirectDraw surface using the active DirectDraw object
5. copies the loaded HBITMAP into that surface
6. destroys the temporary GDI bitmap

### `0x00403770 RegisterBitmapSurface`

Stores:

- the surface pointer
- a copy of its source filename
- a per-entry flag used when the surface is later reloaded

The registry has an observed upper bound of roughly `0x31e` active slots.

### `0x004036E0 MarkRegisteredSurfaceColorKeyed`

Finds the matching registered surface and marks its per-entry flag. The reload routine uses this flag to reapply transparency after the DirectDraw surface has been recreated.

### `0x00403720 UnregisterBitmapSurface`

Clears the registry entry for a surface: pointer, flag, and stored source filename.

### `0x00403830 ReloadRegisteredBitmapSurfaces`

Iterates the entire registry, releases each old COM surface, reloads it from the stored filename through `LoadBitmapToDirectDrawSurface`, and reapplies magenta transparency (`0x00FF00FF`) when the entry is marked.

This is likely tied to DirectDraw surface-loss / display-mode recovery.

### `0x00403B70 SetSurfaceTransparencyColorKey`

Converts the requested RGB value into the actual surface pixel format and calls the DirectDraw surface `SetColorKey` method with source-blit color-key semantics.

## Game-specific anchors

| Address | Working name | Evidence |
|---|---|---|
| `0x00407390` | `LoadOptionsConfig` | direct reference to `loaddata\\options.txt` |
| `0x0040A440` | `LoadDinoLevelData` | indexed dinosaur/difficulty path table and `dino.txt` parsing |
| `0x0040AEA0` | `LoadParkDesignerBoundAreas` | direct reference to `data\\subgamedyp\\boundareas.txt` |

These are working names. As call sites and structures become clearer, names can be refined without losing the address mapping.
