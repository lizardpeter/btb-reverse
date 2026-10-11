/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004303f7; function: FUN_004303f7; body bytes: 74
 * callers: 2; callees: 3; success: True
 */


undefined4 * __thiscall FUN_004303f7(void *this,int param_1)

{
  int iVar1;
  size_t sVar2;
  uint *puVar3;
  
  *(undefined ***)this = &PTR_FUN_0043b5bc;
  iVar1 = *(int *)(param_1 + 8);
  *(int *)((int)this + 8) = iVar1;
  if (iVar1 == 0) {
    *(undefined4 *)((int)this + 4) = *(undefined4 *)(param_1 + 4);
  }
  else {
    sVar2 = _strlen(*(char **)(param_1 + 4));
    puVar3 = (uint *)operator_new(sVar2 + 1);
    *(uint **)((int)this + 4) = puVar3;
    if (puVar3 != (uint *)0x0) {
      FUN_00433630(puVar3,*(uint **)(param_1 + 4));
    }
  }
  return (undefined4 *)this;
}

