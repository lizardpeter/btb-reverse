/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00436598; function: FUN_00436598; body bytes: 40
 * callers: 2; callees: 1; success: True
 */


uint __thiscall FUN_00436598(void *this,int param_1)

{
  uint uVar1;
  
  if (1 < DAT_00449b10) {
    uVar1 = FUN_004365c0(this,param_1,8);
    return uVar1;
  }
  return (byte)PTR_DAT_00449b1c[param_1 * 2] & 8;
}

