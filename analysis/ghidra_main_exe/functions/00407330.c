/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407330; function: LoadUiBitmapNames; body bytes: 90
 * callers: 1; callees: 4; success: True
 */


void LoadUiBitmapNames(void)

{
  FILE *pFVar1;
  int iVar2;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this;
  void *extraout_ECX_01;
  undefined *puVar3;
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_loaddata_uiBitmapName_txt_0043ea1c,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar1 == (FILE *)0x0) {
    NoOpLegacyHook();
    this = extraout_ECX_00;
  }
  puVar3 = &DAT_00490770;
  do {
    iVar2 = crt_fscanf(this,(int *)pFVar1,&DAT_0043e9fc);
    if (iVar2 == -1) break;
    puVar3 = puVar3 + 0x100;
    this = extraout_ECX_01;
  } while ((int)puVar3 < 0x493970);
  crt_fclose(pFVar1);
  return;
}

