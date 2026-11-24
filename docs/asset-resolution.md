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
