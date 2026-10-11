/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004092a0; function: ResumeGlobalBinkMovie; body bytes: 39
 * callers: 5; callees: 3; success: True
 */


void ResumeGlobalBinkMovie(void)

{
  if (DAT_004fc070 != 0) {
    _BinkPause_8(DAT_004fc070,0);
    DAT_004fbfe8 = 0;
    ApplyGlobalBinkVolume();
    DisableInputProcessing();
    return;
  }
  return;
}

