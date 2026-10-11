/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043277a; function: FUN_0043277a; body bytes: 103
 * callers: 6; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0043277a(undefined *param_1)

{
  undefined **ppuVar1;
  int iVar2;
  
  iVar2 = 0;
  DAT_0051c3cc = param_1;
  ppuVar1 = (undefined **)&DAT_004475b8;
  do {
    if (param_1 == *ppuVar1) {
      _DAT_0051c3c8 = *(undefined4 *)(iVar2 * 8 + 0x4475bc);
      return;
    }
    ppuVar1 = ppuVar1 + 2;
    iVar2 = iVar2 + 1;
  } while (ppuVar1 < &PTR_DAT_00447720);
  if (((undefined *)0x12 < param_1) && (param_1 < (undefined *)0x25)) {
    _DAT_0051c3c8 = 0xd;
    return;
  }
  if ((param_1 < (undefined *)0xbc) || (_DAT_0051c3c8 = 8, (undefined *)0xca < param_1)) {
    _DAT_0051c3c8 = 0x16;
  }
  return;
}

