/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00415830; function: UpdateGolfActivity; body bytes: 444
 * callers: 1; callees: 9; success: True
 */


undefined4 UpdateGolfActivity(void)

{
  int iVar1;
  
  EnableInputProcessing();
  if ((DAT_0044ddb0 != 0) || (DAT_0051c2e8 != 0)) {
    UnloadGolfActivityResources();
    if (DAT_0051c2e8 != 0) {
      DAT_0051c2e8 = 0;
      DAT_0044de14 = 0x1c;
    }
    if (DAT_00446f38 != -1) {
      DAT_0044de14 = 0x40;
    }
    return 1;
  }
  if (0 < DAT_0050abb0) {
    DAT_0043ed18 = (uint)(400 < DAT_004fbd30);
    DrawGolfActivity();
    UpdateGolfGameplay();
    if (DAT_004437e0 == 1) {
      if (DAT_0051c30c == 1) {
        DAT_0051c30c = 0;
        CSound_Stop(DAT_0051c2b8);
        PlayActivityMusicByIndex(3);
        PlayManagedSoundById(DAT_0044ddd8,0x7b,0x32,1);
        *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x2fb) * 4 + 0xd98) = 1;
        return 1;
      }
      iVar1 = AnyManagedSoundPlaying(DAT_0044ddd8);
      if (iVar1 == 0) {
        DAT_004437e0 = 2;
        PlayManagedSoundById(DAT_0044ddd8,0x65,0x32,1);
        *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x2e5) * 4 + 0xd98) = 1;
      }
    }
    return 1;
  }
  DAT_0043ed18 = 1;
  iVar1 = AnyManagedSoundPlaying(DAT_0044ddd8);
  if (iVar1 == 1) {
    DrawGolfActivity();
    return 1;
  }
  iVar1 = DAT_00519934 * 400;
  if (*(int *)(&DAT_0051b5c8 + iVar1) == 0) {
    *(undefined4 *)(&DAT_0051b5c8 + iVar1) = 1;
  }
  if (*(int *)(&DAT_0051b5c4 + iVar1) == 1) {
    DAT_00446f38 = 3;
  }
  DAT_0051c32c = 1;
  PreparePlayAgainTransition();
  DAT_0044de14 = 0x3c;
  DAT_0051b418 = 0x36;
  DAT_00446ce8 = 0x1c;
  UnloadGolfActivityResources();
  DAT_0051c2fc = 1;
  return 1;
}

