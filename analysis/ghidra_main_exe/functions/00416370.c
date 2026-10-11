/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00416370; function: DrawHerdingActivity; body bytes: 1573
 * callers: 1; callees: 3; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DrawHerdingActivity(void)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  int *piVar7;
  longlong lVar8;
  int iVar9;
  int *piVar10;
  int iStack_58;
  int *local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  int aiStack_38 [14];
  
  local_54 = *(int **)(DAT_0044de08 + 0xc);
  if (DAT_0050b3b4 < 300) {
    DAT_0051071c = 0;
  }
  else {
    DAT_0051071c = 0x2a0;
    if (DAT_0050b3b4 < 0x3cd) {
      DAT_0051071c = DAT_0050b3b4 + -300;
    }
  }
  if (DAT_0050b3b8 + 0x50 < 0xbe) {
    DAT_00510720 = 0;
  }
  else {
    DAT_00510720 = 0x1f6;
    if (DAT_0050b3b8 + 0x50 < 0x2b5) {
      DAT_00510720 = DAT_0050b3b8 + -0x6e;
    }
  }
  local_44 = DAT_00510720 + 0x17c;
  piVar10 = &local_50;
  local_48 = DAT_0051071c + 600;
  local_50 = DAT_0051071c;
  local_4c = DAT_00510720;
  (**(code **)(*local_54 + 0x1c))(local_54,0x14,0x14,DAT_00510724,piVar10,0);
  puVar3 = DAT_00510760;
  if (0 < (int)DAT_00510760) {
    puVar5 = &DAT_0050afdc;
    puVar4 = &DAT_0050b3a8;
    puVar6 = DAT_00510760;
    do {
      *puVar5 = puVar4;
      puVar4 = puVar4 + 0x19;
      puVar5 = puVar5 + 1;
      puVar6 = (undefined4 *)((int)puVar6 + -1);
    } while (puVar6 != (undefined4 *)0x0);
  }
  FUN_00430b5c(&DAT_0050afdc,puVar3,4,CompareHerdingEntitiesByDepth);
  iVar9 = 0;
  if (0 < (int)DAT_00510760) {
    piVar7 = &DAT_0050afdc;
    do {
      iVar1 = *piVar7;
      iVar2 = *(int *)(iVar1 + 0x30);
      if ((iVar2 != 0x11) && (*(int *)(iVar1 + 8) != -1)) {
        if (iVar2 == 0) {
          iStack_58 = 0;
          local_54 = (int *)0x1;
          local_50 = 0;
          local_4c = 1;
          local_48 = 0;
          local_44 = 1;
          uStack_40 = 0;
          uStack_3c = 1;
          aiStack_38[6] = 3;
          aiStack_38[7] = 3;
          aiStack_38[0] = 0;
          DAT_0050b3dc = ((&iStack_58)[DAT_0050b3b0] * 0x27 + DAT_0050b3ac) * DAT_004439ac;
          DAT_0050b3e4 = DAT_0050b3dc + DAT_004439ac;
          aiStack_38[1] = 0;
          aiStack_38[2] = 1;
          aiStack_38[3] = 1;
          aiStack_38[4] = 2;
          aiStack_38[5] = 2;
          if (DAT_0050b3b0 < 2) {
            DAT_0050b3e0 = 0;
          }
          else {
            DAT_0050b3e0 = aiStack_38[DAT_0050b3b0] * DAT_004439b0;
          }
          DAT_0050b3e8 = DAT_0050b3e0 + DAT_004439b0;
          DAT_0050b3ec = DAT_00510728;
          _DAT_00510598 = (DAT_004439ac - DAT_0051071c) + DAT_0050b3b4;
          _DAT_00510590 = DAT_0050b3b4 - DAT_0051071c;
          _DAT_00510594 = DAT_0050b3b8 - DAT_00510720;
          _DAT_0051059c = (DAT_004439b0 - DAT_00510720) + DAT_0050b3b8;
        }
        else if (iVar2 == 3) {
          if (*(int *)(iVar1 + 0x50) == 0) {
            lVar8 = __ftol();
            *(int *)(iVar1 + 0x2c) = *(int *)(iVar1 + 0x2c) + (int)lVar8;
            if (*(int *)(*piVar7 + 0x2c) < 1) {
              *(undefined4 *)(*piVar7 + 0x2c) = 100;
              *(int *)(*piVar7 + 4) = *(int *)(*piVar7 + 4) + 1;
              *(undefined4 *)(*piVar7 + 0x54) = 1;
              if (5 < *(int *)(*piVar7 + 4)) {
                *(undefined4 *)(*piVar7 + 4) = 0;
              }
            }
          }
          iVar1 = *piVar7;
          *(int *)(iVar1 + 0x34) = (*(int *)(iVar1 + 4) + *(int *)(iVar1 + 8) * 0xc) * 0x35;
          *(int *)(*piVar7 + 0x3c) = *(int *)(*piVar7 + 0x34) + 0x35;
          *(undefined4 *)(*piVar7 + 0x38) = 0;
          *(undefined4 *)(*piVar7 + 0x40) = 0x32;
        }
        else if (iVar2 == 2) {
          if (*(int *)(iVar1 + 0x50) == 0) {
            lVar8 = __ftol();
            *(int *)(iVar1 + 0x2c) = *(int *)(iVar1 + 0x2c) + (int)lVar8;
            if (*(int *)(*piVar7 + 0x2c) < 1) {
              *(undefined4 *)(*piVar7 + 0x2c) = 100;
              *(undefined4 *)(*piVar7 + 0x54) = 1;
              *(int *)(*piVar7 + 4) = *(int *)(*piVar7 + 4) + 1;
              if (6 < *(int *)(*piVar7 + 4)) {
                *(undefined4 *)(*piVar7 + 4) = 3;
              }
            }
          }
          iVar1 = *piVar7;
          *(int *)(iVar1 + 0x34) = (*(int *)(iVar1 + 4) + *(int *)(iVar1 + 8) * 10) * 0x49;
          *(int *)(*piVar7 + 0x3c) = *(int *)(*piVar7 + 0x34) + 0x49;
          *(undefined4 *)(*piVar7 + 0x38) = 0;
          *(undefined4 *)(*piVar7 + 0x40) = 0x45;
        }
        else if (iVar2 == 1) {
          if (*(int *)(iVar1 + 0x50) == 0) {
            lVar8 = __ftol();
            *(int *)(iVar1 + 0x2c) = *(int *)(iVar1 + 0x2c) + (int)lVar8;
            if (*(int *)(*piVar7 + 0x2c) < 1) {
              *(undefined4 *)(*piVar7 + 0x2c) = 100;
              *(undefined4 *)(*piVar7 + 0x54) = 1;
              *(int *)(*piVar7 + 4) = *(int *)(*piVar7 + 4) + 1;
              if (9 < *(int *)(*piVar7 + 4)) {
                *(undefined4 *)(*piVar7 + 4) = 6;
              }
            }
          }
          iVar1 = *piVar7;
          *(int *)(iVar1 + 0x34) = (*(int *)(iVar1 + 4) + *(int *)(iVar1 + 8) * 10) * 0x62;
          *(int *)(*piVar7 + 0x3c) = *(int *)(*piVar7 + 0x34) + 0x62;
          *(undefined4 *)(*piVar7 + 0x38) = 0;
          *(undefined4 *)(*piVar7 + 0x40) = 0x67;
        }
        else if (iVar2 == 7) {
          *(int *)(iVar1 + 0x34) = (*(int *)(iVar1 + 8) * 7 + *(int *)(iVar1 + 4)) * 0x56;
          *(int *)(*piVar7 + 0x3c) = *(int *)(*piVar7 + 0x34) + 0x56;
          *(undefined4 *)(*piVar7 + 0x38) = 0;
          *(undefined4 *)(*piVar7 + 0x40) = 0x57;
          iVar1 = *piVar7;
          _DAT_00510618 = *(int *)(iVar1 + 0xc) - DAT_0051071c;
          _DAT_00510620 = (*(int *)(iVar1 + 0xc) - DAT_0051071c) + 0x56;
          _DAT_0051061c = *(int *)(iVar1 + 0x10) - DAT_00510720;
          _DAT_00510624 = (*(int *)(iVar1 + 0x10) - DAT_00510720) + 0x57;
        }
        else if (iVar2 == 0xd) {
          *(int *)(iVar1 + 0x2c) = *(int *)(iVar1 + 0x2c) + -1;
          if (*(int *)(*piVar7 + 0x2c) < 1) {
            *(undefined4 *)(*piVar7 + 0x2c) = 10;
            *(int *)(*piVar7 + 4) = *(int *)(*piVar7 + 4) + 1;
            if (0xb < *(int *)(*piVar7 + 4)) {
              *(undefined4 *)(*piVar7 + 4) = 0;
            }
          }
          *(int *)(*piVar7 + 0x34) = *(int *)(*piVar7 + 4) * 0xe6;
          *(undefined4 *)(*piVar7 + 0x38) = 0;
          *(int *)(*piVar7 + 0x3c) = *(int *)(*piVar7 + 0x34) + 0xe6;
          *(undefined4 *)(*piVar7 + 0x40) = 0xf0;
        }
        else if (iVar2 == 0xc) {
          *(undefined4 *)(iVar1 + 0x34) = 0;
          *(undefined4 *)(*piVar7 + 0x38) = 0;
          *(undefined4 *)(*piVar7 + 0x3c) = 0x97;
          *(undefined4 *)(*piVar7 + 0x40) = 0xf0;
        }
        else if (iVar2 == 0xe) {
          *(undefined4 *)(iVar1 + 0x34) = 0;
          *(undefined4 *)(*piVar7 + 0x38) = 0;
          *(undefined4 *)(*piVar7 + 0x3c) = 0x2b;
          *(undefined4 *)(*piVar7 + 0x40) = 0x4c;
          *(undefined4 *)(*piVar7 + 0xc) = 0x17c;
          *(undefined4 *)(*piVar7 + 0x10) = 0x16c;
        }
        else {
          if (iVar2 == 0xf) {
            if (*(int *)(iVar1 + 0x2c) != -1) {
              *(int *)(iVar1 + 0x2c) = *(int *)(iVar1 + 0x2c) + -1;
              iVar1 = *piVar7;
              if (*(int *)(iVar1 + 0x2c) == 0) {
                *(int *)(iVar1 + 4) = *(int *)(iVar1 + 4) + 1;
                if (*(int *)(*piVar7 + 4) < 3) {
                  *(undefined4 *)(*piVar7 + 0x2c) = 0x32;
                }
              }
            }
            *(undefined4 *)(*piVar7 + 0xc) = 0x2cd;
            *(undefined4 *)(*piVar7 + 0x10) = 0xa2;
            *(int *)(*piVar7 + 0x34) = *(int *)(*piVar7 + 4) * 0x78;
            *(undefined4 *)(*piVar7 + 0x38) = 0;
          }
          else {
            if (iVar2 != 0x10) goto LAB_004168ec;
            if (*(int *)(iVar1 + 0x2c) != -1) {
              *(int *)(iVar1 + 0x2c) = *(int *)(iVar1 + 0x2c) + -1;
              iVar1 = *piVar7;
              if (*(int *)(iVar1 + 0x2c) == 0) {
                *(int *)(iVar1 + 4) = *(int *)(iVar1 + 4) + 1;
                if (*(int *)(*piVar7 + 4) < 3) {
                  *(undefined4 *)(*piVar7 + 0x2c) = 0x32;
                }
              }
            }
            *(undefined4 *)(*piVar7 + 0xc) = 0x35b;
            *(undefined4 *)(*piVar7 + 0x10) = 0x65;
            *(int *)(*piVar7 + 0x34) = *(int *)(*piVar7 + 4) * 0x78;
            *(undefined4 *)(*piVar7 + 0x38) = 0;
          }
          *(int *)(*piVar7 + 0x3c) = *(int *)(*piVar7 + 0x34) + 0x78;
          *(undefined4 *)(*piVar7 + 0x40) = 0x46;
        }
LAB_004168ec:
        iVar1 = *piVar7;
        BlitColorKeyedSurfaceClipped
                  (*(int **)(iVar1 + 0x44),*(int *)(iVar1 + 0xc),*(int *)(iVar1 + 0x10),
                   (int *)(iVar1 + 0x34));
      }
      iVar9 = iVar9 + 1;
      piVar7 = piVar7 + 1;
    } while (iVar9 < (int)DAT_00510760);
  }
  piVar7 = &DAT_00443ab0;
  iVar9 = 0;
  do {
    if (*(int *)((int)&DAT_00443aa4 + iVar9) == 1) {
      BlitColorKeyedSurfaceClipped
                (*(int **)((int)&DAT_0050af64 + iVar9),*piVar7,piVar7[1],(int *)0x0);
    }
    piVar7 = piVar7 + 2;
    iVar9 = iVar9 + 4;
  } while ((int)piVar7 < 0x443ac8);
  (**(code **)(*piVar10 + 0x1c))(piVar10,0,0,DAT_00510748,0,1);
  if (DAT_00510718 != -1) {
    (**(code **)(*piVar10 + 0x1c))(piVar10,0x11c,0x1a1,(&DAT_005105a0)[DAT_00510718],0,1);
  }
  return;
}

