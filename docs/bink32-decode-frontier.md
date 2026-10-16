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

## Native 2002 block-value decoding (additional partial reconstruction)

`bink32_entropy_commands.cpp` now translates the compressed-value
branch of original `0x300101A0`, from the one-bit command selector
at `0x30010242` through the run commands at `0x30010305–30010364`:

- Literal Huffman codes 0–11 append one byte and update the prior value
- Codes 12–15 repeat the prior byte exactly 4/8/12/32 times, using the
  original `.rdata` lookup values from `0x30047BDc + code`
- The alternate one-bit branch consumes a four-bit immediate value and
  fills the remainder of the output block
- Native output and bit position commit only on complete, bounded success

Another portion of original `0x30010400`, at `0x30010632–30010651`,
combines two decoded Huffman nibbles as a signed-magnitude, seven-bit
quantity re-centered at 128. The C++ implementation tests all 256
possible pairs, including positive values, negative values and negative
zero, plus decoding through the original fixed Huffman symbols.

These are genuine block-value decoding components but do not yet include
full coefficient layouts, motion vectors, iDCT, reference frames or
final YUV->RGB presentation. Full runtime Bink32 removal is still gated.

## Signed coefficients and output pixels (0x30010980 / 0x30012940)

`bink32_signed_coefficients.cpp` now reconstructs the original
`30010980–30010BE7` signed-coefficient bitstream stage. Each
compressed command uses one selector bit: in the Huffman branch,
an original 16-symbol table yields the magnitude, with a sign bit
only for nonzero magnitudes; in the alternate branch, a four-bit
immediate magnitude plus conditional sign fills the remaining
coefficient buffer. The source code is transactional and fails
closed if the compressed stream is truncated. It does not yet
turn coefficient arrays into a Bink image-plane transform.

`bink32_pixel_reconstruction.cpp` implements the **pixel output**
stage of original `30012940`, a motion-predicted 8x8 block whose
decoded residual bytes are added to the corresponding reference-frame
bytes with modulo-256 unsigned wrapping (rather than saturation).
It supports independent reference/output strides, in-place output,
and incomplete/bogus buffers without partial writes. The intra block
copy path is also provided for already decoded eight-by-eight pixels.

The new CTests verify Huffman-code-to-signed-byte decoding, immediate
fill behavior, arithmetic edge cases, reference strides, and full 8x8
pixel output on synthetic inputs. These are **not** proof of a
complete Bink32 frame: the original `30012440` inverse-transform and
residual-producing codec stages still need reconstruction before we
can provide these compositors with actual decoded retail coefficients.

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
