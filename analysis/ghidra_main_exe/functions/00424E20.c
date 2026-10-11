/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00424e20; function: LoadSquirrelData; body bytes: 215
 * callers: 1; callees: 3; success: True
 */


void LoadSquirrelData(void)

{
  FILE *pFVar1;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *pvVar2;
  void *this;
  void *this_00;
  void *this_01;
  void *this_02;
  int iVar3;
  undefined4 *puVar4;
  undefined *puVar5;
  undefined1 local_4 [4];
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_Data_SubGameSquirrel_sqdata_txt_00446a50,&DAT_0043e070);
  puVar4 = &DAT_00446958;
  pvVar2 = extraout_ECX;
  do {
    crt_fscanf(pvVar2,(int *)pFVar1,&DAT_0043e8e4);
    puVar4 = puVar4 + 1;
    pvVar2 = extraout_ECX_00;
  } while ((int)puVar4 < 0x446974);
  puVar5 = &DAT_00446878;
  do {
    iVar3 = 7;
    do {
      crt_fscanf(pvVar2,(int *)pFVar1,&DAT_0043e8e4);
      puVar5 = puVar5 + 4;
      iVar3 = iVar3 + -1;
      pvVar2 = this;
    } while (iVar3 != 0);
  } while ((int)puVar5 < 0x446958);
  crt_fscanf(this,(int *)pFVar1,&DAT_0043e8e4);
  crt_fscanf(this_00,(int *)pFVar1,&DAT_0043e8e4);
  crt_fscanf(local_4,(int *)pFVar1,&DAT_0043e8e4);
  crt_fscanf(this_01,(int *)pFVar1,(byte *)s__d__d__d__d_00444d88);
  crt_fscanf(this_02,(int *)pFVar1,&DAT_0043e8e4);
  crt_fclose(pFVar1);
  return;
}

