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

## Coefficient command-list loops recovered (2026-10-15)

The original 2002 DLL at 0x30011B10 maintains **three distinct pointers**
in its local command list: a prepend-able head ([esp+10]), an append-only
end ([esp+20]), and the current scan pointer ([esp+24]). It seeds
exactly six command bytes at 0x30011B9F..30011BD4:

    10 60 B0 07 0B 0F

Native `bink32_ac_command_pass.cpp` now implements an entire
**single original magnitude-class pass** over this mutable command
list, reusing all four reconstructed dispatch helpers:

- Zero control bytes skip without consuming activation bits.
- Every nonzero control first reads an activation bit; zero skips
  while retaining the control for later passes.
- Opcode 0 rewrites the current control to `base + 0x11`, emits or
  defers four coefficient positions, and RETAINS the scan pointer.
- Opcode 1 rewrites current control to `base + 0x02`, appends three
  controls to the end pointer, and RETAINS the scan pointer.
- Opcode 2 clears the current control, emits/defers four coefficients,
  and advances the scan pointer.
- Opcode 3 emits one signed coefficient, clears the control and advances.
- A deferred coefficient is written to `--head` as opcode 3, becoming
  eligible on a subsequent outer-loop pass, not automatically executed
  during the same pass.

Further review of the original x86 instructions `0x30011B9A–0x30011BF4`
corrected a **substantial earlier porting error**. EAX is initially
1 for constructing the class WORD by `SHL`, but at
`0x30011BE8 MOV AL,BL` it is **reset to the AC mode nibble** before
`0x30011BF4 DEC EAX`. Consequently, low magnitude bit counts are
`mode-1, mode-2, ..., 1`, whereas the separately stored class WORD
starts at `1 << (mode-1)` and shifts right each pass. The previous
power-of-two-derived read widths were incorrect. No shift-count
rollover occurs for any of the sixteen 4-bit AC modes. The original `MOVSX EDI,DI`
+ optional `NEG` + `MOV WORD` wrapping behavior is now modeled via
explicit unsigned 16-bit narrowing and signed reinterpretation.

`bink32_ac_unit_pass.cpp` separately translates the **final +/-1
coefficient pass** at `0x30012030..0x300122E7`, including the second
four-way jump table at `0x3001242C`. Its operations follow the same
mutable head/end/current model but use a single sign bit to emit +1 or
-1 coefficients rather than magnitude-class integers. Both passes are
transactional: on malformed/truncated input they restore the input
cursor, coefficient array and original control-list state.

The new native CTests cover the original six-byte seed, all four
control opcodes, append/prepend ordering, multi-pass state reuse,
source-consistent width/class combinations, 16-bit signed wrap,
and truncation rollback.

**Not yet implemented:** the complete outer-level loop that updates
the magnitude class and repeats the first stage, the full coupling
to the original codec's video-plane block scheduler, and actual
retail-frame parity. No claim of DLL-free playable movies is made.

## All 16 original AC coefficient modes — corrected outer scheduler

The original x86 disassembly of `0x30011B10` establishes the
following exact register sequence:

```asm
30011B9A  mov eax,1
30011BB7  dec ecx             ; ECX originally contains mode nibble
30011BB8  shl eax,cl         ; class bit = 1 << (mode - 1)
30011BD5  mov [esp+28],eax   ; independently tracked class WORD
30011BE8  mov al,bl          ; CRITICAL: overwrite EAX with mode
30011BF4  dec eax            ; first low-bit read width = mode - 1
30012020  sar word [esp+28],1
30012025  dec ecx            ; next pass, mode - 2 bits
3001202A  jne 30011BF0
```

The earlier port mistakenly used `(1 << (mode-1))-1` as the low-bit
read width. That made modes 7–15 look impossible because it conflated
the *class magnitude* with the *number of bits read*.

`bink32_ac_outer_levels.cpp` has now corrected the bit count and
supports the **entire encoded mode nibble range 0–15**, always
transactionally. A zero-mode header skips all AC work; mode 1 invokes
the final signed-unit pass only. For every mode 2–15, there are
exactly `mode-1` magnitude passes, with class bits beginning at
`1 << (mode-1)` and read widths `mode-1, mode-2, ..., 1`,
then one signed-unit pass. The maximum read width is 14 bits,
so neither 32-bit x86 shift masking nor >31-bit bitreader widths
are needed.

