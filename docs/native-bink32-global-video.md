# Original Bink32 movie integration (source-native)

The Windows x86 host decodes both **global full-screen movies** and the
separate looping **instruction walkthrough inset** with the installed retail
\`Exe/binkw32.dll\`. It now:

1. loads and validates the actual Bink export table by absolute path;
2. calls \`BinkOpen\`, \`BinkWait\`, \`BinkDoFrame\`,
   \`BinkCopyToBuffer\`, \`BinkNextFrame\` and \`BinkClose\`;
3. copies decoded 32-bit global movie frames to the existing DirectDraw7
   backbuffer (top-down BGRA, GDI \`StretchDIBits\`/ReleaseDC);
4. reports movie end to the 68-state root only after presenting the
   last decoded frame, never by timeout or fabricated movie completion;
5. decodes the independent walkthrough stream, loops it with the original
   Bink seek operation, and composites it after the instructional screen;
6. rejects native x64 use of the x86 DLL with an explicit error.

The public Bink SDK's first seven \`BINK\` structure fields and
\`BINKSURFACE32 | BINKCOPYALL\` flags are used; see the repository's
\`win32_original_bink_exports\` for all expected retail imports.

The original Bink sound-system callback is connected to the same native
DirectSound8 instance as the game's managed sounds using the historical
`BinkSetSoundSystem(BinkOpenDirectSound, IDirectSound*)` ABI. Actual movie
audio, frame timing, buffer interpretation and walkthrough inset placement
still require video-by-video comparison with the retail executable on
Windows x86. The supplied Bink DLL and media were not available in GitHub
CI. This decoder is unavailable in the x64 executable because the original
DLL is 32-bit.

This improves a real missing video execution path, not a claim that
the ten activities or full original game are already playable.
