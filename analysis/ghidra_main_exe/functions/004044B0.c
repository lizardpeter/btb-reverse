/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004044b0; function: CSound_IsSoundPlaying; body bytes: 88
 * callers: 6; callees: 0; success: True
 */


uint __fastcall CSound_IsSoundPlaying(int param_1)

{
  int *piVar1;
  uint uVar2;
  uint uVar3;
  uint local_4;
  
  uVar2 = 0;
  if (*(int *)(param_1 + 4) == 0) {
    return 0;
  }
  uVar3 = 0;
  if (*(int *)(param_1 + 0x10) != 0) {
    do {
      piVar1 = *(int **)(*(int *)(param_1 + 4) + uVar3 * 4);
      if (piVar1 != (int *)0x0) {
        local_4 = 0;
        (**(code **)(*piVar1 + 0x24))(piVar1,&local_4);
        uVar2 = uVar2 | local_4 & 1;
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < *(uint *)(param_1 + 0x10));
  }
  return uVar2;
}