The tests now cover all sixteen modes. Mode 4 exercises a nonzero
coefficient with high class bit 8 and **3** low bits; mode 15
executes 14 magnitude passes and writes a nonzero negative
signed-16 coefficient using class bit 16384 and **14** low bits.
These fixtures additionally verify exact bit cursor positions and
transactional rejection of truncated streams.

This closes the mode-range gap in the *isolated coefficient scheduler*,
not in Bink frame playback. A full retail video frame is still not
certified: DC extraction, macroblock mode traversal, reference-frame
and plane assembly, and original runtime bit-position parity remain.

## One native AC-mode-to-pixels pipeline (all sixteen modes)

`bink32_ac_block_pixels.cpp` now provides a single transactional
`decode_bink_ac_quantized_block` entry point. Starting at the
appropriate original AC bitstream position and given a
**caller-supplied decoded DC coefficient**, the function:

1. Reads the four-bit original AC mode (0–15).
2. Executes the full recovered mutable coefficient-command list
   with `mode-1` correctly sized magnitude passes and the signed
   unit pass (unless mode is zero).
3. Reads the four-bit quantization selector immediately following
   the AC coefficient stream.
4. Selects the exact, independently checksum-validated original
   DLL quant matrix, and runs the recovered two-pass integer iDCT.
5. Returns a complete 16×16 sample block, the 64 coefficient values,
   mode and quant selector, and the number of compressed bits
   consumed. Invalid/truncated streams do not advance the cursor.

Native CTest exercises **all 16 mode nibbles**. Additional fixtures
exercise nonzero negative AC in mode 1, nonzero AC in mode 4, and
nonzero negative AC in mode 15; all reach the quantized pixel output.
The test also checks strict cursor accounting and rollback.

This is the deepest source-derived, multi-component decoding path so
far. It is not an original complete `.bik` frame: the original
compressed DC mode/value, correct macroblock start positions,
non-transform block modes, per-plane reconstruction, prediction,
frame scheduling and audio still need to be connected. Tests use
crafted compressed sequences and known DC inputs. No original
DLL/retail frame differential validation is claimed.

## Original signed DC plane unpacking and block handoff (2026-10-16)

An inspection of the original 2002 DLL's video block scheduler
`0x3000E400` resolved a significant missing interface:

- At `0x3000E8E5` and `0x3000E8F5`, the original initializes
  two distinct signed-16 sample streams via `0x30010BF0`,
  including local cursor state at `[ebp-0x1D8]` and
  `[ebp-0x248]`.
- At `0x3000EDC8–0x3000EDE6` and
  `0x3000F081–0x3000F0A5`, the scheduler **copies a signed
  little-endian 16-bit DC coefficient from one of those
  predecoded streams**, advances its source pointer by two
  bytes, writes it to coefficient[0], and *then* invokes the
  AC decoder `0x30011B10`.
- Thus the DC value **does not** come from the AC bitstream.
  Attempting to consume its bits there would misalign subsequent
  AC and quantization decoding.

`bink32_dc_stream.cpp` provides `read_original_bink_dc_word`
and `decode_original_bink_block_from_dc_plane`, completing
this original two-stream handoff. It copies one signed-16
word from a bounded DC output stream, calls the recovered
all-mode AC decoder, then selects the original quant matrix
and executes the inverse transform to produce 16x16 samples.
Both the DC byte cursor and AC bit cursor are transactional.

The original `0x30010BF0` predecoder was also investigated.
Its group loop at `0x30010D94–0x30010ECE` emits up to
eight cumulative DC samples per group:

- Read one 4-bit magnitude width (0..15).
- Width zero uses `0x30010E8D–0x30010EBE` to replicate
  the **previous signed 16-bit value** across the group.
- Nonzero width reads one unsigned `width`-bit magnitude
  per sample. A zero magnitude uses no sign bit.
  Each nonzero magnitude consumes one additional sign bit.
- The signed delta is added to the cumulative previous value;
  original x86 writes the low 16 bits for every output sample.

