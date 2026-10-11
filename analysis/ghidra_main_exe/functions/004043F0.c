/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004043f0; function: CSound_SetVolume; body bytes: 61
 * callers: 1; callees: 0; success: True
 */


uint __thiscall CSound_SetVolume(void *this,undefined4 param_1)

{
  int *piVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar3 = 0;
  uVar4 = 0;
  if (*(int *)((int)this + 0x10) == 0) {
    return 0;
  }
  do {
    piVar1 = *(int **)(*(int *)((int)this + 4) + uVar4 * 4);
    uVar2 = (**(code **)(*piVar1 + 0x3c))(piVar1,param_1);
    uVar3 = uVar3 | uVar2;
    uVar4 = uVar4 + 1;
  } while (uVar4 < *(uint *)((int)this + 0x10));
  return uVar3;
}

