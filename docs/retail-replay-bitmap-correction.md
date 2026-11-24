# Retail replay-screen bitmap correction

Original game PE32 SHA-256:
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

The supplied installed/disc data and Ghidra function 0042A2C0 agree:
| State | Literal string address | Actual bitmap |
| --- | --- | --- |
| 0x3c yes/no | 00447094 | Data/ui/playagain/playagainnodiff.bmp |
| 0x3e difficulty | 00447070 | Data/ui/playagain/playagain3.bmp |
| 0x42 Fireworks replay | 004470bc | Data/ui/editplay/Fireworkplayagain.bmp |

All three files exist in the supplied retail inventory. The three obsolete
paths in uiBitmapName.txt slots 19, 20, and 21 are absent. The original
dispatcher never uses those slots for these replay setups: it loads the
literal string, registers the surface, and sets color key 0xff00ff.
The native menu host now follows that behavior. The original catalog is
preserved verbatim; do not invent a filename alias or alter game data.

The generic UI source regression exercises the emitted path and keyed draw
for each of the three replay states. This is not yet visual certification of
the original retained movie background behind the replay overlay. The
native DirectDraw frame-clear and surface restoration policy still needs
physical comparison.
