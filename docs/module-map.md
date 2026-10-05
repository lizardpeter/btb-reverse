# Executable module map

This is a working source-oriented map of the main executable. Boundaries are based on direct asset/data cross-references plus the central activity dispatcher and will be tightened as analysis continues.

| Approximate code range | Module |
|---|---|
| `0x00401000-0x00409DFF` | startup/core, DirectDraw, DirectSound, input, global data helpers |
| `0x00409E00-0x0040AE9F` | Dinosaur |
| `0x0040AEA0-0x004111BF` | Park Designer / DYP |
| `0x004111C0-0x004145EF` | Fireworks |
| `0x004145F0-~0x00417FFF` | Golf |
| `~0x00418000-0x0041A72F` | Herding / SubGame1 |
| `0x0041A730-0x0041DB1F` | Maze |
| `0x0041DB20-0x0042055F` | Bob's Band / music sequencer |
| `0x00420560-0x0042422F` | Spud Maze (data loader begins at `0x00420560`; initializer at `0x004207E0`) |
| `0x00424230-0x00424E1F` | Spud Skate |
| `0x00424E20-~0x0042800F` | Squirrel |
| `0x00428010-0x0042A2BF` | shared activity/front-end helpers |
| `0x0042A2C0-0x0042CD6B` | 68-state central game-flow dispatcher |
| `0x0042CD6C+` | player/profile/UI helpers followed by statically linked CRT code |

## Shared activity music

`0x00428010 PlayActivityMusicByIndex` indexes a fixed table beginning at `0x00446CF0`. Each entry occupies 50 bytes.

| Index | Track |
|---:|---|
| 0 | certificate.wav |
| 1 | dinosaur.wav |
| 2 | dyp.wav |
| 3 | golf.wav |
| 4 | grandopening.wav |
| 5 | maze.wav |
| 6 | petscorner.wav |
| 7 | Skateboardfix.wav |
| 8 | Skateboardrace.wav |
| 9 | squirrelrun.wav |
| 10 | Playagain.wav |

The routine stops/releases the previous music group, loads the selected WAV through the recovered WAV/DirectSound layer, starts it, and records that music is active. `0x004280A0 StopActivityMusic` is its companion teardown routine.
