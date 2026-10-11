/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00426f30; function: GenerateSquirrelConveyorChoices; body bytes: 819
 * callers: 2; callees: 1; success: True
 */


void GenerateSquirrelConveyorChoices(void)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  int *piVar6;
  int *piVar7;
  int *piVar8;
  bool bVar9;
  int *local_88;
  int *local_84;
  int local_80;
  int local_7c;
  int *local_78;
  int local_74;
  int *local_70;
  int *local_6c;
  int *local_68;
  int local_60 [4];
  int local_50 [4];
  int local_40 [8];
  int local_20 [8];
  
  local_84 = (int *)0xffffffff;
  if (DAT_005150cc == 0) {
    local_7c = 3;
    local_80 = 0;
  }
  else if (DAT_005150cc == 1) {
    local_7c = 3;
    local_80 = DAT_005150cc;
  }
  else if (DAT_005150cc == 2) {
    local_7c = 2;
    local_80 = 2;
  }
  iVar2 = *(int *)(&DAT_00515078 + DAT_005150d0 * 4);
  if (iVar2 == 1) {
    DAT_00514f00 = (&DAT_00514f10)[DAT_005150d0 * 4];
    local_84 = *(int **)(&DAT_00514f14 + DAT_005150d0 * 0x10);
  }
  if (iVar2 == 0) {
    DAT_00514f00 = (&DAT_00514f0c)[DAT_005150d0 * 4];
    local_84 = (int *)(&DAT_00514f10)[DAT_005150d0 * 4];
  }
  else if (iVar2 == 2) {
    DAT_00514f00 = *(int *)(&DAT_00514f14 + DAT_005150d0 * 0x10);
    local_84 = (int *)(&DAT_00514f18)[DAT_005150d0 * 4];
  }
  iVar2 = DAT_00514f00;
  if (0 < local_7c) {
    piVar6 = local_50;
    piVar8 = local_40 + 1;
    piVar7 = local_40;
    local_88 = (int *)local_7c;
    do {
      if (local_84 == (int *)0xffffffff) {
        uVar1 = FUN_0042ffc4();
        *piVar7 = DAT_00514f00;
        *piVar8 = (int)uVar1 % 3;
        iVar4 = (int)uVar1 % 3 + DAT_00514f00 * 3;
        iVar2 = DAT_00514f00;
      }
      else {
        iVar4 = (int)local_84 + iVar2 * 3;
        *piVar7 = iVar2;
        *piVar8 = (int)local_84;
        DAT_00514f54 = local_84;
      }
      piVar8 = piVar8 + 2;
      piVar7 = piVar7 + 2;
      *piVar6 = iVar4;
      piVar6 = piVar6 + 1;
      local_88 = (int *)((int)local_88 + -1);
    } while (local_88 != (int *)0x0);
  }
  iVar4 = 0;
  local_88 = (int *)0x0;
  if (0 < local_80) {
    local_78 = local_60;
    piVar6 = local_20 + 1;
    do {
      iVar3 = iVar2;
      if (iVar4 == 0) {
        do {
          uVar1 = FUN_0042ffc4();
          iVar2 = (int)uVar1 % 3;
          local_88 = (int *)iVar2;
        } while (iVar2 == DAT_00514f00);
      }
      else {
        while ((iVar2 == iVar3 || ((int *)iVar2 == local_88))) {
          uVar1 = FUN_0042ffc4();
          iVar3 = DAT_00514f00;
          iVar2 = (int)uVar1 % 3;
        }
      }
      do {
        uVar1 = FUN_0042ffc4();
        iVar3 = (int)uVar1 % 3;
      } while ((int *)iVar3 == local_84);
      iVar2 = iVar3 + iVar2 * 3;
      piVar6[-1] = iVar2;
      *piVar6 = iVar3;
      *local_78 = iVar2;
      local_78 = local_78 + 1;
      iVar4 = iVar4 + 1;
      iVar2 = DAT_00514f00;
      piVar6 = piVar6 + 2;
    } while (iVar4 < local_80);
  }
  uVar1 = FUN_0042ffc4();
  iVar2 = 0;
  DAT_00515098 = (int)uVar1 % 7;
  local_74 = 0;
  local_78 = (int *)0x0;
  if (0 < local_80 + local_7c) {
    local_68 = local_20 + 1;
    local_88 = local_60;
    local_6c = local_40 + 1;
    local_70 = local_50;
    local_84 = &DAT_00514ebc;
    piVar6 = &DAT_00514eb8;
    puVar5 = &DAT_00514ff0;
    piVar8 = &DAT_00515088;
    do {
      uVar1 = FUN_0042ffc4();
      iVar4 = DAT_00515098;
      uVar1 = uVar1 & 0x80000001;
      bVar9 = uVar1 == 0;
      if ((int)uVar1 < 0) {
        bVar9 = (uVar1 - 1 | 0xfffffffe) == 0xffffffff;
      }
      if (bVar9) {
        if (local_74 < local_7c) {
          *puVar5 = 1;
          iVar3 = local_6c[-1];
          *piVar8 = *(int *)(&DAT_004469ac + (iVar2 + iVar4 * 4) * 4) * 9 + *local_70;
          iVar4 = *local_6c;
          *piVar6 = iVar3;
          *local_84 = iVar4;
          local_74 = local_74 + 1;
          local_70 = local_70 + 1;
          local_6c = local_6c + 2;
LAB_0042723d:
          iVar2 = iVar2 + 1;
          piVar8 = piVar8 + 1;
          puVar5 = puVar5 + 1;
          piVar6 = piVar6 + 2;
          local_84 = local_84 + 2;
        }
      }
      else if ((int)local_78 < local_80) {
        *puVar5 = 0;
        *piVar8 = *(int *)(&DAT_004469ac + (iVar2 + iVar4 * 4) * 4) * 9 + *local_88;
        *piVar6 = local_68[-1];
        *local_84 = *local_68;
        local_78 = (int *)((int)local_78 + 1);
        local_88 = local_88 + 1;
        local_68 = local_68 + 2;
        goto LAB_0042723d;
      }
    } while (iVar2 < local_80 + local_7c);
  }
  return;
}

