# Golf draw corrections against the retail PE32

Original image SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe2573a0fbfeccd39c2eff388b05

- 00414B56..00414B8F: aim marker has a 22-pixel center offset on both axes.
- 00414B92..00414C16: compare ground Y against 45; only draw shadow and ball if Y > 45 and attempts remain. Shadow offsets +4,+8; ball Y minus bounce height. Float words 0043B394..0043B3A0 verified in the executable.
- 00414C16..00414C77: Bob row truncates 44 - aim/2, not the reversed row.
- 00414C86..00414DB1: idle frames 0..19, initial counters 5,7, thereafter 5; reaction start pairs 21/21 and 45/60, exclusive end pairs 45/41 and 90/88. Consume one CRT draw per actor at the idle boundary only on the first hit reaction. The aim acceptance resets the reaction latches.
- 00414DB3..00414E70: spectator 0 omits the first pixel in its frame; spectator 1 uses the whole frame.

The original executable was read locally; no original binary or bitmap is included here. Native output and physical rendering are distinct validation layers.
