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

## Activity-specific playback

Spud Skate contains additional Bink calls around `0x00424800-0x00424D00`, including key-frame lookup and seeking. Those routines are activity-specific and are intentionally kept separate from the generic movie layer above.

The external data tables make the generic movie uses easy to classify:

- `loaddata/videoseq.txt` — THQ/BBC/logo/intro sequence
- `loaddata/startupmovie.txt` — per-activity startup movies
- `loaddata/completedmovie.txt` — completion movies
- `loaddata/binkwalk.txt` — walkthrough movie + spoken-help WAV mapping
