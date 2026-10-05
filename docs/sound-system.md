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
| `+0xD98` | `int32_t[80]` | special lifetime/control flag; participates in frame-state stop policy |
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

### `0x00402CF0 PlayManagedSoundById`

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

Scans slots in lifecycle state 2. When a slot should be retired from active
playback, retail calls `StopSoundSlot`, which stops/rewinds its buffer group and
moves the slot to lifecycle state 3.

It does **not** free the slot or destroy the buffer group at this point.
Actual release occurs later through explicit release, cache eviction, or
SoundManager destruction.

## DirectSound buffer-group helpers

The lower-level helpers line up directly with the `IDirectSoundBuffer` vtable.

| Address | Name | Evidence |
|---|---|---|
| `0x00404360` | `PlaySoundBufferGroup` | chooses a buffer, sets volume, calls `Play` |
| `0x00404430` | `StopAllBuffersInGroup` | calls vtable `+0x48` = `Stop` |
| `0x00404470` | `RewindAllBuffersInGroup` | calls vtable `+0x34` = `SetCurrentPosition(0)` |
| `0x004044B0` | `IsAnyBufferPlaying` | calls vtable `+0x24` = `GetStatus` and tests `DSBSTATUS_PLAYING` |

The remaining high-value function here is `0x00403D20`, which constructs/loads the actual DirectSound buffer group from a WAV path. That is the next boundary to type in detail.


## WAV / DirectSound utility layer

The lower sound layer is now mapped almost completely and follows the classic DirectX-era sound utility architecture.

### Sound-manager helpers

- `0x00403C40 SetPrimarySoundBufferFormat` constructs a PCM `WAVEFORMATEX` from channel/rate/bit-depth arguments and calls the primary DirectSound buffer's `SetFormat`.
- `0x00403D20 CreateSoundBufferGroupFromWave` allocates an array of DirectSound buffers, opens a WAV reader, creates the first buffer, duplicates additional buffers when requested, fills them from the WAV, and produces the group used by the managed sound cache.
- `0x00404100 FillSoundBufferFromWave` locks the DirectSound buffer, reads PCM bytes from the WAV reader, writes silence for any unused tail, and unlocks the buffer.
- `0x00404270 RestoreSoundBufferIfLost` checks the lost-buffer status and retries `IDirectSoundBuffer::Restore`.
- `0x004042F0 GetFreeSoundBuffer` prefers a duplicate buffer that is not playing; if all are busy it selects one from the group.

### RIFF/WAV helper

The WAV helper is approximately `0x90` bytes and is backed by the standard WinMM multimedia I/O API:

- `mmioOpenA`
- `mmioDescend`
- `mmioAscend`
- `mmioRead`
- `mmioSeek`
- `mmioGetInfo`
- `mmioAdvance`
- `mmioSetInfo`
- `mmioClose`

Recovered methods:

| Address | Name | Purpose |
|---|---|---|
| `0x00404510` | `WaveFileConstructor` | zeroes initial WAV-reader fields |
| `0x00404530` | `WaveFileDestructor` | closes reader and frees owned format data |
| `0x00404560` | `WaveFileOpen` | opens file/memory source and initializes parsing |
| `0x00404750` | `WaveFileReadFormat` | verifies `RIFF/WAVE`, finds `fmt `, builds `WAVEFORMATEX` |
| `0x004048E0` | `WaveFileGetSize` | returns cached PCM data size |
| `0x004048F0` | `WaveFileReset` | seeks/descends back to the `data` chunk |
| `0x004049B0` | `WaveFileRead` | reads PCM data from MMIO or memory source |
| `0x00404B30` | `WaveFileClose` | closes MMIO and releases reader state |

This closes most of the generic WAV-to-DirectSound path. The remaining sound work is primarily game-specific policy: assigning semantic names to the numeric sound IDs and identifying which activity events trigger each ID.


## DirectSound version

The executable imports `DSOUND.dll` by **ordinal 11**. On the DirectX 8 DirectSound export table, ordinal 11 is `DirectSoundCreate8`, so the root sound object is `IDirectSound8`, not the older `IDirectSound` created by `DirectSoundCreate`.


## Exact 0x7738 layout closure

The retail SoundManager allocation size is `0x7738` bytes. The recovered
field offsets now account for **every byte of that object exactly**.

The two sound-ID byte tables each span:

```text
0x6CC - 0x280 = 0x44C = 1100 entries
```

and:

```text
0xB18 - 0x6CC = 0x44C = 1100 entries
```

The filename catalog spans:

```text
0x6608 - 0x1018 = 0x55F0
0x55F0 / 1100 = 20 bytes per filename record
```

The final metadata table then spans:

```text
0x7738 - 0x6608 = 0x1130
0x1130 / 4 = 1100 int32 entries
```

Therefore the exact retail object is:

| Offset | Exact shape |
|---:|---|
| `0x000` | 80 x 32-bit SoundBufferGroup pointers |
| `0x140` | 80 x int32 sound IDs |
| `0x280` | 1100 x int8 sound-ID -> slot reverse map |
| `0x6CC` | 1100 x uint8 sound-enabled/availability flags |
| `0xB18` | 80 x int32 priority/age values |
| `0xC58` | 80 x int32 slot lifecycle states |
| `0xD98` | 80 x int32 persistence flags |
| `0xED8` | 80 x int32 playback-policy values |
| `0x1018` | 1100 x 20-byte filename records |
| `0x6608` | 1100 x int32 catalog metadata |
| `0x7738` | exact end of object |

Known slot-state values from `PlayManagedSoundById` / `StopSoundSlot` are:

- 0 = free
- 1 = loaded/ready
- 2 = playing
- 3 = stopped

The C++26 reconstruction now contains a byte-exact host-independent model in:

`reconstruction/include/btb/sound_manager.hpp`

with `static_assert` checks for every recovered offset and the total
`0x7738` size.


### Exact 80-slot acquisition policy

`0x00402E10 AcquireAndPlaySound` uses the following retail slot-selection
algorithm:

1. scan slot-state values from 0 through 79;
2. the **first state-0 slot** is selected immediately;
3. if no free slot exists, initialize:
   - candidate = -1
   - best priority = **101**
4. scan all 80 occupied slots;
5. ask whether each slot's currently assigned sound ID is still playing;
6. ignore playing slots;
7. among non-playing slots, choose the one with the numerically lowest
   `priority_or_age` value below the current threshold;
8. if a candidate was found, release that slot and reuse it;
9. if no candidate was found, return `-1`.

Because the initial comparison threshold is 101, a non-playing slot whose
priority value is 101 or greater is not selected by this eviction pass.

After a slot is selected, retail installs:

- `sound_id_by_slot[slot] = sound_id`
- `slot_by_sound_id[sound_id] = slot`
- `priority_or_age[slot] = priority_argument`
- `special_lifetime_flag[slot] = 0`
- `playback_policy[slot] = policy_argument`

It then resolves the 20-byte catalog filename at:

```text
0x1018 + sound_id * 20
```

formats `data\\sound\\%s`, creates a one-buffer DirectSound group, and
starts/initializes playback state.

The pure metadata portion of this policy is reproduced and tested in
`reconstruction/include/btb/sound_manager.hpp`.
