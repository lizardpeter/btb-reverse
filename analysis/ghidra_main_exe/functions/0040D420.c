/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d420; function: HitTestParkDesignerToolbar; body bytes: 69
 * callers: 1; callees: 0; success: True
 */


int HitTestParkDesignerToolbar(void)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  
  iVar2 = 0;
  if (DAT_00441cdc != -1) {
    piVar3 = &DAT_00441ce0;
    do {
      if ((((piVar3[-2] < DAT_004fbd24) && (DAT_004fbd24 < *piVar3)) && (piVar3[-1] < DAT_004fbd30))
         && (DAT_004fbd30 < piVar3[1])) {
        return iVar2;
      }
      piVar1 = piVar3 + 3;
      piVar3 = piVar3 + 4;
      iVar2 = iVar2 + 1;
    } while (*piVar1 != -1);
  }
  return -1;
}

