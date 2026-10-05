# Shared printing subsystem

The game has a reusable Windows bitmap-printing path used by at least **Dinosaur**, **Park Designer**, and **Fireworks**.

The core design is deliberately separated from those activity modules:

```
current DirectDraw backbuffer
        |
        v
ExportBackBufferToPrintBitmap
        |
        v
     PrintMe.bmp
        |
        v
LoadImageA -> HBITMAP
        |
        v
BitmapPrinter object
        |
        v
PrintDlgA -> printer HDC
        |
        v
StartDoc / StartPage
        |
        v
StretchDIBits
        |
        v
EndPage / EndDoc
```

## Activity-level entry point

### `0x00409730 PrintCurrentGameFrame`

This is the common print action called from multiple activities.

Verified call sites include:

- Dinosaur shared in-game controls around `0x00409D52`
- Park Designer around `0x0040E6D6`
- Fireworks around `0x00413A3C`

The routine:

1. calls `ExportBackBufferToPrintBitmap`
2. loads `PrintMe.bmp` with `LoadImageA(..., IMAGE_BITMAP, ..., LR_LOADFROMFILE)`
3. creates a compatible memory DC and selects the bitmap into it
4. reads the bitmap color table when present and creates a palette
5. configures a Win32 `PRINTDLGA`
6. calls `PrintDlgA`
7. constructs a temporary bitmap-printer object
8. installs the loaded HBITMAP/palette in that object
9. sets print scale mode **2**
10. sets the document title to **Bob the Builder - Bob Builds a Park**
11. invokes the printer object's document-print method with the printer HDC
12. frees the `PRINTDLG` global-memory handles and printer-object state

## Backbuffer export

### `0x00409460 ExportBackBufferToPrintBitmap`

The game does not print directly from a DirectDraw surface.

Instead this routine opens:

`PrintMe.bmp`

in binary-write mode and converts the current render surface into a conventional 24-bit BMP.

It:

- queries the active DirectDraw render surface description
- writes a `BITMAPFILEHEADER`
- constructs/writes a 24-bit `BITMAPINFOHEADER`
- inspects the DirectDraw pixel-format channel masks
- derives red/green/blue shifts and widths
- locks/reads the surface pixels
- converts each source pixel into 8-bit R/G/B bytes
- writes bottom-up BMP scanlines
- unlocks the DirectDraw surface

This explains why the printer helper itself only needs to understand normal Win32 bitmap/DIB formats.

## Bitmap printer object

### Object layout

`0x00404DD0 BitmapPrinterConstructor` initializes an object of approximately **0x124 bytes** and installs the vtable at `0x0043B2F4`.

Important recovered fields:

| Offset | Meaning |
|---:|---|
| `+0x04` | owned DIB header/allocation |
| `+0x08` | pointer to owned DIB pixel data |
| `+0x0C` | fixed document-name buffer |
| `+0x110` | configured target `RECT` |
| `+0x120` | print scaling mode |

### `0x00404E30 BitmapPrinterSetBitmap`

Accepts an `HBITMAP` and optional palette. It validates the bitmap with `GetObjectA` and chooses the correct conversion path depending on whether the input is a DIB section or a device-dependent bitmap.

### DDB/DIB import helpers

- `0x00405C40 BitmapPrinterImportDDB`
  - queries a normal `HBITMAP`
  - allocates owned 24-bit DIB storage
  - uses `GetDIBits` to copy its pixels
  - handles palette selection/realization when supplied

- `0x00405EE0 BitmapPrinterImportDIBSection`
  - queries a `DIBSECTION`
  - copies bitmap information/palette data
  - copies the section's pixel bits into owned storage

The embedded error strings directly corroborate these responsibilities:

- `Invalid bitmap.`
- `No bitmap defined.`
- `Error getting DDB info.`
- `Error getting DDB bits.`
- `Error getting DIB section info.`

## Document printing

### `0x00405140 BitmapPrinterPrintDocument`

This is the document lifecycle wrapper. It validates the bitmap/printer DC and then performs:

1. `StartDocA`
2. `StartPage`
3. virtual render-to-DC call
4. `EndPage`
5. `EndDoc`

Failure cleanup still ends the page/document as appropriate.

### `0x00404F90 BitmapPrinterRenderToDC`

Validates the current bitmap and printer DC, computes the effective target geometry, and invokes the low-level DIB renderer.

### `0x004057E0 BitmapPrinterStretchDIBToPrinter`

This is the actual printer raster operation.

It:

- rejects an empty target rectangle
- queries `RASTERCAPS` with `GetDeviceCaps`
- requires stretch-DIB support
- checks printer compatibility with `Escape`
- queries OS version
- sets stretch mode
- calls `StretchDIBits`

Relevant embedded errors include:

- `Invalid printer DC.`
- `Unsupported printer.`
- `Invalid target rectangle.`
- `Error getting DIB info.`
- `Error printing DIB.`

## Print geometry

### `0x00405380 BitmapPrinterSetScaleMode`

Accepts modes 0 through 3. Any other value throws/reports `Invalid scale.`.

The game chooses **mode 2** when printing its activity/certificate image.

### `0x00405490 BitmapPrinterComputeTargetRect`

Uses printer device caps to calculate the output rectangle according to the selected scaling mode.

The queried caps include physical/logical dimensions and resolution values needed to size/position the bitmap on the page.

### `0x00405350 BitmapPrinterSetTargetRect`

Copies an explicit target `RECT` into object offset `+0x110` and switches to the explicit-rectangle mode.

### `0x004056C0 MapClippedPrintRectToSource`

Intersects a requested destination with the configured target rectangle. When only part is printable, it maps that clipping proportionally back into the source rectangle so the correct source subsection is rendered.

## Simple accessors

- `0x00405330 BitmapPrinterSetDocumentName`
- `0x00406970 BitmapPrinterGetDocumentName`
- `0x00406980 BitmapPrinterGetTargetRect`
- `0x004069B0 BitmapPrinterGetScaleMode`

## Shared clipped DirectDraw blitter

The related graphics helper at `0x00415E50` is not Dino-specific.

### `0x00415E50 BlitColorKeyedSurfaceClipped`

It receives a source DirectDraw surface, destination position, and optional source rectangle. It:

1. derives the source bounds when no explicit source RECT is supplied
2. translates destination coordinates through the game's shared camera/origin globals
3. rejects sprites fully outside the **640 x 480** render area
4. clips destination and source rectangles at every screen edge
5. calls the render/back surface's `IDirectDrawSurface7::Blt`
6. uses source color-keying (`DDBLT_KEYSRC`)
7. translates DirectDraw HRESULT failures into the game's graphics-error code global at `0x004439A8`

This is a common sprite compositor used by Dino and many other activity modules. It should therefore stay in the shared graphics layer rather than be reconstructed inside any one minigame.
