/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00429800; function: UpdateOptionsOverlay; body bytes: 662
 * callers: 1; callees: 9; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdateOptionsOverlay(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  longlong lVar6;
  undefined4 uVar7;
  int aiStack_60 [3];
  int *piStack_54;
  int aiStack_50 [5];
  int *piStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  
  if ((DAT_0051c37c != 0) && (iVar2 = AnyManagedSoundPlaying(DAT_0044ddd8), iVar2 == 0)) {
    DAT_0051c2bc = iVar2;
    DAT_0051c37c = iVar2;
    ResetMouseBoundsToGameViewport();
    ResumeGlobalBinkMovie();
    DAT_00446cec = 0xffffffff;
    if (DAT_0051b36c != (int *)0x0) {
      SetCursorSurface(DAT_0051b36c);
    }
  }
  if (1 < DAT_0051c2bc) {
    DAT_0051c2bc = 0;
    ResetMouseBoundsToGameViewport();
    ResumeGlobalBinkMovie();
    return;
  }
  uStack_28 = 0;
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  uStack_2c = 0;
  uStack_30 = DAT_0051c298;
  uStack_34 = 0;
  uStack_38 = 0;
  aiStack_50[4] = 0x42988f;
  piStack_3c = piVar1;
  (**(code **)(*piVar1 + 0x1c))();
  aiStack_50[4] = 1;
  aiStack_50[3] = 0;
  aiStack_50[2] = DAT_0051c288;
  aiStack_50[1] = DAT_00519964;
  aiStack_50[0] = DAT_00519960;
  aiStack_60[2] = 0x4298ae;
  piStack_54 = piVar1;
  (**(code **)(*piVar1 + 0x1c))();
  iVar3 = DAT_005199ac;
  aiStack_50[2] = DAT_005199b4 - DAT_005199ac;
  aiStack_60[2] = 1;
  aiStack_60[1] = 0;
  _DAT_005199b0 = 0x96;
  _DAT_005199b8 = 0xb4;
  iVar2 = *piVar1;
  aiStack_60[0] = DAT_0051c28c;
  uVar7 = 0x96;
  lVar6 = __ftol();
  (**(code **)(iVar2 + 0x1c))(piVar1,(int)lVar6 + iVar3,uVar7);
  iVar2 = DAT_004fbd24;
  if ((DAT_004fbfb8 != 0) &&
     (iVar3 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,&DAT_005199ac), iVar2 = DAT_004fbd24,
     iVar3 != 0)) {
    aiStack_60[0] = DAT_005199b4 - DAT_005199ac;
    lVar6 = __ftol();
    DAT_00446ce0 = (int)lVar6;
    DAT_00446ce4 = (DAT_00446ce0 + -100) * 0x2a;
    if (DAT_0051bca4 != DAT_00446ce4) {
      ApplyGlobalGameVolume(DAT_00446ce4);
      iVar2 = DAT_004fbd24;
    }
  }
  DAT_0051bca4 = DAT_00446ce4;
  aiStack_60[2] = 0x114;
  piStack_54 = (int *)0xfd;
  iVar3 = -1;
  aiStack_60[0] = 0x114;
  aiStack_60[1] = 0xfd;
  aiStack_50[0] = 0x189;
  aiStack_50[1] = 0x138;
  iVar4 = 0;
  piVar5 = aiStack_50;
  while ((((iVar2 <= piVar5[-2] || (*piVar5 <= iVar2)) || (DAT_004fbd30 <= piVar5[-1])) ||
         (piVar5[1] <= DAT_004fbd30))) {
    iVar4 = iVar4 + 1;
    piVar5 = piVar5 + 4;
    if (0 < iVar4) {
LAB_00429a17:
      if ((DAT_004fbfb8 != 0) && (iVar3 != -1)) {
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,aiStack_60[iVar3 * 2],aiStack_60[iVar3 * 2 + 1],
                   *(undefined4 *)(&DAT_0051b3f4 + iVar3 * 8),0,0);
      }
      if (((DAT_004fbfb4 != 0) && (iVar3 != -1)) && (iVar3 == 0)) {
        StopAllManagedSounds(DAT_0044ddd8);
        PlayManagedSoundById(DAT_0044ddd8,0x35e,0x5a,1);
        *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x5de) * 4 + 0xd98) = 1;
        DAT_0051c37c = 1;
      }
      return;
    }
  }
  (**(code **)(*piVar1 + 0x1c))
            (piVar1,aiStack_60[iVar4 * 2],aiStack_60[iVar4 * 2 + 1],
             *(undefined4 *)(iVar4 * 8 + 0x51b3f0),0,0);
  iVar3 = iVar4;
  goto LAB_00429a17;
}

