/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042a240; function: UpdateSceneSetMovie; body bytes: 128
 * callers: 1; callees: 2; success: True
 */


undefined4 UpdateSceneSetMovie(void)

{
  int iVar1;
  
  DAT_0043ee6c = 1;
  if (DAT_0051c38c == 0) {
    OpenGlobalBinkMovie(s_data_movies_sceneset_bik_00447054);
    DAT_0051c38c = DAT_0051c38c + 1;
  }
  else if ((DAT_0051c38c == 1) && (iVar1 = UpdateGlobalBinkMovie(), iVar1 != 0)) {
    DAT_0051c38c = 0;
    DAT_0051c390 = DAT_0051c390 + 1;
    if (0 < DAT_0051c390) {
      DAT_0043ee6c = 1;
      return 1;
    }
  }
  return 0;
}

