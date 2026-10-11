/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00424230; function: LoadSpudSkateTimingData; body bytes: 290
 * callers: 1; callees: 3; success: True
 */


void LoadSpudSkateTimingData(void)

{
  FILE *pFVar1;
  int iVar2;
  void *this;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *extraout_ECX_01;
  void *this_00;
  char *pcVar3;
  undefined *puVar4;
  CHAR *pCVar5;
  undefined4 *puVar6;
  CHAR local_30c [36];
  undefined4 local_2e8 [56];
  undefined4 local_208 [9];
  undefined4 local_1e4 [56];
  undefined4 local_104 [9];
  undefined4 local_e0 [56];
  
  pcVar3 = s_data_subgamespudskate_spuddata1__00446668;
  pCVar5 = local_30c;
  for (iVar2 = 9; iVar2 != 0; iVar2 = iVar2 + -1) {
    *(undefined4 *)pCVar5 = *(undefined4 *)pcVar3;
    pcVar3 = pcVar3 + 4;
    pCVar5 = pCVar5 + 4;
  }
  puVar6 = local_2e8;
  for (iVar2 = 0x38; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  pcVar3 = s_data_subgamespudskate_spuddata2__00446644;
  puVar6 = local_208;
  for (iVar2 = 9; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar6 = *(undefined4 *)pcVar3;
    pcVar3 = pcVar3 + 4;
    puVar6 = puVar6 + 1;
  }
  puVar6 = local_1e4;
  for (iVar2 = 0x38; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  pcVar3 = s_data_subgamespudskate_spuddata3__00446620;
  puVar6 = local_104;
  for (iVar2 = 9; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar6 = *(undefined4 *)pcVar3;
    pcVar3 = pcVar3 + 4;
    puVar6 = puVar6 + 1;
  }
  puVar6 = local_e0;
  for (iVar2 = 0x38; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback(local_30c + DAT_0051c284 * 0x104,&DAT_0043e070);
  crt_fscanf(this,(int *)pFVar1,&DAT_0043e8e4);
  iVar2 = 0;
  this_00 = extraout_ECX;
  if (0 < DAT_00446568) {
    puVar4 = &DAT_00514b74;
    do {
      crt_fscanf(puVar4 + -4,(int *)pFVar1,(byte *)s__d__d__d__d_00444d88);
      iVar2 = iVar2 + 1;
      puVar4 = puVar4 + 0x10;
      this_00 = extraout_ECX_00;
    } while (iVar2 < DAT_00446568);
  }
  iVar2 = 0;
  if (0 < DAT_00446568) {
    do {
      crt_fscanf(this_00,(int *)pFVar1,&DAT_0043e8e4);
      iVar2 = iVar2 + 1;
      this_00 = extraout_ECX_01;
    } while (iVar2 < DAT_00446568);
  }
  crt_fscanf(this_00,(int *)pFVar1,(byte *)s__d__d_0043e920);
  crt_fclose(pFVar1);
  return;
}

