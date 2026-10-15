# Original Fireworks initialization and 23-movie bank

PE32 executable SHA-256: `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

- `004115C0` recovers the original 23-movie load sequence with the exact
  path/order preserved by `ghidra/firework_movies.csv`; 1,500,000-byte
  maximum source read, a durable per-index copy of source Bink bytes, the
  original 124-byte video-surface descriptor with depth 7, four index-range
  width/height combinations, DirectDraw surface registration slot
  `0x0050937c + i*4`, and opening movie handles against their own buffers.
  The first two overlays and their magenta key are loaded before 23 Binks;
  crowd phase resets on load. A failed resource is rejected instead of
  silently substituting a dummy Bink handle.

- `00411C80` recovers the editor bootstrap: install unload callback, mark
  player participation, reset state and the 120-int work array to -1, read
  the original player `firedata[1..5].txt` 72-byte 18-int grid if present,
  decode 18 editor-placement + 12 palette rectangles with existing
  `fireworks::parse_layout`, register 12 palette and 12 overlay bitmaps
  in the original slot order, then two backdrops and the delete cursor.
  Color-key the 24 sprites and cursor and install the original
  `(global-volume-100)*42` sound setting.

The source is compiled into `btb_recompiled_core` and exercised by an
out-of-line C++ test, not merely recorded as Ghidra notes. The original
resource/media loaders and DirectDraw/Bink implementations are deliberately
typed, required provider interfaces. Full media playback requires wiring
them to the live native game. The editor-palette and secondary overlay
filenames remain provider-sourced where not yet verified by this batch.

No original x86 executable differential testing or retail-equivalence
certification is claimed. These two functions remain `source_written`
until Linux C++26, Windows x86/x64 and Python CI pass.
