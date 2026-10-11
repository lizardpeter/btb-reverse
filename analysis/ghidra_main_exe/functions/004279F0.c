/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004279f0; function: UpdateSquirrelActivity; body bytes: 575
 * callers: 1; callees: 13; success: True
 */


undefined4 UpdateSquirrelActivity(void)

{
  uint uVar1;
  int extraout_ECX;
  undefined4 uVar2;
  int iVar3;
  
  EnableInputProcessing();
  if ((DAT_0044ddb0 == 0) && (DAT_0051c2e8 == 0)) {
    NoOpLegacyHook();
    if (DAT_005150e0 == 0) {
      if (DAT_005150d8 == 0) {
        UpdateSquirrelConveyorSlideCycle();
        DrawAndUpdateSquirrelActivityScene();
        UpdateSquirrelPlacementInteraction();
      }
      else {
        UpdateAndDrawSquirrelLevelTransition(extraout_ECX);
        DrawAndUpdateSquirrelActivityScene();
      }
    }
    else {
      DrawAndUpdateSquirrelActivityScene();
      if (DAT_00515058 == 0) {
        StopAllManagedSounds(DAT_0044ddd8);
        iVar3 = 1;
        uVar2 = 0x32;
        uVar1 = FUN_0042ffc4();
        PlayManagedSoundById(DAT_0044ddd8,(int)uVar1 % 3 + 0x299,uVar2,iVar3);
        DAT_00515058 = DAT_00515058 + 1;
      }
      else if (DAT_00515058 == 1) {
        iVar3 = AnyManagedSoundPlaying(DAT_0044ddd8);
        if (iVar3 == 0) {
          DAT_00515058 = DAT_00515058 + 1;
        }
      }
      else {
        DAT_00446f38 = 5;
        PreparePlayAgainTransition();
        UnloadSquirrelActivityResources();
        DAT_0044de14 = 0x3c;
        DAT_0051b418 = 0x28;
        DAT_00446ce8 = 0xffffffff;
        DAT_0051c2fc = 1;
        *(undefined4 *)(&DAT_0051b59c + DAT_00519934 * 400) = 1;
      }
    }
    if (DAT_005150fc < 5) {
      if (DAT_0051c30c == 1) {
        PlayActivityMusicByIndex(9);
        PlayManagedSoundById(DAT_0044ddd8,0x283,0x32,1);
        *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x503) * 4 + 0xd98) = 1;
        DAT_0051c30c = 0;
        return 1;
      }
      iVar3 = AnyManagedSoundPlaying(DAT_0044ddd8);
      if (iVar3 == 0) {
        if (DAT_005150fc == 0) {
          PlayManagedSoundById(DAT_0044ddd8,0x278,0x32,1);
          *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x4f8) * 4 + 0xd98) = 1;
          DAT_005150fc = DAT_005150fc + 1;
          return 1;
        }
        if (DAT_005150fc == 1) {
          PlayManagedSoundById(DAT_0044ddd8,0x284,0x32,1);
          *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x504) * 4 + 0xd98) = 1;
          DAT_005150fc = 5;
        }
      }
    }
    return 1;
  }
  UnloadSquirrelActivityResources();
  if (DAT_0051c2e8 != 0) {
    DAT_0051c2e8 = 0;
    DAT_0044de14 = 4;
  }
  if (DAT_00446f38 != -1) {
    DAT_0044de14 = 0x40;
  }
  return 1;
}

