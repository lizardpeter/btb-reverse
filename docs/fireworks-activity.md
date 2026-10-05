# Fireworks activity

The Fireworks activity is the second minigame being taken from address-level notes into source-level reconstruction.

## Main entry points

- `0x00411C80 InitializeFireworksActivity`
- `0x00413F10 UpdateFireworksActivity`
- `0x004115C0 LoadFireworkMovieBank` (high-confidence working name)
- `0x00411B10 UnloadFireworksActivityResources` (high-confidence working name)

The activity mixes DirectDraw UI composition with a preloaded Bink bank for the firework effects.

## Rectangle data

The initializer reads:

- 18 records from `Data\\SubGameFirework\\graph\\fireworks.txt`
- 12 records from `Data\\SubGameFirework\\graph\\fireworkboxes.txt`

Both files use eight integers per record, representing four 2D corners. The retail executable keeps only corner 0 and corner 2, giving the top-left and bottom-right rectangle.

For `fireworks.txt`, the parser subtracts 20 from X and 30 from Y. During transfer into the runtime hit table it adds 30 back to Y, so the final placement hit rectangles are the source rectangles shifted 20 pixels left.

For `fireworkboxes.txt`, the retained rectangle is copied without that placement shift.

This also explains why malformed values in redundant second/fourth vertices do not change behavior: those values are read but discarded.

## Runtime region table

The initializer builds one common interactive-region table:

1. 18 placement regions, each assigned action/type 12
2. 12 palette regions, assigned type IDs 0 through 11
3. three hard-coded control regions assigned action IDs 27, 25, and 26

That adds 33 activity-specific interactive regions.

The update path beginning around `0x004126F0` iterates these records, hit-tests the current mouse position, and branches on the action/type field.

## Firework type map

The 12 palette bitmap load order is identical to the first 12 entries of the Bink effect bank.

| Type | Firework | Left/single clip | Right clip |
|---:|---|---|---|
| 0 | red airbomb | airbombredleft.bik | airbombredright.bik |
| 1 | small green | smallgreenleft.bik | smallgreenright.bik |
| 2 | medium red | mediumleftred.bik | mediumrightred.bik |
| 3 | large blue | bigblueleft.bik | bigblueright.bik |
| 4 | large red | bigredleft.bik | bigredright.bik |
| 5 | medium green | Mediumgreenleft.bik | Mediumgreenright.bik |
| 6 | small blue | smallblueleft.bik | smallblueright.bik |
| 7 | blue airbomb | airbombblueleft.bik | airbombblueright.bik |
| 8 | red candle | Candlered.bik | single/symmetric |
| 9 | red spinner | Wheelred.bik | single/symmetric |
| 10 | green spinner | Wheelgreen.bik | single/symmetric |
| 11 | blue candle | Candleblue.bik | single/symmetric |

For types 0 through 7, bank index `type + 12` is the matching right-hand movie.

Machine-readable maps:

- `ghidra/firework_types.csv`
- `ghidra/firework_movies.csv`

## 23-entry Bink bank

`0x004115C0` iterates exactly 23 movie paths.

- 0-7: left-facing variants of directional firework types
- 8-11: candle/spinner clips
- 12-19: right-facing variants
- 20: `topmiddle.bik`
- 21: `fireworkcrowdloop.bik`
- 22: `fireworkcrowdend.bik`

The routine opens each source file through the CD-fallback path, reads it into retained memory, initializes a per-movie playback/surface record, and then opens the Bink handle from that retained data. This appears designed to avoid repeated CD seeks while playing a user-authored show.

## Next Fireworks boundary

The remaining high-value part is the authored sequence itself:

- selecting palette type 0-11
- placing it into one of the 18 authored positions
- deriving left/right movie choice
- delete and delete-all behavior
- play/show sequencing
- transition to crowd/top clips and completion/replay

The data and movie bank are now sufficiently understood to implement the parser and type selection independently of DirectDraw/Bink.
