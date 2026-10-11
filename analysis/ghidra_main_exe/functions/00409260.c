/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409260; function: PauseGlobalBinkMovie; body bytes: 50
 * callers: 1; callees: 3; success: True
 */


void PauseGlobalBinkMovie(void)

{
  if (DAT_004fc070 != 0) {
    _BinkPause_8(DAT_004fc070,1);
    DAT_004fbfe8 = 1;
    _BinkSetVolume_12(DAT_004fc070,0,0);
    EnableInputProcessing();
    return;
  }
  return;
}

