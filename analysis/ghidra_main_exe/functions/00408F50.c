/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00408f50; function: CloseGlobalBinkMovie; body bytes: 27
 * callers: 1; callees: 2; success: True
 */


void CloseGlobalBinkMovie(void)

{
  _BinkClose_4(DAT_004fc070);
  DAT_004fc070 = 0;
  EnableInputProcessing();
  return;
}

