/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d7d0; function: DrawParkDesignerSegmentedPondSurface; body bytes: 158
 * callers: 1; callees: 1; success: True
 */


void __cdecl DrawParkDesignerSegmentedPondSurface(int param_1)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int local_10 [4];
  
  iVar2 = DAT_00507e38 * 4;
  if (0 < *(int *)(&DAT_00441844 + iVar2)) {
    iVar3 = 1;
    do {
      local_10[0] = iVar3 * *(int *)(&DAT_00441858 + iVar2);
      local_10[2] = (iVar3 + 1) * *(int *)(&DAT_00441858 + iVar2);
      if (0x805 < local_10[2]) {
        local_10[2] = 0x805;
      }
      local_10[3] = *(int *)(&DAT_00441894 + iVar2);
      local_10[1] = 0;
      BlitColorKeyedSurfaceClipped
                (*(int **)((int)&DAT_00504178 + iVar2),*(int *)(&DAT_00508c10)[param_1],
                 ((int *)(&DAT_00508c10)[param_1])[1],local_10);
      iVar2 = DAT_00507e38 * 4;
      bVar1 = iVar3 < *(int *)(&DAT_00441844 + iVar2);
      iVar3 = iVar3 + 1;
    } while (bVar1);
  }
  return;
}

