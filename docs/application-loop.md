# Application shell and WinMain loop

The retail executable does not have a single large C++ game-object class.
Its top level is a Win32 `WinMain`, global state, small subsystem objects, and
activity-specific global data/callbacks.

## Main entry points

- `0x00402700 WinMain`
- `0x00401E20 CreateMainGameWindow`
- `0x004020D0 MainWindowProc`
- `0x00402440 RunActiveGameFrame`
- `0x00402640 FullscreenSystemKeyHook`

## Window creation

`CreateMainGameWindow` registers class `WINNAME` with
`MainWindowProc`, creates the Bob the Builder window, loads the accelerator
resource, shows/updates the window, and returns both the HWND and accelerator
handle to WinMain.

Retail rendering is fixed at 640x480.

## Display mode

Global `0x0044DE0C` is the windowed/fullscreen mode passed to the display
manager:

- 0 = exclusive fullscreen, 640x480x16
- nonzero = windowed, 640x480 client area

WinMain installs the low-level keyboard hook only when this value is zero.

## MainWindowProc

The window procedure has direct handlers for the Win32 messages that matter to
the game.

### WM_DESTROY

Writes shutdown phase 3, forcing final teardown.

### WM_MOVE

Refreshes the windowed DirectDraw destination rectangle.

### WM_SIZE

Global `0x0044DE10` is the active-frame flag.

Retail clears it for:

- `SIZE_MINIMIZED = 1`
- `SIZE_MAXHIDE = 4`

and sets it for the other observed size states. It then refreshes the display
destination rectangle.

When this flag is zero, WinMain calls `WaitMessage` rather than executing an
active game frame.

### WM_ACTIVATE

Calls `UpdateDirectInputAcquireState`. That helper acquires the keyboard and
mouse DirectInput devices when the active-frame flag is set and unacquires them
when it is clear.

### WM_SETCURSOR

In exclusive fullscreen, retail hides the Win32 cursor with
`SetCursor(NULL)` and consumes the message.

### WM_GETMINMAXINFO

Computes the fixed 640x480 client/window dimensions using current non-client
metrics.

### fullscreen system-command handling

The window procedure suppresses several normal window system commands in
fullscreen mode and contains the display-mode toggle/recreate path.

## Fullscreen keyboard hook

`0x00402640 FullscreenSystemKeyHook` is installed with
`WH_KEYBOARD_LL = 13` only in exclusive fullscreen.

It filters keyboard messages 0x100..0x105 and suppresses system combinations
including the observed:

- Alt+Enter
- Alt+F4
- Alt+Tab
- Alt+Escape
- Control-key combinations used with those paths
- the Control+Alt+Delete-style Delete combination checked by the retail code

Other keyboard events are passed to `CallNextHookEx`.

## Shutdown / credits phase

Global `0x0044DDB0` is an application shutdown phase, not the central
activity state.

| Value | Meaning |
|---:|---|
| 0 | normal game |
| 1 | begin exit credits |
| 2 | exit credits playing |
| 3+ | final activity/UI teardown and process shutdown |

This resolves an earlier apparent anomaly in WinMain.

When phase 0 is active, the no-message path eventually calls
`RunActiveGameFrame`.

When phase 1 is observed, WinMain loads the credits slideshow and increments
the phase to 2.

While phase 2 is active, WinMain advances/draws the credits. Completion
increments the phase to 3.

Only once the phase reaches 3 does WinMain call the high-level flow cleanup,
destroy the display manager, post quit, release DirectInput/DirectSound/sound
manager resources, and complete process shutdown.

Therefore the call to `DestroyDisplayManager` after `RunMainGameFlow` is a
**post-credits shutdown path**, not a per-frame display teardown.

The normal activity state remains global `0x0044DE14`.

## Active frame

### `0x00402440 RunActiveGameFrame`

The normal foreground frame performs these operations:

1. periodically validates the retail CD;
2. reads `timeGetTime` and skips duplicate-timestamp frames;
3. restores/reloads registered DirectDraw bitmap surfaces if requested;
4. reaps finished non-persistent managed sound slots;
5. updates the current help/context interaction path or the main game-flow
   dispatcher;
6. updates shared contextual-help overlay logic when required;
7. polls DirectInput and updates the common keyboard/mouse/cursor globals;
8. calls `IDirectDraw7::TestCooperativeLevel`;
9. handles DirectDraw mode-loss states;
10. presents the back/offscreen surface;
11. calls `IDirectDraw7::RestoreAllSurfaces` if presentation reports the
    corresponding surface-loss result.

Global `0x0044DDD4` stores the previous `timeGetTime` value.

## Surface-loss reload

`PresentDisplay` can set global `0x0051C320`.

On the next active frame, retail:

