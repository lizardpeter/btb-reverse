# Profile initialization and Fireworks editor drawing

Original PE32: `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`

This batch recovers two remaining original entry-point bodies as out-of-line
C++26 functions linked into `btb_recompiled_core`, with executed regression
coverage:

| Address | Source reconstruction |
|---|---|
| `0042D270` | Seven original UI guest surface slots, twenty consecutive button sprite slots and magenta keys, three 255-entry font coordinate tables, 141 ASCII mapping entries, five playerinfo metadata/name records and five original 100-int progress records. Existing `profiles::read_playerinfo` and `progress::read_record` handle real save formats. Missing progress files initialize the first value to -1. |
| `00412E50` | Draws background and authored 18-item Fireworks grid in original row ordering, clipping/presentation of two animated actors, pointer-following and flying-to-target branches, delayed placement, completion feedback, and foreground overlays. The original angle/trigonometric/distance routines remain required native callbacks. |

The font table capacities and guest surface addresses were derived from
the Ghidra global spans rather than treating the C decompiler stack arrays
as separate objects. Original button image paths, unverified filename
capitalization, and some x87 animation registers remain unresolved and
are explicitly adapter-provided or marked as source-model approximations.
No synthetic input devices or Bink decoders are used.

Tests verify the full 27-surface registration, three font tables, five
profile/progress records, missing files, malformed save rejection, original
grid ordering, actor phase transitions and presentation output ordering.
This is source-level reconstruction and NOT verified PE32 ABI fidelity.
Actual game driver wiring, exact Ghidra-vs-PE comparison, and native media
provider behavior remain unfinished.

These addresses remain `source_written` pending Linux C++26, Windows x86
and Windows x64 CI admission. No original-executable differential comparisons
were added; full retail equivalence remains 0/615.
