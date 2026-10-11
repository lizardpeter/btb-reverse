/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00435610; function: FUN_00435610; body bytes: 27
 * callers: 3; callees: 0; success: True
 */


undefined4 __cdecl FUN_00435610(undefined4 param_1)

{
  int iVar1;
  
  if (DAT_0051c44c != (code *)0x0) {
    iVar1 = (*DAT_0051c44c)(param_1);
    if (iVar1 != 0) {
      return 1;
    }
  }
  return 0;
}

