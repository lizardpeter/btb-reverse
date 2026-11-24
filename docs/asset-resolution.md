# Original retail asset lookup

The faithful native host resolves media and artwork against the original game
installation directory first. Some distributions keep part of the original
asset tree on a separately mounted or extracted disc image.

Set `BTB_DISC_ROOT` to that disc's asset root before launching
`btb_game.exe`. The directory should contain the original relative asset
paths (for example, `Data/sound` or `data/movies`). The installed game root
remains authoritative and is always searched first; the disc root is only a
fallback. The same traversal validation and case-insensitive component lookup
apply to both roots.

PowerShell example:

```powershell
$env:BTB_DISC_ROOT = 'D:\'
.\btb_game.exe
```

If the requested file is absent from both roots, the session still stops rather
than substituting silence, a different WAV, or fabricated media. Check the
exact relative path printed in the error against the original installation
and disc image. This setting does not supply missing startup tables: those
remain loaded from the installation's `loaddata` directory.

A fatal frame error is reported once. The host stops its frame timer before
opening the error dialog because Win32 modal dialogs pump a nested message
loop; repeated timer messages must not create recursively stacked dialogs.
