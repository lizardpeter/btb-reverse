# Native front-end visual and Start admission fixes

Original PE32 SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
The original uiHotAreaReplace.txt has three no-difficulty Start rows
encoded as -2 in addition to Skate. Direct disassembly confirms the
Fireworks (42BFF3 -> 42BFF9), Band (42C668 -> 42C66E), and Park Designer
(42C8E3 -> 42C8E9) paths fall through after writing zero. The old route
table incorrectly stopped at that write, trapping players at Start.

00429150 loads numbered/un-numbered bitmap sequences. The eight Activity
Select idle/hover banks are 10/20 frames respectively, and all 240 names
were confirmed against the original Actanims directory inventory.
00428600 draws the active hover frame, optional keyed surface, pressed
bitmap, and other rows' idle animations. The native shared UI now emits
these original paths at the table coordinates. The delay wraps at 8
for hover, and the global idle countdown wraps from zero to five.
The pending managed-audio route still bypasses the animation draw pass.

Remaining physical validation: retained replay movie background, exact
mouse capture/release behavior, persistent difficulty highlight, visual
layer order of walkthroughs and live DirectDraw timing.
