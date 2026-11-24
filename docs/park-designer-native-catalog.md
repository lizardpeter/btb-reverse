# Park Designer native asset and decoration evidence

Source: original Windows PE32 SHA256 `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

- `0040B7E0`: the 20 decoration and 15 bandstand sprite cells, seasonal resource filenames and palette sheets.
- `00441CD8`: 12 strict toolbar rectangles followed by the sentinel; view coordinates come from `00441DA8` and `00441DB8`.
- `00441DD8`: candidate decoration footprint (20 rectangles). `00441B18`: stored-record collision geometry (28 rectangles). These are different tables, including an unusual stored right edge of 353 in entry 2.
- `004104F1..00410858`: decoration placement/move branch within UpdateParkDesignerEditorInteraction. Collision flags, allocation boundary at slot 398, 76-byte record field writes, restored cursor center and conditional shared CRT voice draws are preserved.

The tests exercise all 20 decoration variants, all 15 supported bandstand cells, both seasons, strict toolbar edges, rejected/full placements, RNG consumption and persisted record round trips. No bitmap pixels or original executable are included.

**Integration milestone (2026-11-24):** the Windows host now installs the native Park Designer editor/render service. The catalog is consumed by the seasonal scene renderer, palette, object placement, drag preview and print composition. The original assets remain external. Source-level regression tests exercise the service, but a complete differential retail session and physical Windows printing are not yet certified.
