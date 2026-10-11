/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00416310; function: CompareHerdingEntitiesByDepth; body bytes: 88
 * callers: 0; callees: 0; success: True
 */


int CompareHerdingEntitiesByDepth(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *param_1;
  if ((*(int *)(iVar1 + 0x30) == 0xf) || (*(int *)(iVar1 + 0x30) == 0x10)) {
    return -1;
  }
  iVar2 = *param_2;
  if ((*(int *)(iVar2 + 0x30) != 0xf) && (*(int *)(iVar2 + 0x30) != 0x10)) {
    return (((*(int *)(iVar2 + 0x40) - *(int *)(iVar2 + 0x38)) + *(int *)(iVar2 + 0x10) <
            (*(int *)(iVar1 + 0x40) - *(int *)(iVar1 + 0x38)) + *(int *)(iVar1 + 0x10)) - 1 &
           0xfffffffe) + 1;
  }
  return 1;
}

