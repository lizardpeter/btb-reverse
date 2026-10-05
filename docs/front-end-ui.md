# Front-end UI and transition architecture

The game's front end is strongly data-driven. The executable parses a small family of plaintext files in `loaddata/` and then uses a generic screen loader/drawer/input engine for menus, activity choosers, instruction screens, walkthroughs, and replay prompts.

## Parsed source tables

| File | Purpose |
|---|---|
| `uiBitmapName.txt` | base/background bitmap names |
| `NumUiHotArea.txt` | number of clickable regions in each generic screen |
| `uiHotArea.txt` | hit regions and symbolic actions |
| `uiHotAreaReplace.txt` | hover/selected/depressed images, sound/action metadata, transition values |
| `helpinfo.txt` | contextual help regions and spoken-help sound IDs |
| `binkwalk.txt` | walkthrough movie -> spoken-help WAV mapping |
| `playagain.txt` | play-again overlay rectangles |
| `quitsure.txt` | quit-confirmation layout |

The machine-readable ordered generic-screen map is in `ghidra/ui_screens.csv`.

## Generic UI screen index

Global `0x0051C27C` selects the current generic UI screen. The executable uses it to index the parsed hot-area counts and replacement records.

The 12 table-backed screens are:

| Index | Screen | Hot areas |
|---:|---|---:|
| 0 | Main UI | 8 |
| 1 | Activity selection | 10 |
| 2 | Instruction screen with difficulty | 6 |
| 3 | Dino sub-chooser | 5 |
| 4 | Music sub-chooser | 5 |
| 5 | Spud activity chooser | 4 |
| 6 | Adventure Playground chooser | 4 |
| 7 | Fireworks instruction | 3 |
| 8 | Instruction screen without difficulty | 3 |
| 9 | Play Again: Yes/No | 2 |
| 10 | Play Again: Easy/Medium/Hard | 3 |
| 11 | Fireworks Edit/View replay | 2 |

There is an original-data naming error in `uiHotArea.txt`: screen group 4 is labeled as a second `Dino_sub_chooser`. The corresponding `uiHotAreaReplace.txt` section names it `MUSIC_sub_chooser`, and its placement in the state machine confirms that interpretation.

## Activity-selection actions

The activity-selection table names the top-level choices directly:

- Pets Corner
- Dino Discovery
- Fireworks
- Spud activity
- Squirrel Run
- Music Practice
- Adventure Playground
- Build Your Park
- Back
- Help

The two nested chooser screens are now tied to their executable branches:

- **Spud chooser** -> Spud Skate or Spud Maze
- **Adventure Playground chooser** -> Maze or Golf

The Music chooser contains three sub-choices before entering the Grand Opening/music activity.

## Generic UI functions

### `0x00429150 LoadGenericUIScreenResources`

Uses `0x0051C27C` to select the parsed screen record. It constructs bitmap filenames from the replacement table, loads the surfaces through the shared DirectDraw loader, registers them for surface-loss recovery, and applies magenta transparency to overlay surfaces.

### `0x00428ED0 UnloadGenericUIScreenResources`

Walks the same parsed per-screen arrays, unregisters/releases their DirectDraw surfaces, clears the pointers, and resets generic UI selection state.

### `0x00428E30 DrawGenericUIScreen`

Draws the current front-end composition to the display manager's render surface. It blits the generic background plus the current foreground/overlay surface.

### `0x00428600 UpdateGenericUIScreenInteraction`

This is the main table-driven menu/input interpreter. It:

1. polls the shared input layer
2. finds the current screen's hot areas
3. evaluates rollover/click state
4. switches selected/depressed overlay surfaces
5. triggers UI sounds
6. consumes the action/transition metadata parsed from `uiHotAreaReplace.txt`
7. writes the resulting game-flow action/state

The negative action values emitted by this routine are deliberately interpreted by the surrounding game-flow states rather than being ordinary state-table indices.

### Geometry helpers

- `0x004294D0 PointInRectInclusive` — integer rectangle hit test
- `0x00428520 PointInPolygon` — ray-crossing polygon hit test used by activity-specific irregular hit regions

## Walkthrough subsystem

Walkthrough video is kept separate from the global Bink movie player used for logos/startup/completion movies.

### `0x004281D0 OpenWalkthroughMovie`

Indexes the table populated from `binkwalk.txt`, opens the selected Bink file with the game's normal CD-path fallback, allocates the playback surface, and stores the dedicated walkthrough Bink handle.

The ordered walkthrough table corresponds to:

0. Herding
1. Dinosaur
2. first Spud activity
3. second Spud activity
4. first Adventure Playground activity
5. second Adventure Playground activity
6. Fireworks
7. Squirrel
8. Music / Grand Opening
9. Park Designer

### `0x00428120 UpdateWalkthroughMovie`

