# Initial static analysis

## Primary executable

The real game executable is the installed copy under `Exe/`, not the small disc-root AutoMenu launcher.

| Property | Value |
|---|---|
| File | `Exe/Bob the Builder - Bob Builds a Park.exe` |
| Size | 311,296 bytes |
| SHA-256 | `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05` |
| Format | PE32, Intel i386 |
| Subsystem | Windows GUI |
| Timestamp | 2002-08-06 13:29:47 UTC |
| Image base | `0x00400000` |
| Entry point | `0x00430FEA` |
| Linker | 6.0 |
| Relocations | stripped |
| Symbols | stripped |

### Sections

| Section | VA | Raw size | Role |
|---|---:|---:|---|
| `.text` | `0x00401000` | `0x3A000` | executable code |
| `.rdata` | `0x0043B000` | `0x3000` | imports/read-only data |
| `.data` | `0x0043E000` | `0xD000` | globals and strings |
| `.rsrc` | `0x0051E000` | `0x1000` raw | Win32 resources |

There is no CLR header and no obvious packing/virtualization layer.

## Major runtime dependencies

Verified direct imports include:

- `DDRAW.dll!DirectDrawCreateEx`
- `DINPUT8.dll!DirectInput8Create`
- `DSOUND.dll`
- `WINMM.dll` wave/MMIO routines
- `USER32.dll`
- `KERNEL32.dll`

The executable also dynamically uses `binkw32.dll` and contains references to Bink APIs including `BinkOpen`, `BinkDoFrame`, `BinkCopyToBuffer`, `BinkPause`, `BinkSetVolume`, and `BinkOpenDirectSound`.

## Data-driven architecture

The disc/install tree strongly exposes the game's logical decomposition.

Top-level `Data/` activity directories observed:

- `subgame1`
- `SubGameDino`
- `SubGameDYP`
- `SubGameFirework`
- `subgamegolf`
- `SubGameMaze`
- `subgameopen`
- `SubGameSpudMaze`
- `subgamespudskate`
- `subgamesquirrel`
- `UI`
- `movies`
- `music`
- `sound`

`loaddata/` contains 26 plaintext tables. Particularly useful examples include:

- `startupmovie.txt` - eight activity startup movies
- `binkwalk.txt` - walkthrough Bink movie to spoken-help WAV mappings
- `videoseq.txt` - startup/logo movie sequence
- `options.txt` - UI geometry/state values
- `uiHotArea.txt` / `uiHotAreaReplace.txt` - clickable UI regions
- `spudmaze_nodes.txt` / `maze_nodes.txt` - maze data
- `completedmovie.txt`, `playagain.txt`, `quitsure.txt` - flow/UI data

## First code anchors

These addresses are preliminary labels derived from direct string cross-references and need to be refined in Ghidra.

### Options loader: around `0x00407390`

At `0x00407396`, code pushes the address of the string `loaddata\\options.txt` and calls the common file-opening routine at approximately `0x00406E10`.

This routine then passes a large set of global addresses to a parsing routine, strongly suggesting that it populates the global options/UI geometry structure.

Suggested working name:

`LoadOptionsConfig_00407390`

### Dinosaur activity data: around `0x0040A440`

Code beginning near `0x0040A440` computes an indexed address into the contiguous dinosaur difficulty/path string table beginning at approximately `0x0043EED4`.

The table contains:

- Raptor Easy / Medium / Hard
- Tric Easy / Medium / Hard
- Trex Easy / Medium / Hard

The routine formats a `dino.txt` path, opens it, and repeatedly parses records into global arrays.

Suggested working name:

`LoadDinoLevelData_0040A440`

### Park-designer bounded-area loader: around `0x0040AEA0`

At `0x0040AEAC`, code pushes `data\\subgamedyp\\boundareas.txt`, opens it through the same common loader, and fills a set of fixed-size global tables.

Suggested working name:

`LoadParkDesignerBoundAreas_0040AEA0`

### Fireworks UI/resource setup: around `0x00411500`

The block near `0x00411500` loads `Data\\SubGameFirework\\certprint.bmp` and `certprintdep.bmp` through shared bitmap/surface helper routines. This is a useful entry point for identifying the common bitmap loader and then propagating names through all subgames.

## High-value shared functions to identify next

The earliest static cross-reference work points to several heavily reused helpers:

- `0x00406E10` - common text/data file open/load helper
- `0x0042FBB8` - repeated formatted/record parse helper
- `0x0042FB62` - close/free helper paired with the above
- `0x004038D0` - bitmap/resource load helper candidate
- `0x00403770` / `0x004036E0` / `0x00403B70` - surface/resource lifecycle helpers

These should be resolved before spending time on isolated subgame routines because naming them will clarify a large fraction of the call graph.


## Startup root and bulk table loader

The MSVC CRT entry point at `0x00430FEA` eventually calls `0x00402700` with the normal four `WinMain` arguments. Therefore:

- `0x00402700` = `WinMain`
- `0x00402B00` is the constructor-like initializer for the large game object allocated by `WinMain` (allocation size `0x7738` bytes)

Early in `WinMain`, the executable walks this registry hierarchy:

`HKLM\\Software\\BBC Multimedia\\Bob the Builder - Bob Builds a Park`

and queries `Install Path`.

The function at `0x00407920` is now identified as a bulk global-data loader. It calls, in order, the loaders for:

- UI bitmap names
- UI hot-area replacements
- UI hot areas
- UI hot-area counts
- help WAV mappings
- options
- video sequence
- quit-confirmation data
- Bink walkthrough/help mappings
- startup + completion movie lists
- play-again overlay data

This is a high-value boundary: most of the global UI/movie configuration can be reconstructed independently of the individual minigames.

## Additional verified subsystem entry points

- `0x00407E50 InitializeDirectInput` — creates/configures the DirectInput 8 interfaces/devices.
- `0x00408D90 InitializeBinkAudio` — passes `BinkOpenDirectSound` to `BinkSetSoundSystem`, tying Bink playback to the game's DirectSound object.
