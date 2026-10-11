/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004077f0; function: LoadBinkWalkthroughTable; body bytes: 121
 * callers: 1; callees: 4; success: True
 */


void LoadBinkWalkthroughTable(void)

{
  FILE *pFVar1;
  int iVar2;
  int iVar3;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this;
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_binkwalk_txt_0043ec74,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar1 == (FILE *)0x0) {
    NoOpLegacyHook();
    this = extraout_ECX_00;
  }
  iVar2 = crt_fscanf(this,(int *)pFVar1,(byte *)s__s__s_0043ec54);
  if (iVar2 != -1) {
    iVar2 = 0;
    do {
      iVar3 = crt_fscanf(&DAT_0048e7f4 + iVar2,(int *)pFVar1,(byte *)s__s__s_0043ec54);
      iVar2 = iVar2 + 0x104;
    } while (iVar3 != -1);
  }
  crt_fclose(pFVar1);
  return;
}

