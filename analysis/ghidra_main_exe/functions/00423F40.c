/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00423f40; function: UpdateSpudMazeActivity; body bytes: 438
 * callers: 1; callees: 12; success: True
 */


undefined4 UpdateSpudMazeActivity(void)

{
  int iVar1;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 uVar2;
  int *extraout_EDX;
  int *extraout_EDX_00;
  int *piVar3;
  
  EnableInputProcessing();
  if ((DAT_0044ddb0 != 0) || (DAT_0051c2e8 != 0)) {
    UnloadSpudMazeActivityResources();
    if (DAT_0051c2e8 != 0) {
      DAT_0051c2e8 = 0;
      DAT_0044de14 = 0x16;
    }
    if (DAT_00446f38 != -1) {
      DAT_0044de14 = 0x40;
    }
    return 1;
  }
  if (DAT_005109c8 == 0xf) {
    StopAllManagedSounds(DAT_0044ddd8);
    PlayManagedSoundById(DAT_0044ddd8,0x3e2,0x5a,1);
  }
  if ((DAT_005144c8 < 1) || (DAT_005109c8 < 0)) {
    if (DAT_00512128 == 0) {
      DAT_00446ce8 = 0x38;
      if (DAT_005107d0 < 0x78) {
        iVar1 = UpdateSpudMazeMrBentleyNarrationSequence();
        if (iVar1 == 1) {
          DAT_005107d0 = 0x78;
        }
        DrawAndUpdateSpudMazeScene();
        return 1;
      }
      if ((DAT_005144c8 < 1) &&
         (iVar1 = DAT_00519934 * 400, *(undefined4 *)(&DAT_0051b5bc + iVar1) = 1,
         *(int *)(&DAT_0051b5c0 + iVar1) == 1)) {
        DAT_00446f38 = 2;
      }
      PreparePlayAgainTransition();
      DAT_0044de14 = 0x3c;
      DAT_00446ce8 = 0x16;
      DAT_0051b418 = 0x3a;
      UnloadSpudMazeActivityResources();
      DAT_0051c2fc = 1;
      return 1;
    }
LAB_0042404a:
    DrawSpudMazeScreenTransition();
  }
  else {
    if (DAT_00512128 != 0) goto LAB_0042404a;
    UpdateSpudMazeBobMovement();
    uVar2 = extraout_ECX;
    piVar3 = extraout_EDX;
    if (DAT_00512128 != 0) goto LAB_00424064;
  }
  DrawAndUpdateSpudMazeScene();
  uVar2 = extraout_ECX_00;
  piVar3 = extraout_EDX_00;
LAB_00424064:
  UpdateSpudMazePilchard(uVar2,piVar3);
  if (DAT_0051c30c == 1) {
    CSound_Stop(DAT_0051c2b8);
    PlayActivityMusicByIndex(7);
    DAT_0051c30c = 0;
    PlayManagedSoundById(DAT_0044ddd8,0x2b6,0x32,1);
    *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x536) * 4 + 0xd98) = 1;
  }
  return 0;
}

