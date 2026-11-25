# Installed-data Golf / Squirrel playthrough diagnostic

Build `btb_retail_golf_squirrel_playthrough` from the ordinary C++26 build. Run:

    btb_retail_golf_squirrel_playthrough "/path/to/BTB-BBP" both 1

Each seed exercises all three difficulties and both display modes (12 playthroughs). The probe supplies normal mouse pulses, waits for actual state-machine progression, and attempts one wrong choice before each correct Squirrel placement on Medium and Hard (Easy contains no decoys). Golf locks the live meter rather than injecting shots. The root performs progress writes, unload and replay handoff; the checker verifies the selected profile only. Bitmap source rectangles must fit the installed BMP headers and the portable clipping constraints used by the Windows renderer. Explicit sound filenames must resolve. The original directory is read-only. The Squirrel loader resolves mixed-case retail asset paths using the common resolver.

This checks gameplay and emitted draw commands; simulated eight-update audio duration is not a claim of actual DirectSound timing, pixel parity or interactive device certification. A local installed-data run on November 25, 2026 completed 144/144 playthroughs (12 seeds × three difficulties × two display modes × two activities), covering all five profile selections. Golf: 72 completions, 360 shots, 126,816 updates and 1,782,672 draws. Squirrel: 72 completions, 432 correct and 360 rejected placements, 410,812 updates and 8,796,813 draws. Every submitted crop fit the actual installed BMP dimensions. The metadata fixture and its negative tests also passed. These are read-only core/command-stream tests, not physical Windows certification.

The metadata-only fixture builds synthetic 26-byte BMP headers using the 31 measured original dimensions, plus source-layout numeric input fixtures. It runs all twelve combinations in CTest and verifies missing assets and invalid crops fail rather than being silently accepted. It is explicitly not physical decoding or a real-art playthrough. The Windows build publishes a separate `btb-retail-activity-win64-tools` diagnostic artifact. The shipped Golf data also contains the previously undocumented final optional pair 304 155, now covered in parser regression.

For offline debugging, CI also publishes `btb-source-snapshot` (an exact tracked-source archive, no original media) and `btb-native-linux-audit-tools` (compiled portable probes and the Bink audit). Both are retained for seven days. An artifact can be retrieved in an isolated environment without cloning the private repository directly.
