# Batch reconstruction: source-backed small native functions

Each function below is directly recoverable from its Ghidra pseudocode
(\`analysis/ghidra_main_exe/functions/<address>.c\`). The implementation
is in \`reconstruction/include/btb/retail_leaf_runtime.hpp\` unless it was
already implemented as a matching tested source model. Tests are in
\`reconstruction/tests/retail_leaf_runtime_test.cpp\`.

**Scope:** Host-neutral original state mutations, arithmetic, table indexing,
printer record access, cursor animation steps and low-level Bink call routing.
These are NOT executable-entrypoint symbols and do NOT imply whole-game
runtime, audio, graphics or retail output parity. Original global storage is
provided through explicit struct fields/references. COM/Bink calls are
supplied as callbacks rather than fabricated.

The 32-bit signed overflow behavior of the x86 cursor tests and
time-delta arithmetic is preserved without C++ signed overflow UB. The
\`IsDinoCursorNearPoint\` guard is intentionally a literal reproduction of
the x86 signed conditional: the INT_MIN absolute-value overflow quirk is
preserved. The 64-bit FILETIME helper uses an unsigned wrapping delta, and
the unrelated low-DWORD helpers do not silently switch to 64-bit arithmetic.

Existing tested source was reused for the 5 printer methods, the
\`CWaveFile\` layout and the shared PointInPolygon test. On Windows the
Bink provider still needs to bind the recovered function pointers.
The optional-null callback cases are explicit host safety measures.

Only funcs whose source **and actual C++ tests** were compiled are tracked
as \`source_written\`. CI compilation is not a retail behavior verification.
