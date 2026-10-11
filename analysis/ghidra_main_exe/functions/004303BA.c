/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004303ba; function: FUN_004303ba; body bytes: 61
 * callers: 8; callees: 3; success: True
 */


undefined4 * __thiscall FUN_004303ba(void *this,undefined4 *param_1)

{
  size_t sVar1;
  uint *puVar2;
  
  *(undefined ***)this = &PTR_FUN_0043b5bc;
  sVar1 = _strlen((char *)*param_1);
  puVar2 = (uint *)operator_new(sVar1 + 1);
  *(uint **)((int)this + 4) = puVar2;
  if (puVar2 != (uint *)0x0) {
    FUN_00433630(puVar2,(uint *)*param_1);
  }
  *(undefined4 *)((int)this + 8) = 1;
  return (undefined4 *)this;
}

