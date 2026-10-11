/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00431d39; function: FUN_00431d39; body bytes: 55
 * callers: 1; callees: 1; success: True
 */


uint __thiscall FUN_00431d39(void *this,uint param_1)

{
  uint uVar1;
  
  if (DAT_00449b10 < 2) {
    uVar1 = (byte)PTR_DAT_00449b1c[param_1 * 2] & 4;
  }
  else {
    uVar1 = FUN_004365c0(this,param_1,4);
  }
  if (uVar1 == 0) {
    param_1 = (param_1 & 0xffffffdf) - 7;
  }
  return param_1;
}

