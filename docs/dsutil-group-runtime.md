# Three recovered low-level CSound operations

The original main EXE Ghidra pseudocode for:

- **0x00404430** \`CSound_Stop\`
- **0x00404470** \`CSound_Reset\`
- **0x004044B0** \`CSound_IsSoundPlaying\`

is now implemented in \`reconstruction/src/dsutil_group_runtime.cpp\` using
\`reconstruction/include/btb/dsutil_group_runtime.hpp\`.

These functions share the 20-byte C++ source model of \`RetailCSound32\`
already recovered in \`dsutil.hpp\`.

## Observable native semantics

- Stop calls **every** group buffer's \`IDirectSoundBuffer::Stop\` in index
  order, **OR-ing** every HRESULT return value, without early exit.
- Reset calls each \`SetCurrentPosition(0)\` and likewise ORs the results.
- IsSoundPlaying calls \`GetStatus\` for every non-null group buffer and
  ORs \`status & DSBSTATUS_PLAYING\`. It **ignores the HRESULT**, including a
  failed status query. It does not stop scanning after the first playing buffer.
- A missing group table yields \`CO_E_NOTINITIALIZED\` for Stop/Reset,
  and 0 for IsSoundPlaying. Zero-count groups with a present table return 0.
- The original code assumes non-null Stop/Reset buffer entries and sufficient
  array length. For invalid host providers, the reconstruction safely returns
  \`CO_E_NOTINITIALIZED\` rather than dereferencing invalid pointers. This is
  an explicitly documented host-safety divergence, not asserted retail parity.

The provider passes opaque buffer pointers and static callbacks. The source
model makes **no audio waveform**; it preserves operation order and HRESULT
semantics and can be bound to real DirectSound buffers without allocating on
the audio-frame hot path.

Run \`ctest -R '^dsutil_group_runtime$'\` after building the target
\`btb_dsutil_group_runtime_tests\`.

Status: **source_written** for the three named function addresses.
The fake-COM regression tests demonstrate the recovered call sequence, but
do not yet prove parity on retail DirectSound hardware or the final integrated
game runtime.
