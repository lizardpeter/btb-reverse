/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041d7c0; function: LoadGrandOpeningMachineData; body bytes: 352
 * callers: 1; callees: 3; success: True
 */


void LoadGrandOpeningMachineData(void)

{
  FILE *pFVar1;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *extraout_ECX_01;
  void *extraout_ECX_02;
  void *extraout_ECX_03;
  void *pvVar2;
  void *extraout_ECX_04;
  int iVar3;
  undefined *puVar4;
  undefined4 *puVar5;
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_data_subgameOpen_machinedata_txt_00444d94,&DAT_0043e070);
  if (pFVar1 != (FILE *)0x0) {
    puVar4 = &DAT_00444c80;
    pvVar2 = extraout_ECX;
    do {
      crt_fscanf(pvVar2,(int *)pFVar1,(byte *)s__d__d_0043e920);
      puVar4 = puVar4 + 8;
      pvVar2 = extraout_ECX_00;
    } while ((int)puVar4 < 0x444cd0);
    puVar4 = &DAT_00444cd0;
    do {
      crt_fscanf(puVar4 + 4,(int *)pFVar1,(byte *)s__d__d_0043e920);
      puVar4 = puVar4 + 8;
    } while ((int)puVar4 < 0x444d20);
    iVar3 = 3;
    pvVar2 = extraout_ECX_01;
    do {
      crt_fscanf(pvVar2,(int *)pFVar1,(byte *)s__d__d_0043e920);
      iVar3 = iVar3 + -1;
      pvVar2 = extraout_ECX_02;
    } while (iVar3 != 0);
    iVar3 = 3;
    do {
      crt_fscanf(pvVar2,(int *)pFVar1,(byte *)s__d__d_0043e920);
      iVar3 = iVar3 + -1;
      pvVar2 = extraout_ECX_03;
    } while (iVar3 != 0);
    puVar4 = &DAT_00444d50;
    do {
      crt_fscanf(pvVar2,(int *)pFVar1,&DAT_0043e8e4);
      puVar4 = puVar4 + 4;
      pvVar2 = extraout_ECX_04;
    } while ((int)puVar4 < 0x444d5c);
    puVar5 = &DAT_00444b1c;
    do {
      crt_fscanf(puVar5 + 1,(int *)pFVar1,(byte *)s__d__d__d__d_00444d88);
      puVar5 = puVar5 + 4;
    } while ((int)puVar5 < 0x444bbc);
    crt_fclose(pFVar1);
  }
  return;
}

