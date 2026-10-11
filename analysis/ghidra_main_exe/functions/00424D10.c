/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00424d10; function: UpdateSpudSkateActivity; body bytes: 271
 * callers: 1; callees: 8; success: True
 */


undefined4 UpdateSpudSkateActivity(void)

{
  EnableInputProcessing();
  if ((DAT_0044ddb0 == 0) && (DAT_0051c2e8 == 0)) {
    if (DAT_00514eac == 2) {
      DAT_0043ed18 = 1;
      PreparePlayAgainTransition();
      UnloadSpudSkateActivityResources();
      DAT_0044de14 = 0x3c;
      DAT_00446ce8 = 0x16;
      DAT_0051b418 = 0x1a;
      DAT_0051c2fc = 0;
      return 0;
    }
    DAT_0043ed18 = (uint)(400 < DAT_004fbd30);
    UpdateAndDrawSpudSkatePlayback();
    if (DAT_0051c30c == 1) {
      CSound_Stop(DAT_0051c2b8);
      PlayActivityMusicByIndex(8);
      DAT_0051c30c = 0;
      PlayManagedSoundById(DAT_0044ddd8,0x2f6,0x32,1);
    }
    return 0;
  }
  DAT_0043ed18 = 1;
  UnloadMazeActivityResources();
  if (DAT_0051c2e8 != 0) {
    DAT_0051c2e8 = 0;
    DAT_0044de14 = 0x16;
  }
  if (DAT_00446f38 != -1) {
    DAT_0044de14 = 0x40;
  }
  return 1;
}

