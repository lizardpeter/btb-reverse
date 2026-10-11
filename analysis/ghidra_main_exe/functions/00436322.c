/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00436322; function: FUN_00436322; body bytes: 61
 * callers: 4; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_00436322(uint param_1)

{
  if ((param_1 < DAT_0051da20) &&
     ((*(byte *)((&DAT_0051d920)[(int)param_1 >> 5] + 4 + (param_1 & 0x1f) * 8) & 1) != 0)) {
    return *(undefined4 *)((&DAT_0051d920)[(int)param_1 >> 5] + (param_1 & 0x1f) * 8);
  }
  DAT_0051c3cc = 0;
  _DAT_0051c3c8 = 9;
  return 0xffffffff;
}

