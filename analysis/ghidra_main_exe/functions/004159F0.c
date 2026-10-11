/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004159f0; function: LoadGolfData; body bytes: 695
 * callers: 1; callees: 3; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void LoadGolfData(void)

{
  FILE *pFVar1;
  void *this;
  void *this_00;
  void *this_01;
  void *this_02;
  void *this_03;
  void *this_04;
  void *this_05;
  void *this_06;
  void *this_07;
  void *extraout_ECX;
  void *this_08;
  void *this_09;
  void *extraout_ECX_00;
  void *extraout_ECX_01;
  void *pvVar2;
  void *this_10;
  void *this_11;
  void *this_12;
  void *this_13;
  int iVar3;
  undefined *puVar4;
  char *pcVar5;
  int iVar6;
  
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_data_subgamegolf_golfdata_txt_00443988,&DAT_0043e070);
  crt_fscanf(this,(int *)pFVar1,(byte *)s__d__d_0043e920);
  crt_fscanf(this_00,(int *)pFVar1,&DAT_0043e054);
  crt_fscanf(this_01,(int *)pFVar1,&DAT_0043e8e4);
  crt_fscanf(this_02,(int *)pFVar1,(byte *)s__d__d_0043e920);
  crt_fscanf(this_03,(int *)pFVar1,&DAT_0043e8e4);
  crt_fscanf(this_04,(int *)pFVar1,(byte *)s__d__d_0043e920);
  DAT_0050aeb0 = DAT_0050aeb0 + -2;
  DAT_0050aeb4 = DAT_0050aeb4 + -5;
  iVar3 = 0;
  DAT_00443618 = 0x85;
  DAT_0044361c = 0xae;
  _DAT_0050ac30 = 0x40000000;
  pcVar5 = s_Data_SubGameGolf_flag_bmp_00443620;
  iVar6 = 0;
  do {
    crt_fscanf((void *)((int)&DAT_0050abb8 + iVar3),(int *)pFVar1,(byte *)s__d__d_0043e920);
    crt_fscanf(this_05,(int *)pFVar1,&DAT_0043e054);
    crt_fscanf(this_06,(int *)pFVar1,(byte *)s__d__d_0043e920);
    crt_fscanf((void *)((int)&DAT_0050adb8 + iVar3),(int *)pFVar1,(byte *)s__d__d_0043e920);
    crt_fscanf(this_07,(int *)pFVar1,&DAT_0043e8e4);
    crt_fscanf(&DAT_0050ae58 + iVar6,(int *)pFVar1,&DAT_0043e8e4);
    pcVar5 = pcVar5 + 0x46;
    iVar3 = iVar3 + 8;
    iVar6 = iVar6 + 4;
  } while ((int)pcVar5 < 0x4436f2);
  puVar4 = &DAT_0050ac4c;
  iVar3 = 0;
  pvVar2 = extraout_ECX;
  do {
    crt_fscanf(pvVar2,(int *)pFVar1,&DAT_0043e054);
    crt_fscanf(this_08,(int *)pFVar1,(byte *)s__d__d_0043e920);
    crt_fscanf((void *)((int)&DAT_0050ae08 + iVar3),(int *)pFVar1,(byte *)s__d__d_0043e920);
    crt_fscanf(this_09,(int *)pFVar1,&DAT_0043e8e4);
    puVar4 = puVar4 + 0x46;
    iVar3 = iVar3 + 4;
    pvVar2 = extraout_ECX_00;
  } while ((int)puVar4 < 0x50ad1e);
  puVar4 = &DAT_0050add0;
  do {
    crt_fscanf(pvVar2,(int *)pFVar1,&DAT_0043e8e4);
    puVar4 = puVar4 + 4;
    pvVar2 = extraout_ECX_01;
  } while ((int)puVar4 < 0x50addc);
  puVar4 = &DAT_0050adf0;
  do {
    crt_fscanf(pvVar2,(int *)pFVar1,&DAT_0043e8e4);
    puVar4 = puVar4 + 4;
    pvVar2 = this_10;
  } while ((int)puVar4 < 0x50adfc);
  crt_fscanf(this_10,(int *)pFVar1,(byte *)s__d__d_0043e920);
  crt_fscanf(this_11,(int *)pFVar1,&DAT_0043e8e4);
  crt_fscanf(this_12,(int *)pFVar1,(byte *)s__d__d_0043e920);
  crt_fscanf(this_13,(int *)pFVar1,(byte *)s__d__d_0043e920);
  crt_fclose(pFVar1);
  DAT_0050ad80 = DAT_0050aeb0 + DAT_0050ae44;
  DAT_0050ad7c = DAT_0050aeb4 + DAT_0050ae40;
  return;
}

