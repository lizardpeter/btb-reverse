/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404430; function: CSound_Stop; body bytes: 58
 * callers: 21; callees: 0; success: True
 */


uint __fastcall CSound_Stop(int param_1)

{
  int *piVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  if (*(int *)(param_1 + 4) == 0) {
    return 0x800401f0;
  }
  uVar3 = 0;
  uVar4 = 0;
  if (*(int *)(param_1 + 0x10) != 0) {
    do {
      piVar1 = *(int **)(*(int *)(param_1 + 4) + uVar4 * 4);
      uVar2 = (**(code **)(*piVar1 + 0x48))(piVar1);
      uVar3 = uVar3 | uVar2;
      uVar4 = uVar4 + 1;
    } while (uVar4 < *(uint *)(param_1 + 0x10));
  }
  return uVar3;
}

