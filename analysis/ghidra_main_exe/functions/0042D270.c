/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042d270; function: InitializeAndLoadPlayerProfiles; body bytes: 2297
 * callers: 1; callees: 9; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeAndLoadPlayerProfiles(void)

{
  int *piVar1;
  FILE *pFVar2;
  int *piVar3;
  int iVar4;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *extraout_ECX_01;
  void *extraout_ECX_02;
  void *extraout_ECX_03;
  void *pvVar5;
  void *extraout_ECX_04;
  char *pcVar6;
  undefined4 *puVar7;
  undefined *puVar8;
  char *pcVar9;
  CHAR *pCVar10;
  undefined4 *local_3ec;
  CHAR local_3e8 [29];
  undefined4 local_3cb;
  undefined4 local_3c7;
  undefined4 local_3c3;
  undefined4 local_3bf;
  undefined4 local_3bb;
  undefined1 local_3b7;
  char local_3b6 [29];
  undefined4 local_399;
  undefined4 local_395;
  undefined4 local_391;
  undefined4 local_38d;
  undefined4 local_389;
  undefined1 local_385;
  undefined4 local_384 [7];
  undefined4 local_366;
  undefined4 local_362;
  undefined4 local_35e;
  undefined4 local_35a;
  undefined4 local_356;
  undefined4 local_352 [7];
  undefined4 local_334;
  undefined4 local_330;
  undefined4 local_32c;
  undefined4 local_328;
  undefined4 local_324;
  char local_320 [29];
  undefined4 local_303;
  undefined4 local_2ff;
  undefined4 local_2fb;
  undefined4 local_2f7;
  undefined4 local_2f3;
  undefined1 local_2ef;
  char local_2ee [29];
  undefined4 local_2d1;
  undefined4 local_2cd;
  undefined4 local_2c9;
  undefined4 local_2c5;
  undefined4 local_2c1;
  undefined1 local_2bd;
  undefined4 local_2bc [7];
  undefined4 local_29e;
  undefined4 local_29a;
  undefined4 local_296;
  undefined4 local_292;
  undefined4 local_28e;
  undefined4 local_28a [7];
  undefined4 local_26c;
  undefined4 local_268;
  undefined4 local_264;
  undefined4 local_260;
  undefined4 local_25c;
  undefined4 local_258 [5];
  undefined4 local_244 [7];
  undefined4 local_226;
  undefined4 local_20f;
  undefined4 local_1f4;
  undefined4 local_1d5;
  undefined4 local_1d1;
  undefined4 local_1cd;
  undefined4 local_1c9;
  undefined2 local_1c5;
  undefined1 local_1c3;
  undefined4 local_1c2;
  undefined4 local_1a3;
  undefined4 local_19f;
  undefined4 local_19b;
  undefined4 local_197;
  undefined2 local_193;
  undefined1 local_191;
  undefined4 local_190 [5];
  undefined4 local_17c [7];
  undefined4 local_15e;
  undefined4 local_147;
  undefined4 local_12c;
  undefined4 local_10d;
  undefined4 local_109;
  undefined4 local_105;
  undefined4 local_101;
  undefined2 local_fd;
  undefined1 local_fb;
  undefined4 local_fa;
  undefined4 local_db;
  undefined4 local_d7;
  undefined4 local_d3;
  undefined4 local_cf;
  undefined2 local_cb;
  undefined1 local_c9;
  undefined4 local_c8 [7];
  undefined4 local_aa;
  undefined4 local_a6;
  undefined4 local_a2;
  undefined4 local_9e;
  undefined4 local_9a;
  undefined4 local_96 [7];
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  char local_64 [29];
  undefined4 local_47;
  undefined4 local_43;
  undefined4 local_3f;
  undefined4 local_3b;
  undefined4 local_37;
  undefined1 local_33;
  char local_32 [29];
  undefined4 local_15;
  undefined4 local_11;
  undefined4 local_d;
  undefined4 local_9;
  undefined4 local_5;
  undefined1 local_1;
  
  piVar3 = *(int **)(DAT_0044de08 + 4);
  DAT_0051c248 = 0;
  DAT_0051be48 = 0;
  DAT_0051b39c = LoadBitmapToDirectDrawSurface
                           (piVar3,s_Data_ui_blue_name_boxed_font_bmp_00447508,0,0);
  RegisterBitmapSurface(&DAT_0051b39c,s_Data_ui_blue_name_boxed_font_bmp_00447508);
  DAT_0051b3a4 = LoadBitmapToDirectDrawSurface
                           (piVar3,s_Data_ui_enter_name_boxed_font_bm_004474e4,0,0);
  RegisterBitmapSurface(&DAT_0051b3a4,s_Data_ui_enter_name_boxed_font_bm_004474e4);
  DAT_0051993c = LoadBitmapToDirectDrawSurface
                           (piVar3,s_Data_ui_certificate_boxed_font_b_004474c0,0,0);
  RegisterBitmapSurface(&DAT_0051993c,s_Data_ui_certificate_boxed_font_b_004474c0);
  DAT_00519958 = LoadBitmapToDirectDrawSurface(piVar3,s_Data_ui_tool_icons_bmp_004474a8,0,0);
  RegisterBitmapSurface(&DAT_00519958,s_Data_ui_tool_icons_bmp_004474a8);
  DAT_0051b398 = LoadBitmapToDirectDrawSurface(piVar3,s_Data_ui_fonttools_bmp_00447490,0,0);
  RegisterBitmapSurface(&DAT_0051b398,s_Data_ui_fonttools_bmp_00447490);
  DAT_0051c260 = LoadBitmapToDirectDrawSurface(piVar3,s_Data_ui_Hourglass_bmp_00447478,0,0);
  RegisterBitmapSurface(&DAT_0051c260,s_Data_ui_hourglass_bmp_00447460);
  MarkRegisteredSurfaceColorKeyed(0x51c260);
  SetSurfaceTransparencyColorKey(DAT_0051c260,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51b39c);
  SetSurfaceTransparencyColorKey(DAT_0051b39c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51993c);
  SetSurfaceTransparencyColorKey(DAT_0051993c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51b3a4);
  SetSurfaceTransparencyColorKey(DAT_0051b3a4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x519958);
  SetSurfaceTransparencyColorKey(DAT_00519958,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51b398);
  SetSurfaceTransparencyColorKey(DAT_0051b398,0xff00ff);
  DAT_00519938 = LoadBitmapToDirectDrawSurface(piVar3,s_Data_subgamedyp_delcurs_bmp_00447444,0,0);
  RegisterBitmapSurface(&DAT_00519938,s_Data_subgamedyp_delcurs_bmp_00447444);
  MarkRegisteredSurfaceColorKeyed(0x519938);
  SetSurfaceTransparencyColorKey(DAT_00519938,0xff00ff);
  pcVar6 = s_Data_ui_enternameleftred_bmp_00447424;
  pCVar10 = local_3e8;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pCVar10 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pCVar10 = pCVar10 + 4;
  }
  *pCVar10 = *pcVar6;
  local_3cb = 0;
  local_3c7 = 0;
  local_3c3 = 0;
  local_3bf = 0;
  local_3bb = 0;
  local_3b7 = 0;
  pcVar6 = s_Data_ui_enternameleftdep_bmp_00447404;
  pcVar9 = local_3b6;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar6;
  local_399 = 0;
  local_395 = 0;
  local_391 = 0;
  local_38d = 0;
  local_389 = 0;
  local_385 = 0;
  pcVar6 = s_Data_ui_enternamerightred_bmp_004473e4;
  puVar7 = local_384;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  local_366 = 0;
  local_362 = 0;
  local_35e = 0;
  local_35a = 0;
  local_356 = 0;
  pcVar6 = s_Data_ui_enternamerightdep_bmp_004473c4;
  puVar7 = local_352;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  local_334 = 0;
  local_330 = 0;
  local_32c = 0;
  local_328 = 0;
  local_324 = 0;
  pcVar6 = s_Data_ui_enternamebackred_bmp_004473a4;
  pcVar9 = local_320;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar6;
  local_303 = 0;
  local_2ff = 0;
  local_2fb = 0;
  local_2f7 = 0;
  local_2f3 = 0;
  local_2ef = 0;
  pcVar6 = s_Data_ui_enternamebackdep_bmp_00447384;
  pcVar9 = local_2ee;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar6;
  local_2d1 = 0;
  local_2cd = 0;
  local_2c9 = 0;
  local_2c5 = 0;
  local_2c1 = 0;
  local_2bd = 0;
  pcVar6 = s_Data_ui_enternamestartred_bmp_00447364;
  puVar7 = local_2bc;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  local_29e = 0;
  local_29a = 0;
  local_296 = 0;
  local_292 = 0;
  local_28e = 0;
  pcVar6 = s_Data_ui_enternamestartdep_bmp_00447344;
  puVar7 = local_28a;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  local_26c = 0;
  local_268 = 0;
  local_264 = 0;
  local_260 = 0;
  local_25c = 0;
  pcVar6 = s_Data_ui_helpred_bmp_00447330;
  puVar7 = local_258;
  for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  puVar7 = local_244;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  pcVar6 = s_Data_ui_helpreddep_bmp_00447318;
  puVar7 = &local_226;
  for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  *(char *)((int)puVar7 + 2) = pcVar6[2];
  puVar7 = &local_20f;
  for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  *(undefined1 *)((int)puVar7 + 2) = 0;
  pcVar6 = s_Data_ui_enternamedeletered_bmp_004472f8;
  puVar7 = &local_1f4;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  *(char *)((int)puVar7 + 2) = pcVar6[2];
  local_1d5 = 0;
  local_1d1 = 0;
  local_1cd = 0;
  local_1c9 = 0;
  local_1c5 = 0;
  local_1c3 = 0;
  pcVar6 = s_Data_ui_enternamedeletedep_bmp_004472d8;
  puVar7 = &local_1c2;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  *(char *)((int)puVar7 + 2) = pcVar6[2];
  local_1a3 = 0;
  local_19f = 0;
  local_19b = 0;
  local_197 = 0;
  local_193 = 0;
  local_191 = 0;
  pcVar6 = s_Data_ui_backred_bmp_004472c4;
  puVar7 = local_190;
  for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  puVar7 = local_17c;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  pcVar6 = s_Data_ui_backreddep_bmp_004472ac;
  puVar7 = &local_15e;
  for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  *(char *)((int)puVar7 + 2) = pcVar6[2];
  puVar7 = &local_147;
  for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  *(undefined1 *)((int)puVar7 + 2) = 0;
  pcVar6 = s_Data_ui_playagain_payessel_bmp_0044728c;
  puVar7 = &local_12c;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  *(char *)((int)puVar7 + 2) = pcVar6[2];
  local_10d = 0;
  local_109 = 0;
  local_105 = 0;
  local_101 = 0;
  local_fd = 0;
  local_fb = 0;
  pcVar6 = s_Data_ui_playagain_payesdep_bmp_0044726c;
  puVar7 = &local_fa;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  *(char *)((int)puVar7 + 2) = pcVar6[2];
  local_db = 0;
  local_d7 = 0;
  local_d3 = 0;
  local_cf = 0;
  local_cb = 0;
  local_c9 = 0;
  pcVar6 = s_Data_ui_playagain_panosel_bmp_0044724c;
  puVar7 = local_c8;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  local_aa = 0;
  local_a6 = 0;
  local_a2 = 0;
  local_9e = 0;
  local_9a = 0;
  pcVar6 = s_Data_ui_playagain_panodep_bmp_0044722c;
  puVar7 = local_96;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  local_78 = 0;
  local_74 = 0;
  local_70 = 0;
  local_6c = 0;
  local_68 = 0;
  pcVar6 = s_Data_ui_options_optoksel_bmp_0044720c;
  pcVar9 = local_64;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar6;
  local_47 = 0;
  local_43 = 0;
  local_3f = 0;
  local_3b = 0;
  local_37 = 0;
  local_33 = 0;
  pcVar6 = s_Data_ui_options_optokdep_bmp_004471ec;
  pcVar9 = local_32;
  for (iVar4 = 7; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar9 = pcVar9 + 4;
  }
  *pcVar9 = *pcVar6;
  local_15 = 0;
  pCVar10 = local_3e8;
  local_11 = 0;
  puVar7 = &DAT_0051b3a8;
  local_d = 0;
  local_9 = 0;
  local_5 = 0;
  local_1 = 0;
  do {
    iVar4 = 2;
    do {
      piVar1 = LoadBitmapToDirectDrawSurface(piVar3,pCVar10,0,0);
      *puVar7 = piVar1;
      RegisterBitmapSurface(puVar7,pCVar10);
      MarkRegisteredSurfaceColorKeyed((int)puVar7);
      SetSurfaceTransparencyColorKey((int *)*puVar7,0xff00ff);
      pCVar10 = pCVar10 + 0x32;
      puVar7 = puVar7 + 1;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  } while ((int)puVar7 < 0x51b3f8);
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_loaddata_fontdatabrown_txt_004471d0,&DAT_0043e070);
  puVar8 = &DAT_00517958;
  do {
    iVar4 = crt_fscanf(puVar8 + 4,(int *)pFVar2,(byte *)s__d__d__d__d_00444d88);
    puVar8 = puVar8 + 0x10;
    if (iVar4 == -1) break;
  } while ((int)puVar8 < 0x518948);
  crt_fclose(pFVar2);
  piVar3 = (int *)OpenGameDataFileWithCDFallback(s_loaddata_fontdatablue_txt_004471b4,&DAT_0043e070)
  ;
  puVar8 = &DAT_00516968;
  do {
    iVar4 = crt_fscanf(puVar8 + 4,piVar3,(byte *)s__d__d__d__d_00444d88);
    puVar8 = puVar8 + 0x10;
    if (iVar4 == -1) break;
  } while ((int)puVar8 < 0x517958);
  piVar3 = (int *)OpenGameDataFileWithCDFallback
                            (s_loaddata_fontdatacertificate_txt_00447190,&DAT_0043e070);
  puVar8 = &DAT_00518948;
  do {
    iVar4 = crt_fscanf(puVar8 + 4,piVar3,(byte *)s__d__d__d__d_00444d88);
    puVar8 = puVar8 + 0x10;
    if (iVar4 == -1) break;
  } while ((int)puVar8 < 0x519938);
  DAT_00519930 = 0;
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_ascii_txt_0044717c,&DAT_0043e070);
  puVar8 = &DAT_0051c014;
  pvVar5 = extraout_ECX;
  do {
    iVar4 = crt_fscanf(pvVar5,(int *)pFVar2,&DAT_0043e8e4);
    puVar8 = puVar8 + 4;
    DAT_00519930 = DAT_00519930 + 1;
    if (iVar4 == -1) break;
    pvVar5 = extraout_ECX_00;
  } while ((int)puVar8 < 0x51c248);
  crt_fclose(pFVar2);
  DAT_0051c24c = 0xffffffff;
  DAT_0051b404 = 0;
  DAT_0051c250 = 0xffffffff;
  DAT_0051b408 = 0;
  _DAT_0051c254 = 0xffffffff;
  DAT_0051b40c = 0;
  _DAT_0051c258 = 0xffffffff;
  _DAT_0051b410 = 0;
  _DAT_0051c25c = 0xffffffff;
  _DAT_0051b414 = 0;
  pFVar2 = (FILE *)crt_fopen(s_playerinfo_txt_0044716c,&DAT_0043e070);
  if (pFVar2 != (FILE *)0x0) {
    iVar4 = 0;
    do {
      crt_fscanf((void *)((int)&DAT_0051b404 + iVar4),(int *)pFVar2,(byte *)s__d__d_00447164);
      iVar4 = iVar4 + 4;
    } while (iVar4 < 0x14);
    piVar3 = &DAT_0051b404;
    pvVar5 = extraout_ECX_01;
    do {
      iVar4 = 0;
      if (0 < *piVar3) {
        do {
          crt_fscanf(pvVar5,(int *)pFVar2,&DAT_00444240);
          iVar4 = iVar4 + 1;
          pvVar5 = extraout_ECX_02;
        } while (iVar4 < *piVar3);
      }
      piVar3 = piVar3 + 1;
    } while ((int)piVar3 < 0x51b418);
    crt_fclose(pFVar2);
  }
  puVar7 = &DAT_0051b4d0;
  for (iVar4 = 500; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  puVar7 = &DAT_0051b4d0;
  do {
    *puVar7 = 0;
    puVar7 = puVar7 + 100;
  } while ((int)puVar7 < 0x51bca0);
  local_3ec = &DAT_0051b4d0;
  do {
    crt_sprintf(&DAT_0051bd30,(byte *)s_player_d_txt_00447154);
    pFVar2 = (FILE *)crt_fopen(&DAT_0051bd30,&DAT_0043e070);
    if (pFVar2 == (FILE *)0x0) {
      *local_3ec = 0xffffffff;
    }
    else {
      iVar4 = 100;
      pvVar5 = extraout_ECX_03;
      do {
        crt_fscanf(pvVar5,(int *)pFVar2,&DAT_00444240);
        iVar4 = iVar4 + -1;
        pvVar5 = extraout_ECX_04;
      } while (iVar4 != 0);
      crt_fclose(pFVar2);
    }
    local_3ec = local_3ec + 100;
  } while ((int)local_3ec < 0x51bca0);
  return;
}

