# Bink32 native codec reconstruction: evidence and admission frontier

This is reverse engineering of the user-supplied original 2002-06-25 PE32
`binkw32.dll`, SHA-256
`16515bffb88d058edf3553a76bf8d4a696061a58ab750afccfd5173ccd5b0d7f`.
Do **not** check the proprietary executable binary into this repository.

## Actual 2002 DLL call graph (original x86 addresses)

| VA | Routine | Reconstructed stage |
|---|---|---|
| `30005F40` | `BinkDoFrame` | Audio and video frame packet split (native); full function incomplete |
| `30006140–30006355` | frame track iteration | One LE32 compressed byte count for every audio track, then track bytes |
| `3000647D` | call `3000E270` | Video decode dispatch (not yet translated) |
| `3000E270` | video dispatcher | Invokes `3000E400` for luma/chroma and optional extra planes |
| `3000E400` | large block decoder | Block/entropy/inverse transform reconstruction still outstanding |
| `3000FC00` | entropy-plane initialization | Translated to C++ (`init_entropy_plane`) |
| `3000FCB0` | entropy tree reading | Full 16-symbol permutation construction translated from assembly (synthetic tests pass); frame parity not yet certified |
| `300051C0` | `BinkCopyToBuffer` | Destination format conversion not yet translated |

`reconstruction/src/bink32_packet_decode.cpp` now implements the real
per-frame compressed-track routing and a bounded LSB-first entropy bitreader.
`reconstruction/src/bink32_fixed_huffman.cpp` captures the 976 original
fixed lookup bytes at `30047800–30047BCF`, plus the 16 lookup widths from
`30047BD8` and the table offsets indexed from `30063CA0`. One call
to `read_fixed_huffman_symbol` consumes a source-valid variable-length
codeword from a selected table. The initial dynamic tree control flow is now source-reconstructed as `read_entropy_tree_prefix`: four selector bits, a zero-tree special case, and the branch/submode bits at `3000FD32–3000FDBF`. The original tree permutation construction has now also been reconstructed
in `bink32_dynamic_huffman.cpp` for both main branches and all four
submodes. Specifically, it includes the eight per-pair swap bits, the
hierarchical 2/4/8-element stable interleave network, and the sparse
4-bit symbol-list path. The nested merge helper at `300100E0–3001019F`
is implemented directly in C++. The block decoder `3000E400` calls
the tree routine 23 times in its initialization prologue (2 main,
16 repeated, 5 additional), now modelled as `read_block_entropy_trees`.

Local tests passed the four submodes, corrupted/truncated streams,
cursor accounting, fixed-table-to-permutation symbol remapping, and
50,000 randomized inputs under AddressSanitizer/UBSan. Those are
source-level tests; no live retail 2002 DLL invocation or frame
pixel comparison is claimed.

## Asset-backed validation (local, not GitHub CI)

All 1,590 compressed frame packets from the four supplied retail videos
parsed successfully with source-native packet routing:

| BIK file | Frames | Compressed video bytes | Compressed audio bytes |
|---|---:|---:|---:|
| `designwalthrough.bik` | 445 | 3,675,176 | 0 |
| `designstartup.bik` | 227 | 1,905,436 | 221,492 |
| `designcomplete.bik` | 139 | 1,116,472 | 137,820 |
| `introbig1.bik` | 779 | 7,883,632 | 1,344,820 |

The test uses the full original `SJPi` data unmodified. Tests execute
without the original DLL, FFmpeg, or external processes. The repository
also has a synthetic, release-safe CTest for packet corruption and
cross-byte bit reads, and an exhaustive 976-entry fixed Huffman table test.

The native dynamic-tree constructor was additionally exercised on a
**candidate video-control-word boundary (four bytes into each video packet)**
for all 1,590 retail frames. All 1,590 yielded 23 bounded tree contexts;
6,516 selectors were nonzero and 5,571 symbol permutations were nonidentity.
This is a structural smoke test, not yet proof that the 23 tree reads begin
at the exact same offset used by the 2002 decoder. Bit-exact frame timing,
coefficient decoding and pixel parity remain unverified.

**Important:** These figures certify bitstream packet routing, *not* 1,590
decoded video frames or 1,590 audio waveforms.

## Next implementation order

1. Compare the reconstructed dynamic trees against the original x86 DLL
   on representative compressed bit ranges and certify bit-accurate cursor
   advancement; current CI uses synthetic codeword fixtures.
2. Translate the block symbol stream and coefficient decoding in
   `3000E400`, including the 8×8 inverse transform and inter-frame blocks.
3. Convert decoded original Y/U/V planes into BGRA for the native DirectDraw7
   host and test hashes/pixels against a verified reference decoder.
4. Reconstruct original audio entropy + inverse transform, buffering and
   timestamp sync in `30005F40`.
5. Only after real frame+audio parity, swap the game host's Bink provider
   to this in-process codec and remove `binkw32.dll` as a runtime dependency.

No claim of playable DLL-free video is made by this batch.
