/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00437443; function: FUN_00437443; body bytes: 53
 * callers: 1; callees: 2; success: True
 */


uint __thiscall FUN_00437443(void *this,uint param_1,uint param_2)

{
  uint uVar1;
  undefined2 in_FPUControlWord;
  undefined4 local_8;
  
  local_8 = CONCAT22((short)((uint)this >> 0x10),in_FPUControlWord);
  uVar1 = FUN_0043748e(local_8);
  uVar1 = uVar1 & ~param_2 | param_1 & param_2;
  FUN_00437520(uVar1);
  return uVar1;
}

