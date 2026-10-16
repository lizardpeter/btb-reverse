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

## Decoded pixel output for the all-DC subset of the 8x8 inverse DCT

`bink32_inverse_transform.cpp` now reconstructs the original decoder's
DC-only first-pass optimization at `300113CE–300113FA`: when the seven
non-DC coefficients of a vertical 8-sample column are zero, the original
x86 reads the signed 16-bit DC coefficient, performs a low-32-bit
multiply by the quantization scale, arithmetic-shifts it right by 11,
and broadcasts the intermediate result to all eight rows.

For the **entire 8x8 block containing only its DC coefficient**,
`idct_dc_only_block8` continues through the horizontal pass's
`(+127) >> 8` rounded low-byte result, yielding 64 actual
pixel bytes. Tested positive, negative, zero, x86 product wrapping,
and non-DC rejection. This is a deliberately limited source-derived
subset of the full inverse DCT. It cannot reconstruct normal blocks
with nonzero AC coefficients.

The original interframe addition at `0x30012940` is separately
reconstructed in `bink32_pixel_reconstruction.cpp`. It combines an
8x8 residual block with the previous frame using exact unsigned-byte
wraparound and valid row strides. This produces real pixel bytes from
provided residuals; **it does not yet produce a complete retail frame
from compressed bits**. The general AC inverse-transform and the main
block mode scheduler remain missing.

## General 8x8 iDCT for nonzero AC coefficients (2026-10-15)

The source-derived x86 translation in
`reconstruction/src/bink32_full_idct.cpp` now implements
**the entire 0x30011350..0x30011715 integer inverse DCT path**, rather
than only its DC-only fast path. This includes:

- First-pass 8x8 16-bit coefficient by 32-bit quantization table multiply,
  signed arithmetic right shift by 11, and the original DC-only column
  shortcut when the other seven original coefficients are zero.
- Original 8-point even/odd butterflies with signed fixed-point
  constants `0xB50`, `0x8A9`, `0xEC8`, `-0x14E8`.
- Second-pass horizontal butterfly with wrapping `(+127) >> 8`
  conversion to unsigned 8-bit output.
- Actual x86 2x horizontal and vertical pixel replication, producing
  **16x16 bytes from one 8x8 coefficient block**, visible in duplicated
  packed-DWORD destination stores at 0x300116AD..0x300116F4.
- Explicit modulo-2^32 arithmetic for ADD/SUB/IMUL to avoid C++ signed
  overflow undefined behavior; exact input-length preflight.

The standalone `bink32_original_general_idct` regression includes
DC-only compatibility, vertical/horizontal nonzero AC, arbitrary 64-
coefficient mixed blocks, independently computed output hashes, and
all four 2x2 output-replica positions. GCC and Clang local builds passed
with AddressSanitizer and UndefinedBehaviorSanitizer. GitHub Windows
x86/x64 CI results should be checked separately.

**Important:** this source translation has not yet been differentially
executed against the original DLL with identical quantization tables
and coefficients. Source control-flow equivalence is not yet formal
bit-exact retail certification. It is also not connected to a full
macroblock entropy/quantization parser, so decoding a real complete
frame without the original DLL is still outstanding.

## Exact original quantization matrices and compressed selector bridge

Original 2002 DLL machine code at `0x30011729` computes the location
of a fixed quantization table with

`0x30049E40 + (quant_index << 8)`.

The block decoder at `0x3000F0ED` consumes the 4-bit selector after
the coefficient unpacking helper at `0x3000F0A5`, and calls
`0x30011720` at `0x3000F107`.

`bink32_original_quant.cpp` now embeds **all 16 original quantization
matrices**, each 64 signed 32-bit entries (4,096 bytes total). Source
data came directly from the user-provided original 2002
`binkw32.dll` and was verified against SHA-256:

`4442cb0c6670d276dde6a5cbf51ba4567819cd916d5036f0f306a34c1351404b`

For readability, the C++ file losslessly reconstructs all 1,024
DWORDs at compile time from matrix 15, the original rational scale
factors, and 62 single-unit corrections. A `static_assert` checks an
FNV-1a fingerprint over all 4,096 reconstructed bytes. There are no
approximate weights and no external runtime quantization asset.

The new `decode_original_quantized_block` reads the original 4-bit
quant index from a bounded native bit reader, selects the exact matrix,
and invokes `bink_idct_16x16` with 64 already-decoded coefficients.
Cursor movement is transactional on failure. Tests include selector
0/1/15 DC values, six different full 64-coefficient mixed AC outputs
compared with hashes independently computed using the original
unmodified matrix bytes, packed four-bit selectors, and rejection of
truncated or invalid inputs.

