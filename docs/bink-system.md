# Bink movie playback system

The executable uses RAD Game Tools Bink through `binkw32.dll`. The generic movie layer is concentrated around `0x00408D80-0x004092C6`.

## Initialization

### `0x00408D90 InitializeBinkPlaybackSystem`

This routine prepares the shared playback state from the active DirectDraw environment and binds Bink audio to DirectSound via:

- `BinkSetSoundSystem`
- `BinkOpenDirectSound`

It also records the 640x480 playback/surface geometry used by the generic movie renderer.

## Open/close

### `0x00408E10 OpenBinkMovieWithFallback`

Calls `BinkOpen` using the requested filename. When the direct path fails, it constructs the same alternate game/CD-root path used by the rest of the game and retries.

### `0x00408EA0 CloseBinkMovie`

Thin wrapper around `BinkClose`.

### `0x00408EB0 OpenGlobalBinkMovie`

Opens a movie into the shared global Bink handle, initializes volume/playback state, and switches the shared UI/input state for movie playback.

### `0x00408F50 CloseGlobalBinkMovie`

Closes and clears the global Bink handle and restores the shared state used outside movie playback.

## Frame path

### `0x00408F70 DecodeAndBlitBinkFrame`

The generic frame path is:

1. `BinkDoFrame`
2. lock the destination DirectDraw surface
3. query the Bink-compatible surface type with `BinkDDSurfaceType`
4. `BinkCopyToBuffer` into the locked surface
5. unlock the surface
6. advance with `BinkNextFrame`
7. honor `BinkWait` timing

### `0x004090D0 UpdateGlobalBinkMovie`

Runs the shared/global movie frame loop. It blits the decoded frame to the active DirectDraw surface, advances frames, waits for timing, and closes the global movie when the end is reached.

## Playback controls

| Address | Name | Role |
|---|---|---|
| `0x00408D80` | `RestartBinkMovie` | `BinkGoto(handle, 1, 0)` |
| `0x00409210` | `ApplyGlobalBinkVolume` | converts game volume to Bink volume and calls `BinkSetVolume` |
| `0x00409260` | `PauseGlobalBinkMovie` | `BinkPause(1)`, mute, mark paused |
| `0x004092A0` | `ResumeGlobalBinkMovie` | `BinkPause(0)`, restore volume/state |


### Exact generic playback modes

`0x00408F70 DecodeAndBlitBinkFrame` accepts a seventh integer mode. The
three retail meanings are now closed:

| Mode | Meaning | End-of-stream | Shared input |
|---:|---|---|---|
| 0 | skippable one-shot | return complete | return complete |
| 1 | looping | continue / call `BinkNextFrame` | ignore |
| 2 | unskippable one-shot | return complete | ignore |

The helper also uses global `0x00442A2C` as a first-frame advance latch.
For modes 0 and 2, a successful first call with the latch clear decodes and
blits the current frame without `BinkNextFrame`. It then raises the latch.
Later calls perform `BinkNextFrame` and spin on `BinkWait`. Mode 1 always
performs the advance/wait path, even when the latch starts clear.

`0x00409070 DecodeAndBlitBinkFrameNoAdvance` is separate: it only
`BinkDoFrame`s, locks/copies/unlocks the supplied DirectDraw surface, and
returns without advancing or blitting that surface to the game's backbuffer.

### Global movie loop differences

`0x004090D0 UpdateGlobalBinkMovie` does **not** use the generic first-frame
latch. After a nonterminal frame it always:

1. calls `BinkNextFrame`;
2. spins until `BinkWait` returns zero;
3. blits the shared 640x480 Bink surface at **(0,0)**.

It closes the shared movie when `FrameNum == Frames`. It also closes on any
of the three shared input pulses when global `0x0043EE6C` is exactly 1.

A DirectDraw lock failure returns completion/error from the updater but does not
run the normal close path.

### Exact volume conversion

`ApplyGlobalBinkVolume` is now source-level:

```text
if no global movie: return
if paused:          return

if game_volume < 5:
    bink_volume = 37
else:
    bink_volume = trunc(game_volume * 31000.0 * 0.01)
    bink_volume = max(bink_volume, 37)

BinkSetVolume(global_movie, 0, bink_volume)
```

The two embedded doubles are exactly **31000.0** and **0.01**.

Pause calls `BinkPause(handle,1)`, sets the paused flag, mutes track 0 to
volume 0, and disables shared DirectInput processing. Resume calls
`BinkPause(handle,0)`, clears the paused flag, reapplies the converted
volume, and re-enables input processing.

### Shared surface and open/fallback contract

Initialization creates one **640x480** offscreen DirectDraw surface from a
124-byte `DDSURFACEDESC2` with:

- `dwSize = 0x7C`
- `dwFlags = 7`
- `dwHeight = 480`
- `dwWidth = 640`
- `ddsCaps.dwCaps = 0x40` (`DDSCAPS_OFFSCREENPLAIN`)

It then binds Bink audio through
`BinkSetSoundSystem(BinkOpenDirectSound, 0)`.

The CD fallback used by both open functions is literal:

```text
<letter>:\\<original supplied path>
letter = 'a' + retail_drive_index
```

A drive index of `-1` disables that fallback.

- `OpenBinkMovieWithFallback` uses raw Bink open flags **0x04000000**.
- `OpenGlobalBinkMovie` uses flags **0**, aborts through the retail-CD fatal
  path if both attempts fail, applies volume on success, and disables shared
  input processing.
- `CloseGlobalBinkMovie` closes/clears the global handle and re-enables input.

The C++26 reconstruction is in
`reconstruction/include/btb/bink_system.hpp` and
`reconstruction/src/bink_system.cpp`. Machine-readable globals/modes are in
`ghidra/bink_runtime_globals.csv`.

## Activity-specific playback

Spud Skate contains additional Bink calls around `0x00424800-0x00424D00`, including key-frame lookup and seeking. Those routines are activity-specific and are intentionally kept separate from the generic movie layer above.

The external data tables make the generic movie uses easy to classify:

- `loaddata/videoseq.txt` — THQ/BBC/logo/intro sequence
- `loaddata/startupmovie.txt` — per-activity startup movies
- `loaddata/completedmovie.txt` — completion movies
- `loaddata/binkwalk.txt` — walkthrough movie + spoken-help WAV mapping
