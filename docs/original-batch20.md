# Twenty connected audio, printing and runtime procedures

This batch raises compiled-source coverage from 459 to 479 of 615 (77.9%).
136 originals remain unreviewed. The count means C++ bodies compiled and linked
to executed source tests. It does not certify the complete native game or retail
ABI. No entry is promoted to `behavior_verified`.

## Original procedures

| Address | Reconstructed body | Source group |
| --- | --- | --- |
| 00404750 | Wave RIFF/format parsing, PCM and extended formats | original_wave7 |
| 004049B0 | Buffered MMIO and memory-backed wave reads | original_wave7 |
| 00404B30 | Wave close and output fact-chunk finalization | original_wave7 |
| 00404C60 | Wave RIFF, fmt and fact chunk writing | original_wave7 |
| 00404560 | Wave open, resource fallback and reset | original_wave7 |
| 00403C40 | Primary DirectSound buffer format construction | original_wave7 |
| 00404100 | Sound buffer fill, repetition and silence | original_wave7 |
| 00404E30 | Bitmap selection and import dispatch | original_printer7 |
| 00404F90 | Printer DC validation and render dispatch | original_printer7 |
| 00405140 | Print-document/page lifecycle | original_printer7 |
| 00405309 | Print failure page/document cleanup and rethrow | original_printer7 |
| 00405C40 | DDB import and 24-bit DIB construction | original_printer7 |
| 00405EE0 | DIB section, palette and pixel import | original_printer7 |
| 004057E0 | Printer validation and StretchDIBits dispatch | original_printer7 |
| 00439212 | Ordinary locale-aware uppercase conversion | original_runtime6 |
| 004386BF | CTYPE-aware integer parsing with 32-bit wrap | original_runtime6 |
| 004331F4 | Formatted decimal-point insertion | original_runtime6 |
| 0043316D | Short vararg fetch with advanced-pointer high bits | original_runtime6 |
| 00436CD3 | Multibyte codepage installation and case-table build | original_runtime6 |
| 00433E95 | CRT file mode parsing and stream setup | original_runtime6 |

## Connections and corrections

The wave fill body calls the recovered 00404270 restore, 004048F0 reset and
004049B0 read procedures. Reset's MMIO callbacks now update the same chunk state
used by open and read. Its original source previously changed the chunk ID
before attempting the input seek; instruction comparisons exposed that error.
Both the compiled body and its older source model now change it only after a
successful seek. The restore overload carries a required sleep service into
its existing two-Restore retry loop; it does not omit the requested sleep.

The wave port retains non-advancing memory-backed reads, PCM versus non-PCM
format allocation, short-read cleanup, output dirty flags, stale handles after
some open errors, primary-buffer release behavior, ignored unlock failures,
and silence byte 0x80 only for eight-bit audio. The resource-memory allocation
is retained by the service: the original does not free it during open/close.

Printing retains six virtual/API control-flow bodies plus the explicit original
catch handler. Successful imports dispatch to the reconstructed DDB/DIB bodies;
render and print dispatch to the reconstructed stretch routine. Print failures
call EndPage then EndDoc before rethrowing. The original exception messages,
reconstructed COW String payload, base exception ownership, copy function and
runtime vtable identity 0043B328 are retained in the native error adapter.
Palette sizes, scanline alignment, compressed-section fallback, and the original
RC_DIBTODEV rejection are preserved.

Runtime callers use existing recovered classification, lowercase, NLS mapping,
codepage selection/reset and locale-ID bodies. The five original 48-byte DBCS
codepage rows and masks are extracted from the verified executable, not inferred
from modern codepage settings. The stream parser preserves duplicate-modifier
termination, ignored trailing text, six reset fields, two untouched fields,
and the original 0x1A4 permissions passed to the descriptor opener.

## Validation

- Full Clang 19 C++26 build; 132 CTest tests pass.
- 4,354 independently generated original-x86 comparisons: 2,914 scalar/mode/
  codepage cases; 600 MMIO/open/format/read/close cases; 300 primary-buffer
  cases; 300 complete sound-fill cases; 240 GDI bitmap/document cases.
- Comparisons include returns, state, resulting format/DIB/audio bytes and
  ordered service requests. Expected results execute original x86 instructions,
  not the C++ reconstruction. Win32/COM dependencies have explicit bounded
  test implementations; there is no physical audio or printing certification.
- Native failure tests additionally cover unavailable NLS mapping, generic
  codepage installation, restore retries/sleep, sound-lock failure, invalid
  bitmap/DC/target, raster capability rejection, DIB read failure, and print
  error cleanup. Windows also tests ordinary uppercase through real NLS APIs.
- ASan/UBSan validation covers the new production source groups and the changed
  activity connection, linked to the existing reconstructed core. Leak checking
  is disabled for the restricted container; instrumented dependencies are
  distinguished from the prebuilt core.
- All 20 instruction listings and the corrected reset listing are preserved.
  Linux and Windows x86/x64 CI build and run all three new test targets.

## Remaining boundaries

MMIO, DirectSound and GDI are required typed services. This batch reconstructs
and connects original procedures at source level; it does not install a complete
native audio/printing provider or route every existing live-game caller through
them. Printer target computation at virtual slot 0x24 remains a separately
unreviewed original and is an explicit dependency, not a default success path.
The descriptor opener remains a required service for the stream mode parser.

Native pointer widths, C++ unwinding and host allocations replace retail x86
ABI/SEH details. Some invalid inputs cause original memory faults or infinite
loops; the host guards short spans, wrapping allocations, negative palette
counts, allocation failure and looping reads that make no progress. Those
undefined/error domains are not advertised as retail-equivalent behavior.
