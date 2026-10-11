/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00429aa0; function: UpdateQuitConfirmationOverlay; body bytes: 485
 * callers: 1; callees: 6; success: True
 */


void UpdateQuitConfirmationOverlay(void)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  int *apiStack_60 [4];
  int aiStack_50 [5];
  
  if (DAT_0051c380 == 1) {
    aiStack_50[4] = 0x429abf;
    iVar2 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if (iVar2 == 0) {
      DAT_0044ddb0 = 1;
    }
  }
  if (1 < DAT_0051c2c0) {
    DAT_0051c2c0 = 0;
    aiStack_50[4] = 0x429ae0;
    ResetMouseBoundsToGameViewport();
    aiStack_50[4] = 0x429ae5;
    ResumeGlobalBinkMovie();
    return;
  }
  aiStack_50[4] = 0;
  aiStack_50[3] = 0;
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  aiStack_50[2] = DAT_0051c298;
  aiStack_50[1] = 0;
  aiStack_50[0] = 0;
  apiStack_60[2] = (int *)0x429b09;
  apiStack_60[3] = piVar1;
  (**(code **)(*piVar1 + 0x1c))();
  apiStack_60[2] = (int *)0x1;
  apiStack_60[1] = (int *)0x0;
  apiStack_60[0] = DAT_0051c290;
  (**(code **)(*piVar1 + 0x1c))(piVar1,DAT_00515bd8,DAT_00515bdc);
  if (DAT_0051c380 == 0) {
    aiStack_50[0] = 0xdc;
    aiStack_50[1] = 0x104;
    aiStack_50[3] = 0x13c;
    aiStack_50[4] = 0x14e;
    iVar2 = -1;
    apiStack_60[0] = (int *)0xdc;
    apiStack_60[1] = (int *)0x104;
    apiStack_60[2] = (int *)0x14e;
    apiStack_60[3] = (int *)0x104;
    aiStack_50[2] = 0x131;
    iVar5 = 0;
    piVar3 = aiStack_50 + 2;
    do {
      if ((((piVar3[-2] < DAT_004fbd24) && (DAT_004fbd24 < *piVar3)) && (piVar3[-1] < DAT_004fbd30))
         && (DAT_004fbd30 < piVar3[1])) {
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,apiStack_60[iVar5 * 2],apiStack_60[iVar5 * 2 + 1],
                   *(undefined4 *)(iVar5 * 8 + 0x51b3e0),0,0);
        iVar2 = iVar5;
        if (DAT_00446fe8 != iVar5) {
          PlayManagedSoundById(DAT_0044ddd8,iVar5 + 0x1c5,0x32,2);
        }
        break;
      }
      iVar5 = iVar5 + 1;
      piVar3 = piVar3 + 4;
    } while (iVar5 < 2);
    DAT_00446fe8 = iVar2;
    if ((DAT_004fbfb8 != 0) && (iVar2 != -1)) {
      (**(code **)(*piVar1 + 0x1c))
                (piVar1,apiStack_60[iVar2 * 2],apiStack_60[iVar2 * 2 + 1],
                 *(undefined4 *)(&DAT_0051b3e4 + iVar2 * 8),0,0);
    }
    if ((DAT_004fbfb4 != 0) && (iVar2 != -1)) {
      if (iVar2 == 1) {
        DAT_0051c2c0 = 0;
        return;
      }
      if (iVar2 == 0) {
        DAT_0051c380 = 1;
        StopAllManagedSounds(DAT_0044ddd8);
        iVar2 = 1;
        uVar6 = 0x5a;
        uVar4 = FUN_0042ffc4();
        uVar4 = uVar4 & 0x80000003;
        if ((int)uVar4 < 0) {
          uVar4 = (uVar4 - 1 | 0xfffffffc) + 1;
        }
        PlayManagedSoundById(DAT_0044ddd8,uVar4 + 0x272,uVar6,iVar2);
      }
    }
  }
  return;
}

