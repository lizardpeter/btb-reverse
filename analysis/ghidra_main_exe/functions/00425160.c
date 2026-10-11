/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00425160; function: InitializeSquirrelActivity; body bytes: 1565
 * callers: 1; callees: 8; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeSquirrelActivity(void)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  char *pcVar7;
  CHAR *pCVar8;
  undefined4 *puVar9;
  CHAR local_180 [32];
  undefined4 local_160 [24];
  undefined4 local_100 [8];
  undefined4 local_e0 [24];
  undefined4 local_80 [8];
  undefined4 local_60 [24];
  
  piVar1 = *(int **)(DAT_0044de08 + 4);
  DAT_005150cc = 0;
  DAT_00515058 = 0;
  DAT_005150d8 = 0;
  DAT_00514f08 = 0xffffffff;
  DAT_00514f60 = 0;
  DAT_00514fec = 0xffffffff;
  DAT_00514ed8 = 0;
  DAT_00514f58 = 0;
  DAT_00515000 = 0;
  LoadSquirrelData();
  DAT_00515088 = 0xffffffff;
  DAT_0051508c = 0xffffffff;
  DAT_005150cc = DAT_0051c284;
  DAT_00514ff0 = 0xffffffff;
  _DAT_00515090 = 0xffffffff;
  DAT_00514ff4 = 0xffffffff;
  DAT_004469a4 = 0xffffffff;
  puVar9 = &DAT_00514fbc;
  for (iVar4 = 0xc; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = 0xffffffff;
    puVar9 = puVar9 + 1;
  }
  _DAT_00514ff8 = 0xffffffff;
  DAT_00519940 = UnloadSquirrelActivityResources;
  DAT_005150e4 = 0;
  DAT_005150e8 = 0;
  DAT_005150ec = 0;
  DAT_00515004 = 0;
  DAT_005150f0 = 0;
  DAT_004469a8 = 1;
  DAT_005150f4 = 0;
  DAT_005150f8 = 0;
  DAT_00514f40 = 0;
  DAT_00514f48 = 0x26c;
  _DAT_00514f44 = 0;
  _DAT_00514f4c = 0x1e0;
  _DAT_00514ffc = 0xffffffff;
  _DAT_00515094 = 0xffffffff;
  puVar9 = &DAT_00514f64;
  for (iVar4 = 0xc; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = 0xffffffff;
    puVar9 = puVar9 + 1;
  }
  piVar5 = &DAT_00514f0c;
  do {
    iVar4 = 4;
    piVar6 = piVar5;
    do {
      uVar3 = FUN_0042ffc4();
      piVar5 = piVar6 + 1;
      iVar4 = iVar4 + -1;
      *piVar6 = (int)uVar3 % 3;
      iVar2 = DAT_005150cc;
      piVar6 = piVar5;
    } while (iVar4 != 0);
  } while ((int)piVar5 < 0x514f3c);
  _DAT_00514f1c = DAT_00514f18;
  _DAT_00514f2c = DAT_00514f28;
  DAT_00514f94 = DAT_004467bc - DAT_00515084;
  DAT_00514f98 = *(int *)(&DAT_00446988 + DAT_00514f0c * 4) + DAT_004467c0;
  puVar9 = &DAT_00514f9c;
  for (iVar4 = 8; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  _DAT_00515078 = 0;
  _DAT_0051507c = 0;
  _DAT_00515080 = 0;
  DAT_00514f50 = (iVar2 != 0) + 3;
  pcVar7 = s_Data_SubGameSquirrel_level1_bmp_00446cc0;
  pCVar8 = local_180;
  for (iVar4 = 8; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pCVar8 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    pCVar8 = pCVar8 + 4;
  }
  puVar9 = local_160;
  for (iVar4 = 0x18; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  DAT_005150d0 = 0;
  pcVar7 = s_Data_SubGameSquirrel_level2_bmp_00446ca0;
  puVar9 = local_100;
  for (iVar4 = 8; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    puVar9 = puVar9 + 1;
  }
  puVar9 = local_e0;
  for (iVar4 = 0x18; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  DAT_005150e0 = 0;
  pcVar7 = s_Data_SubGameSquirrel_level3_bmp_00446c80;
  puVar9 = local_80;
  for (iVar4 = 8; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    puVar9 = puVar9 + 1;
  }
  puVar9 = local_60;
  for (iVar4 = 0x18; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  DAT_0051509c = LoadBitmapToDirectDrawSurface(piVar1,local_180 + iVar2 * 0x80,0,0);
  RegisterBitmapSurface(&DAT_0051509c,local_180 + DAT_005150cc * 0x80);
  DAT_005150a0 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameSquirrel_all_bmp_00446c60,0,0);
  RegisterBitmapSurface(&DAT_005150a0,s_Data_SubGameSquirrel_all_bmp_00446c60);
  MarkRegisteredSurfaceColorKeyed(0x5150a0);
  SetSurfaceTransparencyColorKey(DAT_005150a0,0xff00ff);
  DAT_005150a4 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameSquirrel_allconveyor_00446c38,0,0);
  RegisterBitmapSurface(&DAT_005150a4,s_Data_SubGameSquirrel_allconveyor_00446c38);
  MarkRegisteredSurfaceColorKeyed(0x5150a4);
  SetSurfaceTransparencyColorKey(DAT_005150a4,0xff00ff);
  DAT_005150a8 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameSquirrel_hook_bmp_00446c18,0,0);
  RegisterBitmapSurface(&DAT_005150a8,s_Data_SubGameSquirrel_hook_bmp_00446c18);
  DAT_005150b4 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameSquirrel_body_01_bmp_00446bf4,0,0);
  RegisterBitmapSurface(&DAT_005150b4,s_Data_SubGameSquirrel_body_01_bmp_00446bf4);
  DAT_005150b8 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameSquirrel_loftyarmswi_00446bc8,0,0);
  RegisterBitmapSurface(&DAT_005150b8,s_Data_SubGameSquirrel_loftyarmswi_00446bc8);
  DAT_005150bc = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameSquirrel_squriel_1_8_00446ba0,0,0);
  RegisterBitmapSurface(&DAT_005150bc,s_Data_SubGameSquirrel_squriel_1_8_00446ba0);
  MarkRegisteredSurfaceColorKeyed(0x5150a8);
  SetSurfaceTransparencyColorKey(DAT_005150a8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5150b4);
  SetSurfaceTransparencyColorKey(DAT_005150b4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5150b8);
  SetSurfaceTransparencyColorKey(DAT_005150b8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5150bc);
  SetSurfaceTransparencyColorKey(DAT_005150bc,0xff00ff);
  DAT_005150ac = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameSquirrel_eyes_bmp_00446b80,0,0);
  RegisterBitmapSurface(&DAT_005150ac,s_Data_SubGameSquirrel_eyes_bmp_00446b80);
  DAT_005150b0 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameSquirrel_mouth_bmp_00446b60,0,0)
  ;
  RegisterBitmapSurface(&DAT_005150b0,s_Data_SubGameSquirrel_mouth_bmp_00446b60);
  MarkRegisteredSurfaceColorKeyed(0x5150ac);
  SetSurfaceTransparencyColorKey(DAT_005150ac,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5150b0);
  SetSurfaceTransparencyColorKey(DAT_005150b0,0xff00ff);
  DAT_00515068 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameSquirrel_top_bmp_00446b40,0,0);
  RegisterBitmapSurface(&DAT_00515068,s_Data_SubGameSquirrel_top_bmp_00446b40);
  MarkRegisteredSurfaceColorKeyed(0x515068);
  SetSurfaceTransparencyColorKey(DAT_00515068,0xff00ff);
  DAT_0051506c = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameSquirrel_left_bmp_00446b20,0,0);
  RegisterBitmapSurface(&DAT_0051506c,s_Data_SubGameSquirrel_left_bmp_00446b20);
  MarkRegisteredSurfaceColorKeyed(0x51506c);
  SetSurfaceTransparencyColorKey(DAT_0051506c,0xff00ff);
  DAT_00515070 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameSquirrel_right_bmp_00446b00,0,0)
  ;
  RegisterBitmapSurface(&DAT_00515070,s_Data_SubGameSquirrel_right_bmp_00446b00);
  MarkRegisteredSurfaceColorKeyed(0x515070);
  SetSurfaceTransparencyColorKey(DAT_00515070,0xff00ff);
  DAT_00515074 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameSquirrel_bottom_bmp_00446ae0,0,0);
  RegisterBitmapSurface(&DAT_00515074,s_Data_SubGameSquirrel_bottom_bmp_00446ae0);
  MarkRegisteredSurfaceColorKeyed(0x515074);
  SetSurfaceTransparencyColorKey(DAT_00515074,0xff00ff);
  DAT_005150c0 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameSquirrel_conveyor_bm_00446abc,0,0);
  RegisterBitmapSurface(&DAT_005150c0,s_Data_SubGameSquirrel_conveyor_bm_00446abc);
  MarkRegisteredSurfaceColorKeyed(0x5150c0);
  SetSurfaceTransparencyColorKey(DAT_005150c0,0xff00ff);
  DAT_005150c4 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameSquirrel_LOFTYmoves8_00446a94,0,0);
  RegisterBitmapSurface(&DAT_005150c4,s_Data_SubGameSquirrel_LOFTYmoves8_00446a94);
  MarkRegisteredSurfaceColorKeyed(0x5150c4);
  SetSurfaceTransparencyColorKey(DAT_005150c4,0xff00ff);
  DAT_005150c8 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameSquirrel_nutpile_bmp_00446a70,0,0);
  RegisterBitmapSurface(&DAT_005150c8,s_Data_SubGameSquirrel_nutpile_bmp_00446a70);
  MarkRegisteredSurfaceColorKeyed(0x5150c8);
  SetSurfaceTransparencyColorKey(DAT_005150c8,0xff00ff);
  puVar9 = &DAT_004fc084;
  for (iVar4 = 0x14; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  GenerateSquirrelConveyorChoices();
  DAT_005150fc = 0;
  DAT_0051c30c = 1;
  CSound_Stop(DAT_0051c2b8);
  DAT_00446a44 = 1;
  return;
}

