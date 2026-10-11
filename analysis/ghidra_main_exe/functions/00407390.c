/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407390; function: LoadOptionsConfig; body bytes: 150
 * callers: 1; callees: 4; success: True
 */


void LoadOptionsConfig(void)

{
  FILE *pFVar1;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this;
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_options_txt_0043ea90,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar1 == (FILE *)0x0) {
    NoOpLegacyHook();
    this = extraout_ECX_00;
  }
  crt_fscanf(this,(int *)pFVar1,(byte *)s__d__d__d__d__d__d__d__d__d__d__d_0043ea38);
  crt_fclose(pFVar1);
  return;
}

