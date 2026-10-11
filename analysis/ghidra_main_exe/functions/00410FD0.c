/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00410fd0; function: CanPlaceFireworkGridItem; body bytes: 63
 * callers: 2; callees: 0; success: True
 */


undefined1 __cdecl CanPlaceFireworkGridItem(int param_1,int param_2,int param_3)

{
  undefined1 uVar1;
  int iVar2;
  int *piVar3;
  
  uVar1 = 1;
  iVar2 = 0;
  if (0 < *(int *)(&DAT_00442734 + param_3 * 4)) {
    piVar3 = &DAT_0050a678 + param_2 + param_1 * 6;
    while (*piVar3 == -1) {
      iVar2 = iVar2 + 1;
      piVar3 = piVar3 + 1;
      if (*(int *)(&DAT_00442734 + param_3 * 4) <= iVar2) {
        return uVar1;
      }
    }
    uVar1 = 0;
  }
  return uVar1;
}

