/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004079e0; function: UpdateLoadingCursorAnimation; body bytes: 96
 * callers: 2; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdateLoadingCursorAnimation(void)

{
  if (DAT_004fbe68 == DAT_0051c260) {
    PollDirectInputAndUpdateState();
    DAT_004fbfc0 = DAT_004fbfc0 + 1;
    if (5 < DAT_004fbfc0) {
      DAT_004fbfc0 = 0;
    }
    _DAT_004fbd14 = 0;
    DAT_004fbd1c = 100;
    _DAT_004fbd10 = DAT_004fbfc0 * 100;
    DAT_004fbd18 = _DAT_004fbd10 + 100;
  }
  return;
}

