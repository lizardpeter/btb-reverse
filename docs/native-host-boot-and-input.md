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

The host now runs the original \`videoseq.txt\` startup movie table,
then the source-backed five-sign profile selector and name/badge editor.
Accepted profiles are saved in the original text format, followed by the
source-confirmed \`sceneset.bik\` transition to Activity Select. The startup
and scene-set movies use the real global Bink provider and wait for its
end-of-stream observation; \`--skip-bink\` is an explicit non-retail diagnostic
shortcut. Decoder availability remains architecture/configuration-dependent,
and a missing decoder fails visibly.

The native host has registered activity drivers for all ten activities, but
several still have incomplete retail behavior or missing platform providers.
In particular, Park Designer still needs the original 0x0040E440 editor/render
service, Herding needs the continuous motion/collision provider, and the full
set of gameplay, sound, movie, and return-to-menu paths still needs an
interactive acceptance run.

**615/615 compiled source models is not 615/615 live x86 ABI linkage or
verified behavioral equivalence.** Source tests and successful builds do not
certify missing game systems as working.
