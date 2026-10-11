/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004115c0; function: LoadFireworkMovieBank; body bytes: 1355
 * callers: 1; callees: 10; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void LoadFireworkMovieBank(void)

{
  int *piVar1;
  char *pcVar2;
  FILE *pFVar3;
  uint uVar4;
  char *pcVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  char *pcVar9;
  CHAR *pCVar10;
  undefined4 *puVar11;
  undefined4 *puVar12;
  CHAR local_b80 [46];
  undefined4 local_b52 [20];
  undefined4 local_b00 [11];
  undefined4 local_ad2 [20];
  char local_a80 [45];
  undefined4 local_a53;
  undefined4 local_a00;
  undefined4 local_9d5 [21];
  undefined4 local_980 [10];
  undefined4 local_956 [21];
  undefined4 local_900;
  undefined4 local_8d1 [20];
  char local_880 [45];
  undefined4 local_853;
  undefined4 local_800;
  undefined4 local_7d1 [20];
  char local_780 [41];
  undefined4 local_757;
  undefined4 local_700 [10];
  undefined4 local_6d8 [22];
  undefined4 local_680 [10];
  undefined4 local_656 [21];
  undefined4 local_600 [10];
  undefined4 local_5d6 [21];
  undefined4 local_580;
  undefined4 local_551 [20];
  undefined4 local_500;
  undefined4 local_4d1 [20];
  undefined4 local_480 [11];
  undefined4 local_452 [20];
  undefined4 local_400 [11];
  undefined4 local_3d4 [21];
  undefined4 local_380;
  undefined4 local_355 [21];
  undefined4 local_300 [12];
  undefined4 local_2d0 [20];
  undefined4 local_280 [11];
  undefined4 local_252 [20];
  undefined4 local_200 [12];
  undefined4 local_1d0 [20];
  char local_180 [41];
  undefined4 local_157;
  char local_100 [49];
  undefined4 local_cf;
  undefined4 local_80 [12];
  undefined4 local_50 [20];
  
  iVar8 = 0;
  DAT_0050ab78 = 0;
  piVar1 = *(int **)(DAT_0044de08 + 4);
  DAT_0050a4b4 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_minimised_b_00443068,0,0);
  RegisterBitmapSurface(&DAT_0050a4b4,s_Data_SubGameFirework_minimised_b_00443068);
  DAT_0050a5cc = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_certificate_00443040,0,0);
  RegisterBitmapSurface(&DAT_0050a5cc,s_Data_SubGameFirework_certificate_00443040);
  DAT_0050ab6c = 0;
  MarkRegisteredSurfaceColorKeyed(0x50a4b4);
  SetSurfaceTransparencyColorKey(DAT_0050a4b4,0xff00ff);
  pcVar2 = s_Data_SubGameFirework_graph_airbo_00443010;
  pCVar10 = local_b80;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *(undefined4 *)pCVar10 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pCVar10 = pCVar10 + 4;
  }
  *(undefined2 *)pCVar10 = *(undefined2 *)pcVar2;
  puVar11 = local_b52;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_small_00442fe0;
  puVar11 = local_b00;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  puVar11 = local_ad2;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_mediu_00442fb0;
  pcVar9 = local_a80;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar2;
  puVar11 = &local_a53;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  *(undefined1 *)((int)puVar11 + 2) = 0;
  pcVar2 = s_Data_SubGameFirework_graph_bigbl_00442f84;
  puVar11 = &local_a00;
  for (iVar6 = 10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  *(char *)((int)puVar11 + 2) = pcVar2[2];
  puVar11 = local_9d5;
  for (iVar6 = 0x15; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined1 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_bigre_00442f58;
  puVar11 = local_980;
  for (iVar6 = 10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  puVar11 = local_956;
  for (iVar6 = 0x15; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_Mediu_00442f28;
  puVar11 = &local_900;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  *(char *)((int)puVar11 + 2) = pcVar2[2];
  puVar11 = local_8d1;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined1 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_small_00442ef8;
  pcVar9 = local_880;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar2;
  puVar11 = &local_853;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  *(undefined1 *)((int)puVar11 + 2) = 0;
  pcVar2 = s_Data_SubGameFirework_graph_airbo_00442ec8;
  puVar11 = &local_800;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  *(char *)((int)puVar11 + 2) = pcVar2[2];
  puVar11 = local_7d1;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined1 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_Candl_00442e9c;
  pcVar9 = local_780;
  for (iVar6 = 10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar2;
  puVar11 = &local_757;
  for (iVar6 = 0x15; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  *(undefined1 *)((int)puVar11 + 2) = 0;
  pcVar2 = s_Data_SubGameFirework_graph_Wheel_00442e74;
  puVar11 = local_700;
  for (iVar6 = 10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  puVar11 = local_6d8;
  for (iVar6 = 0x16; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  pcVar2 = s_Data_SubGameFirework_graph_Wheel_00442e48;
  puVar11 = local_680;
  for (iVar6 = 10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  puVar11 = local_656;
  for (iVar6 = 0x15; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_Candl_00442e1c;
  puVar11 = local_600;
  for (iVar6 = 10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  puVar11 = local_5d6;
  for (iVar6 = 0x15; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_airbo_00442dec;
  puVar11 = &local_580;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  *(char *)((int)puVar11 + 2) = pcVar2[2];
  puVar11 = local_551;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined1 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_small_00442dbc;
  puVar11 = &local_500;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  *(char *)((int)puVar11 + 2) = pcVar2[2];
  puVar11 = local_4d1;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined1 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_mediu_00442d8c;
  puVar11 = local_480;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  puVar11 = local_452;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_bigbl_00442d60;
  puVar11 = local_400;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  puVar11 = local_3d4;
  for (iVar6 = 0x15; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  pcVar2 = s_Data_SubGameFirework_graph_bigre_00442d34;
  puVar11 = &local_380;
  for (iVar6 = 10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  *(char *)((int)puVar11 + 2) = pcVar2[2];
  puVar11 = local_355;
  for (iVar6 = 0x15; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined1 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_Mediu_00442d04;
  puVar11 = local_300;
  for (iVar6 = 0xc; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  puVar11 = local_2d0;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  pcVar2 = s_Data_SubGameFirework_graph_small_00442cd4;
  puVar11 = local_280;
  for (iVar6 = 0xb; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = *(undefined2 *)pcVar2;
  puVar11 = local_252;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  pcVar2 = s_Data_SubGameFirework_graph_airbo_00442ca4;
  puVar11 = local_200;
  for (iVar6 = 0xc; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  puVar11 = local_1d0;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  pcVar2 = s_Data_SubGameFirework_graph_topmi_00442c78;
  pcVar9 = local_180;
  for (iVar6 = 10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar2;
  puVar11 = &local_157;
  for (iVar6 = 0x15; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  *(undefined1 *)((int)puVar11 + 2) = 0;
  pcVar2 = s_Data_SubGameFirework_graph_firew_00442c44;
  pcVar9 = local_100;
  for (iVar6 = 0xc; iVar6 != 0; iVar6 = iVar6 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar2;
  puVar11 = &local_cf;
  for (iVar6 = 0x13; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  *(undefined2 *)puVar11 = 0;
  *(undefined1 *)((int)puVar11 + 2) = 0;
  pcVar2 = s_Data_SubGameFirework_graph_firew_00442c14;
  puVar11 = local_80;
  for (iVar6 = 0xc; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    puVar11 = puVar11 + 1;
  }
  puVar11 = local_50;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  pcVar2 = (char *)_malloc(1500000);
  do {
    piVar1 = *(int **)(DAT_0044de08 + 4);
    pFVar3 = (FILE *)OpenGameDataFileWithCDFallback(local_b80 + iVar8 * 0x80,&DAT_00441f40);
    uVar4 = FUN_00430937(pcVar2,1,1500000,(int *)pFVar3);
    pcVar5 = (char *)_malloc(uVar4);
    (&DAT_0050a50c)[iVar8] = pcVar5;
    pcVar9 = pcVar2;
    for (uVar7 = uVar4 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
      *(undefined4 *)pcVar5 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar5 = pcVar5 + 4;
    }
    for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *pcVar5 = *pcVar9;
      pcVar9 = pcVar9 + 1;
      pcVar5 = pcVar5 + 1;
    }
    puVar11 = &DAT_00509970 + iVar8 * 0x1f;
    puVar12 = puVar11;
    for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar12 = 0;
      puVar12 = puVar12 + 1;
    }
    *puVar11 = 0x7c;
    (&DAT_00509974)[iVar8 * 0x1f] = 7;
    if (iVar8 < 8) {
      (&DAT_0050997c)[iVar8 * 0x1f] = 0xdc;
      (&DAT_00509978)[iVar8 * 0x1f] = 0x154;
    }
    else if (iVar8 < 0xc) {
      (&DAT_0050997c)[iVar8 * 0x1f] = 0xdc;
LAB_00411aa3:
      (&DAT_00509978)[iVar8 * 0x1f] = 0x8c;
    }
    else if (iVar8 < 0x14) {
      (&DAT_0050997c)[iVar8 * 0x1f] = 200;
      (&DAT_00509978)[iVar8 * 0x1f] = 0x154;
    }
    else {
      if (iVar8 != 0x14) {
        (&DAT_0050997c)[iVar8 * 0x1f] = 0x280;
        goto LAB_00411aa3;
      }
      _DAT_0050a32c = 0xdc;
      _DAT_0050a328 = 200;
    }
    (&DAT_005099d8)[iVar8 * 0x1f] = 0x40;
    (**(code **)(*piVar1 + 0x18))(piVar1,puVar11,&DAT_0050937c + iVar8,0);
    iVar6 = OpenBinkMovieWithFallback((char *)(&DAT_0050a50c)[iVar8]);
    (&DAT_0050aaac)[iVar8] = iVar6;
    crt_fclose(pFVar3);
    iVar8 = iVar8 + 1;
    if (0x16 < iVar8) {
      FUN_00430d2a(pcVar2);
      return;
    }
  } while( true );
}

