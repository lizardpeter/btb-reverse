/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041a4e0; function: LoadMazeData; body bytes: 542
 * callers: 1; callees: 4; success: True
 */


void LoadMazeData(void)

{
  byte bVar1;
  FILE *pFVar2;
  byte *pbVar3;
  int iVar4;
  void *this;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this_00;
  void *this_01;
  void *this_02;
  void *this_03;
  void *this_04;
  void *this_05;
  void *this_06;
  int *piVar5;
  int iVar6;
  byte *pbVar7;
  undefined *puVar8;
  bool bVar9;
  int local_68;
  byte local_64 [100];
  
  iVar6 = 0;
  DAT_00512128 = 0;
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_maze_nodes_txt_00444274,&DAT_0043e070);
  crt_fscanf(this,(int *)pFVar2,&DAT_0043e054);
  piVar5 = (int *)&DAT_00510cfc;
  do {
    crt_fscanf(local_64,(int *)pFVar2,&DAT_0043e054);
    pbVar7 = &DAT_0043e050;
    pbVar3 = local_64;
    do {
      bVar1 = *pbVar3;
      bVar9 = bVar1 < *pbVar7;
      if (bVar1 != *pbVar7) {
LAB_0041a55b:
        iVar4 = (1 - (uint)bVar9) - (uint)(bVar9 != 0);
        goto LAB_0041a560;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar9 = bVar1 < pbVar7[1];
      if (bVar1 != pbVar7[1]) goto LAB_0041a55b;
      pbVar3 = pbVar3 + 2;
      pbVar7 = pbVar7 + 2;
    } while (bVar1 != 0);
    iVar4 = 0;
LAB_0041a560:
    if (iVar4 == 0) {
      puVar8 = &DAT_00510d24;
      do {
        iVar6 = 4;
        do {
          crt_fscanf(puVar8 + 4,(int *)pFVar2,(byte *)s__d__d__d_00444248);
          puVar8 = puVar8 + 0xc;
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
      } while ((int)puVar8 < 0x510db4);
      crt_fscanf(this_01,(int *)pFVar2,&DAT_00444244);
      crt_fscanf(this_02,(int *)pFVar2,(byte *)s__d__d__d_00444248);
      crt_fscanf(this_03,(int *)pFVar2,(byte *)s__d__d__d_00444248);
      crt_fscanf(this_04,(int *)pFVar2,(byte *)s__d__d__d_00444248);
      crt_fscanf(this_05,(int *)pFVar2,(byte *)s__d__d__d_00444248);
      crt_fscanf(this_06,(int *)pFVar2,&DAT_00444240);
      crt_fclose(pFVar2);
      return;
    }
    iVar4 = _strncmp(&DAT_0044426c,(char *)local_64,4);
    this_00 = extraout_ECX;
    if (iVar4 == 0) {
      crt_fscanf(local_64,(int *)pFVar2,&DAT_0043e054);
      iVar6 = iVar6 + 0x1e;
      piVar5 = piVar5 + 1;
      this_00 = extraout_ECX_00;
    }
    crt_fscanf(this_00,(int *)pFVar2,&DAT_0043e8e4);
    crt_fscanf(&DAT_00510e1c + (local_68 + iVar6) * 8,(int *)pFVar2,
               (byte *)s__d__d__d__d__d__d__d__d_00444254);
    (&DAT_00510e1c)[(local_68 + iVar6) * 8] = (&DAT_00510e1c)[(local_68 + iVar6) * 8] + -0xb;
    if (*piVar5 < local_68) {
      *piVar5 = local_68;
    }
  } while( true );
}

