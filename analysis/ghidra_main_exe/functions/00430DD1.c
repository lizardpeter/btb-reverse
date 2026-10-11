/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00430dd1; function: FUN_00430dd1; body bytes: 116
 * callers: 1; callees: 3; success: True
 */


void __cdecl FUN_00430dd1(uint *param_1)

{
  int *piVar1;
  uint dwBytes;
  
  if (DAT_0051da44 == 3) {
    if ((param_1 <= DAT_0051da3c) && (piVar1 = FUN_00434798(param_1), piVar1 != (int *)0x0)) {
      return;
    }
  }
  else if (DAT_0051da44 == 2) {
    if (param_1 == (uint *)0x0) {
      dwBytes = 0x10;
    }
    else {
      dwBytes = (int)param_1 + 0xfU & 0xfffffff0;
    }
    if ((dwBytes <= DAT_0044976c) && (piVar1 = FUN_0043523b(dwBytes >> 4), piVar1 != (int *)0x0)) {
      return;
    }
    goto LAB_00430e34;
  }
  if (param_1 == (uint *)0x0) {
    param_1 = (uint *)0x1;
  }
  dwBytes = (int)param_1 + 0xfU & 0xfffffff0;
LAB_00430e34:
  HeapAlloc(DAT_0051da40,0,dwBytes);
  return;
}

