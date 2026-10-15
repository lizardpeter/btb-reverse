# Four original Win32/CRT boundary procedures (source reconstruction)

Original PE32 SHA-256: `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

The new `original_tail4` library members are out-of-line compiled C++ source
models for original functions:

- `0042FFE2`: Read local and UTC time, recompute daylight state only when the
  UTC year/month/day/hour/minute changes, then call existing recovered
  `004326B8` calendar conversion, optionally store result.
- `00430F00`: Explicit x86 guest stack probe page-touch and final original
  return-address relocation. This is **not** native compiler `__chkstk`.
- `0043A220`: Forward four preserved arguments to the actual RtlUnwind
  boundary (original import thunk).
- `00437BF1`: Forward CRT error code 2 to nonreturning `__amsg_exit`.

Regression tests use fakes for the OS and guest-memory boundaries. They check
UTC-minute cache invalidation and local-time refresh, timezone outcomes,
shared calendar conversion, exact 4-KiB page addresses, final return-address
location, unwind argument forwarding and nonreturning CRT error dispatch.

The original executable is not available to the CI test, so **no new
original-executable differential comparison is claimed**. The Ghidra output
does not recover full Win32 FS:0/SEH or x86 stack ABI. This does not certify
retail compatibility. All four need the corresponding native provider or
application-level caller integration; avoid using this guest probe as native
`__chkstk` and do not replace OS `RtlUnwind` with a fake implementation.

## CI validation, 2026-10-15

Source commit `d18dc087b2249b0e203a4e39850cc28fac355320` passed
[CI run 38753132933](https://github.com/lizardpeter/btb-reverse/actions/runs/38753132933):
Linux C++26 reconstruction (139/139 CTests), Python tool checks,
Windows x64 native game/provider build and tests, and Windows x86
native game/provider build and tests. All four CI jobs reported success.

**558/615 (90.7%)** source bodies compile and link with the new four
admitted; **57 remain**. This is not four new binary-behavior comparisons:
the original executable is not present in CI and no new x86 differential
vectors were generated for this batch. Retail-equivalent certification remains
at 0/615.
