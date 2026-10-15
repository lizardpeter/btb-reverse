# Native full-game save-profile wiring

The native Win32 `GameRoot` host now reads the actual five-profile progress
format and `playerinfo.txt` on initialization, rather than starting from an
in-memory zeroed record. The existing `GameRoot::load_profile_files()` API
validates the original per-player file layouts and initializes all five
`playerN.txt` slots, with source-correct absent-file sentinels.

Save files are deliberately isolated from the original installed game:
- `BTB_PROFILE_DIR` overrides the location when explicitly set.
- Otherwise use `%LOCALAPPDATA%\\BTB-Recompiled\\profiles`.
- If LOCALAPPDATA is unavailable, use a separate
  `BTB-Recompiled-Saves` folder, not the retail installed player directory.

The same profile directory is passed to native Fireworks, Bob's Band,
Park Designer drivers, and the shared `GameRoot` persistence service,
so game progress and per-activity saves share one identity and location.

**Open work:** Startup still selects provisional profile slot zero rather
than presenting the full five-sign original profile UI. The original profile
choice, name-entry, deletion and save-on-exit integration remain necessary.
No original retail files are overwritten by default, and successful load of
a profile is not proof of playable activity behavior.
