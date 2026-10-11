/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407770; function: LoadVideoSequence; body bytes: 122
 * callers: 1; callees: 4; success: True
 */


void LoadVideoSequence(void)

{
  FILE *pFVar1;
  int iVar2;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this;
  void *extraout_ECX_01;
  undefined *puVar3;
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_videoseq_txt_0043ec3c,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar1 == (FILE *)0x0) {
    NoOpLegacyHook();
    this = extraout_ECX_00;
  }
  DAT_0051b3fc = 0;
  puVar3 = &DAT_0051b370;
  do {
    iVar2 = crt_fscanf(this,(int *)pFVar1,(byte *)s__s__d_0043ec1c);
    if (iVar2 == -1) break;
    puVar3 = puVar3 + 4;
    DAT_0051b3fc = DAT_0051b3fc + 1;
    this = extraout_ECX_01;
  } while ((int)puVar3 < 0x51b398);
  crt_fclose(pFVar1);
  return;
}

