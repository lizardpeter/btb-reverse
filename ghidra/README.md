# Ghidra workflow

The repository keeps original game binaries out of Git. Use a local copy of:

`Exe/Bob the Builder - Bob Builds a Park.exe`

Verified SHA-256:

`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`

## Initial import

Import the executable as a normal PE32/x86 Windows binary and allow Ghidra's default analyzers to complete.

The preferred image base is already encoded in the PE: `0x00400000`.

## Apply recovered names

Run:

`ghidra/ApplyKnownSymbols.py`

and select:

`ghidra/known_symbols.csv`

This applies the high-confidence function names already recovered through static analysis.

## Export the current decompilation

Run:

`ghidra/ExportDecompilation.py`

Choose a local output directory. It writes:

- `functions.csv` — address/name/size/caller/callee index
- `decompiled.c` — concatenated Ghidra decompiler output for all discovered functions

Generated output can then be diffed across naming/type passes without committing the original executable.

## Naming policy

Use semantic names only when there is evidence from one or more of:

- direct file/string references
- imported API behavior
- stable call-site behavior
- recovered structure layout
- multiple cross-references agreeing on purpose

Use a descriptive working name with a confidence level in `known_symbols.csv` when behavior is not fully closed.
