/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407af0; function: SetCursorSurface; body bytes: 100
 * callers: 24; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl SetCursorSurface(int *param_1)

{
  undefined4 local_7c;
  undefined4 local_78;
  
  if (param_1 == (int *)0x0) {
    DAT_004fbe68 = (int *)DAT_004fbf88;
    return;
  }
  DAT_004fbe68 = param_1;
  local_7c = 0x7c;
  local_78 = 6;
  (**(code **)(*param_1 + 0x58))(param_1,&local_7c);
  _DAT_004fbd10 = 0;
  _DAT_004fbd14 = 0;
  DAT_004fbd18 = local_78;
  DAT_004fbd1c = local_7c;
  return;
}

