# Bob's Band / Music sequencer — source reconstruction

**Status:** C++26 gameplay source is now present under
`reconstruction/include/btb/bobs_band*.hpp` and
`reconstruction/src/bobs_band*.cpp`. The source has NOT been included in the
unified executable build yet; full-game integration and the next full compile
are deliberately deferred. These modules were written from the original PE32
executable and the installed `machinedata.txt`, not approximated from video.

## Binary ownership and entry points

| RVA/VA | Working name | Evidence / responsibility |
|---|---|---|
| `0x0041D7C0` | `LoadBandMachineData` | Opens `data\\subgameOpen\\machinedata.txt`; reads 10 machine positions, 10 sizes, 3 conductor positions, 3 sizes, 3 frame counts, and 10 palette rectangles |
| `0x0041D920` | `UnloadBandResourcesAndSave` | Releases surfaces/audio; writes 120 32-bit composition cells via C runtime I/O for the selected player/conductor |
| `0x0041DB20` | `InitializeBobsBand` | Sets tables, surfaces, note samples, backing tracks and reloads the selected 480-byte music composition |
| `0x0041F090` | `DrawBobsBandScene` | Draw order, five machine animation updates, conductor editor animation |
| `0x0041F3A0` | `CanPlaceBandNote` | Tests `span(type)` cells within a single 24-column row; strict empty value -1 |
| `0x0041F3F0` | `GetBandWallRowSecond` | Derives row=index/24 and second=index%24 from last selected 132-entry hit-table index |
| `0x0041F430` | `RemoveBandNote` | Walks backward from continuation values >9; removes owner note and its continuation |
| `0x0041F4B0` | `PlaceBandNote` | Writes owner type and continuation 10; starts machine short/long animation only when its current animation is idle |
| `0x0041F550` | `HandleSelectedBrickClick` | Palette switch/cancel, attempted replacement/rollback, peculiar type-9 rollback scan |
| `0x0041F6E0` | `HandleEditorIdleClick` | Palette choice, grid note pickup or deletion, delete cursor normalization |
| `0x0041F820` | `DrawAndUpdateBandToolbar` | Toolbar overlays and hover/click handling |
| `0x0041FB90` | `ResolveBandHotRegion` | Wall/palette/special action hit-table lookup |
| `0x0041FC60` | `GetBandElapsedCentiseconds` | Original high-resolution timer conversion |
| `0x0041FC90` | `UpdateBandPlayback` | 24-second window, five simultaneous pitch rows, sound triggers and conductor playback animation |
| `0x00420030` | `RunBobsBandActivity` | Internal 11-state dispatcher, progress, playback start/stop, UI exit routing |

## Data and grid invariants

The composition occupies **480 bytes**, 120 little-endian signed int32
cells at `0x005123B8..0x00512597`. Grid geometry:

- 5 pitch rows × 24 second columns
- screen-space origin `(44,310)`; 23×19 pixels per cell
- `-1` empty; `0..9` ten machine sound types;
  `10` continuation of a two-second note
- types 0,2,4,6,8 occupy **one** column; types 1,3,5,7,9
  occupy **two** columns
- a long note at second 23 cannot fit, but one at second 22 can
- on an occupied continuation cell, idle-mode deletion resolves the owner's
  original left column; selected-brick placement does **not**
- a selected-brick click on a start note removes that note, tries the new
  placement, and restores the previous note on placement failure
- **retail quirk:** at `0x0041F602`, the unusual `cmp eax,9; jl`
  means a touched sound type 9 takes an additional backwards scan. The
  native code still deletes the clicked note; it is the failed-placement
  **restoration column** that is changed by this scan. The source retains
  this distinction, with a safety stop at row boundary.

Both the editor grid and the 480-byte little-endian composition codec are
implemented in `bobs_band.hpp/.cpp`. The codec can retain nonstandard raw
cells without implicit repair. `structurally_valid()` is a separate
diagnostic, so reading a retail save does not silently rewrite it.

`musicbob1.txt` through `musicbob5.txt`, the equivalent Wendy files, and
`musicfarmer1.txt` through `musicfarmer5.txt` are the 15 player/conductor
composition slots. The executable indexes their fixed 128-byte path slots by
`(player*3 + conductor)*128`. The file-open path and 480-byte read/write
loops are confirmed at `0x0041DA78..` and `0x0041EA1E..`.

## Visual and audio mapping

