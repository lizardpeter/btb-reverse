# Original profile controller and sixteen runtime bodies

These reconstructions use the checked-in original-executable Ghidra corpus
with SHA-256 `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.
They are actual out-of-line C++ bodies with executed source regressions.
Original x86 calling conventions and retail differential equivalence have
not been certified.

## Five-sign controller, 0042E110

`original_profile_screen.cpp` implements all branches in
`UpdatePlayerProfileScreen`: the original shared help/input call order,
variant resets, five centered names and badges, strict eight-area hit test,
sound-specific introduction gates, hover debounce and random consumption,
empty-profile creation, existing-profile voice wait, backing track teardown,
delete cursor/modal/confirmed save clearing, and quit voice/snapshot handling.

The existing-player selection copies the low byte of each original saved
name word, clears the distinct creation index to -1, writes progress word 0
to 1, and advances to original outer state 4. Backing Stop is followed by
a second live existence check before releasing the track, as in retail.

`GameRoot::update_original_profile_screen` invokes this original body against
the ONE live metadata/progress table and commits its selected player/state
transition. The original renderer, sound, help, cursor and file dependencies
are mandatory typed services. A missing service is not treated as successful
gameplay. The source regression checks the live coordinator and the precise
source effects/order.

The original resource initializer 0042D270, complete native profile provider
and state-1 startup adapter remain unfinished. This does not yet replace
the development host's provisional profile 0. No retail assets or native
UI calls were fabricated to conceal those dependencies.

## Runtime batch

| Address | Compiled function |
|---|---|
| 00436231 | Assign original descriptor OS handle and standard-handle mapping |
| 004362A8 | Release descriptor OS handle without changing open flags |
| 0043635F | Commit/FlushFileBuffers, preserving source DOS-error behavior |
| 00439627 | Switch text/binary descriptor mode and return previous mode |
| 00434005 | Allocate/reuse the first inactive 32-byte FILE slot |
| 00431242 | Flush original buffered FILE data, reset cursor/count |
| 004356A5 | Temporary stdout/stderr terminal buffer, inline fallback |
| 00435630 | Original unsigned 64-bit remainder algorithm |
| 004380F0 | strchr result semantics with byte-truncated argument |
| 0043867A | Search the three-word action table by its second word |
| 0043422A | Read module major/minor linker version |
| 004320AF | Match decorated exception types and source qualifier flags |
| 00437A60 | Round/pad decimal digits and propagate decimal carry |
| 004303BA | Construct owned exception message |
| 004303F7 | Copy owned exception message or retain borrowed pointer |
| 00430835 | Optional startup hook, then the two ordered initializer tables |

`original_runtime16.cpp` is in `btb_recompiled_core`; the executable's existing
core verification path calls the batch validator, forcing the actual bodies
into the executable linkage. It reuses the compiled 0043091D initializer
dispatcher and the shared original exception state/destructor.

`BufferedIoLink` also connects existing 00431207/004312A7/0042FB62 callers to
the NEW 00431242 flush and 0043635F commit. The integration regression runs
existing flush-one and flush-all procedures through those actual bodies.
Unrelated upstream close/free/lock/scan callbacks retain their own context.

The FILE and descriptor structures retain 32-byte/eight-byte word layouts.
Guest addresses are explicit 32-bit values. OS handle operations, low-level
descriptor writing and guest allocation require real providers; these are
not silently replaced by host CRT operations or successful no-ops. The batch
does not replace the native application's entire CRT ABI.

## Validation

The full Clang 19 C++26 build links the game and all source tests.
The remainder test exercises 100,000 seeded inputs and boundary values across
both original division branches against unsigned arithmetic results.
Other regressions cover all exception property/CV combinations, descriptor
error preservation, FILE reuse/allocation failure, partial writes, text-mode
flags, decimal carry/padding/negative precision, and borrowed versus owned
exception copies.

Windows x86/x64 CI explicitly builds/runs both new test targets, with assert
enabled even in Release. Compilation and source-state regressions are not
retail behavioral verification.
