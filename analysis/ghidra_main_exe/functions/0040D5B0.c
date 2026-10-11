/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d5b0; function: CompareParkDesignerObjectsForDraw; body bytes: 461
 * callers: 0; callees: 0; success: True
 */


int __cdecl CompareParkDesignerObjectsForDraw(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar1 = *param_1;
  iVar2 = *(int *)(iVar1 + 0x1c);
  if ((iVar2 == -1) && (*(int *)(*param_2 + 0x1c) == -1)) {
    return 0;
  }
  if (iVar2 == -1) {
    return 1;
  }
  iVar3 = *param_2;
  iVar4 = *(int *)(iVar3 + 0x1c);
  if (iVar4 == -1) {
    return -1;
  }
  if (*(int *)(iVar1 + 0x30) < *(int *)(iVar3 + 0x30)) {
    return -1;
  }
  if (*(int *)(iVar3 + 0x30) < *(int *)(iVar1 + 0x30)) {
    return 1;
  }
  iVar6 = *(int *)(iVar1 + 0xc);
  iVar5 = *(int *)(iVar3 + 0xc);
  if (iVar2 == 300) {
    if (*(int *)(iVar1 + 0x38) < 1) {
      if (iVar4 == 300) goto LAB_0040d689;
    }
    else {
      if (iVar4 == 300) {
LAB_0040d689:
        return ((*(int *)(iVar1 + 0x38) <= *(int *)(iVar3 + 0x38)) - 1 & 2) - 1;
      }
      iVar6 = (&DAT_004fcabc)[DAT_00507e3c * 0x13];
    }
  }
  if (iVar4 == 300) {
    if (*(int *)(iVar3 + 0x38) < 1) {
      if (iVar2 == 300) goto LAB_0040d6a6;
    }
    else {
      if (iVar2 == 300) {
LAB_0040d6a6:
        return ((*(int *)(iVar3 + 0x38) <= *(int *)(iVar1 + 0x38)) - 1 & 2) - 1;
      }
      iVar5 = (&DAT_004fcabc)[DAT_00507e3c * 0x13];
    }
  }
  if (((iVar2 < 100) && (*(int *)(iVar1 + 0x38) == 2)) && (*(int *)(iVar1 + 0x3c) == 2)) {
    iVar6 = iVar6 + -10;
  }
  if (((iVar4 < 100) && (*(int *)(iVar3 + 0x38) == 2)) && (*(int *)(iVar3 + 0x3c) == 2)) {
    iVar5 = iVar5 + -10;
  }
  if ((399 < iVar2) && (399 < iVar4)) {
    iVar6 = (*(int *)(iVar1 + 0x38) * 5 + *(int *)(iVar1 + 0x3c)) * 0x10;
    iVar6 = (*(int *)(&DAT_00441b24 + iVar6) + *(int *)(&DAT_00441b1c + iVar6)) / 2 +
            *(int *)(iVar1 + 4);
    iVar5 = (*(int *)(iVar3 + 0x38) * 5 + *(int *)(iVar3 + 0x3c)) * 0x10;
    iVar5 = (*(int *)(&DAT_00441b24 + iVar5) + *(int *)(&DAT_00441b1c + iVar5)) / 2 +
            *(int *)(iVar3 + 4);
  }
  if ((*(int *)(iVar1 + 0x34) < 100) && (iVar2 == 7)) {
    iVar6 = iVar6 + 0x53;
  }
  if ((*(int *)(iVar3 + 0x34) < 100) && (iVar4 == 7)) {
    iVar5 = iVar5 + 0x53;
  }
  return ((iVar5 <= iVar6) - 1 & 0xfffffffe) + 1;
}

