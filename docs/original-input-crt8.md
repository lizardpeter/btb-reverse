# Eight DirectInput and CRT original-address source implementations

These are original-function source models for the identified PE32 executable
SHA-256 \`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05\`.
The functions compile out of line as part of \`btb_recompiled_core\`,
and are linked to the game target; the new regression target executes each
with deliberately explicit native/COM/CRT boundaries.

| Address | Function | Scope |
|---|---|---|
| 00407E50 | InitializeDirectInput | COM device creation, mouse format/cooperation/event/property, profile and sound resources, bitmap registration/keying, keyboard finalization |
| 004084B0 | PollDirectInputAndUpdateState | poll/read/acquire boundaries, original scan-code selection and modifiers, click edges, cursor regions/bounds and draw |
| 00430FEA | CRT entry | GetVersion unpack, heap/SEH/locale/IO startup order, WinMain call and process termination |
| 0043A228 | Register SEH | guest FS:0 exception-list frame insertion via native provider |
| 0043969D | Compare locale strings | one-time ANSI/Wide capability probe, code-page conversion, length and lead-byte branches |
| 00430144 | sprintf | 00432971 formatter call, return and terminator handling; bounded host destination |
| 004335B9 | __cfltcvt | original e/E, f, and fallback formatting dispatch, using required float handlers |
| 00437AD7 | extended-float context | original x87 extraction / 00438E39 provider call with precision 17 |

**Boundaries and remaining work:** This batch has concrete executable C++
control flows and tests, but input devices, BIOS/Win32 process startup, the
locale API and the full original MSVC formatter are injected dependencies, not
certified retail binary equivalents. The game host currently relies on Win32
message-based pointer/click events; it has not yet been switched to these
DirectInput source adapters. Converting those adapters into actual DI8 providers
and validating device acquisition/error HRESULTs on real Windows is still
necessary. The 00432971 formatting engine and 00438E39 original extended-float
decimal converter remain unreviewed dependencies.

The Ghidra recovery contains reconstructed register/stack artifacts. We have
not silently implemented those artifacts as executable undefined behavior.
The format/locale functions preserve the evidenced branch structures, but
bounded storage and requested codepage handling are host-safe adaptations.

**Validation policy:** Initially marked \`source_written\`. Promote each to
\`compiles\` after Linux C++26 and Windows x86/x64 CI and executed regression
pass. No new direct original-executable differential comparisons were generated.
\`behavior_verified\` remains **0** pending full retail equivalence, executable
comparison and native provider/admission validation.

## CI admission — October 15, 2026

Run [38762634028](https://github.com/lizardpeter/btb-reverse/actions/runs/38762634028)
passed **all four jobs**: Linux C++26 (141/141 CTests including this
batch), Python checks, Windows x64 native game, and Windows x86 native
game/provider compilation plus regression. The eight original-address
source bodies are now marked **compiles** (574/615 total, 41 remaining).

No direct original executable comparisons were added. No new fully
retail-equivalent function has been certified. The original DirectInput
native COM provider, formatter/locale backend execution and legacy x86
exception-stack semantics remain external dependencies.
