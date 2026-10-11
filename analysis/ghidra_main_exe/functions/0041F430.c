/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041f430; function: RemoveGrandOpeningEventAtCell; body bytes: 122
 * callers: 2; callees: 1; success: True
 */


int __cdecl RemoveGrandOpeningEventAtCell(int param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  
  cVar3 = CanPlaceGrandOpeningEvent(param_1,param_2,0);
  if (cVar3 == '\0') {
    iVar2 = param_1 * 0x18 + param_2;
    puVar6 = &DAT_005123b8 + iVar2;
    iVar2 = (&DAT_005123b8)[iVar2];
    while (9 < iVar2) {
      piVar1 = puVar6 + -1;
      puVar6 = puVar6 + -1;
      param_2 = param_2 + -1;
      iVar2 = *piVar1;
    }
    iVar4 = param_1 * 0x18 + param_2;
    iVar2 = (&DAT_005123b8)[iVar4];
    (&DAT_005123b8)[iVar4] = 0xffffffff;
    iVar5 = (&DAT_00444be8)[iVar2];
    if (1 < iVar5) {
      puVar6 = &DAT_005123bc + iVar4;
      while (iVar5 = iVar5 + -1, iVar5 != 0) {
        *puVar6 = 0xffffffff;
        puVar6 = puVar6 + 1;
      }
    }
    return iVar2;
  }
  return -1;
}

