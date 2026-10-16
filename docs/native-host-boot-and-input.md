# Native source-game host: primary executable and input admission

## What this change actually integrates

Normal \`btb_game.exe\` startup now routes to the **native GameRoot**
(68-state original router) with DirectDraw 7 and DirectSound and requires the
user's original installed game data. The older GDI Dinosaur demonstration is
no longer silently presented as the whole game.

- \`btb_game.exe\`: look for \`BTB_RETAIL_ROOT\`, beside/above the executable,
  or in the working-directory ancestry; otherwise prompt for the retail root.
- \`btb_game.exe --native-original "D:\Original Game"\`: use a specified root.
- \`btb_game.exe --legacy-dino-preview\`: explicitly run the old GDI demonstration.
- \`btb_game.exe --recompiled-smoke\`: headless linkage check.

An acceptable retail root contains \`loaddata/uiBitmapName.txt\` and \`Data/\`.
No game files or original DLLs are redistributed.

## Actual native input fixes

The native host now maps \`WM_KEYDOWN\`, \`WM_KEYUP\`, \`WM_CHAR\`,
\`WM_LBUTTONDOWN\`, \`WM_LBUTTONUP\`, and \`WM_KILLFOCUS\` into the real
\`ActivityFrameInput\` shared by source-authored gameplay:
- 256-byte keyboard-state array; four directional keys mapped to the
  original Herding left/right/up/down mask \`1/2/4/8\`.
- Single-shot click observation shared between the activity and
  original input pipeline; Space is handled without key-repeat clicks.
- Physical mouse-held state; ASCII typed character for profile/UI logic.
- Monotonic elapsed centiseconds from the actual \`timeGetTime()\` tick,
  including 32-bit timer wrap.

The new \`native_host_input\` test exercises mouse hold/release, click
edge, four directions, keyboard release, typing and focus loss. Windows x86
and x64 builds compile this source and its test.

## **Not** a finished full-game implementation

The native runtime currently has registered source activity drivers for
Dinosaur, Golf, Fireworks, Bob's Band and Park Designer. Five activities
still lack complete registered runtime adapters. The existing Herding driver requires a missing real motion/
collision provider. The Windows x86 host now decodes global and walkthrough
movies through the installed original `Exe/binkw32.dll` and presents their
frames through DirectDraw7. Bink playback, movie audio and walkthrough inset
placement still need retail differential validation, and the x64 host cannot
load this 32-bit DLL. Original profile-selection screens and some runtime
GUI/backend paths are also unfinished.

**615/615 compiled source models is not 615/615 live x86 ABI linkage or
verified behavioral equivalence.** This work fixes the primary executable
selection and real native input routing; it does not certify missing game
systems as working.

The development host loads `profiles/playerinfo.txt` and the five progress
records before selecting its temporary player-zero slot. It still starts at
Activity Select. The new `videoseq.txt` boot model is exercised through the
physical session regression, but must not be advertised as the default
startup/profile workflow until the five-sign selector is connected.
