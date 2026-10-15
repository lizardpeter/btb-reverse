# Fourteen connected heap and runtime procedures

Compiled-source coverage reaches **519/615 (84.4%)**, with **96 unreviewed**.
These bodies are compiled into the core and linked to executed source tests.
Full retail-behavior certification remains **0/615**.

| Address | Recovered body | Group |
| --- | --- | --- |
| 00434F43 | Small-heap-2 region creation and initial page setup | original_heap9 |
| 00434B52 | Small-heap-3 group commit and free-list setup | original_heap9 |
| 00434798 | Small-heap-3 allocation and block splitting | original_heap9 |
| 0043446F | Small-heap-3 free, coalescing and delayed reclamation | original_heap9 |
| 00434C4D | Small-heap-3 in-place growth and shrink | original_heap9 |
| 00435443 | Small-heap-2 page-slot allocation and fragmented search | original_heap9 |
| 00435567 | Small-heap-2 in-place slot resize | original_heap9 |
| 0043523B | Small-heap-2 allocation, page commit and region fallback | original_heap9 |
| 00439B8B | Reallocation across both heaps and the process heap | original_heap9 |
| 0043938B | Extended-significand multiply, normalization and rounding | original_float2 |
| 004395AB | Table-driven extended decimal-power scaling | original_float2 |
| 00437ECA | Signed/unsigned integer conversion and overflow handling | original_support3 |
| 00434257 | OS/environment/linker-based heap selection | original_support3 |
| 00436049 | Runtime error output and program-name formatting | original_support3 |

## Connections

`original_heap9::Link` supplies actual recovered small-heap allocation, ownership
lookup and freeing to `original_memory4::allocate` and `release`. It calls the
existing region lookup, region creation, page decommit/release and slot-free
bodies. Reallocation calls the new in-place resize and allocation bodies,
recovered overlap-aware copy, and recovered free paths. Required services are
native allocation, commit/release, new-handler invocation and guest memory.
There are no default successful heap providers.

The heap port retains 16-byte rounding, size-class bitmaps and counters,
free-list sentinels, splitting/coalescing, last-region scanning, metadata
mutations on failure, delayed spare-group reclamation, table compaction,
retry-size mutations and strict versus inclusive threshold checks. It keeps
small-heap-2's zero-length memset at additional commit: the native commit
provider must supply zero-filled newly committed pages. It also retains the
original full-page counter behavior during repeated recycling.

Extended multiplication calls recovered add-with-carry and 96-bit shift bodies.
It uses integer limbs rather than host floating-point approximations, retaining
original discarded products, guard/sticky bits, denormal handling, rounding,
sign, overflow and zero quirks. Scaling calls this multiplication and uses the
original data bytes and spacing from the SHA-verified executable. Constants
are embedded in the API header so graph source contains the actual tables.

Heap selection calls the new integer parser and recovered linker-version and
string routines. Parsing calls recovered CTYPE and uppercase procedures;
invalid bases, hexadecimal prefixes, saturation, signed/unsigned overflow,
negative unsigned results, end pointers and errno follow the original branches.
Runtime diagnostics use original fixed message strings and call the recovered
dynamic MessageBox loader. Stderr selection, suppressed GUI message 252,
unknown-program fallback and the unusual tail-of-program-name truncation are
retained. Native output remains a required provider and tests capture it.

## Evidence

`tools/build_retail_heap14_vectors.py` executes the original x86 instructions
from the SHA-verified retail executable, including recovered callees. It never
uses the reconstructed C++ to calculate expected results. Win32 heap, virtual
memory, NLS and diagnostic APIs have explicit controlled providers.

The fixture has **9,083 comparisons** and 29 configuration rows:

- 2,400 extended multiplications and 1,200 decimal-power scalings.
- 3,303 allocator/free/reallocation operations across both small heaps and the
  process heap, including 800 simultaneous allocations across multiple groups
  and regions followed by reclamation and compaction.
- 300 direct page allocations and 300 slot-resize cases.
- 1,200 integer parsing, 200 heap-selection and 180 runtime-report cases.

Every heap operation checks its result, eight process-state values, allocation
cursors, a CRC of metadata/committed pages/audit heap storage, and native API
call order. Tests exercise allocation and commit failures, new-handler retry,
ignored reclamation failure, partial free-list reuse and bounded native reserve
exhaustion. The native-service provider retains freed storage only for audit;
it is not a production allocation/cache policy.

All **135 CTest tests** and **16 Python tests** pass. ASan/UBSan passes the new
heap9/float2/support3 sources and existing heap4/memory4/float6/message1/runtime8
sources, linked to the prebuilt core for other dependencies. Leak detection is
disabled. Windows x86/x64 CI includes the target, but publication remains
blocked pending explicit authorization to push to GitHub main.

## Limits

These are source-level bodies operating on explicit PE32 memory and process
state. They are not installed as native Windows CRT replacements or complete
gameplay callers. Actual Win32 provider integration, all input/failure domains
and original retail ABI remain unverified. Host guards reject malformed lists,
invalid current-region pointers, missing page sentinels, out-of-range paragraph
requests and unbounded decimal exponents instead of following retail corruption
or nonterminating paths. Module/environment buffers also receive bounds checks.
