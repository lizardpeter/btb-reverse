# Full-game native Windows platform boundary (C++26, source-first)

The original game is a **Win32 640×480 DirectDraw7 / DirectSound / Bink**
program. The new source units connect the recovered outer state-machine
and source-neutral effect pipeline to actual Windows graphics and audio
APIs. They are not a browser reconstruction, pre-rendered screenshots,
or a miniature game demo.

## End-to-end frame ownership

`NativeGameSession` in
`reconstruction/include/btb/full_game_native_session.hpp`
and `reconstruction/src/full_game_native_session.cpp` now owns the
original per-frame dataflow:

1. Poll device: query **actual** managed-sound buffer status from the
   80 slots and service Bink until the provider observes completion.
2. Call `GameRoot::advance` once using those actual statuses. Movie
   modes 13/14 remain blocked until Bink actually completes; the original
   click pulse is cleared on the completion frame.
3. On a pregame setup, resolve the **real installed/CD** source movie
   and help WAV from `binkwalk.txt`, open the independent walkthrough;
   for the four intro-gated games, also resolve/open their separate
   `startupmovie.txt` global Bink.
4. On pregame Back/Start, close the independent walkthrough. Difficulty
   and Help controls leave it running.
5. Reap managed audio according to recovered native priority and
   input-interruptible policy, then plan ordered sound operations from
   `GameEffectPipeline`.
6. Resolve every BMP/WAV path using `OriginalAssetResolver`, preserving
   original case-insensitive Win32 path semantics, source crop rectangles,
   magenta transparency and draw order.
7. Submit original sound operations, issue bitmap blits, composite the
   walkthrough above instructions, and call the original display present.
8. If the original global Bink is the only active screen (mode 13/14
   intercept), **do not clear its decoded pixels** with an empty normal
   draw list before present.

Missing original art, WAV, movie, sound ID, an uninstalled activity
driver, or a provider failure causes an explicit **sticky fatal session
error** rather than a false "successfully rendered" report. The host
must restart/reinitialize its session to recover; source-level gameplay
state and physical side effects cannot safely be rolled back.

This pipeline distinguishes `PreparedGameFrame` (policy and asset
resolution) from a **physically submitted** `NativeSessionFrame`.

## Actual Windows DirectDraw7 source

`win32_original_directdraw.hpp/.cpp` implements the display backend:

- `DirectDrawCreateEx(...,IID_IDirectDraw7,...)`.
- Fullscreen exclusive: **640×480×16**, flip primary surface with
  one attached backbuffer, `Flip(DDFLIP_WAIT)`.
- Windowed: primary surface plus **640×480 offscreen** render surface
  and `IDirectDrawClipper` attached to the original HWND;
  physical client rectangle is screen-translated for `Blt`.
- Original bitmap paths loaded with `LoadImageW` /
  `LR_LOADFROMFILE|LR_CREATEDIBSECTION`; original pixels uploaded
  into DirectDraw surfaces through GDI.
- Native bitmap registry's >2000-pixel system-memory policy and
  allocation retry; no fake textured quads.
- Source key = **magenta**; mapped through DirectDraw's actual RGB
  pixel masks, e.g. RGB565 0xF81F rather than incorrectly assuming
  a 32-bit magenta integer in a 16-bit surface.
- For each recovered `Draw`, perform source-rectangle to destination
  `Blt` with `DDBLT_KEYSRC` when keyed; preserve layer order.
- Surface-loss recovery restores primary/backbuffer, reloads original
  images, and reissues the **whole frame**, including if loss occurs
  during present.

This implementation is **not** a verified byte-identical recreation
of all retail COM quirks. More DirectDraw parity testing, exact menu
overlay sprite paths and timing, and full activity frame composition
remain necessary.

## Actual Windows DirectSound8 source

`win32_original_directsound.hpp/.cpp` implements a real
`DirectSoundCreate8` provider:

- Loads each original WAV as a RIFF/WAVE file, retaining its original
  format block and optional codec bytes; never guesses that compressed
  WAV data is PCM.
- Creates and locks secondary buffers; copies the shipped WAV sample
  bytes into DirectSound.
- Maintains the retail **80 managed sound slots**, with
  Play/Stop/rewind/release actions supplied by the previously reconstructed
  `RetailSoundManager32` policy. Duplicated initial Play calls are
  preserved at the effect level.
- Reports actual `IDirectSoundBuffer::GetStatus` playing flags each
  frame. The manager no longer needs invented completion timers.
- Preserves a looping music/backing buffer and separately retains
  simultaneous one-shot activity samples until actual playback stops.
- Emits provider failure when an original codec or sound buffer is
  unsupported instead of synthesizing a replacement soundtrack.

It uses DirectSound8 for a native modern Windows provider, not a claim
that the retail 2002 binary was originally linked against the exact
DirectSound8 COM interface or has been digitally compared.

## Complete provider composition and Bink boundary

`win32_original_game_device.hpp/.cpp` implements the
`NativeGameDevice` interface using the DirectDraw7 and DirectSound8
providers. It **requires** an `OriginalBinkProvider`; the latter must
decode original global Bink movies into the common backbuffer and
compose looping walkthroughs **after** the instruction BMP drawing.

The original `binkw32.dll` and its real ABI are indispensable.
`win32_original_bink_exports.hpp/.cpp` safely loads an explicit
absolute path to that named DLL and checks twelve Bink exports, including
legacy decorated `stdcall` names. That loader intentionally does not
assume the layout of the opaque `BINK*` object or pretend that it
has decoded any movie frames.

The **actual Bink frame-copy/timing provider is still not written**.
The original game uses an x86 Win32 DLL; native 64-bit code cannot
load that same original binary in-process. Either an x86 bridge or a
separately verified binary-compatible 64-bit decoder will be required.

## Test and compilation status

A new **source-only** regression
`full_game_native_session_source_test.cpp` covers:

- pregame Bink and spoken-help path resolution and physical-open requests;
- real deferred teardown on Start, not on difficulty selection;
- global Bink mode14 waiting and same-frame cleared click;
- prevention of a blank draw over decoded global Bink frames;
- refusal to claim success if an activity adapter is missing;
- sticky failures on missing BMP assets and failed physical drawing.

This test uses a recording provider only for **source behavior**. It
does not render pixels, call COM, decode Bink, or establish that Windows
linking, live audio, GPU surfaces, or actual retail gameplay passes.

**No full-game compile, link, Windows execution, render capture, or
physical device regression was performed** under the user's instruction
to defer compilation until the complete game source is ready.
The Windows source files have deliberately not been added to the
Dinosaur-only CMake preview. Do not count them as verified executable
functionality.

## Next native blockers

1. Recover and connect the original Bink DLL ABI and
   `OriginalBinkProvider` pixel-copy, looping, timing, and close rules.
2. Complete original menu overlay/depressed/hover frames and Help UI
   geometry and audio, not just full-background BMPs.
3. Connect the eight other original activity driver implementations and
   all sprite/physics/audio lifecycle paths.
4. Add a unified complete-game Windows build; differential-test
   gameplay, UI, movie, sound and persistence against the retail PE32.
