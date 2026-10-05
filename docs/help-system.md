# Contextual help system

The retail game has a global contextual-help mode that temporarily replaces the
normal 68-state activity dispatcher inside the active frame loop.

Main functions:

- `0x00401030 LoadHelpInfo`
- `0x004282C0 UpdateHelpButtonController`
- `0x004011B0 EnterContextualHelpMode`
- `0x00401700 UpdateContextualHelpMode`
- `0x00401000 PlayHelpVoice`

## Runtime globals

High-confidence roles:

| Address | Meaning |
|---|---|
| `0x0044A2A0` | Help-button activation phase: 0 idle, 1 pressed, 2 ready to enter |
| `0x0044DDA0` | contextual-help mode active |
| `0x0044DDAC` | current help context index |
| `0x0044DDA8` | current help hotspot/token, used to avoid replaying the same voice |
| `0x0051C298` | retained background surface composited while help is active |

The active frame does:

```text
if HelpModeActive:
    UpdateContextualHelpMode()
else:
    RunMainGameFlow()

if HelpActivationPhase >= 2:
    EnterContextualHelpMode()
```

so help is a true alternate per-frame mode, not another outer game-flow state.

## Help-button controller

`UpdateHelpButtonController` is called by the interactive front-end and
activity update states that expose Help.

Retail behavior:

1. hit-test/draw the shared Help control;
2. a click changes activation phase from 0 to 1;
3. on the next update, phase 1 changes to phase 2;
4. the helper swaps to the help cursor;
5. active managed sounds and shared activity buffers are stopped;
6. `RunActiveGameFrame` observes phase >=2 and calls
   `EnterContextualHelpMode`.

The enter function resets the activation phase to 0 after switching modes.

## Entering help

`EnterContextualHelpMode`:

1. composites the retained background surface;
2. sets HelpModeActive = 1;
3. clears the previous hotspot token;
4. plays sound **476 = GENH_WEN_01.wav**;
5. selects a help context from the current outer game-flow state;
6. cancels help immediately if that outer state has no supported context.

The exact context mapping is represented in:

- `reconstruction/include/btb/help_system.hpp`
- `ghidra/help_contexts.csv`

## Twenty help contexts

The indices come directly from the ordered sections in `helpinfo.txt`.

| Index | Context |
|---:|---|
| 0 | profile / Enter Name screen |
| 1 | name-and-badge entry popup |
| 2 | Activity Select |
| 3 | Spud activity chooser |
| 4 | Dinosaur chooser |
| 5 | Adventure Playground / Maze-Golf chooser |
| 6 | Bob's Band / Music chooser |
| 7 | pregame instructions without difficulty |
| 8 | pregame instructions with difficulty |
| 9 | Park Designer |
| 10 | Spud Maze |
| 11 | Spud Skate |
| 12 | Dinosaur Discovery |
| 13 | Squirrel Run |
| 14 | Maze |
| 15 | Golf |
| 16 | Bob's Band |
| 17 | Pets Corner |
| 18 | Firework Finale |
| 19 | Progress screen |

## Outer-state mapping

The state table inside `EnterContextualHelpMode` has one entry for each of
the 68 outer game-flow states.

The useful pattern is deliberate: setup/init states generally map to `-1`,
while their interactive/update partner maps to the appropriate help context.

Examples:

| Outer state | Activity state | Help context |
|---:|---|---:|
| `0x05` | Activity Select update | 2 |
| `0x0D` | Pets Corner pregame update | 8 |
| `0x0F` | Pets Corner runtime | 17 |
| `0x11` | Dino chooser update | 4 |
| `0x13` | Dino pregame update | 8 |
| `0x15` | Dino runtime | 12 |
| `0x17` | Spud chooser update | 3 |
| `0x1B` | Spud Skate runtime | 11 |
| `0x21` | Maze runtime | 14 |
| `0x25` | Fireworks runtime | 18 |
| `0x29` | Squirrel Run runtime | 13 |
| `0x2B` | Music chooser update | 6 |
| `0x2F` | Bob's Band runtime | 16 |
| `0x33` | Park Designer runtime | 9 |
| `0x37` | Golf runtime | 15 |
| `0x3B` | Spud Maze runtime | 10 |

### Profile special case

Outer state `0x01` stores sentinel value `-99` in the retail mapping
table.

Retail resolves that sentinel dynamically:

- profile substate zero -> context 0
- nonzero name/badge popup state -> context 1

### Progress-screen override

If global `0x0051C324` indicates the Progress screen overlay, retail forces
help context **19** after the normal state lookup.

## helpinfo.txt records

Each context is a sequence of five-integer records:

```text
left top right bottom sound_id
```

terminated by:

```text
-1 -1 -1 -1 -1
```

`LoadHelpInfo` retains a count for each context and stores the region/sound
records in a fixed global table.

The first record in the in-game contexts is usually the common bottom-right
Help/back region. Typical in-game first record:

```text
579 420 625 465 482
```

where **482 = GENH_WEN_07.wav**.

The Progress-screen first region uses sound 478 instead.

## Help voice playback

`0x00401000 PlayHelpVoice(sound_id, stop_existing)` is used only by this
contextual-help subsystem.

If `stop_existing == 1`, it first stops the managed sound system, then calls
the normal sound manager with:

- requested sound ID
- priority 50
- retail playback flag 2

This makes contextual help speech exclusive rather than layered over the
activity's previous voice line.

## Dynamic activity hit testing

`UpdateContextualHelpMode` does not rely solely on the static rectangles in
`helpinfo.txt`.

For several in-game contexts it first derives live rectangles from the current
activity state. Verified examples include:

- Fireworks active editor/show geometry;
- loose Dinosaur piece rectangles;
- Maze/Adventure runtime objects;
- Bob's Band machine/control geometry;
- other activity-specific runtime objects whose positions can move or scroll.

These branches play the matching activity help IDs and assign private hotspot
tokens so the same voice does not continuously retrigger while the pointer
remains over the object.

After the activity-specific tests, retail iterates the static records for the
current `helpinfo.txt` context.

## Hover behavior

The current hotspot token is stored at `0x0044DDA8`.

Retail only plays a help voice when the newly hit region/token differs from the
stored token. Moving away from applicable dynamic regions clears or changes the
token as appropriate.

This is why help lines fire once on entering a region rather than every frame.

## Exiting help

The first static help region doubles as the help/back exit control.

When region index 0 is hit while the mouse button is active,
`UpdateContextualHelpMode` jumps to its common exit path.

The exit path:

1. clears HelpModeActive;
2. clears the shared transient input/help flag at `0x004FBE54`;
3. stops all managed sounds;
4. restores the normal cursor through `SetCursorSurface(NULL)`;
5. sets global `0x0051C31C` to **50**, a short post-help cursor/input delay.

Normal outer game-flow execution resumes on the next active frame.

## Source reconstruction

`reconstruction/include/btb/help_system.hpp` now preserves:

- all 20 context IDs;
- the exact 68-entry retail outer-state map;
- the dynamic profile sentinel;
- the Progress-screen override;
- strict rectangle hit-test semantics;
- Help-button activation phases;
- generic help intro sound 476;
- post-help delay value 50.

This remains faithful to the retail global-state architecture. A modern
event-driven help/UI system belongs in the later Rust/Vulkan or modern-C++
rewrite, not in the decompilation-equivalent source layer.
