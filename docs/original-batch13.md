# Thirteen audio, printer and runtime procedures

Compiled-source coverage is now **492/615 (80.0%)**, up from 479.
123 originals remain unreviewed. These are recovered C++ bodies linked to
executed source tests. `behavior_verified` remains **0/615**.

| Address | Body | Source group |
| --- | --- | --- |
| 00403D20 | Sound manager wave load, CD fallback, creation and duplication | original_sound2 |
| 00403FF0 | Sound construction, buffer copy, fill and initial positions | original_sound2 |
| 00405490 | Printer target geometry and device queries | original_printer2 |
| 004056C0 | Destination clipping and source coordinate mapping | original_printer2 |
| 00430DD1 | CRT allocation dispatch to two small heaps or process heap | original_memory4 |
| 00430D2A | CRT free dispatch and small-heap ownership lookup | original_memory4 |
| 00433B60 | Byte copy with overlap handling | original_memory4 |
| 00430B5C | Partition/selection hybrid byte sorting | original_memory4 |
| 00435C51 | Wide/ANSI environment-block selection and conversion | original_environment4 |
| 0043931D | Wide environment entries converted and inserted | original_environment4 |
| 00439945 | Environment insertion, replacement, deletion and OS update | original_environment4 |
| 0043874A | Environment lookup and lazy conversion | original_environment4 |
| 00438272 | Runtime MessageBox loading, owner selection and cached calls | original_message1 |

## Connections

Sound creation calls `original_wave7::open_wave` and the new constructor calls
`original_wave7::fill_sound`, which already connects restore/reset/read.
Creation retains the retail fixed 0x80 buffer flags, optional CD-drive prefix,
0x0878000a warning, first-buffer duplication, ignored open/fill/position errors,
unchanged caller output on failures, and successful null output if allocation
of the outer Sound fails. Error cleanup closes and releases the wave and
intermediate array; it intentionally does not invent releases for already
created COM buffers that retail leaks.

`original_printer2::LinkedServices` installs the recovered target calculation
in the virtual slot used by `original_printer7::render_to_dc` and
`print_document`. Its device services remain mandatory. Mode 2 keeps float
ratio stores, signed halving, the raw width/height RECT stores and leaked
window DC. Clipping keeps the original zero-based mapped source offsets,
rounding and the Y ratio's double store. The earlier general printing model
remains separate; these procedures are independently compiled bodies.

Environment block copying calls the new overlap-aware copy. Put/get call the
recovered multibyte delimiter, environment-find and comparison bodies.
Wide-to-narrow conversion calls the new put body and preserves its ignored
insertion failures and unfreed allocation on conversion failure. Deletion
shifts the null terminator and tolerates failed shrink; replacement preserves
the original unfreed old string. The initial-array duplicate operation still
requires a raw-pointer bridge, while the older 00439B24 model uses vectors.

Sorting calls recovered 00430CFE swap and 00430CB0 selection sort. Tests compare
comparator order as well as bytes, including duplicate keys and payloads.
Memory dispatch keeps 32-bit size rounding/wrap and the original null-free
fast path. Required small-heap services expose the internals still awaiting
reconstruction; there is no default successful heap fallback.

## Evidence and validation

The committed instruction listings come from the SHA-verified retail executable.
`tools/build_retail_batch13_vectors.py` executes those instructions in Unicorn,
including the actual x87 `__ftol` dependency for printer calculations. It never
uses the C++ implementation to calculate expected outputs.

The 4,850 cases include 1,000 copy, 600 sort, 1,000 allocation/free, 10 dynamic
message loader, 500 target geometry, 600 clipping, 240 environment block, 300
put/get, 120 wide conversion, 180 sound constructor and 300 sound creation cases.
Named Win32/COM/allocator/NLS dependencies are controlled. The sound cases
isolate the already tested wave-open/fill boundaries in the x86 harness; C++
uses the actual recovered wave bodies with controlled MMIO/audio services.
Prior wave tests continue to exercise successful fill and MMIO paths.
Additional tests cover shared environment-array copying, shrink failure,
allocation failure, disabled lookup, invalid sound arguments and unchanged output.

The complete Clang 19 C++26 build passes **133 CTest tests**, and all **16 Python
tests** pass. ASan/UBSan passes the new source groups plus their wave/activity
implementations, linked to the prebuilt core for older dependencies; leak
detection is disabled because several original paths deliberately retain
allocations. Windows x86/x64 CI includes the new executable.

This is bounded source/state validation. Native DirectSound/MMIO, GDI printing,
process heap/NLS/environment/message providers and full gameplay caller wiring
are not supplied by this batch. Host pointer arrays replace guest pointer words
at typed allocation boundaries. Invalid retail buffer domains receive host
bounds/argument exceptions instead of deliberate corruption. Printer arithmetic
uses host long double for x87 intermediates; MSVC long double has double precision,
so untested precision-boundary inputs need further comparison. None of this
certifies full retail ABI, actual playback, physical printing or game equivalence.
