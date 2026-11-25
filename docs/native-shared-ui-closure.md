# Native shared UI closure: progress, quit, credits
Source identity: original 311296-byte PE32, SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05. No original binaries or assets are included.

- 00427C30: inclusive 30-pixel star-coordinate rectangles, hover latch 00446FE0, narration IDs 955–959, and demand-driven 0042FFC4 random calls for complete groups only. Unfilled stars remain interactive. Slots 63/64 retain retail draw behavior but have no narration.
- 00429AA0: whole-game quit is independent of deletion and leave-activity. Strict Yes/No rectangles, hover voice 453/454, random farewell 626–629 (priority 90, class 1), and managed-voice completion before starting credits. The profile adapter previously routed quit to the delete modal.
- 0042A9A5 and 0042A9F3: state 8 snapshots the preceding rendered scene; state 9 opens quit and returns outer dispatch to state 4.
- 004069C0 and 00406AD0: credits filenames come from the installed loaddata/credits.txt; 128-byte filename slots and ten-entry table. The original elapsed integer seconds are calculated at 0041A700 by dividing FILETIME ticks by 10,000,000. Source uses the host elapsed centiseconds and the equivalent strict >4 seconds threshold. Click pulse advances, and the Win32 host posts WM_CLOSE only after the game frame returns.
- The Windows input carries the same process RNG as the activity drivers; profile narration no longer relies on an uninitialized per-frame random number.

The bitmap-only snapshot is represented by draw commands, not an actual captured DirectDraw surface. Credits and quit have source regressions; physical audio, display, and native window teardown remain to be verified in an interactive Windows run. No new function-coverage claims are made here.

## Options and WAV volume

00407390 and 00429800 supply the authored options origin and slider width; runtime overrides slider Y to 150..180 and uses strict OK bounds 276..393, 253..312. Draw the pointer at the preceding volume, then apply the held-pointer change; voice 862 gates dismissal. Native device dispatch applies (percent-100)*42 attenuation to music/activity WAV buffers, not managed speech. Existing and subsequently created music buffers use the new setting. The remaining 004093C0 stop/restart behavior and 00409210 Bink gain are separate work, as are cinematic-backed overlay snapshots and live gain validation. The source does not invent a new Options entry in the retail menu data.
