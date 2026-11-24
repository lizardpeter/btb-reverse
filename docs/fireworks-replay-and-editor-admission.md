# Fireworks retained replay and editor admission

Original PE32 SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
0042C08B processes Back in internal state >=9 using replay class 2 and
saved outer state 25, retaining the initialized activity. 0042CA34 resumes
state 0 (Edit) or 8 (Play, without the global intro). The native root
previously disabled Back and ignored the two replay action flags.
004126F0 only admits the first twelve placement regions with Bob's
state-1 latch and the last six with Wendy's, and picks up existing cells
with the normal cursor. Enforce that in the production editor, so candle
or wheel types cannot enter the right-side movie bank's invalid range.
00413450 composites the keyed minimised.bmp frame after movie pixels;
the native device now submits those tagged draws after movie overlays.
The source scene is retained for replay, with the layout saved on No.
The original exact cursor offset/warp behavior, surface-loss restoration
of dynamic movie pixels, and physical Windows input remain to be tested.
