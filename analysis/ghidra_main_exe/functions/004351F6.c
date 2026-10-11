/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004351f6; function: FUN_004351f6; body bytes: 69
 * callers: 2; callees: 1; success: True
 */


void __cdecl FUN_004351f6(int param_1,int param_2,byte *param_3)

{
  int *piVar1;
  
  piVar1 = (int *)(param_1 + 0x18 + (param_2 - *(int *)(param_1 + 0x10) >> 0xc) * 8);
  *piVar1 = *piVar1 + (uint)*param_3;
  *param_3 = 0;
  piVar1[1] = 0xf1;
  if ((*piVar1 == 0xf0) && (DAT_0051c444 = DAT_0051c444 + 1, DAT_0051c444 == 0x20)) {
    FUN_004350dd(0x10);
  }
  return;
}

