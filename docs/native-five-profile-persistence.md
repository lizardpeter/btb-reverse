# Original five-profile saves in the native game

The native host registers the real five-sign selector, name-entry popup and
delete confirmation. Normal startup plays the source movie sequence before
showing the profile selector. The diagnostic shortcut
`btb_game.exe --native-original <installed root> --profile-slot 3 --skip-bink`
selects slot 3 without using that selector.

## Save-directory policy

- `BTB_PROFILE_DIR` overrides storage.
- Otherwise use `%LOCALAPPDATA%\BTB-Recompiled\profiles`.
- Without LOCALAPPDATA, use `BTB-Recompiled-Saves` inside the installed root.
- The same directory owns five original-format progress files, playerinfo,
  Fireworks, Park Designer, and the three Bob's Band compositions.

The native host now stages and closes all six central profile files in
`.btb-profile-write` before replacing any existing files. It backs up the
old files during replacement and attempts rollback if a rename fails.
Nonregular destinations are rejected. An existing transaction directory
blocks loads and writes until inspected; do not automatically delete it.
This is a native compatibility safeguard, not a recovered retail transaction
protocol. It is not a power-loss-atomic or fsync-backed transaction, and
per-activity files are still saved by their respective drivers.

Re-entering the five-sign selector resets its stale previous outer-state
latches; existing-player selection and confirmed deletion are persisted
immediately. Source regressions cover repeated selection, all five slots,
invalid metadata, obstructed destinations, and an interrupted write marker.

Physical Windows verification and exact retail quit/credits behavior remain
separate milestones.
