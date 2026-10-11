/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00424360; function: InitializeSpudSkateActivity; body bytes: 814
 * callers: 1; callees: 12; success: True
 */


void InitializeSpudSkateActivity(void)

{
  undefined4 *puVar1;
  char *pcVar2;
  FILE *pFVar3;
  uint uVar4;
  char *pcVar5;
  int *piVar6;
  int iVar7;
  uint uVar8;
  void *extraout_ECX;
  void *this;
  void *extraout_ECX_00;
  undefined4 *puVar9;
  int iVar10;
  char *pcVar11;
  undefined *puVar12;
  undefined4 *puVar13;
  CHAR *pCVar14;
  char *local_110;
  CHAR aCStack_104 [36];
  undefined4 auStack_e0 [56];
  
  piVar6 = *(int **)(DAT_0044de08 + 4);
  iVar10 = 0;
  DAT_00514eb0 = 0;
  DAT_005148b8 = 0;
  DAT_00514eac = 0;
  DAT_00514ea4 = 0;
  DAT_00514ea8 = 0;
  DAT_0044656c = 0xffffffff;
  LoadSpudSkateTimingData();
  puVar9 = &DAT_004fc084;
  for (iVar7 = 0x14; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  DAT_00519940 = UnloadSpudSkateActivityResources;
  DAT_00514ea0 = LoadBitmapToDirectDrawSurface
                           (piVar6,s_Data_SubGameSpudSkate_MINIMISED__00446754,0,0);
  RegisterBitmapSurface(&DAT_00514ea0,s_Data_SubGameSpudSkate_minimised__00446730);
  MarkRegisteredSurfaceColorKeyed(0x514ea0);
  SetSurfaceTransparencyColorKey(DAT_00514ea0,0xff00ff);
  DAT_005148b4 = LoadBitmapToDirectDrawSurface
                           (piVar6,s_Data_SubGameSpudSkate_skatespuds_00446704,0,0);
  RegisterBitmapSurface(&DAT_005148b4,s_Data_SubGameSpudSkate_skatespuds_00446704);
  MarkRegisteredSurfaceColorKeyed(0x5148b4);
  SetSurfaceTransparencyColorKey(DAT_005148b4,0xff00ff);
  DAT_00514b60 = LoadBitmapToDirectDrawSurface(piVar6,s_Data_SubGameSpudSkate_1_bmp_004466e8,0,0);
  RegisterBitmapSurface(&DAT_00514b60,s_Data_SubGameSpudSkate_1_bmp_004466e8);
  MarkRegisteredSurfaceColorKeyed(0x514b60);
  SetSurfaceTransparencyColorKey(DAT_00514b60,0xff00ff);
  DAT_00514b64 = LoadBitmapToDirectDrawSurface(piVar6,s_Data_SubGameSpudSkate_2_bmp_004466cc,0,0);
  RegisterBitmapSurface(&DAT_00514b64,s_Data_SubGameSpudSkate_2_bmp_004466cc);
  MarkRegisteredSurfaceColorKeyed(0x514b64);
  SetSurfaceTransparencyColorKey(DAT_00514b64,0xff00ff);
  DAT_00514b68 = LoadBitmapToDirectDrawSurface(piVar6,s_Data_SubGameSpudSkate_3_bmp_004466b0,0,0);
  RegisterBitmapSurface(&DAT_00514b68,s_Data_SubGameSpudSkate_3_bmp_004466b0);
  MarkRegisteredSurfaceColorKeyed(0x514b68);
  SetSurfaceTransparencyColorKey(DAT_00514b68,0xff00ff);
  pcVar2 = (char *)_malloc(7000000);
  puVar9 = &DAT_005148cc;
  local_110 = s_Data_SubGameSpudSkate_bad_bik_00446068;
  do {
    pFVar3 = (FILE *)OpenGameDataFileWithCDFallback(local_110,&DAT_00441f40);
    uVar4 = FUN_00430937(pcVar2,1,7000000,(int *)pFVar3);
    pcVar5 = (char *)_malloc(uVar4);
    *(char **)((int)&DAT_00514620 + iVar10) = pcVar5;
    pcVar11 = pcVar2;
    for (uVar8 = uVar4 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
      *(undefined4 *)pcVar5 = *(undefined4 *)pcVar11;
      pcVar11 = pcVar11 + 4;
      pcVar5 = pcVar5 + 4;
    }
    puVar1 = puVar9 + -3;
    for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *pcVar5 = *pcVar11;
      pcVar11 = pcVar11 + 1;
      pcVar5 = pcVar5 + 1;
    }
    piVar6 = *(int **)(DAT_0044de08 + 4);
    puVar13 = puVar1;
    for (iVar7 = 0x1f; iVar7 != 0; iVar7 = iVar7 + -1) {
      *puVar13 = 0;
      puVar13 = puVar13 + 1;
    }
    *puVar1 = 0x7c;
    puVar9[-2] = 7;
    *puVar9 = 600;
    puVar9[-1] = 0x17c;
    puVar9[0x17] = 0x40;
    (**(code **)(*piVar6 + 0x18))(piVar6,puVar1,(int)&DAT_00514e8c + iVar10,0);
    iVar7 = OpenBinkMovieWithFallback(*(char **)((int)&DAT_00514620 + iVar10));
    *(int *)((int)&DAT_00514b2c + iVar10) = iVar7;
    crt_fclose(pFVar3);
    local_110 = local_110 + 0x80;
    puVar9 = puVar9 + 0x1f;
    iVar10 = iVar10 + 4;
  } while ((int)local_110 < 0x4462e8);
  FUN_00430d2a(pcVar2);
  puVar9 = &DAT_00514634;
  for (iVar7 = 0xa0; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar9 = 0xffffffff;
    puVar9 = puVar9 + 1;
  }
  pcVar2 = s_data_subgamespudskate_soundinfo__0044668c;
  pCVar14 = aCStack_104;
  for (iVar7 = 9; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pCVar14 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pCVar14 = pCVar14 + 4;
  }
  puVar9 = auStack_e0;
  for (iVar7 = 0x38; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  piVar6 = (int *)OpenGameDataFileWithCDFallback(aCStack_104,&DAT_0043e070);
  puVar12 = &DAT_00514640;
  do {
    iVar7 = 4;
    do {
      crt_fscanf(puVar12 + -8,piVar6,(byte *)s__d__d__d__d__d_0043e040);
      puVar12 = puVar12 + 0x14;
      iVar7 = iVar7 + -1;
    } while (iVar7 != 0);
  } while ((int)puVar12 < 0x5148c0);
  puVar9 = &DAT_00514b40;
  this = extraout_ECX;
  do {
    crt_fscanf(this,piVar6,&DAT_0043e8e4);
    puVar9 = puVar9 + 1;
    this = extraout_ECX_00;
  } while ((int)puVar9 < 0x514b60);
  DAT_0051c30c = 1;
  return;
}