**Remaining boundary:** The 64 coefficients still need to be
reconstructed from the actual movie's macroblock entropy stream,
especially the original `0x30011B10` coefficient unpacker and
its block-mode scheduler in `0x3000E400`. Passing this module's
synthetic and raw-table-driven tests does **not** constitute
bit-exact video-frame parity with the 2002 DLL. Runtime playback
must continue using the original DLL until frame and audio decoding
have been independently certified.

## Source-native pixel generation for the original zero-AC block type

The coefficient unpacking routine at `0x30011B10` reads an AC-mode
nibble and clears the scratch space for coefficients 1–63.
When the nibble is zero, control reaches `0x30012030`, then jumps
to `0x300122E7` without decoding any AC coefficients. The original
caller has already initialized the DC coefficient at offset zero,
which this routine deliberately preserves.

`decode_bink_zero_ac_block` now implements this exact limited
path: given the **already-decoded DC value**, it transactionally reads
the zero-AC mode nibble, then the next four-bit original quant selector,
chooses the recovered 64-entry quant matrix, and runs the general
integer inverse transform. Tests cover all-DC outputs for matrix
indices 0, 1, and 15, and fail-closed behavior for nonzero modes and
truncated bitstreams.

This is the first source-backed chain that can generate a complete
16x16 block of sample bytes using the original coefficient mode and
quantization bits. **It is not complete frame playback:** the upstream
DC decoding, nonzero-AC branches, macroblock scheduler, color-plane
combination, reference-frame handling, and audio are still outstanding.
Pixel parity against a running copy of the retail DLL has not yet
been demonstrated.

## Nonzero AC: original branch-table command expansion and direct coefficient (2026-10-15)

Inspected the supplied original `binkw32.dll` machine code for
`0x30011B10..30012316`. The first coefficient-code indirect jump
at `0x30011C4B` indexes four addresses stored at `0x3001241C`:

| Command low 2 bits | Original target | Recovered behavior |
|---|---|---|
| 0 | `0x30011C52` | Rewrite source command to `base+0x11`, then fall through to the four-slot decoder used by case 2 |
| 1 | `0x30011C61` | Expand packed command into 4 index-coded control bytes |
| 2 | `0x30011C93` | Partially inspected, further bit-dependent logic outstanding |
| 3 | `0x30011F7F` | Direct signed 16-bit coefficient from magnitude-class bits |

`bink32_ac_nonzero.cpp` adds native C++ for branch 1, emitting
`(command & 0xfc) + {0x02,0x12,0x22,0x32}` with the same eight-bit
wrap semantics as the original x86 byte stores. For branch 3 it reads
the remaining magnitude-class low bits and a sign bit, combines them
with the original outer loop's `1 << (mode-1)` high-magnitude bit,
and stores the resulting signed 16-bit coefficient into
`coefficients[command >> 2]`. The consumed command byte is zeroed
after success, as in original `0x30012003`.

The previous decoding pass has to supply the exact high-magnitude-class
bit and remaining bit width; the function deliberately does **not**
guess the original outer-loop state. Both operations are independently
source-derived and have release-safe tests for all supported magnitude
widths, signs, destination indices, command-byte wrapping and
truncated-bitstream rollback. Local GCC C++23 and Clang ASan/UBSan
tests passed.

These are original subbranches of the **nonzero-AC** coefficient
unpacker, not a complete translation of `0x30011B10`. The remaining
dispatch cases, nested bit-dependent coefficient placement, all
outer-loop transitions, exact original runtime bit-cursor parity, and
movie-frame assembly still need implementation. We do not claim the
result is a complete decoder or DLL-free working video.

## Additional nonzero-AC opcode 2 (group-of-four)

The original `0x30011C93` branch from jump table `0x3001241C`
handles **four consecutive coefficient positions**. Each begins
with a 1-bit decision:

- Zero: read the current magnitude-class low bits, OR with the
  original outer-loop high bit, then read its sign and store a
  signed 16-bit coefficient at `base_slot + {0,1,2,3}`.
- One: defer that slot by placing `(slot << 2) | 3` in the
  pending-control stack rather than decoding its magnitude now.

The original x86 grows the pending-command buffer backward from
its end pointer (`0x30011CC6`, `0x30011D7D`, `0x30011EEB`).
`bink32_ac_group4.cpp` implements this order faithfully, including
a bounded four-coefficient scratch transaction and all-or-nothing
bit cursor/output writes. The new CTest covers mixed positive/negative
coefficients, postponed commands, all deferred, truncated input,
slot bounds and unchanged state on failure.

Together with `bink32_ac_nonzero.cpp`, native C++ now models
**all four branch-table entries** of original `0x30011C4B` at the
isolated-operation level. Branch 0 is an alias/fallthrough of case 2
with the original `base + 0x11` rewrite; its exact source positions
are `0x30011C52..0x30011C5F`. The wider dynamic control-list
state and complete outer coefficient loop remain unfinished.
Do not interpret isolated opcode tests as bit-exact real-frame decoding.

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
