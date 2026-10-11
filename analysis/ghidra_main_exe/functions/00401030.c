/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00401030; function: LoadHelpInfo; body bytes: 376
 * callers: 1; callees: 3; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void LoadHelpInfo(void)

{
  byte bVar1;
  FILE *pFVar2;
  byte *pbVar3;
  int iVar4;
  void *extraout_ECX;
  void *this;
  void *extraout_ECX_00;
  int iVar5;
  byte *pbVar6;
  undefined *puVar7;
  undefined4 *puVar8;
  bool bVar9;
  int local_cc;
  byte local_c8 [200];
  
  iVar5 = 0;
  puVar8 = &DAT_0044a2a4;
  for (iVar4 = 0x19; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar8 = 1;
    puVar8 = puVar8 + 1;
  }
  DAT_0044a2a0 = 0;
  DAT_0044dda0 = 0;
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_helpinfo_txt_0043e058,&DAT_0043e070);
  local_cc = 1;
  this = extraout_ECX;
  do {
    if (iVar5 == 1) {
      _DAT_0044a560 = 0x5c;
      _DAT_0044a564 = 0x131;
      _DAT_0044a568 = 0x8f;
      _DAT_0044a56c = 0x165;
      _DAT_0044a570 = 0x1dd;
    }
    else {
      (&DAT_0044a308)[iVar5 * 0x96] = 0x14;
      (&DAT_0044a30c)[iVar5 * 0x96] = 0x1a4;
      (&DAT_0044a310)[iVar5 * 0x96] = 0x3f;
      (&DAT_0044a314)[iVar5 * 0x96] = 0x1ce;
      (&DAT_0044a318)[iVar5 * 0x96] = 0x1dd;
    }
    crt_fscanf(this,(int *)pFVar2,&DAT_0043e054);
    pbVar6 = local_c8;
    pbVar3 = &DAT_0043e050;
    do {
      bVar1 = *pbVar3;
      bVar9 = bVar1 < *pbVar6;
      if (bVar1 != *pbVar6) {
LAB_0040112d:
        iVar4 = (1 - (uint)bVar9) - (uint)(bVar9 != 0);
        goto LAB_00401132;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar9 = bVar1 < pbVar6[1];
      if (bVar1 != pbVar6[1]) goto LAB_0040112d;
      pbVar3 = pbVar3 + 2;
      pbVar6 = pbVar6 + 2;
    } while (bVar1 != 0);
    iVar4 = 0;
LAB_00401132:
    if (iVar4 == 0) break;
    puVar7 = &DAT_0044a328 + iVar5 * 600;
    while( true ) {
      crt_fscanf(puVar7 + 4,(int *)pFVar2,(byte *)s__d__d__d__d__d_0043e040);
      if (*(int *)(puVar7 + -0xc) == -1) break;
      puVar7 = puVar7 + 0x14;
      (&DAT_0044a2a4)[iVar5] = (&DAT_0044a2a4)[iVar5] + 1;
    }
    iVar5 = iVar5 + 1;
    local_cc = local_cc + 1;
    this = extraout_ECX_00;
  } while (local_cc < 0x19);
  crt_fclose(pFVar2);
  return;
}

