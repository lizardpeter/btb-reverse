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
| `+0xB18` | `int32_t[80]` | managed-sound priority; lower values are evicted first when finished |
| `+0xC58` | `int32_t[80]` | slot lifecycle/playback state |
| `+0xD98` | `int32_t[80]` | input-interruptible flag; value 1 allows shared input pulses to stop/rewind the sound |
| `+0xED8` | `int32_t[80]` | cross-sound arbitration class |
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

High-level managed playback entry. Its retail signature is semantically:

```text
PlayManagedSoundById(sound_id, priority, arbitration_class)
```

The three arguments are now closed.

- **sound_id** indexes the 1100-entry WAV catalog.
- **priority** is stored at slot offset `+0xB18`. When all 80 slots are occupied, finished slots with the numerically lowest priority below 101 are selected for eviction first.
- **arbitration_class** is stored at `+0xED8` and controls cross-sound admission/preemption before the requested sound starts.

The exact arbitration classes used by retail are:

| Value | Meaning |
|---:|---|
| 0 | no cross-sound arbitration scan |
| 1 | exclusive blocker once playing |
| 2 | preemptible managed voice/effect |

For an incoming class greater than zero, retail scans all currently playing managed slots. If it encounters an existing class-1 sound, the **incoming request is rejected immediately**. If it encounters an existing class-2 sound, incoming class 1 or 2 stops/rewinds that existing slot before the scan continues. Incoming values above 2 do not preempt a class-2 slot.

If the requested sound is already mapped and actively playing, retail returns status 2 without restarting it. Otherwise it acquires/loads or restarts the mapped slot and returns status 1.

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

Scans slots in lifecycle state 2. The field at `+0xD98` is now proven to be
an **input-interruptible flag**, not a persistence flag.

For every active slot retail applies this exact rule:

```text
if input_interruptible &&
   (shared_input_pulse_A || shared_input_pulse_B):
    StopSoundSlot(slot)
else if DirectSound group is no longer playing:
    StopSoundSlot(slot)
```

So a flagged spoken/UI sound still stops normally when its WAV ends; the flag
only adds the ability for shared user input to interrupt it immediately.
`StopSoundSlot` stops and rewinds the group and moves lifecycle state 2 -> 3.

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

`0x00403D20` is now closed as the DirectSound/WAV buffer-group constructor described below.


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
| `0xB18` | 80 x int32 priority values |
| `0xC58` | 80 x int32 slot lifecycle states |
| `0xD98` | 80 x int32 input-interruptible flags |
| `0xED8` | 80 x int32 arbitration classes |
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
   `priority` value below the current threshold;
8. if a candidate was found, release that slot and reuse it;
9. if no candidate was found, return `-1`.

Because the initial comparison threshold is 101, a non-playing slot whose
priority value is 101 or greater is not selected by this eviction pass.

After a slot is selected, retail installs:

- `sound_id_by_slot[slot] = sound_id`
- `slot_by_sound_id[sound_id] = slot`
- `priority_or_age[slot] = priority_argument`
- `special_lifetime_flag[slot] = 0`
- `playback_policy[slot] = arbitration_class_argument`

It then resolves the 20-byte catalog filename at:

```text
0x1018 + sound_id * 20
```

formats `data\\sound\\%s`, creates a one-buffer DirectSound group, and
starts/initializes playback state.

The pure metadata portion of this policy is reproduced and tested in
`reconstruction/include/btb/sound_manager.hpp`.


### Source-level managed SoundManager runtime

The 80-slot policy is now implemented in
`reconstruction/src/sound_manager.cpp`, not only represented as layout
helpers.

The constructor's exact metadata defaults are:

- 80 group pointers = null;
- 80 assigned sound IDs = -1;
- 80 priorities = **101**;
- 80 slot states = **0 / Free**;
- 80 arbitration classes = **-1**;
- 1100 sound-ID -> slot bytes = **-1 / 0xFF**;
- 1100 sound-enabled bytes = **1**.

The complete `PlayManagedSoundById` / `AcquireAndPlaySound` policy is now
modeled:

1. disabled catalog entry -> return **0**;
2. if the sound's mapped slot is already lifecycle state 2 -> return **2**
   immediately;
3. for incoming arbitration class >0, scan playing managed slots:
   - existing class 1 rejects the incoming request;
   - existing class 2 is stopped/rewound by incoming class 1 or 2;
4. if the sound is unmapped:
   - choose the first free state-0 slot;
   - otherwise evict the nonplaying slot with the lowest priority below 101;
   - install sound-ID/slot mappings, priority, interruptible=0 and arbitration
     class;
   - load `data\\sound\\<catalog filename>`;
