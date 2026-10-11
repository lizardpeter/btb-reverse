/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00436e6c; function: FUN_00436e6c; body bytes: 74
 * callers: 1; callees: 0; success: True
 */


int __cdecl FUN_00436e6c(int param_1)

{
  int iVar1;
  bool bVar2;
  
  if (param_1 == -2) {
    DAT_0051c62c = 1;
                    /* WARNING: Could not recover jumptable at 0x00436e86. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = GetOEMCP();
    return iVar1;
  }
  if (param_1 == -3) {
    DAT_0051c62c = 1;
                    /* WARNING: Could not recover jumptable at 0x00436e9b. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = GetACP();
    return iVar1;
  }
  bVar2 = param_1 == -4;
  if (bVar2) {
    param_1 = DAT_0051c690;
  }
  DAT_0051c62c = (uint)bVar2;
  return param_1;
}

