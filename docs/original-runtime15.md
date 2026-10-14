# Fifteen connected runtime reconstructions

This batch adds 15 original procedure bodies, moving compiled-source coverage
from 444 to 459 of 615 (74.6%). There are 156 unreviewed originals remaining.
All source is built in `btb_recompiled_core`, exercised by source tests, and
reachable through the linked game-core smoke. This is not complete native
feature integration or retail executable ABI equivalence.

## Original procedures

| Address | Reconstructed body | Direct connections |
| --- | --- | --- |
| 0043646F | Multibyte character to UTF-16 | Required NLS service |
| 004365C0 | Scalar CTYPE classification | 00438404; existing 00436598 / 00431D39 wrappers |
| 004370B3 | Narrow LCMapString with ANSI/wide selection | Existing 0043991A bounded length; actual Win32 NLS adapter |
| 00438404 | Narrow GetStringType with ANSI/wide selection | Actual Win32 NLS adapter |
| 004375A9 | Locale-aware ordinary tolower | 004365C0 and 004370B3 |
| 004327E1 | Multibyte toupper | 004370B3 and recovered case table |
| 00436F12 | Multibyte case-table construction | 00438404 and 004370B3 |
| 00436B93 | Daylight-saving transition calculation | Shared recovered time globals |
| 004369E7 | Daylight-saving membership | 00436B93, cached northern/southern transitions |
| 004326B8 | Calendar to signed 32-bit time | Existing 00436774 initialization latch, 004369E7 |
| 0043854D | CRT signal raise | Existing 0043867A table lookup |
| 0043576F | Exception-code to signal dispatch | Existing 004358B0 lookup |
| 00433824 | Unhandled C++ exception filter | Required termination / hook services |
| 0043669F | FILE byte pushback | Existing 00437371 allocation |
| 00433888 | FILE input refill | Existing 00437371 allocation and 00431D70 byte-input caller |

The locale adapter calls MultiByteToWideChar, WideCharToMultiByte, LCMapStringA/W,
GetStringTypeA/W and GetCPInfo on Windows. It does not replace those APIs with
host ASCII case conversion. The ANSI/wide availability caches, strict sizing
flags versus conversion flags, embedded-NUL truncation, sort-key byte capacities,
lead-byte processing, and ordinary versus multibyte packed-return byte order
are retained. `ClassificationLink` supplies real reconstructed classification
for existing character wrappers.

The input bridge shares the recovered buffer, pointer and count state with
00431D70. It retains allocation failure's inline two-byte buffer, string-stream
pushback restrictions, EOF/error flags, descriptor text flags, and the original
512-to-4096 buffer-capacity transition.

## Validation

- Full Clang 19 C++26 build; 129/129 CTest tests pass.
- 7,500 independent original-x86 cases: 1,500 transition calculations, 1,500 DST
  classifications, 1,500 calendar conversions, 2,000 pushback/refill cases and
  1,000 signal dispatch cases. Tests compare returns, complete relevant state
  and ordered signal service traces. Inputs and expected results are generated
  from the SHA-verified retail EXE using Unicorn, not the reconstructed C++.
- Locale tests exercise all 256 byte values, ANSI fallback, both availability
  failures, conversion failures, DBCS, sort-key sizes, NLS call/flag ordering,
  and actual reconstructed classification callers. On Windows they additionally
  invoke the real NLS adapter, including case-table construction.
- ASan/UBSan runs cover both new source groups and connected runtime dependencies.
- Existing Linux, Windows x86 and Windows x64 CI target/test lists include both
  new groups. New Windows results are separate from local validation.

## Fidelity boundaries

Guest handler identities and time globals are explicit state, not replacements
for host C++ runtime globals. Descriptor reads, signal handlers, unhandled
exception providers, termination and timezone initialization remain required
services. Full native FILE/exception/timezone providers are not installed by
this batch. Native vectors replace original stack allocation / x86 SEH plumbing
in the NLS wrappers; this is source reconstruction rather than the retail ABI.

Disassembly shows 004326B8 does not write the temporary `tm_sec` and `tm_min`
words. Its automatic DST classification can depend on incoming stack contents
at a boundary. The port takes these two words as an explicit required input;
reference vectors seed them in the original stack. It does not invent modern
calendar behavior or silently initialize that original dependency. The original
1970..2038 year limit, signed 32-bit wrap and pre-2007 US fallback rules survive.

The signal table starts at 00449770; 00449778 is its action field. Floating-point
handling clears the action fields of the configured range and preserves codes.
The existing table lookup functions are reused. Required provider exits are
non-returning. Callback exceptions use host C++ unwinding, not retail SEH.

The original case-table builder ignores NLS errors then reads unwritten stack
buffers. The port explicitly rejects failure in that undefined-input domain.
Successful NLS processing and the GetCPInfo-failure ASCII branch are tested.
No original is promoted to `behavior_verified`: bounded comparisons do not
certify all retail behavior or full-game integration.