`decode_original_bink_dc_delta_group` now reconstructs that
complete short-group operation in native C++, with strict
one-to-eight sample bounds, signed 16-bit wrap and transactional
input/output. The tests include variable widths 0, 1, 3, 4,
and 15; positive, negative, zero and wraparound deltas;
back-to-back group state; and a constructed compressed
DC-group → predecoded DC words → original AC/quant/iDCT
pixel block chain.

**Remaining:** the overall `0x30010BF0` plane header, output
length/allocation behavior, the original starting/cumulative
sample state, 0x3000E400 macroblock scheduler and actual
retail-frame parity are not reconstructed end-to-end.
All current DC-group validation uses controlled source
bitstreams; it does not certify original .bik frame pixels.

## Continuous DC plane reconstruction from compressed deltas

The original decoder at `0x30010D94–0x30010ECE` does not stop
after one group; it advances through consecutive groups of
`min(remaining, 8)` signed delta values. New native C++ entry point
`decode_original_bink_dc_delta_plane` implements this **whole
group loop** and emits a contiguous little-endian signed-16
word buffer directly compatible with
`decode_original_bink_block_from_dc_plane`.

It carries the previous decoded sample from one group into the
next, obeys the variable-width and conditional sign-bit rules,
and caps output at 1,048,576 DC words to prevent untrusted
allocation abuse. It commits the source bit cursor only after
the entire requested plane finishes. Tests cover consecutive
nonzero- and zero-width groups, word ordering, malformed
streams, zero-length planes, and the resulting words feeding
two native AC/quant/iDCT pixel blocks.

**Important input boundary:** The caller must supply the
original plane header's starting predictor and exact output
word count. Those fields are not yet reconstructed from retail
compressed video end to end. Neither these tests nor the
standalone plane decoder certify an original frame as complete.

## Complete Bink DC context records: count, initial sample, and deltas

Original `0x3000FC00` initializes the 0x30-byte DC plane decoder
context. Its fields include a count-bit width at offset `+0x28`,
an initial-sample width at `+0x08`, and the signed-value flag
at `+0x0C`. At `0x30010C07–0x30010C74`, the original
`0x30010BF0` routine extracts the encoded output sample count.
A zero count terminates the record without producing DC values.

For a nonzero count, `0x30010C91–0x30010D6C` reads the
**initial** DC sample. In signed mode it consumes
`value_width-1` unsigned magnitude bits and an extra sign bit
only if that magnitude is nonzero; in unsigned mode it consumes
`value_width` bits directly. The source then writes its initial
16-bit word at `0x30010D74`. The remaining `count-1` samples
are generated by the original width-nibble delta groups already
translated at `0x30010D94–0x30010ECE`.

New C++ `decode_original_bink_dc_context_packet` implements this
**whole record**. It accepts context widths supplied by the
original frame initializer, returns consecutive little-endian
signed-word bytes compatible with the AC/quantization handoff,
and commits all bitstream changes only on success. It has tests
for signed initial positive/negative/zero values, unsigned
initial values, empty counts, output-size limits, malformed
truncation, and a three-block source-native pixel chain.

**Missing upstream boundary:** The original `0x3000E400`
video scheduler must still bind each context's widths, output
count, and source starting bit offset to the corresponding
encoded frame and select the correct block/plane mode. We
do not yet have a verified full retail-frame decode. The
current context tests use controlled compressed fixtures.

## Reused original 0x3000FC00 entropy-plane configuration

The pre-existing native `init_entropy_plane` reconstruction
already computes the original 0x30-byte context's
`table_bit_width` (from symbol-base and width/8 multiplier),
`symbol_bits`, and signed-value `prefix_mask`. The new
`decode_original_bink_dc_context_packet(source, context)`
overload now uses these exact recovered fields to decode a
complete DC substream, eliminating manual width/signedness
arguments at the integration boundary.

The regression invokes `init_entropy_plane(0,0x200,64,11,1,true)`,
verifies the resulting ten-bit count-width and signed eleven-bit
value configuration, and successfully decodes a three-word
[100, 101, 99] example. It also verifies unsigned contexts
and malformed-context rollback.

The original video-block scheduler at `0x3000E400` still needs
to select correct context parameters, DC packet bit boundaries,
and macroblock modes for each original retail frame. These
tests establish this particular source-native interface, not
full frame parity.

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
