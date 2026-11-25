# Native shared UI closure: progress, quit, credits
Source identity: original 311296-byte PE32, SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05. No original binaries or assets are included.

- 00427C30: inclusive 30-pixel star-coordinate rectangles, hover latch 00446FE0, narration IDs 955–959, and demand-driven 0042FFC4 random calls for complete groups only. Unfilled stars remain interactive. Slots 63/64 retain retail draw behavior but have no narration.
- 00429AA0: whole-game quit is independent of deletion and leave-activity. Strict Yes/No rectangles, hover voice 453/454, random farewell 626–629 (priority 90, class 1), and managed-voice completion before starting credits. The profile adapter previously routed quit to the delete modal.
- 0042A9A5 and 0042A9F3: state 8 snapshots the preceding rendered scene; state 9 opens quit and returns outer dispatch to state 4.
- 004069C0 and 00406AD0: credits filenames come from the installed loaddata/credits.txt; 128-byte filename slots and ten-entry table. The original elapsed integer seconds are calculated at 0041A700 by dividing FILETIME ticks by 10,000,000. Source uses the host elapsed centiseconds and the equivalent strict >4 seconds threshold. Click pulse advances, and the Win32 host posts WM_CLOSE only after the game frame returns.
- The Windows input carries the same process RNG as the activity drivers; profile narration no longer relies on an uninitialized per-frame random number.

The bitmap-only snapshot is represented by draw commands, not an actual captured DirectDraw surface. Credits and quit have source regressions; physical audio, display, and native window teardown remain to be verified in an interactive Windows run. No new function-coverage claims are made here.

## Options and WAV volume

00407390 and 00429800 supply the authored options origin and slider width; runtime overrides slider Y to 150..180 and uses strict OK bounds 276..393, 253..312. Draw the pointer at the preceding volume, then apply the held-pointer change; voice 862 gates dismissal. Native device dispatch applies (percent-100)*42 attenuation to music/activity WAV buffers, not managed speech. Existing and subsequently created music buffers use the new setting. The remaining 004093C0 stop/restart behavior, cinematic-backed overlay snapshots, and live gain validation remain separate work. The source does not invent a new Options entry in the retail menu data.

00409210's global Bink volume uses 37 below 5 percent and percent*310 otherwise, including each newly opened global movie at 00408F3E. The x86 reference backend calls BinkSetVolume; the native PCM backend converts the linear RAD gain to DirectSound dB without changing decoded samples. The reconstructed video/audio decoder remains the x64 production backend. Physical gain parity and cinematic pause/resume still require verification.

## Movie interruption and captured overlays

The session now invokes the original pause policy before physical observation. The x86 reference calls BinkPause and suppresses track gain while paused (00409260 / 004092A0). The native decoder stops the existing PCM buffer without seeking, keeps its completed/playing state separate from pause, excludes paused wall time from its video/audio-master clock, and restores the current decoded picture after resume. Physical EOS is not accepted during modal interception.

The DD7 renderer uses independent 640x480 system-memory surfaces for replay, contextual Help, and global movie snapshots. Help captures the fully composed frame once (including the independent pregame walkthrough) and renders only the cursor over that capture. Global movie pixels are restored before each unpaused device observation, including BinkWait ticks after a fullscreen Flip; modal artwork is composited over the saved picture rather than advancing the movie. A snapshot is not synthesized from missing assets. The session tests cover this call ordering, stale EOS suppression, and Help/replay separation. Device-loss behavior across full display-manager recreation and live Windows playback still require testing; snapshot loss is treated as a provider failure, not a successful frame.

Profile Help follows the strict 00446F18 rectangle (21,420,65,465) and the distinct name-popup rectangle 00446F28 (92,305,143,357), read from the retail PE32 data section. Both enter the shared captured Help controller using their appropriate helpinfo.txt contexts. The name-popup route preserves unfinished profile data; deletion and pending selection/acceptance gates cannot open Help. Source regressions exercise entering, narration, and exit in both contexts. The shipped Options filenames were independently checked against the original Data/UI/Options directory.

## Window commands and lifecycle

004020D0's 0x9c41 command opens Options (51C2BC), not contextual Help. The root now emits the original 572 intro voice, rejects repeated/nested activation, and routes menu command 0x3e9 to the separate credits phase at 44DDB0. The host defers WM_CLOSE while a native frame/print dialog owns device resources; minimized normal gameplay does not advance. Incomplete startup does not write profile data. On credit entry, the native compatibility host explicitly closes movies and stops managed/backing/activity audio to prevent stale video/audio from leaking over slides; exact retail credit-time audio retention is not claimed. Actual backing WAV playing state is sampled via DirectSound GetStatus rather than assumed true.
