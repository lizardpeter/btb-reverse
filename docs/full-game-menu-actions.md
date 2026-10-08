# Retail negative menu action closure — 2002 PE32 x86 evidence

This pass is a **direct re-disassembly of the verified installed**
`Exe/Bob the Builder - Bob Builds a Park.exe` (311,296 bytes, SHA-256
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`).
It is not a hypothetical mapping derived only from the names of the
twelve `loaddata/uiHotAreaReplace.txt` screens.

Source and evidence:

- `reconstruction/include/btb/full_game_menu_actions.hpp` — constexpr
  control/choice policy, preserving the original negative action values.
- `reconstruction/src/full_game_runtime.cpp` — consumes the selected
  action **after** the generic UI's managed click voice has completed.
- `ghidra/gameflow_menu_action_routes.csv` — per-branch executable
  handler address, destination state and modified global.
- `reconstruction/tests/full_game_menu_actions_source_test.cpp` —
  source-only regression cases for all recovered branches (not executed).

## Original 68-state dispatcher branch tables

The native update handlers dispatch negative chooser actions by indexing
six-entry pointers using **`action + 6`**, meaning -6 maps to slot zero,
-1 maps to slot five. The actual tables are:

| Update state | Jump table | −4 | −3 | −2 | −1 | −6 |
|---|---|---|---|---|---|---|
| Dino 0x11 | `0x0042CE94` | 0x42AE55 | 0x42AE37 | 0x42AE1B | 0x42C8CD | 0x42AE77 |
| Spud 0x17 | `0x0042CEC4` | unsupported | 0x42B2CF | 0x42B2B9 | 0x42C8CD | 0x42B2E5 |
| Adventure 0x1D | `0x0042CF0C` | unsupported | 0x42B916 | 0x42B900 | 0x42C8CD | 0x42B92C |
| Music 0x2B | `0x0042CF84` | 0x42C50A | 0x42C4F4 | 0x42C4DE | 0x42C8CD | 0x42C524 |

The table also has a default entry at action -5, jumping to the native
no-op handler `0x42CD65`, rather than a real selection.

### Exact chooser consequences

- Dino **-2/-3/-4**: switch outer state to pregame **0x12**, save a
  subgame/species index **0/1/2** to original global `0x51C2E4`, and
  copy the corresponding dword `0x51C344/348/34C` into variant global
  `0x51C284`. These dwords are loader-zero-initialized originally and
  rewritten by later Dino pregame setup. The source model retains their
  values instead of assuming the copied variant is always the species index.
- Spud **-2/-3**: outer states **0x38 Spud Maze** or **0x18 Spud
  Skate**, with `0x51C2E4` set to **0/1**.
- Adventure **-2/-3**: outer states **0x1E Maze** or **0x34
  Golf**, with `0x51C2E4` set to **0/1**.
- Music **-2/-3/-4**: all enter outer state **0x2C Bob's Band
  pregame**, with `0x51C2E4` set to **0 Bob, 1 Wendy, 2
  Farmer Pickles**. The unified `BobsBandDriver` now consumes this
  selected value when it loads the correct 480-byte saved composition.
- **-1 Back** in these four choosers targets **0x04 Activity Select**.
  The shared x86 handler is `0x42C8CD`.
- **-6 Help** keeps the current chooser update state. Contextual Help
  presentation and speech are still separate global UI work.
- **Activity Select -1** differs: x86 `0x42A917` returns to **0x01
  Player Profiles**, not to the Activity Select setup.

## Exact Play Again control

The `0x3D` replay Yes/No updater handles only the shipped **-20**
(Yes) and **-21** (No) values. Yes dispatches based on
`0x51C2FC`, the game-owned replay class:

- `1` → **0x3E**: choose another difficulty.
- `2` → **0x42**: choose Fireworks Edit or View.
- other values → original **saved state** at `0x51B418` without a new
  difficulty/menu choice.
- No (`-21`) → shared Bink/movie transition **0x40**.

These branches are present at `0x42CB5C..0x42CC27`.

The `0x3F` replay-difficulty updater recognizes **-30/-31/-32**,
writes **0/1/2** to `0x51C284` (Easy/Medium/Hard), stops the active
activity music, clears the replay class, then restores the saved state
from `0x51B418`. See `0x42CC73..0x42CCE9`.

Fireworks replay state **0x43**, at `0x42CA34`, has separate actions:

- `-20` Edit → **0x25 FireworksRun**, reset view mode to zero,
  restart editor/music/surface logic.
- `-21` View → **0x25 FireworksRun**, set the original inner mode
  `0x50A5BC` to **8**.

These are source-level **effect flags** until the rebuilt Fireworks
runtime can execute the corresponding resource and animation calls.

## Integrated game state and persistence

The shared `GameRoot` now applies supported negative actions to its
outer state and retains the source values instead of throwing away the
actions returned by `GenericUiScreenDriver`.

In addition:

- Golf's original exit at `0x4158BB..0x4158DE` stores saved state
  **0x36** and replay class **1** before reaching Play Again.
- Bob's Band exit at `0x4202A4..0x4202C9` stores saved state **0x2E**
  and replay class **1**.
- The unified `GolfDriver` now accepts the Easy/Medium/Hard value
  selected by a replay menu when it initializes a fresh round.
- The unified `BobsBandDriver` receives the Music chooser's selected
  conductor (Bob/Wendy/Farmer Pickles), instead of silently using a
  fixed Bob-only constructor default.
- Original source selection is tagged by chooser origin so that
  unrelated Spud/Music/Dino selection globals cannot accidentally
  change Golf's difficulty or Bob's conductor on a later activity.

## Verification boundary

This is more than a symbolic state map: it closes multiple original
menu input branches and activity-selection side effects. It is **not**
a verified executable, completed 12-screen front-end, running Bink
backend, or visually correct DirectDraw renderer.

New regression *source* fixtures check branch destinations, conductor
choices, replay difficulty and actual activity-root field propagation.
Following the current project instruction, **no full compilation, CTest
run, or new executable was performed**. Native screenshot/input/sound
differential tests remain necessary for full reversal.
