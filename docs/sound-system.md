# Sound system architecture

The object allocated by `WinMain` with size `0x7738` and stored at global `0x0044DDD8` is **not** the main game object. It is the game's managed sound-effect system.

This corrects the earliest working label `GameAppConstructor` for `0x00402B00`.

## Construction

### `0x00402B00 SoundManagerConstructor`

The constructor:

1. initializes **80 active sound slots**
2. opens `data\\sound\\binklist.txt`
3. parses entries with a `"%s %d"`-style format
4. builds a filename catalog and numeric sound-ID table
5. initializes the reverse sound-ID -> active-slot mapping

Despite the filename, `Data/sound/binklist.txt` is a WAV catalog, not a list of Bink videos. The source data contains entries such as:

```
AS_BOB_01.wav   0
AS_BOB_02.wav   1
...
DYP_B_BOB_01.wav 201
...
```

The catalog continues for hundreds of voice/effect assets.

## Recovered structure layout

The following offsets are directly supported by access patterns in the executable. Names are semantic working names.

| Offset | Shape | Meaning |
|---:|---|---|
| `+0x000` | `SoundBufferGroup *[80]` | active DirectSound buffer groups |
| `+0x140` | `int32_t[80]` | sound ID assigned to each active slot |
| `+0x280` | byte table | reverse map: sound ID -> active slot; `0xFF` means unloaded |
| `+0x6CC` | byte table | per-sound enable/availability flags |
| `+0xB18` | `int32_t[80]` | priority/age value used for eviction |
| `+0xC58` | `int32_t[80]` | slot lifecycle/playback state |
| `+0xD98` | `int32_t[80]` | persistence / auto-reap suppression flag |
| `+0xED8` | `int32_t[80]` | additional per-slot mode/flag |
| `+0x1018` | fixed string records | sound filename catalog; record stride `0x14` |
| `+0x6608` | dword table | numeric catalog metadata / sound IDs |

The space between the filename and numeric tables is sufficient for roughly 1,100 filename records at the observed `0x14` stride.

## Managed-slot functions

### `0x00402BC0 ReleaseSoundSlot`

Releases the slot's sound-buffer group, clears its sound-ID association, and resets the reverse lookup.

### `0x00402C20 AnyManagedSoundPlaying`

Scans all 80 managed slots and asks whether any active buffer group is currently playing.

### `0x00402C60 IsSoundIdPlaying`

Uses the sound-ID reverse mapping at `+0x280` to find the active slot, then delegates to the DirectSound buffer-group status helper.

### `0x00402C90 StopAllManagedSounds`

Scans the 80 slots and stops the active ones.

### `0x00402CC0 StopSoundSlot`

Marks a slot stopped, calls the group-wide DirectSound stop helper, and rewinds the group's buffers to position zero.

### `0x00402CF0 PlaySoundById`

High-level managed playback entry. It handles existing/reused slots and falls through to the acquire/load path when the sound is not resident.

The exact argument semantics are still being refined; the first argument is the sound ID, while the remaining arguments include priority/playback policy.

### `0x00402E10 AcquireAndPlaySound`

This is the core cache/load routine:

1. looks for a free slot among the 80 active slots
2. if full, selects an evictable low-priority finished slot
3. installs the sound ID <-> slot mappings
4. formats `data\\sound\\%s` using the catalog filename
5. loads/creates the DirectSound buffer group
6. starts playback
7. records the active slot state

This explains why the game can refer to voice/effect assets by compact integer IDs throughout the activity code.

### `0x00402F60 ReapFinishedSounds`

Scans active slots and releases finished sounds that are not marked persistent.

## DirectSound buffer-group helpers

The lower-level helpers line up directly with the `IDirectSoundBuffer` vtable.

| Address | Name | Evidence |
|---|---|---|
| `0x00404360` | `PlaySoundBufferGroup` | chooses a buffer, sets volume, calls `Play` |
| `0x00404430` | `StopAllBuffersInGroup` | calls vtable `+0x48` = `Stop` |
| `0x00404470` | `RewindAllBuffersInGroup` | calls vtable `+0x34` = `SetCurrentPosition(0)` |
| `0x004044B0` | `IsAnyBufferPlaying` | calls vtable `+0x24` = `GetStatus` and tests `DSBSTATUS_PLAYING` |

The remaining high-value function here is `0x00403D20`, which constructs/loads the actual DirectSound buffer group from a WAV path. That is the next boundary to type in detail.
