/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d870; function: DrawParkDesignerPondSurfaceSegment; body bytes: 140
 * callers: 1; callees: 1; success: True
 */


void __cdecl DrawParkDesignerPondSurfaceSegment(int param_1)

{
  int *piVar1;
  int iVar2;
  int local_10 [4];
  
  piVar1 = (int *)(&DAT_00508c10)[param_1];
  iVar2 = piVar1[0x10];
  if (0 < iVar2) {
    iVar2 = iVar2 + *(int *)(&DAT_00441844 + piVar1[0xf] * 4);
  }
  local_10[0] = *(int *)(&DAT_00441858 + piVar1[0xf] * 4) * iVar2;
  local_10[2] = *(int *)(&DAT_00441858 + piVar1[0xf] * 4) * (iVar2 + 1);
  if (0x805 < local_10[2]) {
    local_10[2] = 0x805;
  }
  local_10[1] = 0;
  local_10[3] = *(int *)(&DAT_00441894 + piVar1[0xf] * 4);
  BlitColorKeyedSurfaceClipped((int *)(&DAT_00504178)[piVar1[0xf]],*piVar1,piVar1[1],local_10);
  return;
}

