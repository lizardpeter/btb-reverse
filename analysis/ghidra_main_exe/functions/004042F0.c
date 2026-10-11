/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004042f0; function: CSound_GetFreeBuffer; body bytes: 111
 * callers: 1; callees: 1; success: True
 */


undefined4 __fastcall CSound_GetFreeBuffer(uint param_1)

{
  int *piVar1;
  uint uVar2;
  uint local_4;
  
  if (*(int *)(param_1 + 4) == 0) {
    return 0;
  }
  uVar2 = 0;
  local_4 = param_1;
  if (*(int *)(param_1 + 0x10) != 0) {
    do {
      piVar1 = *(int **)(*(int *)(param_1 + 4) + uVar2 * 4);
      if (piVar1 != (int *)0x0) {
        local_4 = 0;
        (**(code **)(*piVar1 + 0x24))(piVar1,&local_4);
        if ((local_4 & 1) == 0) break;
      }
      uVar2 = uVar2 + 1;
    } while (uVar2 < *(uint *)(param_1 + 0x10));
  }
  if (uVar2 != *(uint *)(param_1 + 0x10)) {
    return *(undefined4 *)(*(int *)(param_1 + 4) + uVar2 * 4);
  }
  uVar2 = FUN_0042ffc4();
  return *(undefined4 *)(*(int *)(param_1 + 4) + (uVar2 % *(uint *)(param_1 + 0x10)) * 4);
}

