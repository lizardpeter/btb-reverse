/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041fb90; function: HitTestBobsBandRegions; body bytes: 208
 * callers: 1; callees: 0; success: True
 */


undefined4 HitTestBobsBandRegions(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (DAT_00512188 == 1) {
    iVar3 = DAT_004fbd30 + 10;
    iVar4 = DAT_004fbd24 + 10;
  }
  else {
    iVar3 = DAT_004fbd30 + 5;
    iVar4 = DAT_004fbd24 + 5;
  }
  iVar2 = 0;
  if (0 < DAT_00509378) {
    piVar1 = &DAT_00512790;
    do {
      if ((((piVar1[4] != -1) && (*piVar1 < iVar4)) && (iVar4 < piVar1[2])) &&
         ((piVar1[1] < iVar3 && (iVar3 < piVar1[3])))) {
        DAT_0050a5c0 = iVar2;
        if (DAT_00512188 == 1) {
          DAT_004fbd30 = iVar3 + -10;
          iVar3 = -10;
        }
        else {
          DAT_004fbd30 = iVar3 + -5;
          iVar3 = -5;
        }
        DAT_004fbd24 = iVar4 + iVar3;
        return (&DAT_005127a0)[iVar2 * 5];
      }
      iVar2 = iVar2 + 1;
      piVar1 = piVar1 + 5;
    } while (iVar2 < DAT_00509378);
  }
  if (DAT_00512188 == 1) {
    DAT_004fbd24 = iVar4 + -10;
    DAT_004fbd30 = iVar3 + -10;
    return 0xffffffff;
  }
  DAT_004fbd24 = iVar4 + -5;
  DAT_004fbd30 = iVar3 + -5;
  return 0xffffffff;
}

