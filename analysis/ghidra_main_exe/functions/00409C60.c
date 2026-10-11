/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409c60; function: IsDinoCursorNearPoint; body bytes: 50
 * callers: 1; callees: 0; success: True
 */


uint __cdecl IsDinoCursorNearPoint(int param_1,int param_2,int param_3)

{
  uint uVar1;
  
  uVar1 = DAT_004fbd24 - param_1 >> 0x1f;
  uVar1 = (DAT_004fbd24 - param_1 ^ uVar1) - uVar1;
  if (((int)uVar1 <= param_3) &&
     (uVar1 = DAT_004fbd30 - param_2 >> 0x1f, uVar1 = (DAT_004fbd30 - param_2 ^ uVar1) - uVar1,
     (int)uVar1 <= param_3)) {
    return CONCAT31((int3)(uVar1 >> 8),1);
  }
  return uVar1 & 0xffffff00;
}

