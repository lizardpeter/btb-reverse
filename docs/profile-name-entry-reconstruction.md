# Profile name entry and compiled-source closure, 2026-10-14

The baseline is main commit `0e015a3ad03cdd7a89d2267d8935899461ce3ce5`.
The executable evidence is the checked-in Ghidra corpus for SHA-256
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

## New original routine

`profile_name_entry.cpp` reconstructs `UpdateEnterNamePopup`, address
`0x0042DCC0`. It preserves:

- The first non-shift pressed DirectInput scan code among indices 0..236;
  same-key debounce, release-to-rearm, backspace and eight-glyph input cap.
- The six strict mouse rectangles in source order. Four select/pressed
  button surfaces and two voice-only regions remain distinct.
- Original hover IDs 456, 460, 321 and 322, priority 50, class 2.
  Acceptance queues ID 320 and freezes input until managed audio is idle.
- Badge wrap through six choices, name-only and badge-only acceptance,
  cancellation, selected player, progress word zero = 1, and state 0x41.
- Stale unused glyph backing words, rather than zeroing the entire record.

`GameRoot::begin_profile_name_entry` installs an actual state-1 caller.
It requires source font and badge rectangles from the asset provider.
The coordinator first performs the setup-only frame, then draws the current
name before processing keyboard input, matching `0x0042A2C0` ordering.
It emits original bitmap paths, centered glyphs, inclusive-bottom font
rectangles, cursor blink and shared audio commands. No generated assets
are substituted. The empty badge sentinel is guarded by the portable
presentation adapter rather than submitting a negative source rectangle.

## Persistent profile corrections

The source initializer `0x0042D270` marks missing `playerN.txt` by setting
progress word zero to -1. `load_profile_files` previously left it zero.
All other 99 words stay zero. Existing saves still load their actual values.

The confirmed-delete branch in `0x0042E110` truncates the selected player's
Park Designer, Fireworks and three music files. `delete_profile` now performs
these writes when a save directory is configured. It preserves other players'
files and the existing exact 15-word visible-progress reset. Host I/O failure
is returned instead of being silently ignored; earlier successful truncations
are not rolled back, consistent with the original sequential file operations.

`write_playerinfo` validates all name lengths before output, avoiding a host
out-of-bounds read and a partial metadata file on invalid state.

## Build evidence and counts

The full project was compiled locally with Clang 19.1.1 in C++26 mode,
including `btb_recompiled_core`, `btb_game`, and every test target.
The 615-entry architecture constexpr audit needs a larger Clang evaluation
budget; local configuration used `-fconstexpr-steps=10000000`.
All **118 CTest tests passed**, with assertions enabled.

The Bob's Band source test was present on disk but absent from CMake.
It is now registered and linked against `btb_recompiled_core`.

All 93 prior `source_written` entries have a registered compiled/executed
regression and are promoted to `compiles` under the repository's existing
stage definition. Adding `0x0042DCC0` produces **388/615 (63.1%)** at that
stage, **227 unreviewed**, and **zero retail-behavior-verified**.
This is not 94 newly translated functions: 93 were existing source and one
is new. The evidence ledger's compiled stage means linked to source tests;
it does not prove every function is invoked by the native game executable.
Machine-readable run evidence is in
`analysis/reconstruction/compile-validation-20261014.json`.

## Remaining completion work

The native development host still uses its provisional profile 0 startup.
The complete five-sign selector, initial profile resource service, source
keyboard translation/physical observations and the full 0x41 scene movie
handoff need integration before replacing that startup. The name-entry
caller is exercised through the coordinator but is not advertised as an
end-to-end native profile menu.

Bink presentation, the central Park Designer editor, full Herding motion,
remaining activity initialization/callers and original x86 differential
validation remain substantial work. Compilation and synthetic source
fixtures do not establish retail-equivalent gameplay.
