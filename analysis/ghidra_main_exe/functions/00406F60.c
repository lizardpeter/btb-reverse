/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406f60; function: LoadNumUiHotAreaTable; body bytes: 98
 * callers: 1; callees: 4; success: True
 */


void __fastcall LoadNumUiHotAreaTable(int param_1)

{
  FILE *pFVar1;
  int iVar2;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this;
  void *extraout_ECX_01;
  int *piVar3;
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_loaddata_NumUiHotArea_txt_0043e904,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar1 == (FILE *)0x0) {
    NoOpLegacyHook();
    this = extraout_ECX_00;
  }
  piVar3 = &DAT_0048fb40;
  while( true ) {
    iVar2 = crt_fscanf(this,(int *)pFVar1,&DAT_0043e8e4);
    if ((iVar2 == -1) || (param_1 == -1)) break;
    *piVar3 = param_1;
    piVar3 = piVar3 + 1;
    this = extraout_ECX_01;
  }
  crt_fclose(pFVar1);
  return;
}

