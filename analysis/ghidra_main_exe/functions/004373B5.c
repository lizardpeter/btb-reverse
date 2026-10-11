/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004373b5; function: FUN_004373b5; body bytes: 38
 * callers: 2; callees: 0; success: True
 */


byte __cdecl FUN_004373b5(uint param_1)

{
  if (DAT_0051da20 <= param_1) {
    return 0;
  }
  return *(byte *)((&DAT_0051d920)[(int)param_1 >> 5] + 4 + (param_1 & 0x1f) * 8) & 0x40;
}

