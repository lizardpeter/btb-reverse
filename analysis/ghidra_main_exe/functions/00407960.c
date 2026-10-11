/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407960; function: BeginLoadingCursorAnimation; body bytes: 92
 * callers: 2; callees: 2; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void BeginLoadingCursorAnimation(void)

{
  _DAT_004fbd10 = 0;
  DAT_004fbe68 = DAT_0051c260;
  DAT_004fbd18 = 100;
  DAT_004fbd1c = 100;
  _DAT_004fbd14 = 0;
  DAT_004fbfc4 = 1;
  if (DAT_0051c33c == 1) {
    CSound_Reset((int)DAT_0051c2b8);
    CSound_Play(DAT_0051c2b8,0,1);
  }
  return;
}

