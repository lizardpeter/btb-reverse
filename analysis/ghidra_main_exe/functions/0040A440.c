/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040a440; function: LoadDinoLevelData; body bytes: 225
 * callers: 1; callees: 4; success: True
 */


void LoadDinoLevelData(void)

{
  FILE *pFVar1;
  int iVar2;
  void *this;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *pvVar3;
  void *extraout_ECX_01;
  int iVar4;
  undefined4 *puVar5;
  void *local_88;
  int local_84;
  CHAR local_80 [128];
  
  iVar4 = 0;
  crt_sprintf(local_80,(byte *)s__s_dino_txt_0043f854);
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback(local_80,&DAT_0043e070);
  crt_fscanf(this,(int *)pFVar1,&DAT_0043e8e4);
  puVar5 = &DAT_005101cc;
  pvVar3 = extraout_ECX;
  while ((iVar2 = crt_fscanf(pvVar3,(int *)pFVar1,(byte *)s__d__d_0043e920), iVar2 != -1 &&
         (local_84 != -1))) {
    puVar5[-1] = local_84;
    *puVar5 = local_88;
    iVar4 = iVar4 + 1;
    puVar5 = puVar5 + 2;
    pvVar3 = local_88;
  }
  iVar2 = 0;
  pvVar3 = extraout_ECX_00;
  if (0 < DAT_0043ee8c) {
    do {
      crt_fscanf(pvVar3,(int *)pFVar1,&DAT_0043e8e4);
      iVar2 = iVar2 + 1;
      pvVar3 = extraout_ECX_01;
    } while (iVar2 < DAT_0043ee8c);
  }
  crt_fclose(pFVar1);
  (&DAT_005101c8)[iVar4 * 2] = 0xffffffff;
  (&DAT_005101cc)[iVar4 * 2] = 0xffffffff;
  return;
}

