/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00429fe0; function: UpdateLeaveActivityConfirmation; body bytes: 433
 * callers: 1; callees: 3; success: True
 */


void UpdateLeaveActivityConfirmation(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *apiStack_60 [4];
  int aiStack_50 [5];
  
  if (1 < DAT_0051c2c8) {
    DAT_0051c2c8 = 0;
    ResetMouseBoundsToGameViewport();
    ResumeGlobalBinkMovie();
    return;
  }
  aiStack_50[4] = 0;
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  aiStack_50[3] = 0;
  aiStack_50[2] = DAT_0051c298;
  aiStack_50[1] = 0;
  aiStack_50[0] = 0;
  apiStack_60[2] = (int *)0x42a027;
  apiStack_60[3] = piVar1;
  (**(code **)(*piVar1 + 0x1c))();
  apiStack_60[2] = (int *)0x1;
  apiStack_60[1] = (int *)0x0;
  apiStack_60[0] = DAT_0051c290;
  (**(code **)(*piVar1 + 0x1c))(piVar1,DAT_00515bd8,DAT_00515bdc);
  aiStack_50[0] = 0xdc;
  aiStack_50[1] = 0x104;
  aiStack_50[3] = 0x13c;
  aiStack_50[4] = 0x14e;
  iVar3 = -1;
  apiStack_60[0] = (int *)0xdc;
  apiStack_60[1] = (int *)0x104;
  apiStack_60[2] = (int *)0x14e;
  apiStack_60[3] = (int *)0x104;
  aiStack_50[2] = 0x131;
  iVar4 = 0;
  piVar2 = aiStack_50 + 2;
  while ((((DAT_004fbd24 <= piVar2[-2] || (*piVar2 <= DAT_004fbd24)) || (DAT_004fbd30 <= piVar2[-1])
          ) || (piVar2[1] <= DAT_004fbd30))) {
    iVar4 = iVar4 + 1;
    piVar2 = piVar2 + 4;
    if (1 < iVar4) {
LAB_0042a101:
      DAT_00446ff0 = iVar3;
      if ((DAT_004fbfb8 != 0) && (iVar3 != -1)) {
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,apiStack_60[iVar3 * 2],apiStack_60[iVar3 * 2 + 1],
                   *(undefined4 *)(&DAT_0051b3e4 + iVar3 * 8),0,0);
      }
      if ((DAT_004fbfb4 != 0) && (iVar3 != -1)) {
        if (iVar3 == 1) {
          DAT_0051c2c8 = 0;
          ResumeGlobalBinkMovie();
          ResetMouseBoundsToGameViewport();
          DAT_00446cec = 0xffffffff;
          return;
        }
        if (iVar3 == 0) {
          DAT_0051c2c8 = iVar3;
          ResetMouseBoundsToGameViewport();
          ResumeGlobalBinkMovie();
          DAT_0051c2e8 = 1;
          DAT_00446cec = 0xffffffff;
        }
      }
      return;
    }
  }
  (**(code **)(*piVar1 + 0x1c))
            (piVar1,apiStack_60[iVar4 * 2],apiStack_60[iVar4 * 2 + 1],
             *(undefined4 *)(iVar4 * 8 + 0x51b3e0),0,0);
  iVar3 = iVar4;
  if (DAT_00446ff0 != iVar4) {
    PlayManagedSoundById(DAT_0044ddd8,iVar4 + 0x1c5,0x32,2);
  }
  goto LAB_0042a101;
}

