# btb-reverse

Reverse-engineering workspace for **Bob the Builder: Bob Builds a Park** (Windows, 2002).

## Source material

The original game files are intentionally **not committed** to this repository. Point the tooling at a local copy of the installed game or CD contents.

Verified primary game executable:

- path: `Exe/Bob the Builder - Bob Builds a Park.exe`
- size: `311296` bytes
- SHA-256: `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`
- format: PE32 / i386 / Windows GUI
- linker: Microsoft 6.0-era
- timestamp: 2002-08-06 13:29:47 UTC
- image base: `0x00400000`
- entry point: `0x00430FEA`

The 45,056-byte root-level `Bob the Builder - Bob Builds a Park.exe` is a launcher/AutoMenu executable, not the main game.

## Repository layout

- `docs/` - hand-maintained reverse-engineering notes
- `tools/` - dependency-free scanners for the original files
- `analysis/generated/` - generated reports that are safe to regenerate
- `ghidra/` - Ghidra project support/scripts as they are added

## First scan

Run:

```bash
python tools/inventory.py "/path/to/BTB-BBP" --out analysis/generated/inventory.json
python tools/exe_strings.py "/path/to/BTB-BBP/Exe/Bob the Builder - Bob Builds a Park.exe" --out analysis/generated/exe-strings.json
```

On Windows PowerShell:

```powershell
python tools/inventory.py "C:\path\to\BTB-BBP" --out analysis/generated/inventory.json
python tools/exe_strings.py "C:\path\to\BTB-BBP\Exe\Bob the Builder - Bob Builds a Park.exe" --out analysis/generated/exe-strings.json
```

## Current reversing direction

The main executable is small and unprotected enough that the plan is to recover it subsystem-by-subsystem:

1. startup / WinMain / main state machine
2. file/data-table loader
3. UI and bitmap/surface manager
4. DirectDraw renderer
5. DirectInput input layer
6. DirectSound/WAV layer
7. Bink movie layer
8. individual subgames and park designer
9. reconstruct clean source-level structures and names
10. build a behaviorally equivalent clean-room implementation

See `docs/initial-analysis.md` for the first verified anchors.
