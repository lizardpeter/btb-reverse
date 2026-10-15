# Eight display, device and gameplay procedures

Compiled-source coverage is 554/615 (90.1%), with 61 functions remaining.
These bodies compile into the core and link to executed source tests.
Full retail behavior certification remains 0/615.

| Address | Recovered procedure |
|---|---|
| 0042EAD0 | DirectDrawCreateEx import jump |
| 0042FB50 | DirectInput8Create import jump |
| 0042FB56 | DSOUND ordinal 11 / DirectSoundCreate8 import jump |
| 00403080 | Initialize fullscreen display |
| 004031E0 | Initialize windowed display |
| 00401E20 | Create main game window |
| 004112A0 | Load Fireworks editor resources |
| 004232B0 | Move Pilchard toward graph node |

## Connections and recovered details

The native DirectDraw renderer calls the recovered fullscreen/windowed bodies.
Their provider invokes actual Win32 and DirectDraw7 APIs. Native sound startup
calls the recovered sound import thunk through a DirectSoundCreate8 provider.
The three six-byte import jumps have deliberately small forwarding source
bodies; they are counted separately because they are original function entries.
The DirectInput provider resolves the actual dinput8.dll entry point, but this
batch does not connect it to the game's complete input initialization.

Windowed setup restores original style changes, size adjustment, z-order and
work-area positioning, plus clipper release on both failure and success paths.
Fullscreen setup restores capability flags and the extra back-buffer AddRef.
Native shutdown balances both acquired references. Providers preserve native
pointer width and translate semantic descriptor fields into the platform's
DDSURFACEDESC2 size rather than imposing the original 124-byte PE32 layout.
Partial initialization results are adopted for native failure cleanup.

Window creation preserves original class/title capitalization, resource IDs,
metrics and early-return behavior. It is tested separately; the current native
host has its own window procedure and resource setup.

The Fireworks loader preserves load/register/key ordering and the original
registration-slot mistake for the depressed delete-all image. Pilchard movement
preserves the obstruction branch that tests obstruction_b == 4 inside the
obstruction_a branch. It calls recovered integer-distance, angle and float-to-
integer helpers. Invalid host node indices throw instead of reading arbitrary
memory. Both gameplay bodies still need complete activity caller integration.

## Validation and limits

The generator checks executable SHA-256
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`
and executes the unmodified original x86 bodies in Unicorn. Win32, COM and
bitmap dependencies are controlled boundaries. Expected outputs do not come
from reconstructed C++. Comparisons include failure returns, partial outputs,
descriptor fields, callback arguments and order, aliases and actor state.

All 3,580 comparisons pass: 200 fullscreen, 300 windowed, 200 window creation,
100 per import, 80 bitmap-loading and 2,500 movement cases. Movement permits
at most two final-float ULPs, or absolute error 0.00002 when both magnitudes are
below one, across host transcendental implementations. On the tested Linux
host, zero components required that tolerance. This does not certify x87 status
flags or unrestricted floating-point inputs.

The full Clang 19 C++26 build, 138 CTest tests and 16 Python tests pass.
ASan/UBSan pass the new bodies, maze input and tests; older dependencies use
prebuilt archives and leak detection is disabled.
Windows x86/x64 CI includes the new test. The preceding publication, 67ce6f4,
passed Windows x86/x64, Linux C++ and Python jobs. New native device behavior
still needs execution on Windows with original assets and comparison to retail.
Compiled coverage does not measure complete game fidelity or native OS ABI parity.

Evidence: `analysis/reconstruction/device8-validation.json`, eight instruction
listings and `analysis/reconstruction/retail-device8-vectors.{txt,json}`.
