# Thirteen connected file, timezone, directory and heap procedures

This batch adds thirteen recovered C++ bodies, bringing local compiled-source
coverage to **505/615 (82.1%)**, with **110** unreviewed. Every new body is in the
C++26 core and linked to an executed source test. Full retail-behavior
certification remains **0/615**.

| Address | Body | Source group |
| --- | --- | --- |
| 00437BFA | CRT descriptor open and text-file Ctrl-Z trimming | original_file7 |
| 00433961 | Binary/text descriptor read, CR/LF translation and lookahead | original_file7 |
| 0043407D | Binary/text descriptor write and append handling | original_file7 |
| 004390CC | File growth, truncation and position restoration | original_file7 |
| 0043285C | Buffered output character and mode switching | original_file7 |
| 00430A52 | Buffered item write | original_file7 |
| 00430937 | Buffered item read | original_file7 |
| 00436789 | Environment/system timezone initialization | original_system2 |
| 004300BE | Directory change and hidden drive environment variable | original_system2 |
| 0043519F | Small-heap-2 pointer ownership/page lookup | original_heap4 |
| 00435087 | Small-heap-2 region release and list removal | original_heap4 |
| 004350DD | Small-heap-2 empty-page decommit and region reclamation | original_heap4 |
| 00434AA1 | Small-heap-3 region-table expansion and reservation | original_heap4 |

## Source linkage

`original_file7::Link` supplies the real descriptor open/write/read bodies to
existing open-mode parsing, flush/commit, buffered input, and close code.
`IoState::DescriptorView` now supports the actual lazily allocated 32-descriptor
blocks as well as existing contiguous callers. Allocation, open, seek, flush,
close and read/write all reference the same descriptors; flags and handles are
not copied between separate tables.

The buffered read bridge calls the recovered 00433888 refill, which calls the
recovered buffer allocator and descriptor read. The write bridge calls recovered
00431242 flush and the new output-character body; copies call 00433B60. A test
runs the complete existing mode parser -> descriptor allocator -> new open ->
new buffered write -> existing flush -> existing seek -> new buffered read ->
existing refill -> existing fclose cleanup chain. It checks file bytes,
allocation cleanup and final descriptor release.

`original_system2::TimeLink` connects the existing calendar initializer to the
new timezone body. The timezone body uses recovered environment lookup and the
CTYPE-aware integer parser. Its test reaches that chain through calendar time
conversion. Cached TZ strings retain the original early-return behavior,
including reset transition years and system-rules flag, and the environment
branch retains its existing daylight bias. System zone names use the required
codepage, 0x220 flags, 63-byte capacity and default-character rejection.
Directory changes call the recovered multibyte uppercase body before updating
the hidden `=C:` variable, and retain the original UNC-path exception.

`original_heap4::LookupLink` supplies recovered small-heap lookup to
`original_memory4::release`. A test executes that connection through its required
block-free service. Decommit calls recovered region release, including head
retention, current-region changes, reverse traversal and original counter
behavior. Region creation retains failed-allocation field mutations, table
capacity changes, the zeroed-control allocation flag and VirtualAlloc parameters.

## Original behavior retained

Text reads preserve pending lookahead, CR/LF boundary peeks, pipe/character versus
seekable-file handling, Ctrl-Z EOF flags, the FCRLF bit, and broken-pipe/access
error distinctions. Text writes retain the original 1024-byte block threshold
(which can prepare 1025 bytes after newline expansion), newline accounting,
partial-write returns, append seek and device Ctrl-Z behavior. Resizing restores
the original file position and text mode, and keeps the original access-error
remapping. Open retains Win32 access/share/disposition/attribute combinations,
permission-mask effects, descriptor errors and conditional Ctrl-Z trimming.

## Validation

`tools/build_retail_io13_vectors.py` runs the SHA-verified original x86
instructions in Unicorn. It supplies controlled Win32 file, timezone, allocation,
NLS and virtual-memory services. Expected output never comes from reconstructed
C++. The original Win32-to-errno table is included as fixture metadata.

There are **3,920 original x86 comparisons**: 360 each for the seven file bodies,
300 TZ-string cases, 120 system-zone cases, 120 directory cases, 500 heap lookup,
120 region release, 120 decommit and 120 region creation cases. File comparisons
include return/error values, raw file bytes, descriptor flags/lookahead, all
FILE words, buffer hashes, allocation counts and native service-call order.
The complete C++26 regression suite has **134 CTest tests** and the Python suite
has **16 tests**. ASan/UBSan covers the new groups and their runtime16,
runtime15, startup11, environment4 and memory4 source dependencies, linked to
prebuilt core dependencies for the remaining older code; leak detection is off.

Windows x86/x64 workflow targets include this batch. Those CI runs have not
been triggered: automatic approval review requires explicit authorization to
publish the local source commits to GitHub main.

## Limits

The required Win32 file, process-environment, timezone and heap providers remain
external interfaces; this batch does not install a native game provider or
certify actual Windows file operations. Guest buffer addresses remain explicit
PE32 addresses mapped by required memory services. Callback and native service
failure results are controlled by the tests.

Retail input-buffer and list corruption domains receive host range/cycle guards.
In particular, descriptor text-write accounting can produce a negative result
other than -1 after an extremely short expanded-text write. The standalone body
retains that result, but buffered item writing rejects it before following the
retail code into invalid pointer/count arithmetic. Buffered fwrite fixtures use
complete native writes or explicit failure; short-write quirks are compared at
the descriptor boundary. Zero-progress growth also receives a host exception.
These bounds and fixtures are not full native ABI or game-behavior certification.