Original `machinedata.txt` was inspected from the user's installed game
files. It contains the actual ten short/long machine source coordinates and
sizes plus conductor metadata and palette hitboxes. Unlike an earlier rough
visual geometry spreadsheet, the loader-derived values are source-authoritative
for these fields; hard-coded alternate dimensions are not used.

Machine identities and sound types:
`0/1 Roley`, `2/3 Muck`, `4/5 Lofty`,
`6/7 Dizzy`, `8/9 Scoop`. The editor image assets are
`sound1.bmp..sound10.bmp` and `hsound1.bmp..hsound10.bmp`.
The full-screen background is `music_01.bmp`, conductor character sheets
are `BOBINSTAND.bmp`, `wendyINSTAND.bmp` and
`picklesINSTAND.bmp`, and the toolbar is `toolbar.bmp`.

Draw order is original: **background → note sprites (row-major) → Scoop →
Dizzy → Lofty → Muck → Roley → selected conductor → toolbar**.

Five machine channels have modes `0=idle`, `1=1-second short sheet`,
`2=2-second long sheet`. When active, they add the shared frame delta to a
timer; a timer exceeding 3 advances one animation frame and resets.
Short sheets contain 15 animation frames; long sheets contain 30.
A new note trigger never interrupts a currently animated machine.

The editor conductor advances after a six-tick window. Idle frames 0..19
may extend through 20..49 with a 1-in-3 random choice; frame 50 wraps to 0.
During playback, conductor frames are:
Bob 50..89, Wendy 50..84, Farmer Pickles 50..104;
animation advances after counter >3.

Native sound bank indexing is **`40 + type` for top row**,
`30 + type` for the next, descending to `type` for the bottom.
That means the filenames for top-row Roley short notes end with
`roley1_5.wav`; bottom-row notes end with `roley1_1.wav`.
The backing tracks are `bobmt.wav`, `Wendymt.wav`, and `fpmt.wav`.
Clicking a palette machine previews the center-pitch WAV bank; the editor
does not play a new note just because a brick is picked up.

## State and playback integration

`Editor`, `Playback`, `MachineAnimations`, and `Activity` now have
separate responsibilities. The activity controller provides a frame command
list for the eventual universal host and does not directly call COM APIs.

- `0` Edit idle; `1` brick carried, `2` draw only;
  `3..7` native no-op entries retained in the state enum
- Play button arms delayed start and voices `conductor_voice_base+1`
  (sound bases Bob=510, Wendy=523, Pickles=536).
  **The same frame cannot start playback**, even if audio was idle before
  the Play click. The next updates wait until managed audio finishes.
- `8` begins playback: save selected conductor progress as **slot 52/53/54
  = 1**, set shared completion code **6**, capture performance counter,
  reset previous elapsed centiseconds to **999999**, rewind/start backing
  track, and enter `9`.
- `9` uses integer `elapsed_centiseconds/100`. The first bucket triggers
  second 0. Each change of second triggers sound samples from all five rows
  that have a start note in that column. Continuations are skipped.
  Once the second counter is 24 or later, no new note is triggered.
  Playback returns to edit mode on Stop or after the backing track ends.
- `10` saves/exits to Play Again (outer state **`0x3C`**, context **`0x2E`**).

Delete mode auto-cancels unless cursor Y is strictly between 289 and 401 or
a deletion happened on the current frame. The Clear All button requests the
original Yes/No modal; only a nonzero/affirmative answer wipes all 120 cells.

## Remaining work before full game

These pieces still require **integration**, not claims of completeness:

1. Feed `BandFrame` draw commands to the original bitmap compositor or
   universal renderer, including live surface source/destination rectangles.
2. Connect requested backing/note sound files to the shared DirectSound
   reconstruction and implement retail voice arbitration with the exact
   random-draw order.
3. Wire the global 68-state game router, UI conductor chooser, native
   player/profile file layout, save/load callbacks, and progress persistence
   to this activity.
4. Complete toolbar hover/pressed rendering and modal return behavior in
   the shared UI host. Fill the state-2 and dormant code paths if evidence
   shows normal-game reachability.
5. Run original-game differential capture tests, then full-game compile and
   integrated tests when all other game modules are similarly connected.

**Compilation, preview packaging, and CI verification of these new modules
were intentionally not requested at this stage.** These commits add source
reconstruction and evidence; they do not establish a working Band build.
