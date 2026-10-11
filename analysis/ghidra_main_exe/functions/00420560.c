/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00420560; function: LoadSpudMazeData; body bytes: 630
 * callers: 1; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void LoadSpudMazeData(void)

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
  void *extraout_ECX_01;
  void *pvVar5;
  void *extraout_ECX_02;
  int *piVar6;
  int iVar7;
  byte *pbVar8;
  undefined *puVar9;
  bool bVar10;
  int local_6c;
  byte local_64 [100];
  
  iVar7 = 0;
  DAT_00512128 = 0;
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_loaddata_spudmaze_nodes_txt_00445f24,&DAT_0043e070);
  crt_fscanf(this,(int *)pFVar2,&DAT_0043e054);
  piVar6 = (int *)&DAT_00514498;
  do {
    crt_fscanf(local_64,(int *)pFVar2,&DAT_0043e054);
    pbVar8 = &DAT_0043e050;
    pbVar3 = local_64;
    do {
      bVar1 = *pbVar3;
      bVar10 = bVar1 < *pbVar8;
      if (bVar1 != *pbVar8) {
LAB_004205db:
        iVar4 = (1 - (uint)bVar10) - (uint)(bVar10 != 0);
        goto LAB_004205e0;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar10 = bVar1 < pbVar8[1];
      if (bVar1 != pbVar8[1]) goto LAB_004205db;
      pbVar3 = pbVar3 + 2;
      pbVar8 = pbVar8 + 2;
    } while (bVar1 != 0);
    iVar4 = 0;
LAB_004205e0:
    if (iVar4 == 0) {
      puVar9 = &DAT_00510d24;
      do {
        iVar7 = 4;
        do {
          crt_fscanf(puVar9 + 4,(int *)pFVar2,(byte *)s__d__d__d_00444248);
          puVar9 = puVar9 + 0xc;
          iVar7 = iVar7 + -1;
        } while (iVar7 != 0);
      } while ((int)puVar9 < 0x510e14);
      crt_fscanf(this_00,(int *)pFVar2,&DAT_00444244);
      _DAT_0044421c = 0x40800000;
      crt_fscanf(this_01,(int *)pFVar2,(byte *)s__d__d__d_00444248);
      crt_fscanf(this_02,(int *)pFVar2,(byte *)s__d__d__d_00444248);
      crt_fscanf(this_03,(int *)pFVar2,(byte *)s__d__d__d_00444248);
      crt_fscanf(this_04,(int *)pFVar2,(byte *)s__d__d__d_00444248);
      crt_fscanf(this_05,(int *)pFVar2,&DAT_00444240);
      piVar6 = &DAT_004459cc;
      pvVar5 = extraout_ECX_01;
      do {
        iVar7 = 0;
        if (0 < *piVar6) {
          do {
            crt_fscanf(pvVar5,(int *)pFVar2,(byte *)s__d__d_0043e920);
            iVar7 = iVar7 + 1;
            pvVar5 = extraout_ECX_02;
          } while (iVar7 < *piVar6);
        }
        piVar6 = piVar6 + 1;
      } while ((int)piVar6 < 0x4459e0);
      crt_fclose(pFVar2);
      return;
    }
    iVar4 = _strncmp(&DAT_0044426c,(char *)local_64,4);
    pvVar5 = extraout_ECX;
    if (iVar4 == 0) {
      crt_fscanf(local_64,(int *)pFVar2,&DAT_0043e054);
      iVar7 = iVar7 + 0x1e;
      piVar6 = piVar6 + 1;
      pvVar5 = extraout_ECX_00;
    }
    crt_fscanf(pvVar5,(int *)pFVar2,&DAT_0043e8e4);
    crt_fscanf(&DAT_00510e1c + (local_6c + iVar7) * 8,(int *)pFVar2,
               (byte *)s__d__d__d__d__d__d__d__d_00444254);
    (&DAT_00510e1c)[(local_6c + iVar7) * 8] = (&DAT_00510e1c)[(local_6c + iVar7) * 8] + -0xb;
    if (*piVar6 < local_6c) {
      *piVar6 = local_6c;
    }
  } while( true );
}

