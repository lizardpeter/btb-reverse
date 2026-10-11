/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409ca0; function: IsDinoCursorInsideRect; body bytes: 43
 * callers: 4; callees: 0; success: True
 */


undefined4 __cdecl IsDinoCursorInsideRect(int *param_1)

{
  uint3 uVar1;
  
  uVar1 = (uint3)((uint)param_1 >> 8);
  if ((((*param_1 <= DAT_004fbd24) && (DAT_004fbd24 <= param_1[2])) && (param_1[1] <= DAT_004fbd30))
     && (DAT_004fbd30 <= param_1[3])) {
    return CONCAT31(uVar1,1);
  }
  return (uint)uVar1 << 8;
}

