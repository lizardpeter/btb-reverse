/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00411010; function: PlaceFireworkGridItem; body bytes: 109
 * callers: 2; callees: 1; success: True
 */


char __cdecl PlaceFireworkGridItem(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  char cVar2;
  int iVar3;
  
  cVar2 = CanPlaceFireworkGridItem(param_1,param_2,param_3);
  if (cVar2 != '\0') {
    iVar1 = *(int *)(&DAT_00442734 + param_3 * 4);
    if (param_4 != 0) {
      (&DAT_0050a678)[param_2 + param_1 * 6] = param_3;
    }
    if (1 < iVar1) {
      iVar3 = 1;
      do {
        if (param_4 != 0) {
          (&DAT_0050a678)[iVar3 + param_1 * 6 + param_2] = param_3 + 0xe;
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 < iVar1);
    }
    return '\x01';
  }
  return '\0';
}

