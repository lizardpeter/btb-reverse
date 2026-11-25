# Squirrel Run retail sprite presentation

Evidence: PE32 SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05, read-only original image; original BMP headers were also read locally.

- 004467E8, stride 8: first words are 0,3,7,10,13,16,19,22,25,28,31,48,63,66. 004266C2 adds the substep to the motion-key bank start. Render the updated frame rather than a static 0/3 image.
- 00426238..004262BA: idle animation uses banks 31..62 (restarts 31 for rand&3=0, otherwise 48) and 72..86 for special D keys 3,4,13. The 86-cell original BMP has no index 86; native omits that invalid source blit rather than indexing outside the sheet.
- 004265C7..004265E9: high bank transition can reset absolute Y to 237,205,173 according to the connector latched at section start.
- 00426A62..00426B89: eye crop follows X/30; mouth uses its idle pose or voice animation. 0044699C/A0 starts at (172,0), and initialization preserves the process actor position; the return-home target is (93,185).
- 00426DA3..00426DED: hook at X+28,0 with source rectangle (0,436-Y,47,445).
- 00426CC9..00426D07: nut pile offsets are 80,50,20, independent of the connector piece IDs.

Verified original asset dimensions: eyes 3968x35 (31x128 cells), mouth 360x47 (3x120 cells), arm 6789x192 (31x219 cells), hook 48x446, squirrel 8772x92 (86x102 cells). No original binary or copyrighted bitmap is checked in.

The remaining alternate scrolling branch and full on-device visual equivalence are separate from these source-backed corrections.
