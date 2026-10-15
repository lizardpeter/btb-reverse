# Eight connected original source reconstructions

Original executable SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.

These are C++26 source-level reconstructive implementations of eight remaining original entrypoint bodies. The native service boundaries are required, rather than optional dummy callbacks.

| Original address | Reconstructed scope |
|---|---|
| 0042D060 | Release profile UI bitmaps in original slot order, serialize 5 metadata and progress records |
| 0040B190 | Save Park Designer; destroy 20 objects; clear 300 table words; release 82 registered surface slots; stop music |
| 004074F0 | Parse quitsure numeric/string record and clear six rows of four matching placeholder labels |
| 00427C30 | Draw progress screen, 15 original star coordinates, grouped progress voice selection, quit/back handling |
| 00406C00 | Exit-on-removed-CD platform and audio shutdown with original error and quit sequencing |
| 00406CB0 | Read six CD error strings and scan drives a-z for the actual volume label |
| 00409460 | Convert original 16-bit locked surface into BMP with source 44-byte info write and header-size quirk |
| 00409730 | Call the BMP converter and orchestrate Win32 printer dialog, palette, printer and cleanup |

The profile body calls the preexisting player-profile and progress serialization
implementations, rather than inventing a second file format. Printing calls the
newly recovered BMP converter directly. The progress screen calls the existing
retail progress-group / star-coordinate functions.

The host has source tests for each branch and for representative error paths.
All 8 count initially as **source_written**, not verified until the Linux and
Windows x86/x64 C++ CI jobs pass. The source-level adapters are not in-memory
replacements for the original PE32 DirectDraw/DirectSound/Win32 ABI. In
particular, failure semantics for malformed quit files and invalid 16-bit
surface masks are intentionally bounded; original unsafe behavior is not
simulated. Original x86 differential vectors for these bodies are not yet
available; **0 fully retail-verified functions** remains the correct count.

The CD check is faithfully modeled, not bypassed. The printing routine retains
the original suspicious BMP-header length and channel-shift behavior until
against-original binary testing can settle them.

Native callers for full game/UI resource ownership, input, sprite drawing
and actual printing remain to be wired into a complete retail-equivalent host.

## Validation / gate

All four jobs passed in [GitHub Actions run 38759035193](https://github.com/lizardpeter/btb-reverse/actions/runs/38759035193):
Linux GCC 14 C++26 compilation and 140/140 CTests; Python tools;
Windows x64 and x86 native game/provider builds and regression tests.
All eight bodies are therefore marked **compiles** in the source
registry (566/615 total, 49 remaining).

This is a compile and source-contract validation, not original PE32
behavioral equivalence. Original x86 differential comparisons added: **0**.
Full retail equivalence certified: **0/615**. Full OS/DirectDraw device
behavior and top-level gameplay remain incomplete.
