/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409990; function: DrawDinoActivity; body bytes: 708
 * callers: 1; callees: 1; success: True
 */


void DrawDinoActivity(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int *piStack_70;
  undefined4 uStack_6c;
  int *piStack_68;
  undefined4 uStack_64;
  int aiStack_60 [4];
  int aiStack_30 [12];
  
  piVar5 = *(int **)(DAT_0044de08 + 0xc);
  aiStack_60[3] = 0;
  aiStack_60[2] = 0;
  aiStack_60[1] = DAT_004fc9e8;
  aiStack_60[0] = 0;
  uStack_64 = 0;
  uStack_6c = 0x4099b2;
  piStack_68 = piVar5;
  (**(code **)(*piVar5 + 0x1c))();
  iVar2 = 0;
  aiStack_30[0] = 100;
  aiStack_30[1] = 0x14;
  aiStack_30[2] = 0x3c;
  aiStack_30[3] = 0x23;
  aiStack_30[4] = 0x4b;
  aiStack_30[5] = 0x83;
  do {
    iVar4 = *(int *)((int)&DAT_004fca74 + iVar2) + 1;
    *(int *)((int)&DAT_004fca74 + iVar2) = iVar4;
    if (6 < iVar4) {
      *(undefined4 *)((int)&DAT_004fca74 + iVar2) = 0;
      *(int *)((int)&DAT_004fca6c + iVar2) = *(int *)((int)&DAT_004fca6c + iVar2) + 1;
    }
    iVar4 = *(int *)((int)&DAT_0043f7f8 + iVar2);
    if (iVar4 != -1) {
      uVar1 = *(undefined4 *)(&stack0xffffffb8 + iVar4 * 4);
      *(int *)((int)&DAT_004fca7c + iVar2) = iVar4;
      *(undefined4 *)((int)&DAT_0043f7f8 + iVar2) = 0xffffffff;
      *(undefined4 *)((int)&DAT_004fca6c + iVar2) = uVar1;
    }
    if ((*(int *)((int)&DAT_004fca6c + iVar2) <
         *(int *)(&stack0xffffffb8 + *(int *)((int)&DAT_004fca7c + iVar2) * 4)) ||
       (aiStack_30[*(int *)((int)&DAT_004fca7c + iVar2)] <= *(int *)((int)&DAT_004fca6c + iVar2))) {
      if (*(int *)((int)&DAT_0043f7f8 + iVar2) == -1) {
        *(undefined4 *)((int)&DAT_004fca7c + iVar2) = 0;
      }
      else {
        *(int *)((int)&DAT_004fca7c + iVar2) = *(int *)((int)&DAT_0043f7f8 + iVar2);
        *(undefined4 *)((int)&DAT_0043f7f8 + iVar2) = 0xffffffff;
      }
      *(undefined4 *)((int)&DAT_004fca6c + iVar2) =
           *(undefined4 *)(&stack0xffffffb8 + *(int *)((int)&DAT_004fca7c + iVar2) * 4);
    }
    iVar2 = iVar2 + 4;
  } while (iVar2 < 8);
  piStack_70 = aiStack_60 + 2;
  uStack_6c = 1;
  aiStack_60[2] = DAT_004fca6c * 0x71;
  aiStack_60[3] = 0;
  (**(code **)(*piVar5 + 0x1c))
            (piVar5,*(undefined4 *)(&stack0xffffffb8 + DAT_004fc43c * 8),
             *(undefined4 *)(&stack0xffffffbc + DAT_004fc43c * 8),DAT_004fc2ac);
  aiStack_60[0] = 0x16e;
  piStack_70 = (int *)(DAT_004fca70 * 0x5d);
  aiStack_60[1] = 0x14;
  piStack_68 = (int *)((int)piStack_70 + 0x5d);
  aiStack_60[2] = 0x19e;
  aiStack_60[3] = 0x15;
  uStack_6c = 0;
  uStack_64 = 0x69;
  (**(code **)(*piVar5 + 0x1c))
            (piVar5,aiStack_60[DAT_004fc43c * 2],aiStack_60[DAT_004fc43c * 2 + 1],DAT_004fc434,
             &piStack_70,1);
  iVar2 = 0;
  if (0 < DAT_0043ee8c) {
    piVar5 = &DAT_004fc45c;
    do {
      if (piVar5[-1] == 4) {
        BlitColorKeyedSurfaceClipped((int *)piVar5[6],piVar5[-5],piVar5[-4],piVar5);
      }
      iVar2 = iVar2 + 1;
      piVar5 = piVar5 + 0xc;
    } while (iVar2 < DAT_0043ee8c);
  }
  iVar2 = 0;
  if (0 < DAT_0043ee8c) {
    piVar5 = &DAT_004fc458;
    do {
      if (piVar5[5] == 2) {
        iVar4 = *(int *)(&DAT_0043eeb4 + DAT_004fc424 * 4) + DAT_004fc404;
        iVar3 = *(int *)(&DAT_0043ee94 + DAT_004fc424 * 4) + DAT_004fc400;
        piVar6 = (int *)piVar5[7];
LAB_00409c37:
        BlitColorKeyedSurfaceClipped(piVar6,iVar3,iVar4,piVar5 + 1);
      }
      else {
        iVar4 = *piVar5;
        if (((((iVar4 == 0) || (iVar4 == 1)) || (iVar4 == 2)) || (iVar4 == 3)) && (piVar5[5] != 0))
        {
          iVar4 = piVar5[-1];
          iVar3 = piVar5[-2];
          piVar6 = (int *)piVar5[7];
          goto LAB_00409c37;
        }
      }
      iVar2 = iVar2 + 1;
      piVar5 = piVar5 + 0xc;
    } while (iVar2 < DAT_0043ee8c);
  }
  return;
}

