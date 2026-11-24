# Windows x64 standalone preview packaging

The independent [standalone preview workflow](../.github/workflows/win64-standalone-preview.yml)
builds only the game and its linked-core smoke, so a usable test image can
be produced without waiting for every source regression target. The build
uses the static MSVC runtime across all project targets and fails on dynamic
Microsoft C/C++ runtime or external codec imports (including delay imports).
Normal Windows system DLLs are still required.

The artifact contains only the reconstructed btb_game.exe and a JSON report
with its SHA-256, machine architecture and imports. This is not a signed or
retail-equivalence-certified release. Use the user's own original game
installation; do not distribute original assets or executable.

Run from PowerShell:

```powershell
.\btb_game.exe --native-original "C:\Games\BTB-BBP"
```

Use a copy of the original installation during evaluation. Native saves are
kept separately from the retail profile files. Test normal playback first;
`--skip-bink` is only a diagnostic fallback. The existing repo-checks suite
remains the broader integration gate; the standalone package verifies the
linked-core executable and PE imports, not a live Windows playthrough.
