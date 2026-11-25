# Golf airborne motion recovery

Source: original executable SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05, Ghidra `analysis/ghidra_main_exe/functions/00415080.c`, states 2 and 3.

The previous native Round had only ground-plane ball movement and tested target collisions at every physics update. State 2 initializes the separate bounce height (0050AE50), direction (0050AE38), amplitude (0050ABCC), and rising counter (0050AC04). The first amplitude is ((power / 2 + 500) * 16) / 1000. State 3 applies min(counter, 8) on ascent, decrements the counter, then descends in min(amplitude, 8) steps. On ground contact, subtract one third from amplitude using integer division; disable the next bounce below 3. Target detection at LAB_004153FC is only reached on the ground. The native renderer now keeps the shadow on the ground and offsets the ball by the separate height.

The mouse-derived aim is clamped before keyboard adjustments, as in state 0. Tests cover the ascent/descent sequence, alternating host-frame cadence, rejection of an airborne target overlap, and reset. The native Round retains its typed landing transition if friction and target capture coincide; raw multiple-target state increments, recorded bounce coordinates, and exact sprite placement/crop still require further audit. This is source-backed recovery, not live retail comparison.
