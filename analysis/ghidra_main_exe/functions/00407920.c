/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407920; function: LoadGlobalDataTables; body bytes: 61
 * callers: 1; callees: 11; success: True
 */


undefined4 LoadGlobalDataTables(void)

{
  int extraout_ECX;
  
  LoadUiBitmapNames();
  LoadUiHotAreaReplacements();
  LoadUiHotAreas();
  LoadNumUiHotAreaTable(extraout_ECX);
  LoadHelpWavTable();
  LoadOptionsConfig();
  LoadVideoSequence();
  LoadQuitConfirmationData();
  LoadBinkWalkthroughTable();
  LoadStartupAndCompletionMovies();
  LoadPlayAgainOverlayData();
  return 1;
}

