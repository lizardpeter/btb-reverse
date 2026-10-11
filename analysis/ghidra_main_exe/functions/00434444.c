/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00434444; function: FUN_00434444; body bytes: 43
 * callers: 3; callees: 0; success: True
 */


uint __cdecl FUN_00434444(int param_1)

{
  uint uVar1;
  
  uVar1 = DAT_0051da38;
  while( true ) {
    if (DAT_0051da38 + DAT_0051da34 * 0x14 <= uVar1) {
      return 0;
    }
    if ((uint)(param_1 - *(int *)(uVar1 + 0xc)) < 0x100000) break;
    uVar1 = uVar1 + 0x14;
  }
  return uVar1;
}

