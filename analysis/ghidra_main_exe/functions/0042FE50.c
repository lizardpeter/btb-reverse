/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042fe50; function: FUN_0042fe50; body bytes: 123
 * callers: 2; callees: 1; success: True
 */


int __cdecl FUN_0042fe50(uint param_1,int param_2,int param_3,uint *param_4,uint *param_5)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint local_8;
  
  uVar2 = param_1;
  local_8 = *(uint *)(param_1 + 0xc);
  iVar1 = *(int *)(param_1 + 0x10);
  uVar3 = local_8;
  param_1 = local_8;
  if (-1 < param_2) {
    do {
      if (uVar3 == 0xffffffff) {
        FUN_00432662();
      }
      uVar3 = uVar3 - 1;
      if (((*(int *)(iVar1 + 4 + uVar3 * 0x14) < param_3) &&
          (param_3 <= *(int *)(iVar1 + uVar3 * 0x14 + 8))) || (uVar3 == 0xffffffff)) {
        param_2 = param_2 + -1;
        local_8 = param_1;
        param_1 = uVar3;
      }
    } while (-1 < param_2);
  }
  uVar3 = uVar3 + 1;
  *param_4 = uVar3;
  *param_5 = local_8;
  if ((*(uint *)(uVar2 + 0xc) < local_8) || (local_8 < uVar3)) {
    FUN_00432662();
  }
  return iVar1 + uVar3 * 0x14;
}

