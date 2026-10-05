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
