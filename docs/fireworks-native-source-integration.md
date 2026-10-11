# Retail Fireworks original-memory integration

This is an integrated C++ reconstruction improvement, not an increase in the 615-function architecture inventory.

Original SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.

Direct executable evidence: original .data table at VA 0x00442734, file offset 0x42734, contains twelve little-endian 32-bit values, ALL equal to 1. This confirms the 12 retail firework types all occupy exactly one authored grid cell.

Three source functions use this same original table: CanPlaceFireworkGridItem (0x00410FD0), PlaceFireworkGridItem (0x00411010), RemoveFireworkGridItem (0x00411080).

fireworks_retail_core.hpp implements a single authoritative 18-int grid with original -1 sentinel and type IDs 0..11. The existing Fireworks Sequence, editor, and playback scheduling operate on this shared source state, not a detached helper model. Sequence exposes retail_grid and load_retail_grid to connect the existing exact 72-byte file reader/writer to live activity state. The slots compatibility view is produced from that single native representation.

Corrupt saved grids are rejected atomically as an explicit modern safety improvement, not as a claim about retail behavior with corrupt input.

btb_fireworks_retail_integration_tests covers exact table constants, original placement and removal decisions through Sequence, source file binary round-trips, authoring playback events, failed-import atomicity, and full-grid state. Existing Fireworks editor and sequence tests must continue to pass.

Original DirectDraw, Bink, DirectSound providers and complete retail game executable remain separate work.