5. state 1 transitions to state 2 and starts the CSound;
6. state 3 restarts the existing CSound and transitions back to state 2.

Two original-code quirks are intentionally preserved:

- **new sounds are started twice**: `AcquireAndPlaySound` calls
  `CSound::Play(0,0)` immediately after creation, writes slot state 1, and
  then the caller changes state 1 -> 2 and calls `CSound::Play(0,0)` again;
- if low-level CSound creation fails after the mappings are installed, the
  mapped slot remains state 0, yet the public managed-play function can still
  return **1 / accepted**.

Another impossible/error edge is documented but host-safened: if all 80 slots
are occupied and every one is still playing (or has priority >=101), retail
acquisition returns -1 and the x86 caller subsequently indexes slot -1. The
C++26 reconstruction reports that would-invalid-index condition instead of
performing undefined memory access.

`ReapFinishedSounds` is also source-level: only lifecycle-state-2 slots are
examined; an input-interruptible slot stops immediately on either shared input
pulse, and every active slot stops normally when its DirectSound group is no
longer playing. Stopping changes state 2 -> 3, calls group Stop, then rewinds
all duplicate buffers to position zero.

The full policy is covered by C++26 regression tests.


## DirectX SDK DSUtil lineage

The lower WAV/DirectSound layer is now identified as a customized copy of
Microsoft's DirectX SDK **DSUtil** helper family.

The retail binary preserves the characteristic class/method organization:

- `CSoundManager`
- `CSound`
- `CWaveFile`

and the method sequence:

- `CSoundManager::SetPrimaryBufferFormat`
- `CSoundManager::Create`
- `CSound::FillBufferWithSound`
- `CSound::RestoreBuffer`
- `CSound::GetFreeBuffer`
- `CSound::Play`
- `CSound::Stop`
- `CSound::Reset`
- `CSound::IsSoundPlaying`
- `CWaveFile::Open`
- `CWaveFile::ReadMMIO`
- `CWaveFile::GetSize`
- `CWaveFile::ResetFile`
- `CWaveFile::Read`
- `CWaveFile::Close`

The EXE is not identical to later DX9/DXUT copies, so the reconstruction keeps
the **retail 2002 layouts and behavior** rather than importing a later header.

### Retail CSoundManager

Exact object size: **0x04 bytes**.

| Offset | Field |
|---:|---|
| `+0x00` | `IDirectSound8 *m_pDS` |

The game's `CSoundManager::Initialize` is customized to take the primary
format arguments together with HWND/cooperative level:

```text
Initialize(hwnd, coop_level, channels, sample_rate, bits_per_sample)
```

The observed startup call is equivalent to:

```text
channels        = 2
sample_rate     = 22050
bits_per_sample = 16
```

and calls `SetPrimaryBufferFormat` internally.

### Retail CSound

Exact object size: **0x14 bytes**.

| Offset | Field |
|---:|---|
| `+0x00` | vtable |
| `+0x04` | `IDirectSoundBuffer **m_apDSBuffer` |
| `+0x08` | `DWORD m_dwDSBufferSize` |
| `+0x0C` | `CWaveFile *m_pWaveFile` |
| `+0x10` | `DWORD m_dwNumBuffers` |

Unlike later SDK samples, this build has **no `m_dwCreationFlags` member**.

Its `Play` method likewise takes only retail priority/play flags. After
starting the selected DirectSound buffer it applies the game's shared volume
through `IDirectSoundBuffer::SetVolume`.

The game adds a small `CSound::SetVolume` helper that applies a volume to
every duplicate buffer in the object.

### Retail CWaveFile

Exact object size: **0x90 bytes**.

| Offset | Field |
|---:|---|
| `+0x00` | `WAVEFORMATEX *m_pwfx` |
| `+0x04` | `HMMIO m_hmmio` |
| `+0x08` | `MMCKINFO m_ck` |
| `+0x1C` | `MMCKINFO m_ckRiff` |
| `+0x30` | `DWORD m_dwSize` |
| `+0x34` | `MMIOINFO m_mmioinfoOut` (0x48 bytes on retail x86) |
| `+0x7C` | `DWORD m_dwFlags` |
| `+0x80` | `BOOL m_bIsReadingFromMemory` |
| `+0x84` | `BYTE *m_pbData` |
| `+0x88` | `BYTE *m_pbDataCur` |
| `+0x8C` | `ULONG m_ulDataSize` |

The 0x90-byte allocation ends immediately after `m_ulDataSize`; the extra
resource-buffer members found in some later SDK variants are absent here.

These exact host-independent layouts are now represented by:

`reconstruction/include/btb/dsutil.hpp`

with C++26 offset/size tests.
