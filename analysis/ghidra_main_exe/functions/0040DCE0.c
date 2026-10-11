/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040dce0; function: ClearBandstandObjectsByCategorySelector; body bytes: 61
 * callers: 1; callees: 0; success: True
 */


void __cdecl ClearBandstandObjectsByCategorySelector(int param_1)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = &DAT_00500648;
  do {
    if (param_1 == 0) {
      iVar1 = *piVar2;
      if ((iVar1 != 0) && (iVar1 != 1)) {
joined_r0x0040dd0b:
        if (iVar1 != 2) goto LAB_0040dd10;
      }
      piVar2[-7] = -1;
    }
    else if (param_1 == 1) {
      iVar1 = *piVar2;
      goto joined_r0x0040dd0b;
    }
LAB_0040dd10:
    piVar2 = piVar2 + 0x13;
    if (0x5023f7 < (int)piVar2) {
      return;
    }
  } while( true );
}

