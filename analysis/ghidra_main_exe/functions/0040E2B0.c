/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040e2b0; function: CompactParkDesignerObjectRecordFamilies; body bytes: 294
 * callers: 1; callees: 0; success: True
 */


void CompactParkDesignerObjectRecordFamilies(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int *piVar7;
  int *local_4;
  
  iVar3 = -1;
  iVar6 = 0;
  local_4 = &DAT_004fcacc;
  do {
    if (*local_4 == -1) {
      iVar4 = iVar6 + 1;
      iVar1 = DAT_00507a28;
      if (iVar4 < 100) {
        piVar5 = local_4 + 0x13;
LAB_0040e2d6:
        if (*piVar5 == -1) goto code_r0x0040e2db;
        piVar5 = &DAT_004fcab0 + iVar4 * 0x13;
        piVar2 = local_4 + -7;
        for (iVar3 = 0x13; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = *piVar5;
          piVar5 = piVar5 + 1;
          piVar2 = piVar2 + 1;
        }
        (&DAT_004fcacc)[iVar4 * 0x13] = 0xffffffff;
        local_4[6] = iVar6;
        iVar3 = iVar6;
        iVar1 = iVar6;
        if ((*local_4 != 100) && (iVar1 = DAT_00507a28, *local_4 == 7)) {
          DAT_005079fc = iVar6;
        }
      }
LAB_0040e330:
      DAT_00507a28 = iVar1;
      if (iVar4 == 100) break;
    }
    local_4 = local_4 + 0x13;
    iVar6 = iVar6 + 1;
  } while ((int)local_4 < 0x4fe87c);
  if (iVar3 != -1) {
    DAT_00509340 = iVar3 + 1;
  }
  iVar3 = -1;
  iVar6 = 300;
  piVar5 = &DAT_005023dc;
  do {
    if (*piVar5 == -1) {
      iVar4 = iVar6 + 1;
      if (iVar4 < 400) {
        piVar2 = piVar5 + 0x13;
LAB_0040e376:
        if (*piVar2 == -1) goto code_r0x0040e37b;
        piVar2 = &DAT_004fcab0 + iVar4 * 0x13;
        piVar7 = piVar5 + -7;
        for (iVar3 = 0x13; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar7 = *piVar2;
          piVar2 = piVar2 + 1;
          piVar7 = piVar7 + 1;
        }
        piVar5[6] = iVar6;
        (&DAT_004fcacc)[iVar4 * 0x13] = 0xffffffff;
        iVar3 = iVar6;
      }
LAB_0040e3b6:
      if (iVar4 == 400) break;
    }
    piVar5 = piVar5 + 0x13;
    iVar6 = iVar6 + 1;
  } while ((int)piVar5 < 0x50418c);
  if (iVar3 != -1) {
    DAT_00441dc8 = iVar3 + 1;
  }
  return;
code_r0x0040e2db:
  piVar5 = piVar5 + 0x13;
  iVar4 = iVar4 + 1;
  if (0x4fe87b < (int)piVar5) goto LAB_0040e330;
  goto LAB_0040e2d6;
code_r0x0040e37b:
  piVar2 = piVar2 + 0x13;
  iVar4 = iVar4 + 1;
  if (0x50418b < (int)piVar2) goto LAB_0040e3b6;
  goto LAB_0040e376;
}

