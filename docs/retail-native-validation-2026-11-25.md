# Native installed-data validation — 2026-11-25

Local GCC 14 C++26 Release build, source snapshot 2faa631ec5e3b3b94570bdc689f0f09298aebcca with the corrected diagnostic Easy choice selection. Retail PE SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05. Original files were read privately, never added to source control.

## Activity playthroughs

| Activity | Completions | Gameplay updates | Draw commands |
|---|---:|---:|---:|
| Golf | 72/72 | 126816 | 1782672 |
| Squirrel Run | 72/72 | 410812 | 8796813 |
| Total | 144/144 | 537628 | 10579485 |

All three difficulties, both original display modes, seeds 1–12; all five profiles. Golf accepted 360 shots; Squirrel accepted 432 correct and 360 incorrect choices. All submitted BMP crops and explicit audio paths validated against original files. The diagnostic was initially incorrectly expecting an Easy decoy, and the controller (not the game) was fixed. The separate real herd.txt Pets Corner probe completed 9/9, 12/12 and 15/15 animals at seed 1 across the three difficulties. Voice duration is modeled, not measured on a Windows device.

## Original-instruction comparisons (actual production source checkout)

The checked-in tools were compiled and run against the verified original PE. All 178178 Golf shot scenarios, 31252395 frame comparisons matched (trace 5bf54240cf8eca71). Squirrel: selector 69984, actor 308074, conveyor 82944, slide 73800, assembly 5225472 (9f2c9d55e0505c56), transition 34392 (b7693742661f6ad9), drop 124416 (12eebd7bdda884a6), placement 227682 (8a73b5a3fe5d3b9d). These are translated original instruction comparisons, not direct execution of the PE.

## Bink native structural decode

All 61 identified paths in the original global movies (33), Spud Skate (5), and Fireworks (23) banks decoded, 12732 frames and 44 audio tracks, 61034880 PCM16 bytes. Zero failed or partial paths. This structural audit does not establish pixel or PCM equivalence by itself, and does not claim other folders have been exhaustively inventoried. Physical Windows audio/video/input parity remains open.

Independent FFmpeg 7.1.5 comparison against the same local production build: 61/61 original paths, 12,732 exact video frames (all planes), 44/44 PCM16 tracks with identical lengths; 30,517,440 sample values, 9,806 values differing by one LSB, maximum difference 1 LSB. The full local C++26 build also passed all 254 CTests, including the metadata fixture and its negative cases. These are codec and command-stream results, not physical device certification.

## Maze installed-data completion

New native diagnostic `btb_retail_maze_playthrough` passed 48/48 runs against the original map, sprite and explicit audio files: 12 seeds with all three difficulties plus a deliberate 900-update idle wait on Hard to exercise NPC spawning. Total: 432 packages, 144 screen crossings, 152,262 gameplay updates, 28,371 active NPC updates, and 2,271,768 submitted draw commands (2,251,768 intersected the viewport). All bitmap bounds and named audio paths validated. All selected-profile completion writes and replay handoffs passed. Seven of eight NPC phases observed (0,1,2,4,5,6,7); phase 3 not covered. The controller observes public state and supplies ordinary keyboard input with a modeled 50-Hz clock and eight-update voice durations. It does not modify the game state or simulate the actual Windows sound/device clock. The original map files stay local.

    cmake --build build/faithful --target btb_retail_maze_playthrough
    build/faithful/btb_retail_maze_playthrough /path/to/BTB-BBP 12

## Spud Maze installed-data completion

The `btb_retail_spud_maze_playthrough` probe passed 72/72 complete native GameRoot playthroughs on the installed five-screen map and actual repair bitmaps. Seeds 1–12, all three difficulties, both original display modes, all five profiles. Totals: 498 repairs, 318 screen crossings, 96 hammer drops, 95 recoveries, 335,653 updates, and 4,354,572 draw commands (4,336,069 visible). Every crop, explicit audio path, completion progress and replay handoff passed. The navigator submits actual directional input and click pulses, using original public graph/repair/hammer observations. It preserves the active corridor axis until node capture and excludes impossible zero-input portal returns; initial test-controller timeouts were corrected without changing the game. The synthetic 50-Hz clock and eight-update voice model remain explicit; physical rendering and sound-device equivalence remain open.

    cmake --build build/faithful --target btb_retail_spud_maze_playthrough
    build/faithful/btb_retail_spud_maze_playthrough /path/to/BTB-BBP 12

## Dinosaur installed-data completion

The native Dinosaur probe completed all nine original layouts (three species × three difficulties), seeds 1–12: 108/108 runs, 1,104 accepted and 1,104 rejected placements, 108 certificates and print requests, 5,604 gameplay updates and 71,280 submitted/visible draw commands. Actual original bitmap dimensions and named audio files were checked. The original virtual cursor warp is now applied consistently to the placement test and drawing, the shared CRT stream consumes the original separate pickup draws and conditional reaction draws plus the mode-2 discarded draw, and certificate entry occurs only on a later update after finalization and the original audio gate. The certificate replaces the underlying puzzle commands, and the pressed print overlay uses the input-held state. The native Dinosaur unit regression passed locally. Modeled voices and emitted print requests are not physical playback or printer certification.

    cmake --build build/faithful --target btb_retail_dino_playthrough
    build/faithful/btb_retail_dino_playthrough /path/to/BTB-BBP 12

## Spud Skate installed-data validation

12/12 complete native runs, all three difficulties and four quality levels, scoring 0/16/32/48 across the original sixteen stunt opportunities. 16,068 gameplay updates and 62,688 actual decoded frame upload callbacks. The x64 native host now shares the portable five-stream bank with the probe: synchronized frame 44 is presented immediately at the first loop boundary; the second frame-660 marker immediately presents end.bik frame 1; end observation retains displayed frame 44. Space-held input and pre-update score drawing restored. Mixed-case original paths resolve without duplicating files. The probe checks profile-specific badge prerequisites, replay handoff, decoded plane sizes and real bitmap crops. Actual DirectDraw upload and sound playback require Windows validation. The optional legacy x86 retail DLL comparison adapter retains its previous timing behavior.

    cmake --build build/faithful --target btb_retail_spud_skate_playthrough
    build/faithful/btb_retail_spud_skate_playthrough /path/to/BTB-BBP

## Bob's Band installed-data validation

15/15 runs, all three conductors across all five profiles, using actual machine data, art and WAV assets. Each composition played all 50 pitch/type samples (750 total playback requests), validated 19,845 gameplay updates and 1,112,775 bitmap draw commands, exited through ordinary Back/Yes and reloaded every 480-byte composition. Original 20-byte last.txt verified. State 8 now captures a fresh playback epoch; previously an application running longer than 24 seconds could emit no notes. All tests begin at 40,000 centiseconds. Startup voices and interruptibility restored; idle and reaction random draws use the shared CRT source; delete lifetime checked on click rather than every frame. The full-game Band regression passed locally. Audio output timing is modeled, not physical sound-device verification.

    cmake --build build/faithful --target btb_retail_bobs_band_playthrough
    build/faithful/btb_retail_bobs_band_playthrough /path/to/BTB-BBP /new/temporary/save-directory
