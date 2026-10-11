/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041f3a0; function: CanPlaceGrandOpeningEvent; body bytes: 74
 * callers: 2; callees: 0; success: True
 */


undefined1 __cdecl CanPlaceGrandOpeningEvent(int param_1,int param_2,int param_3)

{
  undefined1 uVar1;
  int iVar2;
  int *piVar3;
  
  iVar2 = 0;
  uVar1 = 1;
  if (0 < (int)(&DAT_00444be8)[param_3]) {
    piVar3 = &DAT_005123b8 + param_2 + param_1 * 0x18;
    while ((*piVar3 == -1 && (iVar2 + param_2 < 0x18))) {
      iVar2 = iVar2 + 1;
      piVar3 = piVar3 + 1;
      if ((int)(&DAT_00444be8)[param_3] <= iVar2) {
        return uVar1;
      }
    }
    uVar1 = 0;
  }
  return uVar1;
}