Decodes/blits the walkthrough frame and loops the movie at the end. While the pointer is over the walkthrough panel, it also uses the companion `binkwalk.txt` help mapping to play the activity's spoken-help WAV through the managed sound system.

### `0x004280D0 CloseWalkthroughMovie`

Closes the walkthrough Bink handle, unregisters/releases its DirectDraw playback surface, and frees the copied path/storage buffer.

## Recovered chooser and pregame state pairs

The central state machine now has semantic names for the front-end states before each activity:

| States | Meaning |
|---|---|
| `0x0C / 0x0D` | Herding pregame/instruction/walkthrough |
| `0x10 / 0x11` | Dino chooser |
| `0x12 / 0x13` | Dino pregame/instruction/walkthrough |
| `0x16 / 0x17` | Spud chooser |
| `0x18 / 0x19` | Spud Skate pregame |
| `0x38 / 0x39` | Spud Maze pregame |
| `0x1C / 0x1D` | Adventure Playground chooser |
| `0x1E / 0x1F` | Maze pregame |
| `0x34 / 0x35` | Golf pregame |
| `0x22 / 0x23` | Fireworks pregame |
| `0x26 / 0x27` | Squirrel pregame |
| `0x2A / 0x2B` | Music chooser |
| `0x2C / 0x2D` | Grand Opening/music pregame |
| `0x30 / 0x31` | Park Designer pregame |

This explains the apparently non-linear state numbering: chooser states branch to activity-specific pregame pairs, which then advance to the already recovered init/run activity pair.

## Play-again flow

The late state-machine block is now decoded directly from the screen tables:

- `0x3C` — load **Play Again Yes/No** (generic screen 9)
- `0x3D` — update that choice
- `0x3E` — load **Play Again with difficulty** (screen 10)
- `0x3F` — update Easy/Medium/Hard replay selection
- `0x42` — load **Fireworks Edit/View replay** (screen 11)
- `0x43` — update the Fireworks replay choice
- `0x40` — shared Bink/movie transition used while returning to the saved/default front-end state

`0x00429500 UpdatePlayAgainPrompt` uses the rectangles loaded from `playagain.txt`, tracks the active choice, and feeds the result back into this flow.

## Contextual help screens

`helpinfo.txt` names 20 help contexts, including:

- Enter Name
- Activity Select
- Spud Select
- Dino Select
- Crazy Golf Select
- Music Select
- pregame with/without difficulty
- Park Designer in-game
- Spud Maze in-game
- Spud Skate in-game
- Dino in-game
- Squirrel
- Golf Maze / Golf
- Music
- Pets Corner
- Fireworks
- Progress Screen

These records provide another direct bridge from anonymous numeric sound IDs and hit rectangles to user-facing game semantics.


## Modal precedence

Before dispatching the normal 68-state game-flow table, `RunMainGameFlow` checks four modal flags in a fixed order. This means these overlays temporarily intercept the frame without changing the underlying activity state.

1. options flag `0x0051C2BC` -> `0x00429800 UpdateOptionsOverlay`
2. generic Yes/No confirmation state `0x0051C2D0` -> `0x00429C90 UpdateYesNoConfirmationOverlay`
3. whole-game quit flag `0x0051C2C0` -> `0x00429AA0 UpdateQuitConfirmationOverlay`
4. leave-current-activity flag `0x0051C2C8` -> `0x00429FE0 UpdateLeaveActivityConfirmation`

### Options

`UpdateOptionsOverlay` draws `data\\ui\\options\\optionsscreen.bmp` and `pointer.bmp`, moves the pointer across the slider geometry loaded from `options.txt`, converts that position into the game's global volume value, and propagates it to DirectSound and Bink playback.

### Whole-game quit confirmation

`UpdateQuitConfirmationOverlay` uses the shared quit background and Yes/No button art loaded from `quitsure.txt`. No simply dismisses the modal. Yes stops active sounds, chooses one of several exit voice lines, waits for that line to finish, and then raises the application-level quit flag.

### Generic Yes/No confirmation

`UpdateYesNoConfirmationOverlay` is a reusable modal, not the contextual-help renderer. Callers set:

- `0x0051C2D0` — modal lifecycle/active state
- `0x0051C310` — underlay/context selector
- `0x0051C294` — caller-selected modal bitmap
- `0x0051C2DC` — result flag written by the modal

The modal redraws the underlying screen according to context before compositing the confirmation bitmap. Context values currently observed are:

- 0 -> underlay helper `0x0042DB70`
- 1 -> `0x0040DB50`
- 2 -> `0x0041F090`
- 3 -> `0x00412E50` (Fireworks edit screen)

Fireworks **Delete All** is confirmed evidence for context 3: it loads `data\\ui\\DeleteFireworks.bmp`, activates this modal, and consumes `0x0051C2DC` on return.

The two modal buttons are fixed rectangles around X=220..312 / 334..421 and Y=260..312. A Yes click writes result 1; No writes result 0. Both dismiss the modal and restore normal input/Bink pause state.

