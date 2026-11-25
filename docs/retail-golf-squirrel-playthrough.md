# Installed-data Golf / Squirrel playthrough diagnostic

Build `btb_retail_golf_squirrel_playthrough` from the ordinary C++26 build. Run:

    btb_retail_golf_squirrel_playthrough "/path/to/BTB-BBP" both 1

Each seed exercises all three difficulties and both display modes (12 playthroughs). The probe supplies normal mouse pulses, waits for actual state-machine progression, and attempts one wrong choice before each correct Squirrel placement. Golf locks the live meter rather than injecting shots. The root performs progress writes, unload and replay handoff; the checker verifies the selected profile only. Bitmap source rectangles must fit the installed BMP headers and the portable clipping constraints used by the Windows renderer. Explicit sound filenames must resolve. The original directory is read-only. The Squirrel loader resolves mixed-case retail asset paths using the common resolver.

This checks gameplay and emitted draw commands; simulated eight-update audio duration is not a claim of actual DirectSound timing, pixel parity or interactive device certification. No installed-data results are claimed until the tool runs on actual files.
