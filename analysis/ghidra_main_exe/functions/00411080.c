/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00411080; function: RemoveFireworkGridItem; body bytes: 86
 * callers: 2; callees: 1; success: True
 */


int __cdecl RemoveFireworkGridItem(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  undefined4 *puVar5;
  
  cVar3 = CanPlaceFireworkGridItem(param_1,param_2,0);
  if (cVar3 != '\0') {
    return -1;
  }
  iVar1 = param_2 + param_1 * 6;
  iVar2 = (&DAT_0050a678)[iVar1];
  (&DAT_0050a678)[iVar1] = 0xffffffff;
  iVar4 = *(int *)(&DAT_00442734 + iVar2 * 4);
  if (1 < iVar4) {
    puVar5 = &DAT_0050a67c + iVar1;
    while (iVar4 = iVar4 + -1, iVar4 != 0) {
      *puVar5 = 0xffffffff;
      puVar5 = puVar5 + 1;
    }
  }
  return iVar2;
}

