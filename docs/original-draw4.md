# Four original-PE32 draw coordinators

Source executable SHA-256:
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`

Four original-address out-of-line C++26 functions, with typed state,
source-derived draw ordering and dedicated executed regression:

| Address | Recovered source control flow |
|---|---|
| 00409990 DrawDinoActivity | Draws original background and character/interface sprite sheets, advances both animation counters and pending phase changes, then traverses the original 48-byte entity list in two separate rendering passes. |
| 0041C110 DrawMazeActivity | Renders the screen's 4 node/actor records, chooses original 12-byte node positions or explicit moving coordinates, updates player and secondary sprite animation, sound/event timing, and the original timer display state. |
| 00414A60 DrawGolfActivity | Background and split-digit score, marker count, angle/magnitude projected ball/trails, club sprite rectangle, two animated character sheets and three auxiliary sprites, plus swing meter. Retains original lost-surface retry as an explicit host boundary. |
| 00416370 DrawHerdingActivity | Clamps scrolling camera, depth-sorts pointers without mutating world entity order, handles distinct original type 1/2/3/7/12/13/14/15/16 sprite clip and tick rules, then renders fixed props, foreground and status overlay. |

Differences requiring follow-up original executable differential comparison:
the original x87 projection/truncation, unwound DirectDraw vtables, missing
game-global source rectangles and atypical animation states, unverified
entity timer cadence and some format-specific sprite assets. For example,
the retail renderer uses an original qsort depth comparator while this
host representation uses a stable depth sort with explicit tie-breaking.
Those changes must be evaluated instead of being represented as exact
retail equivalence.

All four source functions call a required platform `IO` interface for
rendering, sound, random values and source-specific geometry. These
interfaces are mandatory dependencies, **not** simulated DirectDraw or
silently fabricated game assets. Live native-game caller admission and
real resource providers remain unfinished.

This patch is a genuine four-function *source model* batch, not a retail
binary-identical port. The functions remain `source_written` until
Linux C++26 and Windows x86/x64 builds and regression tests pass.
No additional original-executable comparisons were performed; the fully
retail-verified count remains 0/615.
