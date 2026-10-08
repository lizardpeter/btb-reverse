# Shared table-driven chooser and replay menu runtime

The original frontend is not twelve hand-built screens. It is one native
UI engine parameterized by `loaddata/NumUiHotArea.txt`,
`uiHotArea.txt`, and `uiHotAreaReplace.txt`.

New source-level modules:

- `reconstruction/include/btb/full_game_generic_ui.hpp`
- `reconstruction/src/full_game_generic_ui.cpp`
- `reconstruction/tests/full_game_generic_ui_source_test.cpp`

The generic runtime uses the existing `GenericUiCatalog` parser and
`front_end_ui_runtime.hpp` logic for managed hover-voice sequencing,
pressed/click sounds, current one-based selected-area index, and click
resolution after the managed voice completes. It rejects invalid catalog
sizes instead of making up rows.

## The seven confirmed source-integrated chooser/replay pairs

`GameRoot` now accepts a `FrontEndDriver` for these source-proven
pairs, based on the **real** 68-state dispatcher and 12-screen table indices:

| Outer setup/update | Generic screen | Description |
|---|---:|---|
| 0x10 / 0x11 | 3 | Dinosaur chooser |
| 0x2A / 0x2B | 4 | Music / Bob's Band conductor chooser |
| 0x16 / 0x17 | 5 | Spud Skate vs Spud Maze chooser |
| 0x1C / 0x1D | 6 | Adventure Playground / Maze vs Golf |
| 0x3C / 0x3D | 9 | Play Again: Yes/No |
| 0x3E / 0x3F | 10 | Play Again: difficulty |
| 0x42 / 0x43 | 11 | Fireworks edit/view replay |

State **0x04 / 0x05** (Activity Select, screen 1) still uses the existing
specialized `ActivitySelectDriver` to preserve its unique Fireworks Finale
progress gate. The new `GenericUiScreenDriver` can express screen 1 too,
but is not substituted over the already specialized root adapter.

Screens 0,2,7,8 remain outside the source-hosted pair table because some
are retained/unreachable or require instruction, walkthrough, Bink movie,
difficulty or other pregame behavior. Do not pretend they are fully routed.

## Geometry details

The original `uiHotArea.txt` was re-read from the uploaded game install.
Its format is polygon vertices terminated by `-1 -1`, with `-2 -2`
advancing screens. It contains some **non-axis-aligned / malformed-looking**
four-vertex rows:

- Instruction With Difficulty's walkthrough region has `(404,381)`,
  `(507,381)`, `(507,436)`, `(404,433)`: not a strict rectangle.
- Replay Difficulty's Easy region includes `(202,260)`,
  `(256,260)`, `(256,345)`, `(202,315)`: trapezoidal.
- Its Medium region also has slanted edges.
- A retained main-screen Options region repeats one corner instead of
  forming a four-distinct-corner rectangle.

`point_in_original_polygon` therefore uses ray crossing rather than
fitting a bounding box. Boundary coordinates are excluded. The
Activity Select fast path remains rectangle-specialized because its ten
original areas are rectangular.

This is a source-neutral hit-region implementation intended to honor all
the shipped geometry. Full executable differential testing of exact edge
semantics is still pending.

## Original action values

The retail `uiHotAreaReplace.txt` retains negative actions that do not
directly jump into the 68-state table:

- Back = **-1**, Help = **-6**
- Chooser or instruction options **-2..-5**
- Replay Yes/Edit = **-20**, No/View = **-21**
- Replay difficulty **-30..-32**

The shared driver returns `negative_ui_action` to the enclosing native
state's owner; it does not invent a target state from these numbers. Positive
actions in the valid original 0..67 range can route to the corresponding
outer state after managed voice completion. The Options flag **-99** stays
a separate modal action.

### Still missing before end-to-end completeness

The routing layer is now available, but the native per-state interpretation
of these negative action values, screen-specific backdrop/sprite drawing,
UI surface lifetimes, pregame walkthrough transitions, real DirectSound
completion, and the movie bridge remain to be connected.

The original `loaddata/uiBitmapName.txt` also contains 24 ordered bitmap
paths plus an `END.bmp` sentinel. Those map to page states, not a one-to-one
list of twelve generic UI screen indices. A future compositor must use the
real outer-state selection logic rather than indexing backgrounds with the
12-screen generic table index.

**No full compile or preview packaging was performed** for these source
changes; source regressions remain staged for the eventual complete game.
