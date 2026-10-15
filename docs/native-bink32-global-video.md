# Original Bink32 global movie integration (source-native)

This batch replaces the always-failing \`MissingOriginalBinkDecoder\` for
**global full-screen Bink movies** with real calls to the supplied retail
\`Exe/binkw32.dll\` on Win32 x86. The Windows host now:

1. loads and validates the actual Bink export table by absolute path;
2. calls \`BinkOpen\`, \`BinkWait\`, \`BinkDoFrame\`,
   \`BinkCopyToBuffer\`, \`BinkNextFrame\` and \`BinkClose\`;
3. copies decoded 32-bit Bink output to the existing DirectDraw7
   backbuffer (top-down BGRA, GDI \`StretchDIBits\`/ReleaseDC);
4. reports movie end to the 68-state root only after presenting the
   last decoded frame, never by timeout or fabricated movie completion;
5. rejects native x64 use of the x86 DLL with an explicit error.

The public Bink SDK's first seven \`BINK\` structure fields and
\`BINKSURFACE32 | BINKCOPYALL\` flags are used; see the repository's
\`win32_original_bink_exports\` for all expected retail imports.

**Not yet certified**: Original installed Bink binary and actual movie
asset have not been present in GitHub CI. The ABI/buffer flags are compiled
but still need video-by-video validation. The original Bink sound-system callback is now connected to the same
native DirectSound8 instance as the game's managed sounds using the
historical `BinkSetSoundSystem(BinkOpenDirectSound, IDirectSound*)` ABI.
**Audible retail video playback is still unverified** without the original
DLL and Bink assets on a real x86 Windows machine.

**Walkthrough/instructional inset movie placement and associated spoken
WAV arbitration remain unavailable**, and the implementation still
rejects walkthrough opens rather than inventing where or when to render
them. These remain essential to complete original UI flows. It is also
not possible to use the original 32-bit DLL in the x64 executable.

This improves a real missing video execution path, not a claim that
the ten activities or full original game are already playable.
