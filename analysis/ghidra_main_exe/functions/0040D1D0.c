/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d1d0; function: ApplyParkDesignerSeason; body bytes: 258
 * callers: 2; callees: 0; success: True
 */


void __cdecl ApplyParkDesignerSeason(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  int iVar9;
  int *piVar10;
  undefined4 *puVar11;
  
  uVar6 = DAT_0050929c;
  uVar5 = DAT_00509298;
  uVar4 = DAT_00509294;
  uVar3 = DAT_00509290;
  uVar2 = DAT_0050928c;
  uVar1 = DAT_00509288;
  puVar7 = &DAT_00508450;
  if (param_1 == 0) {
    do {
      puVar7[-0x3c] = uVar1;
      *puVar7 = uVar2;
      puVar7[0x3c] = uVar3;
      puVar7 = puVar7 + 0xc;
    } while ((int)puVar7 < 0x508540);
    DAT_005092ac = DAT_005092b4;
    puVar7 = &DAT_00507b3c;
  }
  else {
    do {
      puVar7[-0x3c] = uVar4;
      *puVar7 = uVar5;
      puVar7[0x3c] = uVar6;
      puVar7 = puVar7 + 0xc;
    } while ((int)puVar7 < 0x508540);
    puVar7 = &DAT_004fca94;
    DAT_005092ac = DAT_005092b0;
  }
  DAT_00507b50 = *(undefined4 *)(&DAT_00507a44 + param_1 * 4);
  puVar11 = &DAT_00504178;
  for (iVar9 = 5; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar11 = *puVar7;
    puVar7 = puVar7 + 1;
    puVar11 = puVar11 + 1;
  }
  puVar7 = (undefined4 *)(&DAT_00507a4c + param_1 * 4);
  puVar11 = (undefined4 *)0x507e34;
  piVar10 = &DAT_00507b70;
  do {
    if (piVar10[2] == -1) {
      uVar1 = *puVar7;
      iVar9 = 5;
      puVar8 = &DAT_00507fa0 + (piVar10[1] + *piVar10 * 4) * 0x3c;
      do {
        *puVar8 = uVar1;
        puVar8 = puVar8 + 0xc;
        iVar9 = iVar9 + -1;
      } while (iVar9 != 0);
    }
    else {
      *puVar11 = *puVar7;
    }
    puVar11 = puVar11 + 1;
    piVar10 = piVar10 + 3;
    puVar7 = puVar7 + 2;
  } while ((int)puVar11 < 0x507e58);
  return;
}