1. calls `ReloadRegisteredBitmapSurfaces`;
2. clears the reload flag;
3. clears transient help/cursor interaction state;
4. stops current managed sounds where required;
5. restores the default cursor state.

Because the bitmap registry retains source filenames and transparency-key
flags, the game can reconstruct resource surfaces after DirectDraw loss.

## DirectDraw cooperative-level handling

The frame's `IDirectDraw7` vtable calls are now resolved:

- vtable +0x68 = `TestCooperativeLevel`
- vtable +0x64 = `RestoreAllSurfaces`

If cooperative testing reports wrong-mode state, the display manager is
recreated with the current windowed/fullscreen setting.

Other temporary exclusive-mode failures cause a 10 ms sleep and retry on a
later frame.

## Retail CD presence check

Every 100 active-frame calls, retail checks the configured install/CD drive.

It requires:

- drive type 5, `DRIVE_CDROM`
- volume label `BTB-BBP`

Failure enters `0x00406C00 AbortForRemovedRetailCD`.

That fatal path:

- marks global `0x004FBD00`
- destroys the DirectDraw display
- displays the `CD Removed` / `Error` message box
- posts quit
- releases shared input/audio/sound resources
- unhooks the fullscreen keyboard hook when applicable
- exits through the CRT

## Message pump

WinMain uses the standard Win32 pattern around:

- `PeekMessageA`
- `GetMessageA`
- `TranslateAcceleratorA`
- `TranslateMessage`
- `DispatchMessageA`

If no message is waiting and the game is active, it executes the state/credits
logic described above.

If the window is inactive/minimized, it uses `WaitMessage` and refreshes the
frame-time baseline instead of busy-looping.

## Reconstruction

`reconstruction/include/btb/application_state.hpp` now records the recovered
retail values without imposing a modern application class:

- `ShutdownPhase`
- `DisplayMode`
- active-frame behavior for WM_SIZE states
- fullscreen-hook condition
- fixed 640x480x16 retail mode
- 100-frame CD validation interval

Modern application architecture belongs in the later rewrite; the source
reconstruction preserves the original global-state design.


## Source-level active-frame control

`0x00402440 RunActiveGameFrame` is now represented by
`reconstruction/include/btb/application_runtime.hpp`.

The exact early-return ordering is preserved:

1. increment the CD-validation counter;
2. validate only when the incremented value is **>100**, then reset it to 0;
3. call `timeGetTime`; if the delta from the previous value is zero, return
   before any game/sound/input/present work;
4. if the bitmap-reload flag is set, reload registered surfaces and clear it;
5. when that reload occurred while contextual-help mode was active, retail also
   clears help mode and the primary input pulse, stops managed sounds, restores
   the default cursor, and writes shared delay **50**;
6. store the new time value;
7. reap managed sound slots;
8. clear the shared per-frame flag;
9. dispatch either contextual-help update or the 68-state main game flow;
10. if contextual-help activation phase is >=2, run its shared overlay/update
    helper;
11. poll DirectInput and update shared cursor/button state;
12. call `IDirectDraw7::TestCooperativeLevel`;
13. present the display only when cooperative level allows it.

The exact cooperative-level negative branches are:

- **0x887600E1** or **0x88760245**: `Sleep(10)` and return success without
  presenting;
- **0x8876024B**: recreate the display using the current windowed/fullscreen
  mode and return;
- any other failure: propagate it.

If `PresentDisplay` returns **0x887601C2 / DDERR_SURFACELOST**, retail calls
the shared `RestoreAllSurfaces` wrapper and still returns success from the
active-frame routine.

### DirectInput shared cursor runtime

The common DirectInput/cursor portion is now represented in
`reconstruction/include/btb/input_runtime.hpp`.

Initialization uses DirectInput version **0x0800**. The pointer device uses
cooperative flags **5**; the keyboard uses **0x16**. After setup retail applies
the active/inactive acquire state and enables global input processing.

`UpdateDirectInputAcquireState` has a small exact quirk: when the primary
device pointer is null it returns **1**. Otherwise frame-active state acquires
both devices, inactive/minimized state unacquires both, and the function
returns zero.

The default shared cursor bounds are:

```text
min X = 2
min Y = 2
max X = 637
max Y = 477
```

Bounds are applied to **cursor + hotspot**, so clamping stores
`boundary - hotspot` back into cursor X/Y.

The keyboard direction/action mask is derived directly from the 256-byte DIK
array:

- DIK_LEFT  0xCB -> bit 0x01
- DIK_RIGHT 0xCD -> bit 0x02
- DIK_UP    0xC8 -> bit 0x04
- DIK_DOWN  0xD0 -> bit 0x08
- DIK_SPACE 0x39 -> bit 0x10

Keyboard cursor movement is one pixel per frame normally and **four pixels**
when the shared fast-cursor flag is nonzero.

The machine-readable global map is
`ghidra/input_runtime_globals.csv`.
