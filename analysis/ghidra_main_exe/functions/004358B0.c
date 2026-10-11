/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004358b0; function: FUN_004358b0; body bytes: 67
 * callers: 1; callees: 0; success: True
 */


int * __cdecl FUN_004358b0(int param_1)

{
  int *piVar1;
  
  piVar1 = &DAT_00449770;
  if (DAT_00449770 != param_1) {
    do {
      piVar1 = piVar1 + 3;
      if (&DAT_00449770 + DAT_004497f0 * 3 <= piVar1) break;
    } while (*piVar1 != param_1);
  }
  if ((&DAT_00449770 + DAT_004497f0 * 3 <= piVar1) || (*piVar1 != param_1)) {
    piVar1 = (int *)0x0;
  }
  return piVar1;
}

