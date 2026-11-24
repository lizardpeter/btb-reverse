# Retail input capture and retained difficulty
Source: original executable SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
00428600 stores the selected pressed row at 00446CEC, permits held hover only
on the captured row, and arms the deferred transition on the release pulse
004FBFB4, not the down pulse 004FBE54. The real Win32 input path now keeps
the press location even for a down/up occurring between ticks. Source-only
fixtures may still inject an already-completed click. Both UI drivers use
the same capture policy. Exact edge/hover timing still needs Windows parity.

0042A2C0 retains separate difficulty globals: Herding 51C340; three Dino
species 51C344/348/34C; Spud Maze 51C350; Golf 51C354; Maze 51C358;
Squirrel 51C35C. Restore the relevant value before driver setup and tag the
original instruction state so the selected difficulty is used even without
a new click. The previous root only retained Herding and leaked the common
variant between games. The 00428600 difficulty indicator uses the selected
row's hover frame at the secondary coordinates before interactive art.
Regression sources cover all six difficulty pairs, independent Dino species,
capture/release mismatches, rapid taps and the selected artwork coordinates.
