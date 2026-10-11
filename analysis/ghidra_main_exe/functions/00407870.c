/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407870; function: LoadStartupAndCompletionMovies; body bytes: 172
 * callers: 1; callees: 4; success: True
 */


void LoadStartupAndCompletionMovies(void)

{
  FILE *pFVar1;
  int iVar2;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *extraout_ECX_01;
  void *extraout_ECX_02;
  void *extraout_ECX_03;
  void *pvVar3;
  void *extraout_ECX_04;
  undefined *puVar4;
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_loaddata_startupmovie_txt_0043ecc8,&DAT_0043e070);
  pvVar3 = extraout_ECX;
  if (pFVar1 == (FILE *)0x0) {
    NoOpLegacyHook();
    pvVar3 = extraout_ECX_00;
  }
  puVar4 = &DAT_00493970;
  do {
    iVar2 = crt_fscanf(pvVar3,(int *)pFVar1,&DAT_0043e9fc);
    if (iVar2 == -1) break;
    puVar4 = puVar4 + 0x100;
    pvVar3 = extraout_ECX_01;
  } while ((int)puVar4 < 0x494570);
  crt_fclose(pFVar1);
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_loaddata_completedmovie_txt_0043ecac,&DAT_0043e070);
  pvVar3 = extraout_ECX_02;
  if (pFVar1 == (FILE *)0x0) {
    NoOpLegacyHook();
    pvVar3 = extraout_ECX_03;
  }
  puVar4 = &DAT_0048daf0;
  do {
    iVar2 = crt_fscanf(pvVar3,(int *)pFVar1,&DAT_0043e9fc);
    if (iVar2 == -1) break;
    puVar4 = puVar4 + 0x100;
    pvVar3 = extraout_ECX_04;
  } while ((int)puVar4 < 0x48e6f0);
  crt_fclose(pFVar1);
  return;
}

