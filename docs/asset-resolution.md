# Original retail asset lookup

The faithful native executable uses the supplied original game directory
directly. The original filenames are resolved with case-insensitive path
component matching; no assets are renamed or substituted.

The original 004281D0 walkthrough opener consumes only the BIK filename.
The separately numbered narration in Data/sound/binklist.txt is handled
by the managed sound system. The second column of binkwalk.txt is retained
as historical metadata, not a required file. No extra disc-root setting
is needed to enter the retail instruction screens.

The reusable asset resolver still supports an explicitly supplied second
root for callers that actually model the original CD fallback. If neither
root contains a required asset, diagnostics identify both probes. The
Windows executable does not require or read BTB_DISC_ROOT.

A genuinely missing requested bitmap, WAV, or BIK stops the affected
session with the real filename and path diagnostic; no replacement asset
or fabricated playback is used.

A fatal frame error is reported once. The host stops its frame timer before
opening the error dialog because Win32 modal dialogs pump a nested message
loop; repeated timer messages must not create recursively stacked dialogs.

## Native activity resource lifetime

The native DirectDraw cache previously retained every source bitmap from
all visited activities until process shutdown. The retail activity
initializers and unloaders instead manage their own surface groups.
The host now releases cached source bitmaps at activity initialization
and after the final composed unload frame. The saved draw stream and
physical replay-underlay surface remain independent so replay and
lost-surface recovery can recreate source bitmaps as needed. Within a
frame, duplicate exact filename requests resolve once; a later frame
still checks the original directory. Error messages include the exact
source asset and rectangle when a sprite extends beyond its bitmap.
This is a host resource-lifetime correction, not a change to assets or
retail gameplay. Tests cover replay capture -> release -> present order,
replay retention, repeated draw order, and missing-file revalidation.
