# Explicit native video-skip gameplay mode

The default x64 C++26 `btb_game.exe` uses the source-reconstructed native
Bink provider and requires authentic original movies. **No video frames are synthesized and no playback completion
is invented in the normal mode.**

For native gameplay development while the Bink codec is being reconstructed:

```powershell
btb_game.exe --native-original "C:\\Users\\dgera\\Downloads\\BTB-BBP" --skip-bink
```

`--skip-bink` also works with automatic original-data detection:

```powershell
btb_game.exe --skip-bink
```

This is an **explicit non-retail compatibility mode**, not a solution for Bink.
The native session continues to use the retail 68-state `GameRoot` and actual
DirectDraw/DirectSound commands, but skips `open_walkthrough_movie` and
`open_global_movie` requests. If the source dispatcher enters intercepted
movie mode 13 or 14, the next frame acknowledges that movie transition so
the associated non-video setup/update path can proceed. No original `.bik` is required for that transition in this mode.
Legacy `binkwalk.txt` help WAV names are not required in either mode.

The source frontend menus, user-selected difficulty and input routing are not
replaced with a custom menu. Ordinary UI bitmap and sound references still have
to resolve from the original installed game.

## Scope and limits

- Video skip does **not** test the native Bink decoder.
- Skipped movies are never reported as decoded, rendered, heard or verified.
- All ten activity adapters are registered; this mode makes their non-video
  paths easier to exercise.
- Specialized movie-backed activity mechanics may still require their native
  providers. Profile selection and five-profile persistence are implemented
  but require real-installation validation.
- Retail visual, audio and full-playthrough equivalence remain unverified.

The Release-safe `full_game_native_session_source` regression includes video-free
Golf pregame selection, skipped Squirrel global intro mode 14, and a strict-default
negative test that still refuses a missing Bink file.
