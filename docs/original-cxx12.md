# Twelve connected C++ exception dispatch procedures

Compiled-source coverage is 546/615 (88.8%), with 69 unreviewed functions.
These original bodies are compiled into the core and linked to executed tests.
Full retail behavior certification remains 0/615.

| Address | Recovered procedure |
|---|---|
| 00432371 | Copy or bind a caught exception object |
| 00432301 | Restore catch state and conditionally destroy exception object |
| 0042FBE7 | Remove registration and resume catch continuation |
| 0042FD27 | Invoke the SE translator under its registration |
| 00431DC5 | Dispatch C++ exception handler and unwind paths |
| 004321C0 | Enter selected catch handler |
| 00431E60 | Search try blocks and catchable type list |
| 0043223B | Invoke catch funclet and clean up its saved state |
| 00432007 | Translate foreign exception or select catch-all |
| 0042FDDB | Translator registration handler |
| 0042FC78 | Original EAX-FunctionInfo frame-handler adapter |
| 0042FD02 | Nested catch registration handler |

## Source connections

These functions call one another directly. Catch search calls the recovered
004320AF RTTI matcher, including qualifiers and reference-only restrictions.
Object copy calls the recovered 0043259C PMD pointer adjustment, 00430500 byte
copy, 0043670D/00436729/00436745 pointer probes and 0042FC1B/0042FC22 object-call
trampolines. Catch entry calls the recovered 0042FC29 target unwind and 0043210C
C++ map walker. The selected catch invokes 0042FCAE/004325C0 frame-setting and
00432535 destruction. Invalid metadata enters unexpected/terminate and the
recovered fatal-runtime sequence. Required providers have no success defaults.

The copy path preserves by-reference and simple-type handling, virtual-base
adjustment, optional copy construction and the extra virtual-copy argument.
Catch cleanup restores `[frame-4]` and the previous active exception/context;
a nonzero continuation triggers C++ object destruction. A rethrow filter yields
zero and retains the original object. Other escaping exceptions run cleanup
before propagating. Foreign catch-all selection examines the final handler in
the original table. Newer exception magic can forward to the ThrowInfo callback.

Continuation state carries the original target, ESP and EBP explicitly.
The translator records nesting, function metadata and dispatch frame; the
registration handler distinguishes unwind flags from search and resumes the
original saved translator continuation.

## Validation

The generator verifies EXE SHA-256
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`
and executes the unmodified original bodies in Unicorn. Expected results never
come from reconstructed C++. Pointer-probe results, constructors, destructors,
funclets, translator/version callbacks, RtlUnwind, exit and continuation targets
are controlled boundaries. The recovered dispatch/search/copy/unwind code runs
between these boundaries.

There are 3,420 comparisons: 800 object-copy cases; 300 each for catch cleanup
and catch invocation; 400 each for dispatch and search; 200 each for the two
frame adapters, foreign exceptions, catch entry and translator handler;
120 translator calls; and 100 catch continuations. Tests compare output fields,
callback arguments/order, return values, notification records and normalized
registration state, including nonreturning continuation paths. Translated search
also exercises final object destruction. Native integration checks additionally
cover catch rethrows, exceptions escaping a catch and raised translated
exceptions completing their saved continuation.

The full Clang 19 C++26 build, 137 CTest tests and 16 Python tests pass.
ASan/UBSan checks cover the new source and selected direct dependencies;
older dependencies come from the prebuilt core, and leak detection is disabled.
Windows x86/x64 CI includes this target. The preceding 181ebc9 publication passed
both Windows jobs, Linux C++ and Python checks.

## Scope

The native API represents guest addresses and registration chains explicitly.
Its required platform/funclet providers do not install a PE32 OS exception
registration on x64. `Raised` and `ResumeTransfer` describe guest source-execution
transfers, and are not claimed to be the original Windows exception ABI.
Original OS SEH dispatch, native stack admission and full gameplay integration
still require implementation/validation. A one-megabyte RTTI-name guard bounds
malformed input instead of reading arbitrary host memory.

Evidence: `analysis/reconstruction/cxx12-validation.json`, the twelve instruction
listings, and `retail-cxx12-vectors.{txt,json}`.
