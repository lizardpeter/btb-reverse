# Original five-profile saves in the native game

Native `btb_game.exe` now reads and writes the five **original-format**
`player1.txt` through `player5.txt` records, together with
`playerinfo.txt`, using `GameRoot::load_profile_files` and
`GameRoot::save_profile_files`. These C++26 source-format routines are
already part of the compiled original game core.

## Select a saved player

The original five-sign profile **screen** is not yet registered with the native
Win32 host, so the launcher defaults to original slot 1. Until its original
selection UI is hosted, any slot can be selected explicitly:

```powershell
btb_game.exe --native-original "C:\\path\\to\\BTB-BBP" --profile-slot 3 --skip-bink
```

`--profile-slot` accepts **1 through 5** only. Each selected player owns
the real independent completion slots; it does not duplicate them in memory.
The `--skip-bink` flag is optional and is completely independent of profiles.

## Save-directory policy

- Explicit `BTB_PROFILE_DIR` environment variable overrides storage.
- Otherwise default to `%LOCALAPPDATA%\\BTB-Recompiled\\profiles`.
- If LOCALAPPDATA is absent, use a separate `BTB-Recompiled-Saves`
  folder inside the supplied installed-game root, never overwriting
  original root-level retail `playerN.txt` files by default.
- Use the same selected save directory for Fireworks, Bob's Band,
  Park Designer and the central GameRoot progress files.

The host creates its save directory, loads the actual five player records,
and persists records on normal native window close. Failed loads and writes
are surfaced explicitly, rather than pretending the saves were successful.

## Limitations

This does **not** implement the original graphical five-sign profile selector,
player name-edit/delete flows, hard-crash recovery, or retail end-to-end
save/load equivalence. In particular, incomplete game activity drivers are
not made playable by preserving profiles. The implementation is deliberately
independent of the ongoing Bink codec and movie integration.
