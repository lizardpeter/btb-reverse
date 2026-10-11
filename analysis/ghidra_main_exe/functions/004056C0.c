/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004056c0; function: MapClippedPrintRectToSource; body bytes: 275
 * callers: 0; callees: 2; success: True
 */


undefined4 __thiscall MapClippedPrintRectToSource(int param_1,LPRECT param_2,int *param_3)

{
  BOOL BVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  
  BVar1 = IntersectRect(param_2,param_2,(RECT *)(param_1 + 0x110));
  if (BVar1 != 0) {
    lVar3 = __ftol();
    *param_3 = (int)lVar3;
    lVar4 = __ftol();
    param_3[2] = (int)lVar4 + (int)lVar3;
    lVar3 = __ftol();
    param_3[1] = (int)lVar3;
    lVar4 = __ftol();
    iVar2 = (int)lVar4 + (int)lVar3;
    param_3[3] = iVar2;
    return CONCAT31((int3)((uint)iVar2 >> 8),1);
  }
  return 0;
}

