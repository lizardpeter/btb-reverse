/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00439e9e; function: FUN_00439e9e; body bytes: 43
 * callers: 1; callees: 3; success: True
 */


uint * __cdecl FUN_00439e9e(uint *param_1)

{
  size_t sVar1;
  uint *puVar2;
  
  if (param_1 != (uint *)0x0) {
    sVar1 = _strlen((char *)param_1);
    puVar2 = (uint *)_malloc(sVar1 + 1);
    if (puVar2 != (uint *)0x0) {
      puVar2 = FUN_00433630(puVar2,param_1);
      return puVar2;
    }
  }
  return (uint *)0x0;
}

