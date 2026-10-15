# Fifteen original exception and runtime procedures

The compiled-source inventory is 534/615 (86.8%), with 81 unreviewed functions.
This milestone means real C++ bodies compiled into the core and linked to
executed tests. Full retail behavior certification remains 0/615.

| Original address | Recovered procedure |
|---|---|
| 00430495 | Construct and raise the three-parameter MSVC C++ exception |
| 0042FC1B | Object-call trampoline retaining this and stack arguments |
| 0042FC22 | Second object-call trampoline |
| 0042FC29 | Unwind to target and splice the original registration chain |
| 0042FCAE | Install catch registration and invoke the setting-frame helper |
| 0042FE50 | Select try-block map interval for a nesting depth |
| 0042FECC | Global unwind with original continuation identity |
| 0042FF0E | Walk local scope entries and invoke finally handlers |
| 0042FF76 | Detect abnormal termination from live unwind state |
| 0043210C | Walk C++ unwind map and invoke cleanup funclets |
| 00432535 | Destroy the exception object through its ThrowInfo |
| 0043260C | Invoke terminate handler, suppress handler exceptions, abort |
| 00432662 | Invoke unexpected handler and enter terminate |
| 004304D0 | Convert x87 ST(0) to signed 64-bit integer toward zero |
| 0043A2F5 | Get allocation size through either small heap or HeapSize |

## Connections and original details

The exception procedures call each other directly: the destructor uses the
object trampoline, invalid unwind metadata enters unexpected and terminate,
and catch and C++ cleanup invoke the already recovered 004325C0 setting-frame
helper. Termination uses the recovered 0043675D fatal-runtime procedure, retaining
report 10, signal 22 and exit `(3, 1, 0)` ordering. Required callbacks and operating
system services have no default-success implementation.

The original local-unwind helper rereads the live frame level after every
callback. Its 0042FFA2 notification uses `[EBP+8]`, despite the pushed 0x101
constant; the executable comparisons exposed this distinction. C++ unwind reads
the next-state map after the callback and terminates if a guest C++ exception
escapes its cleanup handler. Catch registrations retain function metadata and
the incremented nesting depth.

The integer converter consumes the actual ten-byte extended representation,
so it does not depend on the host's long-double precision. It implements the
original truncation and integer-indefinite results for masked invalid/overflow
cases. Ghidra's `ROUND` expression would produce the wrong rounding behavior.

Allocation-size lookup calls the existing region and page lookup bodies. Mode 3
returns the block header minus nine; mode 2 returns the paragraph byte times
sixteen. The remaining path invokes the required HeapSize service.

## Validation

The executable is checked against SHA-256
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.
The independent Python generator executes original x86 instructions in Unicorn;
it never derives expected values from reconstructed C++.

There are 6,020 comparisons: 2,500 raw extended conversions, 600 allocation-size
lookups, 400 local unwind cases, 400 C++ unwind cases, 400 try-range cases,
300 each for throw, object trampoline, catch and abnormal termination,
200 exception destructors, 160 target unwinds, 80 global unwinds and
80 terminate/unexpected cases. The cases compare callback arguments, ordering,
return values, output fields, normalized registration state and notification
records. Invalid try-range metadata exercises the nonreturning abort path.
Native tests additionally cover exceptions thrown by termination handlers and
C++ unwind funclets.

The complete Clang 19 C++26 build, 136 CTest tests and 16 Python tests pass.
ASan/UBSan passes for the new sources and selected direct dependencies, linked
with the prebuilt core for older routines; leak detection is disabled.
Windows x86/x64 CI includes the new test target. Publication 471abd6 containing
the preceding forty-function batches passed both Windows jobs and Linux checks.

## Limits

The source API represents a thread registration chain with native typed
records. It does not inject PE32 FS:[0] records into the host OS, recreate an x86
stack frame on x64, or claim Windows exception dispatch compatibility. Required
OS and funclet providers still need native game integration. Raw conversion
comparisons check returned integers and original control-word restoration;
unmasked FPU traps and FPU status flags are outside this API. Full gameplay
coverage and retail equivalence remain unfinished.

Evidence is in `analysis/reconstruction/unwind15-validation.json`, the 15 original
instruction listings, and `retail-unwind15-vectors.{txt,json}`.
