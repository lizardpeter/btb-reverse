/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043439f; function: FUN_0043439f; body bytes: 93
 * callers: 1; callees: 5; success: True
 */


undefined4 __cdecl FUN_0043439f(int param_1)

{
  undefined **ppuVar1;
  
  DAT_0051da40 = HeapCreate((uint)(param_1 == 0),0x1000,0);
  if (DAT_0051da40 != (HANDLE)0x0) {
    DAT_0051da44 = FUN_00434257();
    if (DAT_0051da44 == 3) {
      ppuVar1 = (undefined **)FUN_004343fc(0x3f8);
    }
    else {
      if (DAT_0051da44 != 2) {
        return 1;
      }
      ppuVar1 = FUN_00434f43();
    }
    if (ppuVar1 != (undefined **)0x0) {
      return 1;
    }
    HeapDestroy(DAT_0051da40);
  }
  return 0;
}

