/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042a1a0; function: UpdateStartupVideoSequence; body bytes: 158
 * callers: 1; callees: 2; success: True
 */


undefined4 UpdateStartupVideoSequence(void)

{
  int iVar1;
  
  DAT_004fc07c = 1;
  DAT_0043ee6c = (uint)(*(int *)(&DAT_0051b370 + DAT_0051c388 * 4) != 1);
  if (DAT_0051c384 == 0) {
    OpenGlobalBinkMovie(&DAT_00515128 + DAT_0051c388 * 0x104);
    DAT_0051c384 = DAT_0051c384 + 1;
  }
  else if (DAT_0051c384 == 1) {
    iVar1 = UpdateGlobalBinkMovie();
    if (iVar1 != 0) {
      DAT_0051c388 = DAT_0051c388 + 1;
      DAT_0051c384 = 0;
      if (DAT_0051b3fc <= DAT_0051c388) {
        DAT_004fc07c = 0;
        DAT_0043ee6c = 1;
        return 1;
      }
    }
  }
  return 0;
}

