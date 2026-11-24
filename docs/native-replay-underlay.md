# Native replay underlay
Original executable SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
0042CFD0 copies the actual backbuffer into the reusable 51C298 surface,
stops/relinquishes activity music, sets the replay latch and requests 573.
00428E30 uses that captured surface as the opaque full-frame backdrop,
then blits the replay bitmap with its magenta key. Previously the native
ordered renderer cleared to black before the keyed bitmap.

The native session now captures real composed pixels at replay preparation,
then submits the replay setup/update frames through a dedicated underlay
path. Spud Skate caches its previous composed movie frame before the
activity unload destroys its Bink surfaces; this preserves the actual
last visible movie-backed scene instead of an empty bitmap-only frame.
The native system-memory surface is not retained through a full DD device
reset; a missing/lost snapshot is explicitly rejected, not silently forged.
Exact completion-tick Spud frame and display reset parity remain open.
Global movie open frames no longer trigger an unrelated UI clear.
A source session fixture verifies draw -> capture -> present, reuse of
the snapshot across both replay screens, and keyed overlay submission.
