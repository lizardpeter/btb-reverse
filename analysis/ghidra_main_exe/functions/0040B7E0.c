/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040b7e0; function: InitializeParkDesignerActivity; body bytes: 6630
 * callers: 1; callees: 6; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeParkDesignerActivity(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  char cVar8;
  int *piVar9;
  char *pcVar10;
  CHAR *pCVar11;
  char *pcVar12;
  int iVar13;
  int local_628;
  int local_624;
  int aiStack_61c [16];
  CHAR local_5dc [28];
  undefined4 local_5c0;
  undefined4 local_5bc;
  undefined4 local_5b8;
  undefined4 local_5b4;
  undefined4 local_5b0;
  undefined2 local_5ac;
  undefined4 local_5aa [7];
  undefined4 local_58e;
  undefined4 local_58a;
  undefined4 local_586;
  undefined4 local_582;
  undefined4 local_57e;
  undefined2 local_57a;
  undefined4 local_578 [7];
  undefined4 local_55c;
  undefined4 local_558;
  undefined4 local_554;
  undefined4 local_550;
  undefined4 local_54c;
  undefined2 local_548;
  undefined4 local_546 [7];
  undefined4 local_52a;
  undefined4 local_526;
  undefined4 local_522;
  undefined4 local_51e;
  undefined4 local_51a;
  undefined2 local_516;
  undefined4 local_514 [8];
  undefined4 local_4f4;
  undefined4 local_4f0;
  undefined4 local_4ec;
  undefined4 local_4e8;
  undefined2 local_4e4;
  undefined4 local_4e2 [8];
  undefined4 local_4c2;
  undefined4 local_4be;
  undefined4 local_4ba;
  undefined4 local_4b6;
  undefined2 local_4b2;
  char local_4b0 [29];
  undefined4 local_493;
  undefined4 local_48f;
  undefined4 local_48b;
  undefined4 local_487;
  undefined4 local_483;
  undefined1 local_47f;
  undefined4 local_47e [7];
  undefined4 local_462;
  undefined4 local_45e;
  undefined4 local_45a;
  undefined4 local_456;
  undefined4 local_452;
  undefined2 local_44e;
  undefined4 local_44c [7];
  undefined4 local_42e;
  undefined4 local_42a;
  undefined4 local_426;
  undefined4 local_422;
  undefined4 local_41e;
  undefined4 local_41a [7];
  undefined4 local_3fc;
  undefined4 local_3f8;
  undefined4 local_3f4;
  undefined4 local_3f0;
  undefined4 local_3ec;
  CHAR local_3e8 [33];
  undefined4 local_3c7;
  undefined4 local_3c3;
  undefined4 local_3bf;
  undefined4 local_3bb;
  undefined1 local_3b7;
  char local_3b6 [33];
  undefined4 local_395;
  undefined4 local_391;
  undefined4 local_38d;
  undefined4 local_389;
  undefined1 local_385;
  undefined4 local_384;
  undefined4 local_361;
  undefined4 local_35d;
  undefined4 local_359;
  undefined2 local_355;
  undefined1 local_353;
  CHAR local_350 [2];
  CHAR aCStack_34e [25];
  undefined4 local_335;
  undefined4 local_331;
  undefined4 local_32d;
  undefined4 local_329;
  undefined4 local_325;
  undefined2 local_321;
  undefined1 local_31f;
  undefined4 local_31e [7];
  undefined4 local_300;
  undefined4 local_2fc;
  undefined4 local_2f8;
  undefined4 local_2f4;
  undefined4 local_2f0;
  undefined4 local_2ec [7];
  undefined4 local_2ce;
  undefined4 local_2ca;
  undefined4 local_2c6;
  undefined4 local_2c2;
  undefined4 local_2be;
  undefined4 local_2ba;
  undefined4 local_29f;
  undefined4 local_29b;
  undefined4 local_297;
  undefined4 local_293;
  undefined4 local_28f;
  undefined2 local_28b;
  undefined1 local_289;
  undefined4 local_288 [7];
  undefined4 local_26a;
  undefined4 local_266;
  undefined4 local_262;
  undefined4 local_25e;
  undefined4 local_25a;
  undefined4 local_256 [7];
  undefined4 local_238;
  undefined4 local_234;
  undefined4 local_230;
  undefined4 local_22c;
  undefined4 local_228;
  undefined4 local_224 [6];
  undefined4 local_20a [6];
  char local_1f2 [29];
  undefined4 local_1d5;
  undefined4 local_1d1;
  undefined4 local_1cd;
  undefined4 local_1c9;
  undefined4 local_1c5;
  undefined1 local_1c1;
  char local_1c0 [29];
  undefined4 local_1a3;
  undefined4 local_19f;
  undefined4 local_19b;
  undefined4 local_197;
  undefined4 local_193;
  undefined1 local_18f;
  undefined4 local_18e [7];
  undefined4 local_170;
  undefined4 local_16c;
  undefined4 local_168;
  undefined4 local_164;
  undefined4 local_160;
  char local_15c [33];
  undefined4 local_13b;
  undefined4 local_137;
  undefined4 local_133;
  undefined4 local_12f;
  undefined1 local_12b;
  char local_12a [33];
  undefined4 local_109;
  undefined4 local_105;
  undefined4 local_101;
  undefined4 local_fd;
  undefined1 local_f9;
  undefined1 local_f8 [8];
  int iStack_f0;
  int iStack_ec;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  
  DAT_0050936c = 1;
  DAT_00446ce8 = 4;
  _DAT_00508c08 = 0;
  DAT_0050418c = 0xffffffff;
  DAT_00507b60 = 0xffffffff;
  DAT_00509358 = 0;
  DAT_00509364 = 0;
  DAT_00509368 = 0;
  DAT_00509348 = 0;
  piVar9 = *(int **)(DAT_0044de08 + 4);
  DAT_0051c2dc = 0;
  DAT_00519940 = UnloadParkDesignerActivityResources;
  DAT_005107d0 = 0;
  DAT_00507a20 = 0;
  DAT_00507a24 = 0;
  DAT_00507a00 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_cursor_bmp_004425fc,0,0);
  RegisterBitmapSurface(&DAT_00507a00,s_Data_SubGameDYP_cursor_bmp_004425fc);
  MarkRegisteredSurfaceColorKeyed(0x507a00);
  SetSurfaceTransparencyColorKey(DAT_00507a00,0xff00ff);
  puVar3 = &DAT_004fcae8;
  do {
    puVar3[-7] = 0xffffffff;
    *puVar3 = 0xffffffff;
    puVar3 = puVar3 + 0x13;
  } while ((int)puVar3 < 0x5041a8);
  DAT_00507a2c = 0xffffffff;
  DAT_00507a30 = 0xffffffff;
  _DAT_00507a34 = 0xffffffff;
  DAT_00509340 = 0;
  DAT_00441dc8 = 300;
  DAT_00441dcc = 200;
  _DAT_00507a38 = 0xffffffff;
  DAT_00507c54 = 0xffffffff;
  DAT_00507a28 = 0xffffffff;
  DAT_00507e3c = 0xffffffff;
  DAT_005079fc = 0xffffffff;
  DAT_00507a40 = 0;
  DAT_00509344 = 0;
  DAT_00508ad0 = LoadBitmapToDirectDrawSurface(piVar9,s_data_ui_printbar_bmp_0043f978,0,0);
  RegisterBitmapSurface(&DAT_00508ad0,s_data_ui_printbar_bmp_0043f978);
  MarkRegisteredSurfaceColorKeyed(0x508ad0);
  SetSurfaceTransparencyColorKey(DAT_00508ad0,0xff00ff);
  DAT_00509250 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_largegras_bmp_00440744,0,0);
  RegisterBitmapSurface(&DAT_00509250,s_Data_SubGameDYP_largegras_bmp_00440744);
  DAT_00509254 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_background_bmp_004425dc,0,0)
  ;
  RegisterBitmapSurface(&DAT_00509254,s_Data_SubGameDYP_background_bmp_004425dc);
  MarkRegisteredSurfaceColorKeyed(0x509254);
  SetSurfaceTransparencyColorKey(DAT_00509254,0xff00ff);
  DAT_00509258 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_lilly_bmp_004407c4,0,0);
  RegisterBitmapSurface(&DAT_00509258,s_Data_SubGameDYP_lilly_bmp_004407c4);
  DAT_0050925c = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_reeds_01_bmp_00440844,0,0);
  RegisterBitmapSurface(&DAT_0050925c,s_Data_SubGameDYP_reeds_01_bmp_00440844);
  DAT_00509260 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_reed_02_bmp_004408c4,0,0);
  RegisterBitmapSurface(&DAT_00509260,s_Data_SubGameDYP_reed_02_bmp_004408c4);
  DAT_00509264 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_rock_02_bmp_00440944,0,0);
  RegisterBitmapSurface(&DAT_00509264,s_Data_SubGameDYP_rock_02_bmp_00440944);
  DAT_00509268 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_rock_01_bmp_004409c4,0,0);
  RegisterBitmapSurface(&DAT_00509268,s_Data_SubGameDYP_rock_01_bmp_004409c4);
  DAT_0050926c = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_rock_01_bmp_004409c4,0,0);
  RegisterBitmapSurface(&DAT_0050926c,s_Data_SubGameDYP_rock_01_bmp_004409c4);
  DAT_00509270 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_lilly_bmp_00440a44,0,0);
  RegisterBitmapSurface(&DAT_00509270,s_Data_SubGameDYP_lilly_bmp_00440a44);
  DAT_00509274 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_lilly_bmp_00440ac4,0,0);
  RegisterBitmapSurface(&DAT_00509274,s_Data_SubGameDYP_lilly_bmp_00440ac4);
  DAT_005092b4 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_fountains_1_5_sq_004425b0,0,0);
  RegisterBitmapSurface(&DAT_005092b4,s_Data_SubGameDYP_fountains_1_5_sq_004425b0);
  DAT_005092b0 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_fountains_1_00442580,0,0);
  RegisterBitmapSurface(&DAT_005092b0,s_Data_SubGameDYP_SNOW_fountains_1_00442580);
  DAT_005092ac = DAT_005092b4;
  DAT_004fca90 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_ponddummy_bmp_00442560,0,0);
  RegisterBitmapSurface(&DAT_004fca90,s_Data_SubGameDYP_ponddummy_bmp_00442560);
  DAT_004fcaa8 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_ponddummy2_bmp_00442540,0,0)
  ;
  RegisterBitmapSurface(&DAT_004fcaa8,s_Data_SubGameDYP_ponddummy2_bmp_00442540);
  DAT_00507b54 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_fountaindummy_bm_0044251c,0,0);
  RegisterBitmapSurface(&DAT_00507b54,s_Data_SubGameDYP_fountaindummy_bm_0044251c);
  MarkRegisteredSurfaceColorKeyed(0x4fca90);
  SetSurfaceTransparencyColorKey(DAT_004fca90,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x4fcaa8);
  SetSurfaceTransparencyColorKey(DAT_004fcaa8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x507b54);
  SetSurfaceTransparencyColorKey(DAT_00507b54,0xff00ff);
  DAT_00508c0c = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_delcurs_bmp_00442500,0,0);
  RegisterBitmapSurface(&DAT_00508c0c,s_Data_SubGameDYP_delcurs_bmp_00442500);
  MarkRegisteredSurfaceColorKeyed(0x508c0c);
  SetSurfaceTransparencyColorKey(DAT_00508c0c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092b4);
  SetSurfaceTransparencyColorKey(DAT_005092b4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092b0);
  SetSurfaceTransparencyColorKey(DAT_005092b0,0xff00ff);
  local_624 = 0;
  local_628 = 0;
  do {
    puVar3 = (undefined4 *)((int)&DAT_00507b3c + local_628);
    piVar4 = LoadBitmapToDirectDrawSurface
                       (piVar9,s_Data_SubGameDYP_roundsquqre_01_b_00441344 + local_624,0,0);
    *puVar3 = piVar4;
    RegisterBitmapSurface(puVar3,s_Data_SubGameDYP_roundsquqre_01_b_00441344 + local_624);
    MarkRegisteredSurfaceColorKeyed((int)puVar3);
    SetSurfaceTransparencyColorKey((int *)*puVar3,0xff00ff);
    piVar4 = LoadBitmapToDirectDrawSurface
                       (piVar9,s_Data_SubGameDYP_SNOW_roundsquqre_004415c4 + local_624,0,0);
    *(undefined4 *)((int)&DAT_004fca94 + local_628) = piVar4;
    RegisterBitmapSurface
              ((undefined4 *)((int)&DAT_004fca94 + local_628),
               s_Data_SubGameDYP_SNOW_roundsquqre_004415c4 + local_624);
    MarkRegisteredSurfaceColorKeyed((int)&DAT_004fca94 + local_628);
    SetSurfaceTransparencyColorKey(*(int **)((int)&DAT_004fca94 + local_628),0xff00ff);
    *(undefined4 *)((int)&DAT_00504178 + local_628) = *puVar3;
    local_624 = local_624 + 0x80;
    local_628 = local_628 + 4;
  } while (local_624 < 0x280);
  DAT_00509278 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_bushes_bmp_004424e4,0,0);
  RegisterBitmapSurface(&DAT_00509278,s_Data_SubGameDYP_bushes_bmp_004424e4);
  DAT_0050927c = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_trees_bmp_004424c8,0,0);
  RegisterBitmapSurface(&DAT_0050927c,s_Data_SubGameDYP_trees_bmp_004424c8);
  DAT_00509280 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_benchs_bmp_004424ac,0,0);
  RegisterBitmapSurface(&DAT_00509280,s_Data_SubGameDYP_benchs_bmp_004424ac);
  DAT_00509284 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_flowers_bmp_00442490,0,0);
  RegisterBitmapSurface(&DAT_00509284,s_Data_SubGameDYP_flowers_bmp_00442490);
  MarkRegisteredSurfaceColorKeyed(0x509278);
  SetSurfaceTransparencyColorKey(DAT_00509278,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50927c);
  SetSurfaceTransparencyColorKey(DAT_0050927c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x509280);
  SetSurfaceTransparencyColorKey(DAT_00509280,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x509284);
  SetSurfaceTransparencyColorKey(DAT_00509284,0xff00ff);
  DAT_00509288 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_bases_01_bmp_00442470,0,0);
  RegisterBitmapSurface(&DAT_00509288,s_Data_SubGameDYP_bases_01_bmp_00442470);
  DAT_0050928c = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_pillars_bmp_00442454,0,0);
  RegisterBitmapSurface(&DAT_0050928c,s_Data_SubGameDYP_pillars_bmp_00442454);
  DAT_00509290 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_roofs_bmp_00442438,0,0);
  RegisterBitmapSurface(&DAT_00509290,s_Data_SubGameDYP_roofs_bmp_00442438);
  DAT_00509294 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_bases_01_bm_00442414,0,0);
  RegisterBitmapSurface(&DAT_00509294,s_Data_SubGameDYP_SNOW_bases_01_bm_00442414);
  DAT_00509298 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_pillars_bmp_004423f0,0,0);
  RegisterBitmapSurface(&DAT_00509298,s_Data_SubGameDYP_SNOW_pillars_bmp_004423f0);
  DAT_0050929c = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_SNOW_roofs_bmp_004423d0,0,0)
  ;
  RegisterBitmapSurface(&DAT_0050929c,s_Data_SubGameDYP_SNOW_roofs_bmp_004423d0);
  MarkRegisteredSurfaceColorKeyed(0x509288);
  SetSurfaceTransparencyColorKey(DAT_00509288,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50928c);
  SetSurfaceTransparencyColorKey(DAT_0050928c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x509290);
  SetSurfaceTransparencyColorKey(DAT_00509290,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x509294);
  SetSurfaceTransparencyColorKey(DAT_00509294,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x509298);
  SetSurfaceTransparencyColorKey(DAT_00509298,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50929c);
  SetSurfaceTransparencyColorKey(DAT_0050929c,0xff00ff);
  DAT_005092a0 = DAT_00509288;
  DAT_005092a4 = DAT_0050928c;
  DAT_005092a8 = DAT_00509290;
  DAT_005092b8 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_largegras_b_004423ac,0,0);
  RegisterBitmapSurface(&DAT_005092b8,s_Data_SubGameDYP_SNOW_largegras_b_004423ac);
  DAT_005092bc = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_bushes_bmp_0044238c,0,0);
  RegisterBitmapSurface(&DAT_005092bc,s_Data_SubGameDYP_SNOW_bushes_bmp_0044238c);
  DAT_005092c0 = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_SNOW_trees_bmp_0044236c,0,0)
  ;
  RegisterBitmapSurface(&DAT_005092c0,s_Data_SubGameDYP_SNOW_trees_bmp_0044236c);
  DAT_005092c4 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_benchs_bmp_0044234c,0,0);
  RegisterBitmapSurface(&DAT_005092c4,s_Data_SubGameDYP_SNOW_benchs_bmp_0044234c);
  DAT_005092c8 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_flowers_bmp_00442328,0,0);
  RegisterBitmapSurface(&DAT_005092c8,s_Data_SubGameDYP_SNOW_flowers_bmp_00442328);
  DAT_005092cc = LoadBitmapToDirectDrawSurface(piVar9,s_Data_SubGameDYP_SNOW_lilly_bmp_00442308,0,0)
  ;
  RegisterBitmapSurface(&DAT_005092cc,s_Data_SubGameDYP_SNOW_lilly_bmp_00442308);
  DAT_005092d0 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_reeds_01_bm_004422e4,0,0);
  RegisterBitmapSurface(&DAT_005092d0,s_Data_SubGameDYP_SNOW_reeds_01_bm_004422e4);
  DAT_005092d4 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_reed_02_bmp_004422c0,0,0);
  RegisterBitmapSurface(&DAT_005092d4,s_Data_SubGameDYP_SNOW_reed_02_bmp_004422c0);
  DAT_005092d8 = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_rock_02_bmp_0044229c,0,0);
  RegisterBitmapSurface(&DAT_005092d8,s_Data_SubGameDYP_SNOW_rock_02_bmp_0044229c);
  DAT_005092dc = LoadBitmapToDirectDrawSurface
                           (piVar9,s_Data_SubGameDYP_SNOW_rock_01_bmp_00442278,0,0);
  RegisterBitmapSurface(&DAT_005092dc,s_Data_SubGameDYP_SNOW_rock_01_bmp_00442278);
  MarkRegisteredSurfaceColorKeyed(0x5092b8);
  SetSurfaceTransparencyColorKey(DAT_005092b8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092bc);
  SetSurfaceTransparencyColorKey(DAT_005092bc,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092c0);
  SetSurfaceTransparencyColorKey(DAT_005092c0,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092c4);
  SetSurfaceTransparencyColorKey(DAT_005092c4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092c8);
  SetSurfaceTransparencyColorKey(DAT_005092c8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092cc);
  SetSurfaceTransparencyColorKey(DAT_005092cc,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092d0);
  SetSurfaceTransparencyColorKey(DAT_005092d0,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092d4);
  SetSurfaceTransparencyColorKey(DAT_005092d4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092d8);
  SetSurfaceTransparencyColorKey(DAT_005092d8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5092dc);
  SetSurfaceTransparencyColorKey(DAT_005092dc,0xff00ff);
  _DAT_00507a4c = DAT_00509278;
  _DAT_00507a44 = DAT_00509250;
  _DAT_00507a54 = DAT_0050927c;
  _DAT_00507a5c = DAT_00509280;
  _DAT_00507a64 = DAT_00509284;
  _DAT_00507a7c = DAT_00509260;
  _DAT_00507a84 = DAT_00509264;
  _DAT_00507a8c = DAT_00509268;
  _DAT_00507a48 = DAT_005092b8;
  _DAT_00507a50 = DAT_005092bc;
  _DAT_00507a58 = DAT_005092c0;
  _DAT_00507a60 = DAT_005092c4;
  _DAT_00507a68 = DAT_005092c8;
  _DAT_00507a70 = DAT_005092cc;
  _DAT_00507a78 = DAT_005092d0;
  _DAT_00507a80 = DAT_005092d4;
  _DAT_00507a88 = DAT_005092d8;
  _DAT_00507a6c = DAT_00509258;
  _DAT_00507a90 = DAT_005092dc;
  DAT_00507b50 = DAT_00509250;
  DAT_00507e40 = DAT_00509250;
  _DAT_00507e44 = DAT_00509258;
  _DAT_00507e4c = DAT_00509260;
  DAT_00507b70 = 2;
  DAT_00507b7c = 2;
  _DAT_00507b88 = 2;
  _DAT_00507b8c = 2;
  _DAT_00507b94 = 2;
  _DAT_00507ba4 = 2;
  _DAT_00507bb0 = 2;
  _DAT_00507bbc = 2;
  _DAT_00507bc0 = 2;
  _DAT_00507bc8 = 2;
  _DAT_00507bd4 = 2;
  _DAT_00507e50 = DAT_00509264;
  _DAT_00507a74 = DAT_0050925c;
  DAT_00507b74 = 0;
  DAT_00507b78 = 0xffffffff;
  DAT_00507b80 = 1;
  DAT_00507b84 = 0xffffffff;
  _DAT_00507b90 = 0xffffffff;
  _DAT_00507b98 = 3;
  _DAT_00507b9c = 0xffffffff;
  _DAT_00507ba0 = 0;
  _DAT_00507ba8 = 0;
  _DAT_00507bac = 0;
  _DAT_00507bb4 = 1;
  _DAT_00507bb8 = 0;
  _DAT_00507bc4 = 0;
  _DAT_00507bcc = 3;
  _DAT_00507bd0 = 0;
  _DAT_00507bd8 = 4;
  _DAT_00507e48 = DAT_0050925c;
  _DAT_00507e54 = DAT_00509268;
  _DAT_00507e58 = DAT_00509270;
  _DAT_00507e5c = DAT_00509274;
  MarkRegisteredSurfaceColorKeyed(0x509258);
  SetSurfaceTransparencyColorKey(DAT_00509258,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50925c);
  SetSurfaceTransparencyColorKey(DAT_0050925c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x509260);
  SetSurfaceTransparencyColorKey(DAT_00509260,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x509264);
  SetSurfaceTransparencyColorKey(DAT_00509264,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x509268);
  SetSurfaceTransparencyColorKey(DAT_00509268,0xff00ff);
  puVar3 = &DAT_00507b18;
  pcVar10 = s_Data_SubGameDYP_arrows_bmp_00440bc4;
  do {
    piVar4 = LoadBitmapToDirectDrawSurface(piVar9,pcVar10,0,0);
    *puVar3 = piVar4;
    RegisterBitmapSurface(puVar3,pcVar10);
    MarkRegisteredSurfaceColorKeyed((int)puVar3);
    SetSurfaceTransparencyColorKey((int *)*puVar3,0xff00ff);
    pcVar10 = pcVar10 + 0x80;
    puVar3 = puVar3 + 1;
  } while ((int)pcVar10 < 0x441044);
  pcVar10 = s_Data_SubGameDYP_pondmodetray_bmp_00442254;
  pCVar11 = local_3e8;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pCVar11 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pCVar11 = pCVar11 + 4;
  }
  *pCVar11 = *pcVar10;
  local_3c7 = 0;
  local_3c3 = 0;
  local_3bf = 0;
  local_3bb = 0;
  local_3b7 = 0;
  pcVar10 = s_Data_SubGameDYP_bandmodetray_bmp_00442230;
  pcVar12 = local_3b6;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pcVar12 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pcVar12 = pcVar12 + 4;
  }
  *pcVar12 = *pcVar10;
  local_395 = 0;
  local_391 = 0;
  local_38d = 0;
  local_389 = 0;
  local_385 = 0;
  pcVar10 = s_Data_SubGameDYP_designmodetray_b_0044220c;
  puVar3 = &local_384;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  *(char *)((int)puVar3 + 2) = pcVar10[2];
  local_361 = 0;
  puVar3 = &DAT_00507a14;
  local_35d = 0;
  pCVar11 = local_3e8;
  local_359 = 0;
  local_355 = 0;
  local_353 = 0;
  do {
    piVar4 = LoadBitmapToDirectDrawSurface(piVar9,pCVar11,0,0);
    *puVar3 = piVar4;
    RegisterBitmapSurface(puVar3,pCVar11);
    MarkRegisteredSurfaceColorKeyed((int)puVar3);
    SetSurfaceTransparencyColorKey((int *)*puVar3,0xff00ff);
    puVar3 = puVar3 + 1;
    pCVar11 = pCVar11 + 0x32;
  } while ((int)puVar3 < 0x507a20);
  pcVar10 = s_Data_SubGameDYP_pondred_bmp_004421f0;
  pCVar11 = local_5dc;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pCVar11 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pCVar11 = pCVar11 + 4;
  }
  local_5c0 = 0;
  local_5bc = 0;
  local_5b8 = 0;
  local_5b4 = 0;
  local_5b0 = 0;
  local_5ac = 0;
  pcVar10 = s_Data_SubGameDYP_ponddep_bmp_004421d4;
  puVar3 = local_5aa;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  local_58e = 0;
  local_58a = 0;
  local_586 = 0;
  local_582 = 0;
  local_57e = 0;
  local_57a = 0;
  pcVar10 = s_Data_SubGameDYP_bandred_bmp_004421b8;
  puVar3 = local_578;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  local_55c = 0;
  local_558 = 0;
  local_554 = 0;
  local_550 = 0;
  local_54c = 0;
  local_548 = 0;
  pcVar10 = s_Data_SubGameDYP_banddep_bmp_0044219c;
  puVar3 = local_546;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  local_52a = 0;
  local_526 = 0;
  local_522 = 0;
  local_51e = 0;
  local_51a = 0;
  local_516 = 0;
  pcVar10 = s_Data_SubGameDYP_decoratered_bmp_0044217c;
  puVar3 = local_514;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  local_4f4 = 0;
  local_4f0 = 0;
  local_4ec = 0;
  local_4e8 = 0;
  local_4e4 = 0;
  pcVar10 = s_Data_SubGameDYP_decoratedep_bmp_0044215c;
  puVar3 = local_4e2;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  local_4c2 = 0;
  local_4be = 0;
  local_4ba = 0;
  local_4b6 = 0;
  local_4b2 = 0;
  pcVar10 = s_Data_SubGameDYP_vuiewred_bmp_0044213c;
  pcVar12 = local_4b0;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pcVar12 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pcVar12 = pcVar12 + 4;
  }
  *pcVar12 = *pcVar10;
  local_493 = 0;
  local_48f = 0;
  local_48b = 0;
  local_487 = 0;
  local_483 = 0;
  local_47f = 0;
  pcVar10 = s_Data_SubGameDYP_viewdep_bmp_00442120;
  puVar3 = local_47e;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  local_462 = 0;
  local_45e = 0;
  local_45a = 0;
  local_456 = 0;
  local_452 = 0;
  local_44e = 0;
  pcVar10 = s_Data_SubGameDYP_deletered_bmp_00442100;
  puVar3 = local_44c;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  local_42e = 0;
  local_42a = 0;
  local_426 = 0;
  local_422 = 0;
  local_41e = 0;
  pcVar10 = s_Data_SubGameDYP_deletedep_bmp_004420e0;
  puVar3 = local_41a;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  local_3fc = 0;
  pCVar11 = local_5dc;
  local_3f8 = 0;
  puVar3 = &DAT_00504190;
  local_3f4 = 0;
  local_3f0 = 0;
  local_3ec = 0;
  do {
    local_628 = 2;
    do {
      piVar4 = LoadBitmapToDirectDrawSurface(piVar9,pCVar11,0,0);
      *puVar3 = piVar4;
      RegisterBitmapSurface(puVar3,pCVar11);
      MarkRegisteredSurfaceColorKeyed((int)puVar3);
      SetSurfaceTransparencyColorKey((int *)*puVar3,0xff00ff);
      pCVar11 = pCVar11 + 0x32;
      puVar3 = puVar3 + 1;
      local_628 = local_628 + -1;
    } while (local_628 != 0);
  } while ((int)puVar3 < 0x5041b8);
  pcVar10 = s_Data_SubGameDYP_summer_bmp_004420c4;
  pCVar11 = local_350;
  for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pCVar11 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pCVar11 = pCVar11 + 4;
  }
  *(undefined2 *)pCVar11 = *(undefined2 *)pcVar10;
  pCVar11[2] = pcVar10[2];
  local_335 = 0;
  local_331 = 0;
  local_32d = 0;
  local_329 = 0;
  local_325 = 0;
  local_321 = 0;
  local_31f = 0;
  pcVar10 = s_Data_SubGameDYP_summerred_bmp_004420a4;
  puVar3 = local_31e;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  local_300 = 0;
  local_2fc = 0;
  local_2f8 = 0;
  local_2f4 = 0;
  local_2f0 = 0;
  pcVar10 = s_Data_SubGameDYP_summerdep_bmp_00442084;
  puVar3 = local_2ec;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  local_2ce = 0;
  local_2ca = 0;
  local_2c6 = 0;
  local_2c2 = 0;
  local_2be = 0;
  pcVar10 = s_Data_SubGameDYP_winter_bmp_00442068;
  puVar3 = &local_2ba;
  for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  *(char *)((int)puVar3 + 2) = pcVar10[2];
  local_29f = 0;
  local_29b = 0;
  local_297 = 0;
  local_293 = 0;
  local_28f = 0;
  local_28b = 0;
  local_289 = 0;
  pcVar10 = s_Data_SubGameDYP_winterred_bmp_00442048;
  puVar3 = local_288;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  local_26a = 0;
  local_266 = 0;
  local_262 = 0;
  local_25e = 0;
  local_25a = 0;
  pcVar10 = s_Data_SubGameDYP_winterdep_bmp_00442028;
  puVar3 = local_256;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  local_238 = 0;
  local_234 = 0;
  local_230 = 0;
  local_22c = 0;
  local_228 = 0;
  pcVar10 = s_Data_SubGameDYP_print_bmp_0044200c;
  puVar3 = local_224;
  for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  puVar3 = local_20a;
  for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  pcVar10 = s_Data_SubGameDYP_printred_bmp_00441fec;
  pcVar12 = local_1f2;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pcVar12 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pcVar12 = pcVar12 + 4;
  }
  *pcVar12 = *pcVar10;
  local_1d5 = 0;
  local_1d1 = 0;
  local_1cd = 0;
  local_1c9 = 0;
  local_1c5 = 0;
  local_1c1 = 0;
  pcVar10 = s_Data_SubGameDYP_printdep_bmp_00441fcc;
  pcVar12 = local_1c0;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pcVar12 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pcVar12 = pcVar12 + 4;
  }
  *pcVar12 = *pcVar10;
  local_1a3 = 0;
  local_19f = 0;
  local_19b = 0;
  local_197 = 0;
  local_193 = 0;
  local_18f = 0;
  pcVar10 = s_Data_SubGameDYP_deleteall_bmp_00441fac;
  puVar3 = local_18e;
  for (iVar5 = 7; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar10;
  local_170 = 0;
  local_16c = 0;
  local_168 = 0;
  local_164 = 0;
  local_160 = 0;
  pcVar10 = s_Data_SubGameDYP_deleteallred_bmp_00441f88;
  pcVar12 = local_15c;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pcVar12 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pcVar12 = pcVar12 + 4;
  }
  *pcVar12 = *pcVar10;
  local_13b = 0;
  local_137 = 0;
  local_133 = 0;
  local_12f = 0;
  local_12b = 0;
  pcVar10 = s_Data_SubGameDYP_deletealldep_bmp_00441f64;
  pcVar12 = local_12a;
  for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
    *(undefined4 *)pcVar12 = *(undefined4 *)pcVar10;
    pcVar10 = pcVar10 + 4;
    pcVar12 = pcVar12 + 4;
  }
  *pcVar12 = *pcVar10;
  local_109 = 0;
  pCVar11 = local_350;
  local_105 = 0;
  puVar3 = &DAT_00508ad4;
  local_101 = 0;
  local_fd = 0;
  local_f9 = 0;
  do {
    local_628 = 3;
    do {
      piVar4 = LoadBitmapToDirectDrawSurface(piVar9,pCVar11,0,0);
      *puVar3 = piVar4;
      RegisterBitmapSurface(puVar3,pCVar11);
      MarkRegisteredSurfaceColorKeyed((int)puVar3);
      SetSurfaceTransparencyColorKey((int *)*puVar3,0xff00ff);
      pCVar11 = pCVar11 + 0x32;
      puVar3 = puVar3 + 1;
      local_628 = local_628 + -1;
    } while (local_628 != 0);
  } while ((int)puVar3 < 0x508b04);
  iVar5 = 0;
  piVar9 = &DAT_004fcacc;
  do {
    iVar13 = *piVar9;
    if (iVar13 != -1) {
      if (99 < iVar13) {
        iVar13 = iVar13 + -0x5c;
      }
      (**(code **)(*(&DAT_00507e40)[iVar13] + 0x58))((&DAT_00507e40)[iVar13],local_f8);
      local_101 = CONCAT31(0x7c,(undefined1)local_101);
      local_fd = 0x600;
      local_f9 = 0;
      (**(code **)(*(&DAT_00507e40)[iVar13] + 0x58))((&DAT_00507e40)[iVar13],(int)&local_101 + 1);
      piVar9[-4] = piVar9[-6] + iStack_f0;
      piVar9[-5] = piVar9[-7] + iStack_ec;
      piVar9[2] = 1;
      piVar9[3] = iStack_ec;
      piVar9[4] = iStack_f0;
      piVar9[5] = (&DAT_0043fac4)[iVar13];
      piVar9[6] = iVar5;
    }
    piVar9 = piVar9 + 0x13;
    iVar5 = iVar5 + 1;
  } while ((int)piVar9 < 0x4fcf8c);
  _DAT_00507ef4 = 0x2a;
  _DAT_00507efc = 0x2a;
  _DAT_00507f04 = 0x2a;
  _DAT_00507f0c = 0x2a;
  _DAT_00507f14 = 0x2a;
  _DAT_00507ef0 = 0x44;
  _DAT_00507ef8 = 0x44;
  _DAT_00507f00 = 0x44;
  _DAT_00507f08 = 0x44;
  _DAT_00507f10 = 0x44;
  _DAT_00507f18 = 0x32;
  _DAT_00507f20 = 0x32;
  _DAT_00507f28 = 0x32;
  _DAT_00507f30 = 0x32;
  _DAT_00507f38 = 0x32;
  _DAT_00507f1c = 0x9b;
  _DAT_00507f24 = 0x9b;
  _DAT_00507f2c = 0x9b;
  _DAT_00507f34 = 0x9b;
  _DAT_00507f3c = 0x9b;
  _DAT_00507f40 = 0x29;
  _DAT_00507f48 = 0x29;
  _DAT_00507f50 = 0x29;
  _DAT_00507f58 = 0x29;
  _DAT_00507f60 = 0x29;
  _DAT_00507f44 = 0x36;
  _DAT_00507f4c = 0x36;
  _DAT_00507f54 = 0x36;
  _DAT_00507f5c = 0x36;
  _DAT_00507f6c = 0x2d;
  _DAT_00507f74 = 0x2d;
  _DAT_00507f7c = 0x2d;
  _DAT_00507f84 = 0x2d;
  _DAT_00507f8c = 0x2d;
  _DAT_00507f68 = 0x34;
  _DAT_00507f70 = 0x34;
  _DAT_00507f78 = 0x34;
  _DAT_00507f80 = 0x34;
  _DAT_00507f88 = 0x34;
  _DAT_00507d9c = 0x16;
  _DAT_00507da4 = 0x16;
  _DAT_00507dac = 0x16;
  _DAT_00507db0 = 0x1e;
  _DAT_00507db8 = 0x1e;
  DAT_00507b14 = 0;
  DAT_00507b5c = 0;
  _DAT_00507a3c = 0;
  DAT_00507ae4 = 0;
  DAT_00507ae8 = 0;
  DAT_00507aec = 0;
  _DAT_00507af0 = 0;
  _DAT_00507af4 = 0;
  DAT_00508bf4 = 0;
  _DAT_00508bf8 = 0;
  _DAT_00508bfc = 3;
  _DAT_00507f64 = 0x4d;
  _DAT_00507d98 = 0x4b;
  _DAT_00507da0 = 0x4b;
  _DAT_00507da8 = 0xffffffff;
  _DAT_00507db4 = 0xf;
  _DAT_00507dbc = 0xf;
  _DAT_00507dc0 = 0x4b;
  _DAT_00507dc4 = 0x8c;
  local_628 = 0;
  do {
    iVar5 = 0;
    do {
      iVar13 = 0;
      do {
        iVar6 = (iVar13 + (iVar5 + local_628 * 4) * 5) * 0x30;
        *(undefined4 *)(&DAT_00507fb8 + iVar6) = 0xffffffff;
        if (local_628 == 0) {
          (&DAT_00507fbc)[(iVar13 + iVar5 * 5) * 0xc] = DAT_00507b30;
        }
        else if (local_628 == 1) {
          *(undefined4 *)((iVar13 + iVar5 * 5) * 0x30 + 0x50837c) = DAT_00507b34;
        }
        else if (local_628 == 2) {
          *(undefined4 *)((iVar13 + iVar5 * 5) * 0x30 + 0x50873c) = DAT_00507b38;
        }
        *(int *)(&DAT_00507fac + iVar6) = iVar5 * 0x3d;
        *(int *)(&DAT_00507fb4 + iVar6) = iVar5 * 0x3d + 0x3d;
        *(int *)(&DAT_00507fa8 + iVar6) = iVar13 * 0x22;
        *(int *)(&DAT_00507fb0 + iVar6) = iVar13 * 0x22 + 0x22;
        if (local_628 == 0) {
          if (iVar5 == 0) {
            (&DAT_00507fa0)[iVar13 * 0xc] = DAT_0050926c;
            (&DAT_00507fa4)[iVar13 * 0xc] = 5;
          }
          else if (iVar5 == 1) {
            iVar6 = iVar13 * 3 + 0xf;
            (&DAT_00507fa0)[iVar6 * 4] = DAT_00509274;
            (&DAT_00507fa4)[iVar6 * 4] = 7;
          }
          else if (iVar5 == 2) {
            if (iVar13 == 0) {
              _DAT_00508184 = 1;
              _DAT_00508180 = DAT_00509258;
            }
            else if (iVar13 == 1) {
              _DAT_005081b4 = 2;
              _DAT_005081b0 = DAT_0050925c;
            }
            else if (iVar13 == 2) {
              _DAT_005081e4 = 3;
              _DAT_005081e0 = DAT_00509260;
            }
            else if (iVar13 == 3) {
              _DAT_00508214 = 4;
              _DAT_00508210 = DAT_00509264;
            }
            else if (iVar13 == 4) {
              _DAT_00508244 = 5;
              _DAT_00508240 = DAT_00509268;
            }
          }
          iVar6 = iVar13 + iVar5 * 5;
          piVar9 = (int *)(&DAT_00507fa0)[iVar6 * 0xc];
          if (piVar9 != (int *)0x0) {
            uStack_7c = 0x7c;
            uStack_78 = 6;
            (**(code **)(*piVar9 + 0x58))(piVar9,&uStack_7c);
            (&DAT_00507f90)[iVar6 * 0xc] = 0;
            (&DAT_00507f94)[iVar6 * 0xc] = 0;
            (&DAT_00507f98)[iVar6 * 0xc] = uStack_70;
            (&DAT_00507f9c)[iVar6 * 0xc] = uStack_74;
          }
        }
        else if (local_628 == 2) {
          aiStack_61c[8] = 0x7c;
          aiStack_61c[9] = 0x41;
          iVar6 = iVar13 + (iVar5 + 8) * 5;
          aiStack_61c[10] = 0x68;
          aiStack_61c[0xb] = 199;
          aiStack_61c[0xc] = 0x5d;
          aiStack_61c[0xd] = 0x4f;
          *(int *)(&DAT_00507fb8 + iVar6 * 0x30) = iVar13 + iVar5 * 5;
          aiStack_61c[0xe] = 0x72;
          aiStack_61c[0xf] = 0x54;
          if (iVar5 == 0) {
            *(int **)(&DAT_00508720 + iVar13 * 0x30) = DAT_00509278;
          }
          else if (iVar5 == 1) {
            *(int **)(iVar13 * 0x30 + 0x508810) = DAT_0050927c;
          }
          else if (iVar5 == 2) {
            *(int **)(iVar13 * 0x30 + 0x508900) = DAT_00509280;
          }
          else if (iVar5 == 3) {
            *(int **)(iVar13 * 0x30 + 0x5089f0) = DAT_00509284;
          }
          iVar1 = aiStack_61c[iVar5 * 2 + 8];
          iVar7 = iVar13 * iVar1;
          (&DAT_00507f90)[iVar6 * 0xc] = iVar7;
          (&DAT_00507f98)[iVar6 * 0xc] = iVar7 + iVar1;
          uVar2 = *(undefined4 *)(local_5dc + (iVar5 * 2 + -7) * 4);
          (&DAT_00507f94)[iVar6 * 0xc] = 0;
          (&DAT_00507f9c)[iVar6 * 0xc] = uVar2;
        }
        else if (local_628 == 1) {
          aiStack_61c[0] = 0x7f;
          aiStack_61c[1] = 0x48;
          aiStack_61c[2] = 0x11;
          aiStack_61c[3] = 0x40;
          aiStack_61c[4] = 0x7f;
          aiStack_61c[5] = 0x51;
          aiStack_61c[6] = 0xffffffff;
          aiStack_61c[7] = 0xffffffff;
          if (iVar5 == 0) {
            (&DAT_00508360)[iVar13 * 0xc] = DAT_005092a0;
          }
          else if (iVar5 == 1) {
            (&DAT_00508450)[iVar13 * 0xc] = DAT_005092a4;
          }
          else if (iVar5 == 2) {
            (&DAT_00508540)[iVar13 * 0xc] = DAT_005092a8;
          }
          iVar1 = aiStack_61c[iVar5 * 2];
          iVar7 = iVar13 * iVar1;
          iVar6 = iVar13 + (iVar5 + 4) * 5;
          (&DAT_00507f90)[iVar6 * 0xc] = iVar7;
          (&DAT_00507f98)[iVar6 * 0xc] = iVar7 + iVar1;
          iVar1 = aiStack_61c[iVar5 * 2 + 1];
          (&DAT_00507f94)[iVar6 * 0xc] = 0;
          (&DAT_00507f9c)[iVar6 * 0xc] = iVar1;
        }
        iVar13 = iVar13 + 1;
      } while (iVar13 < 5);
      iVar5 = iVar5 + 1;
    } while (iVar5 < 4);
    local_628 = local_628 + 1;
  } while (local_628 < 3);
  LoadParkDesignerData();
  StopAllManagedSounds(DAT_0044ddd8);
  DAT_00441f18 = 1;
  cVar8 = 0 < DAT_00509340;
  if (300 < DAT_00441dc8) {
    cVar8 = cVar8 + '\x01';
  }
  if (200 < DAT_00441dcc) {
    cVar8 = cVar8 + '\x01';
  }
  DAT_0051c30c = (uint)(cVar8 == '\0');
  DAT_00509354 = 1;
  return;
}

