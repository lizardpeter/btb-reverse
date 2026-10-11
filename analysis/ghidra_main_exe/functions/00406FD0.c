/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406fd0; function: LoadUiHotAreas; body bytes: 295
 * callers: 1; callees: 4; success: True
 */


void LoadUiHotAreas(void)

{
  FILE *pFVar1;
  int iVar2;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this;
  int iVar3;
  int iVar4;
  int iVar5;
  void *local_6c;
  int local_68;
  
  iVar5 = 0;
  iVar4 = 0;
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_uiHotArea_txt_0043e944,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar1 == (FILE *)0x0) {
    NoOpLegacyHook();
    this = extraout_ECX_00;
  }
  crt_fscanf(this,(int *)pFVar1,&DAT_0043e054);
  iVar3 = 0;
  while( true ) {
    iVar2 = crt_fscanf(&local_6c,(int *)pFVar1,(byte *)s__d__d_0043e920);
    if ((iVar2 == -1) || (local_68 == -99)) break;
    if (local_68 == -2) {
      local_68 = -1;
      iVar2 = iVar4 + (iVar3 + iVar5) * 0xd;
      local_6c = (void *)0xffffffff;
      iVar3 = iVar3 + 0x20;
      iVar5 = 0;
      iVar4 = 0;
      (&DAT_004839c8)[iVar2 * 2] = 0xffffffff;
      (&DAT_004839cc)[iVar2 * 2] = 0xffffffff;
      crt_fscanf((void *)0xffffffff,(int *)pFVar1,&DAT_0043e054);
    }
    else if (local_68 == -1) {
      iVar4 = iVar4 + (iVar3 + iVar5) * 0xd;
      (&DAT_004839c8)[iVar4 * 2] = 0xffffffff;
      (&DAT_004839cc)[iVar4 * 2] = local_6c;
      iVar5 = iVar5 + 1;
      iVar4 = 0;
      crt_fscanf(local_6c,(int *)pFVar1,&DAT_0043e054);
    }
    else {
      iVar2 = (iVar3 + iVar5) * 0xd + iVar4;
      iVar4 = iVar4 + 1;
      (&DAT_004839c8)[iVar2 * 2] = local_68;
      (&DAT_004839cc)[iVar2 * 2] = local_6c;
    }
  }
  crt_fclose(pFVar1);
  return;
}