### Contextual help data

`helpinfo.txt` is still the game's contextual-help table, but it is consumed elsewhere in the front-end/activity help paths rather than by `0x00429C90`.

### Leave-current-activity confirmation

`UpdateLeaveActivityConfirmation` deliberately remains separate from the whole-game quit modal even though it reuses much of the Yes/No art. Accepting it raises global `0x0051C2E8`, which the activity update routines poll as their common abort/return-to-front-end request.

## Startup movie helpers

`0x0042A1A0 UpdateStartupVideoSequence` walks the ordered `videoseq.txt` table through the shared Bink player. Its state consists of the current sequence index and playback phase; once every entry has completed it reports success to the outer flow.

`0x0042A240 UpdateSceneSetMovie` is a dedicated helper for `data\\movies\\sceneset.bik`; the main flow uses it as one of the early front-end transitions.


## Player profiles and name entry

The retail profile system is separate from the generic table-backed UI.

### Player-profile screen

`0x0042E110 UpdatePlayerProfileScreen` draws and updates the five wooden profile signs.

A profile is considered occupied when it has either:

- a non-empty name, or
- a badge selection.

This matters because the retail UI permits a **badge-only profile**.

The screen supports:

- choosing an existing profile and going directly to Activity Select;
- choosing an empty slot and opening the name/badge popup;
- deleting an existing profile;
- Back/quit behavior.

### Name/badge popup

`0x0042DB70 DrawEnterNamePopup` renders:

- the name-entry background;
- the selected badge;
- the typed name centered using the boxed blue-font glyph table;
- the blinking insertion cursor.

`0x0042DCC0 UpdateEnterNamePopup` handles the keyboard, badge arrows, Start, and Back.

A new profile is routed through state `0x41`, which plays
`data\\movies\\sceneset.bik`, before Activity Select. An existing profile
goes directly to state `0x04`.

### Name-entry encoding

Each profile reserves **9 int32 glyph-code slots**, but retail accepts at most
**8 typed characters**.

The live key path:

1. scans DirectInput key states;
2. ignores both Shift scan codes `0x2A` and `0x36`;
3. handles scan code `0x0E` as Backspace;
4. refuses new characters once length reaches 8;
5. requires the translated character code to be greater than `0x21` and no
   greater than `0x7F`;
6. only accepts physical scan codes through `0x35`;
7. stores:

```text
glyph_index = translated_character_code - 0x21
```

The renderer uses that stored byte directly as the index into the
`fontdatablue.txt` glyph rectangles.

`loaddata/ascii.txt` is still loaded at startup into a separate global table,
but no live runtime consumer has been proven in this executable. It is therefore
treated as retained/legacy source data rather than silently substituted into the
name-entry path.

### playerinfo.txt

Profile metadata is saved in `playerinfo.txt`.

Retail opens it with `wb` on save but writes ASCII integers with `fprintf`.

The format is two passes:

1. five `name_length badge_index` pairs;
2. for each profile, exactly `name_length` glyph-code integers.

In memory:

- five name lengths live in a five-int table;
- five badge indices live in a five-int table;
- each name has a fixed **9-int / 0x24-byte** backing slot.

The buildable reconstruction is in:

- `reconstruction/include/btb/player_profiles.hpp`
- `reconstruction/src/player_profiles.cpp`
- `reconstruction/tests/player_profiles_test.cpp`

### Retail profile deletion

Deleting a profile:

1. sets its name length to 0;
2. sets its badge index to -1;
3. deliberately leaves the stale nine-int name backing slot untouched;
4. zeros only player-progress slots **50..64**;
5. truncates that player's:
   - `dypdataN.txt`
   - `firedataN.txt`
   - `musicbobN.txt`
   - `musicwendyN.txt`
   - `musicfarmerN.txt`

The numbered `playerN.txt` progress file is not truncated at that moment;
the zeroed in-memory progress record is written later by the normal profile-save
path.

## Early dispatcher states

The remaining early states are now separated into live and retained paths.

| State | Meaning |
|---:|---|
| `0x00` | startup movie sequence, then profile-sign setup |
| `0x01` | live five-slot profile/name-entry flow |
| `0x02` | immediate redirect to Activity Select |
| `0x03` | retained generic `Main_start_screen` / old Activity Select variant |
| `0x04` | live Activity Select setup |
| `0x05` | live Activity Select update |
| `0x06` | generic UI teardown for retained path |
| `0x07` | deliberate no-op |
| `0x08` | whole-game quit-confirmation underlay/setup |
| `0x09` | activate whole-game quit modal and restore Activity Select underneath |

The original tables explain state `0x03`: generic screen 0,
`Main_start_screen`, contains the same eight activity tiles as the live
Activity Select but lacks the screen-1 Back/Help rows. Normal startup and
profile selection bypass it and use screen 1.
