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
