/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004280d0; function: CloseWalkthroughMovie; body bytes: 72
 * callers: 1; callees: 3; success: True
 */


void CloseWalkthroughMovie(void)

{
  if (DAT_0051be3c != (int *)0x0) {
    CloseBinkMovie(DAT_0051bd2c);
    UnregisterBitmapSurface(0x51be3c);
    if (DAT_0051be3c != (int *)0x0) {
      (**(code **)(*DAT_0051be3c + 8))(DAT_0051be3c);
      DAT_0051be3c = (int *)0x0;
    }
    FUN_00430d2a(DAT_0051c2f0);
  }
  return;
}

