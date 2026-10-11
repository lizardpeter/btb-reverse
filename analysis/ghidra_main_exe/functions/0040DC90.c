/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040dc90; function: ClearPondPrimaryObjectsBySelector; body bytes: 78
 * callers: 1; callees: 0; success: True
 */


void __cdecl ClearPondPrimaryObjectsBySelector(int param_1)

{
  int *piVar1;
  
  piVar1 = &DAT_004fcacc;
  do {
    if (param_1 == 0) {
      *piVar1 = -1;
      DAT_00507a28 = 0xffffffff;
    }
    else if (param_1 == 1) {
      if (*piVar1 == 7) {
        *piVar1 = -1;
        DAT_005079fc = 0xffffffff;
      }
    }
    else if (((param_1 == 2) && (*piVar1 != 7)) && (*piVar1 < 100)) {
      *piVar1 = -1;
    }
    piVar1 = piVar1 + 0x13;
  } while ((int)piVar1 < 0x4fe87c);
  return;
}

