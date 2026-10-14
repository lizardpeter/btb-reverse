# Eight original string/memory bodies from executable instructions

The original 311,296-byte executable was downloaded and SHA-256 verified:
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.
The checked-in instruction listings and register/exception evidence are in
`analysis/retail_instruction_evidence`. The executable itself is not committed.

| Address | Reconstructed body |
|---|---|
| 00406790 | Allocate rounded string buffer and retry on exception |
| 004067E9 | Catch-all exact-size allocation retry |
| 00406810 | Copy old content, release/decrement old ownership, attach new buffer |
| 004063A0 | Assign buffer, preserving the original small-capacity reuse policy |
| 00406460 | Grow/trim/clear, returning the original AL boolean result |
| 004068A0 | Detach a shared buffer using original strlen semantics |
| 00406150 | Whole-string sharing, substring copies and in-place self erasure |
| 00430500 | Overlap-aware memory move |

The missing register values were resolved from instructions, not guessed:
EBX is the string object, ESI the selected capacity, EBP+8 the new allocation,
and EBP-0x14 the saved capacity. The capacity is `(requested | 31)`, with the
original overflow checks. The exception metadata at 0043BFF0 points through
0043C020 to 0043C038; its null type descriptor establishes that the first
allocation is protected by **catch-all**, followed by an exact-size retry.

The string object retains the allocator token, data pointer, length and
capacity. On x86 its size is 16 bytes and the pointer/length offsets are 4/8.
Ownership is the actual byte BEFORE the data allocation: 0 and FF release
storage; other values decrement on teardown. Whole-string copies share only
below FE. Detach updates the shared byte, allocates independent storage and
uses the source strlen operation, including its embedded-NUL behavior.

The new functions call each other and reuse the compiled 00406350 teardown
and 00406440 string-end bodies. They are in `btb_recompiled_core`, and the
executable's core verification path invokes their actual bodies.

Regression coverage includes rounded/exact retries, catch-all exceptions,
failure of both allocations, capacity reuse, retained versus freed zero-size
growth, shared copies, FE/FF sharing limits, embedded-NUL detach, self
substring erasure, and every bounded overlap direction/count against memmove.
The string allocation tests track every live allocation. Address/undefined
sanitizer checks pass; LeakSanitizer was disabled because this environment
cannot read its process task directories.

`build_retail_pure_vectors.py` also executes the original PE32's 00435630,
004380F0 and 00430500 instructions in Unicorn. The native regression matches
**1,030 remainder, 144 strchr and 704 memmove vectors** against those original
results. Data, source SHA, emulator version and vector SHA are checked in.
This is bounded valid-input reference coverage, not a certificate of all
retail states, original calling conventions or full-game equivalence.

Host invalid-state guards use C++ exceptions for impossible allocation/end
ranges; source exception object ABI and x86 SEH execution remain unfinished.
In particular, the retry helper is a source-level catch-handler translation,
not an installable replacement for the original EBP/SEH landing pad.

The batch now has 25 newly implemented bodies: one profile controller,
sixteen runtime functions and these eight connected string/memory functions.
Total compiled/tested source coverage is **413/615 (67.2%)**, with **202**
unreviewed entries. Whole-function retail verification remains **0/615**.
