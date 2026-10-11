/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00419600; function: InitializeHerdingActivity; body bytes: 3458
 * callers: 1; callees: 8; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeHerdingActivity(void)

{
  float fVar1;
  int *piVar2;
  undefined4 *puVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  char *pcVar9;
  int iVar10;
  int iVar11;
  CHAR *pCVar12;
  int iVar13;
  longlong lVar14;
  int local_18c;
  int local_188;
  CHAR local_180 [30];
  undefined4 local_162 [24];
  undefined4 local_100 [8];
  undefined4 local_e0 [24];
  undefined4 local_80;
  undefined4 local_61 [24];
  
  piVar7 = *(int **)(DAT_0044de08 + 4);
  _DAT_00510620 = 0xfffffff7;
  _DAT_00510624 = 0xfffffff7;
  DAT_0051071c = 0;
  DAT_00510720 = 0;
  DAT_00443aa4 = 1;
  DAT_00443aa8 = 1;
  _DAT_00443aac = 1;
  DAT_00519940 = UnloadHerdingActivityResources;
  _DAT_00510618 = 0xfffffff6;
  _DAT_0051061c = 0xfffffff6;
  if (DAT_0044de0c == 1) {
    DAT_0051c284 = 0;
  }
  DAT_0050b31c = 0;
  puVar3 = &DAT_004fc084;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  DAT_00510718 = 0xffffffff;
  puVar3 = &DAT_0050af78;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0xffffffff;
    puVar3 = puVar3 + 1;
  }
  puVar3 = &DAT_0050af14;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0xffffffff;
    puVar3 = puVar3 + 1;
  }
  puVar3 = &DAT_005105ac;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0xffffffff;
    puVar3 = puVar3 + 1;
  }
  _DAT_005105fc = 0;
  ApplyGlobalGameVolume((DAT_00446ce0 + -100) * 0x2a);
  DAT_00510760 = 0;
  DAT_00510764 = 0;
  iVar6 = 0;
  puVar3 = &DAT_0050b3ac;
  do {
    puVar3[-1] = iVar6;
    *puVar3 = 0;
    puVar3[1] = 0;
    puVar3[2] = 0;
    puVar3[3] = 0;
    puVar3[9] = 0;
    puVar3[10] = 0;
    puVar3[0xb] = 0x11;
    puVar3[0x10] = 0;
    puVar3 = puVar3 + 0x19;
    iVar6 = iVar6 + 1;
  } while ((int)puVar3 < 0x5101cc);
  LoadHerdingCoordinateData();
  DAT_00510724 = LoadBitmapToDirectDrawSurface
                           (piVar7,s_Data_SubGame1_bk_1_revised_01_bm_00443d40,0,0);
  RegisterBitmapSurface(&DAT_00510724,s_Data_SubGame1_bk_1_revised_01_bm_00443d40);
  DAT_0051072c = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_DUCK_01_bmp_00443d24,0,0);
  RegisterBitmapSurface(&DAT_0051072c,s_Data_SubGame1_DUCK_01_bmp_00443d24);
  DAT_00510730 = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_bunny_bmp_00443d0c,0,0);
  RegisterBitmapSurface(&DAT_00510730,s_Data_SubGame1_bunny_bmp_00443d0c);
  DAT_00510734 = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_sheep_shadow_bmp_00443cec,0,0)
  ;
  RegisterBitmapSurface(&DAT_00510734,s_Data_SubGame1_sheep_shadow_bmp_00443cec);
  DAT_00510738 = LoadBitmapToDirectDrawSurface
                           (piVar7,s_Data_SubGame1_scrufty_sprite_8bi_00443cc4,0,0);
  RegisterBitmapSurface(&DAT_00510738,s_Data_SubGame1_scrufty_sprite_8bi_00443cc4);
  DAT_0051073c = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_duckbag_bmp_00443ca8,0,0);
  RegisterBitmapSurface(&DAT_0051073c,s_Data_SubGame1_duckbag_bmp_00443ca8);
  DAT_00510740 = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_rabbitbag_bmp_00443c8c,0,0);
  RegisterBitmapSurface(&DAT_00510740,s_Data_SubGame1_rabbitbag_bmp_00443c8c);
  DAT_00510744 = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_sheepbag_bmp_00443c70,0,0);
  RegisterBitmapSurface(&DAT_00510744,s_Data_SubGame1_sheepbag_bmp_00443c70);
  DAT_00510748 = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_uisurround_bmp_00443c50,0,0);
  RegisterBitmapSurface(&DAT_00510748,s_Data_SubGame1_uisurround_bmp_00443c50);
  DAT_0051074c = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_traviscab_bmp_00443c34,0,0);
  RegisterBitmapSurface(&DAT_0051074c,s_Data_SubGame1_traviscab_bmp_00443c34);
  DAT_00510750 = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_trailer1_bmp_00443c18,0,0);
  RegisterBitmapSurface(&DAT_00510750,s_Data_SubGame1_trailer1_bmp_00443c18);
  DAT_00510754 = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_trailer2_bmp_00443bfc,0,0);
  RegisterBitmapSurface(&DAT_00510754,s_Data_SubGame1_trailer2_bmp_00443bfc);
  DAT_00510758 = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_gateleft_bmp_00443be0,0,0);
  RegisterBitmapSurface(&DAT_00510758,s_Data_SubGame1_gateleft_bmp_00443be0);
  DAT_0051075c = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGame1_gateright_bmp_00443bc4,0,0);
  RegisterBitmapSurface(&DAT_0051075c,s_Data_SubGame1_gateright_bmp_00443bc4);
  pcVar9 = s_Data_SubGame1_ducktoolbar_bmp_00443ba4;
  pCVar12 = local_180;
  for (iVar6 = 7; iVar6 != 0; iVar6 = iVar6 + -1) {
    *(undefined4 *)pCVar12 = *(undefined4 *)pcVar9;
    pcVar9 = pcVar9 + 4;
    pCVar12 = pCVar12 + 4;
  }
  *(undefined2 *)pCVar12 = *(undefined2 *)pcVar9;
  puVar3 = local_162;
  for (iVar6 = 0x18; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = 0;
  pcVar9 = s_Data_SubGame1_rabbittoolbar_bmp_00443b84;
  puVar3 = local_100;
  for (iVar6 = 8; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = *(undefined4 *)pcVar9;
    pcVar9 = pcVar9 + 4;
    puVar3 = puVar3 + 1;
  }
  puVar3 = local_e0;
  for (iVar6 = 0x18; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  pcVar9 = s_Data_SubGame1_sheeptoolbar_bmp_00443b64;
  puVar3 = &local_80;
  for (iVar6 = 7; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = *(undefined4 *)pcVar9;
    pcVar9 = pcVar9 + 4;
    puVar3 = puVar3 + 1;
  }
  *(undefined2 *)puVar3 = *(undefined2 *)pcVar9;
  *(char *)((int)puVar3 + 2) = pcVar9[2];
  puVar3 = local_61;
  for (iVar6 = 0x18; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  *(undefined1 *)puVar3 = 0;
  puVar3 = &DAT_005105a0;
  pCVar12 = local_180;
  do {
    piVar4 = LoadBitmapToDirectDrawSurface(piVar7,pCVar12,0,0);
    *puVar3 = piVar4;
    RegisterBitmapSurface(puVar3,pCVar12);
    MarkRegisteredSurfaceColorKeyed((int)puVar3);
    SetSurfaceTransparencyColorKey((int *)*puVar3,0xff00ff);
    puVar3 = puVar3 + 1;
    pCVar12 = pCVar12 + 0x80;
  } while ((int)puVar3 < 0x5105ac);
  MarkRegisteredSurfaceColorKeyed(0x510758);
  SetSurfaceTransparencyColorKey(DAT_00510758,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51075c);
  SetSurfaceTransparencyColorKey(DAT_0051075c,0xff00ff);
  DAT_00510728 = LoadBitmapToDirectDrawSurface
                           (piVar7,s_Data_SubGame1_Pickles_1_8bit_bmp_00443b40,0,0);
  RegisterBitmapSurface(&DAT_00510728,s_Data_SubGame1_Pickles_1_8bit_bmp_00443b40);
  MarkRegisteredSurfaceColorKeyed(0x510728);
  SetSurfaceTransparencyColorKey(DAT_00510728,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51073c);
  SetSurfaceTransparencyColorKey(DAT_0051073c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x510740);
  SetSurfaceTransparencyColorKey(DAT_00510740,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x510744);
  SetSurfaceTransparencyColorKey(DAT_00510744,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x510738);
  SetSurfaceTransparencyColorKey(DAT_00510738,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x510748);
  SetSurfaceTransparencyColorKey(DAT_00510748,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51072c);
  SetSurfaceTransparencyColorKey(DAT_0051072c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x510730);
  SetSurfaceTransparencyColorKey(DAT_00510730,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x510734);
  SetSurfaceTransparencyColorKey(DAT_00510734,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51074c);
  SetSurfaceTransparencyColorKey(DAT_0051074c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x510750);
  SetSurfaceTransparencyColorKey(DAT_00510750,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x510754);
  SetSurfaceTransparencyColorKey(DAT_00510754,0xff00ff);
  iVar6 = DAT_00510760;
  (&DAT_0050b3c8)[DAT_00510760 * 0x19] = (float)_DAT_005101e0;
  (&DAT_0050b3cc)[iVar6 * 0x19] = (float)_DAT_005101e4;
  lVar14 = __ftol();
  DAT_0050b3b4 = (undefined4)lVar14;
  lVar14 = __ftol();
  iVar11 = DAT_0051c284;
  DAT_0050b3b8 = (undefined4)lVar14;
  iVar10 = iVar6 + 1;
  (&DAT_0050b3d4)[iVar6 * 0x19] = 0x32;
  (&DAT_0050b3d8)[iVar6 * 0x19] = 0;
  iVar13 = 600;
  iVar6 = 0x226;
  local_18c = 0;
  DAT_00510760 = iVar10;
  do {
    local_188 = iVar11 + 3;
    if (0 < iVar11 + 3) {
      do {
        (&DAT_0050b3b4)[iVar10 * 0x19] = iVar13;
        (&DAT_0050b3b8)[iVar10 * 0x19] = iVar6;
        uVar5 = FUN_0042ffc4();
        uVar5 = uVar5 & 0x80000007;
        if ((int)uVar5 < 0) {
          uVar5 = (uVar5 - 1 | 0xfffffff8) + 1;
        }
        (&DAT_0050b3b0)[iVar10 * 0x19] = uVar5;
        (&DAT_0050b3d8)[iVar10 * 0x19] = local_18c + 1;
        *(undefined4 *)(&DAT_0050b400 + iVar10 * 100) = 0;
        (&DAT_0050b3e0)[iVar10 * 0x19] = 0;
        (&DAT_0050b3e4)[iVar10 * 0x19] = 0x66;
        (&DAT_0050b3e8)[iVar10 * 0x19] = 0x5f;
        (&DAT_0050b3c8)[iVar10 * 0x19] = (float)(int)(&DAT_0050b3b4)[iVar10 * 0x19];
        (&DAT_0050b3cc)[iVar10 * 0x19] = (float)(int)(&DAT_0050b3b8)[iVar10 * 0x19];
        *(float *)(iVar10 * 100 + 0x50b3c4) =
             (float)(int)(&DAT_0050b3b0)[iVar10 * 0x19] * _DAT_0043b39c;
        (&DAT_0050b3f0)[iVar10 * 0x19] = 0;
        (&DAT_0050b3f4)[iVar10 * 0x19] = 0;
        if ((&DAT_0050b3d8)[iVar10 * 0x19] == 1) {
          (&DAT_0050b3ec)[iVar10 * 0x19] = DAT_00510734;
        }
        else if ((&DAT_0050b3d8)[iVar10 * 0x19] == 3) {
          (&DAT_0050b3ec)[iVar10 * 0x19] = DAT_0051072c;
        }
        else {
          (&DAT_0050b3ec)[iVar10 * 0x19] = DAT_00510730;
        }
        (&DAT_0050b3f8)[iVar10 * 0x19] = 0;
        iVar13 = iVar13 + 0x96;
        iVar10 = DAT_00510760 + 1;
        DAT_00510764 = DAT_00510764 + 1;
        if (0x44c < iVar13) {
          iVar13 = 100;
          iVar6 = iVar6 + 100;
        }
        local_188 = local_188 + -1;
        DAT_00510760 = iVar10;
      } while (local_188 != 0);
    }
    local_18c = local_18c + 1;
  } while (local_18c < 3);
  _DAT_0050b304 = 0x35d;
  _DAT_0050b2fc = 0x2e0;
  DAT_0050b314 = 0x2e0;
  DAT_0050b30c = 0x3ff;
  _DAT_0050b300 = 0xb7;
  DAT_0050b318 = 0xb7;
  DAT_0050b310 = 0x151;
  _DAT_0050b308 = 0x8e;
  _DAT_00510600 = 0x2a7;
  _DAT_00510604 = 0x87;
  _DAT_00510608 = 0x357;
  _DAT_0051060c = 0x5b;
  _DAT_00510610 = 0x438;
  _DAT_00510614 = 0x108;
  DAT_0050b320 = 0x21f;
  DAT_0050b324 = 0x8e;
  DAT_0050b328 = 600;
  DAT_0050b32c = 0x80;
  _DAT_0050b330 = 0x2eb;
  _DAT_0050b334 = 99;
  _DAT_0050b338 = 0x292;
  _DAT_0050b33c = 0x76;
  _DAT_0050b340 = 0x2b8;
  _DAT_0050b344 = 0x6e;
  DAT_0050b348 = 0x335;
  DAT_0050b34c = 0x60;
  DAT_0050b350 = 0x39b;
  DAT_0050b354 = 0x43;
  _DAT_0050b358 = 0x34a;
  _DAT_0050b35c = 0x54;
  _DAT_0050b360 = 0x362;
  _DAT_0050b364 = 0x58;
  _DAT_0050b368 = 0x37a;
  _DAT_0050b36c = 0x61;
  DAT_0050b370 = 0x3f2;
  DAT_0050b374 = 0xdb;
  _DAT_0050b378 = 0x47e;
  _DAT_0050b37c = 0x127;
  _DAT_0050b380 = 0x440;
  _DAT_0050b384 = 0x12d;
  _DAT_0050b388 = 0x411;
  _DAT_0050b38c = 0xf2;
  _DAT_0050b390 = 1099;
  _DAT_0050b394 = 0xfa;
  iVar6 = 0;
  do {
    iVar11 = (&DAT_00443b08)[iVar6 * 2];
    iVar13 = (&DAT_00443b0c)[iVar6 * 2];
    piVar4 = &DAT_0050b34c;
    piVar7 = &DAT_0050b324 + iVar6 * 10;
    do {
      piVar7[-1] = piVar7[-1] - iVar11;
      *piVar7 = *piVar7 - iVar13;
      if (iVar6 == 1) {
        piVar4[-1] = piVar4[-1] + -0x23;
        *piVar4 = *piVar4 + 10;
      }
      else if (iVar6 == 2) {
        piVar4[9] = piVar4[9] + -0x1b;
        piVar4[10] = piVar4[10] + 0x24;
      }
      piVar2 = DAT_0051074c;
      piVar4 = piVar4 + 2;
      piVar7 = piVar7 + 2;
    } while ((int)piVar4 < 0x50b374);
    iVar6 = iVar6 + 1;
  } while (iVar6 < 3);
  iVar6 = iVar10 + 1;
  (&DAT_0050b3b4)[iVar10 * 0x19] = 0;
  (&DAT_0050b3b8)[iVar10 * 0x19] = 300;
  (&DAT_0050b3b0)[iVar10 * 0x19] = 0;
  (&DAT_0050b3d8)[iVar10 * 0x19] = 0xd;
  (&DAT_0050b3dc)[iVar10 * 0x19] = 0;
  (&DAT_0050b3e0)[iVar10 * 0x19] = 0;
  (&DAT_0050b3e4)[iVar10 * 0x19] = 0xe6;
  (&DAT_0050b3e8)[iVar10 * 0x19] = 0x7a;
  (&DAT_0050b3c8)[iVar10 * 0x19] = (float)(int)(&DAT_0050b3b4)[iVar10 * 0x19];
  (&DAT_0050b3cc)[iVar10 * 0x19] = (float)(int)(&DAT_0050b3b8)[iVar10 * 0x19];
  *(float *)(iVar10 * 100 + 0x50b3c4) = (float)(int)(&DAT_0050b3b0)[iVar10 * 0x19] * _DAT_0043b39c;
  (&DAT_0050b3f0)[iVar10 * 0x19] = 0;
  (&DAT_0050b3f4)[iVar10 * 0x19] = 0;
  (&DAT_0050b3ec)[iVar10 * 0x19] = piVar2;
  iVar11 = iVar10 + 2;
  (&DAT_0050b3b4)[iVar6 * 0x19] = 0xe6;
  (&DAT_0050b3b8)[iVar6 * 0x19] = 300;
  (&DAT_0050b3b0)[iVar6 * 0x19] = 0;
  (&DAT_0050b3d8)[iVar6 * 0x19] = 0xc;
  (&DAT_0050b3dc)[iVar6 * 0x19] = 0;
  (&DAT_0050b3e0)[iVar6 * 0x19] = 0;
  (&DAT_0050b3e4)[iVar6 * 0x19] = 200;
  (&DAT_0050b3e8)[iVar6 * 0x19] = 100;
  piVar4 = DAT_00510754;
  piVar7 = DAT_00510750;
  (&DAT_0050b3c8)[iVar6 * 0x19] = (float)(int)(&DAT_0050b3b4)[iVar6 * 0x19];
  (&DAT_0050b3cc)[iVar6 * 0x19] = (float)(int)(&DAT_0050b3b8)[iVar6 * 0x19];
  *(float *)(iVar6 * 100 + 0x50b3c4) = (float)(int)(&DAT_0050b3b0)[iVar6 * 0x19] * _DAT_0043b39c;
  (&DAT_0050b3f0)[iVar6 * 0x19] = 0;
  (&DAT_0050b3f4)[iVar6 * 0x19] = 0;
  (&DAT_0050b3ec)[iVar6 * 0x19] = piVar7;
  iVar6 = iVar10 + 3;
  (&DAT_0050b3b4)[iVar11 * 0x19] = 400;
  (&DAT_0050b3b8)[iVar11 * 0x19] = 0x145;
  (&DAT_0050b3b0)[iVar11 * 0x19] = 0;
  (&DAT_0050b3d8)[iVar11 * 0x19] = 0xe;
  (&DAT_0050b3dc)[iVar11 * 0x19] = 0;
  (&DAT_0050b3e0)[iVar11 * 0x19] = 0;
  (&DAT_0050b3e4)[iVar11 * 0x19] = 0x2d;
  (&DAT_0050b3e8)[iVar11 * 0x19] = 0x3c;
  (&DAT_0050b3c8)[iVar11 * 0x19] = (float)(int)(&DAT_0050b3b4)[iVar11 * 0x19];
  (&DAT_0050b3cc)[iVar11 * 0x19] = (float)(int)(&DAT_0050b3b8)[iVar11 * 0x19];
  *(float *)(iVar11 * 100 + 0x50b3c4) = (float)(int)(&DAT_0050b3b0)[iVar11 * 0x19] * _DAT_0043b39c;
  (&DAT_0050b3f0)[iVar11 * 0x19] = 0;
  (&DAT_0050b3f4)[iVar11 * 0x19] = 0;
  (&DAT_0050b3ec)[iVar11 * 0x19] = piVar4;
  (&DAT_0050b3d4)[iVar6 * 0x19] = 0xffffffff;
  (&DAT_0050b3b4)[iVar6 * 0x19] = 0x2b9;
  (&DAT_0050b3b8)[iVar6 * 0x19] = 0x8e;
  (&DAT_0050b3b0)[iVar6 * 0x19] = 0;
  (&DAT_0050b3d8)[iVar6 * 0x19] = 0xf;
  (&DAT_0050b3dc)[iVar6 * 0x19] = 0;
  (&DAT_0050b3e0)[iVar6 * 0x19] = 0;
  (&DAT_0050b3e4)[iVar6 * 0x19] = 0x78;
  (&DAT_0050b3e8)[iVar6 * 0x19] = 0x46;
  piVar7 = DAT_00510758;
  iVar11 = iVar10 + 4;
  DAT_0050b39c = iVar6;
  DAT_0050b3a0 = iVar11;
  (&DAT_0050b3c8)[iVar6 * 0x19] = (float)(int)(&DAT_0050b3b4)[iVar6 * 0x19];
  (&DAT_0050b3cc)[iVar6 * 0x19] = (float)(int)(&DAT_0050b3b8)[iVar6 * 0x19];
  *(float *)(iVar6 * 100 + 0x50b3c4) = (float)(int)(&DAT_0050b3b0)[iVar6 * 0x19] * _DAT_0043b39c;
  (&DAT_0050b3f0)[iVar6 * 0x19] = 0;
  (&DAT_0050b3f4)[iVar6 * 0x19] = 0;
  (&DAT_0050b3ec)[iVar6 * 0x19] = piVar7;
  iVar13 = iVar10 + 5;
  (&DAT_0050b3d4)[iVar11 * 0x19] = 0xffffffff;
  (&DAT_0050b3b4)[iVar11 * 0x19] = 0x347;
  (&DAT_0050b3b8)[iVar11 * 0x19] = 0x51;
  (&DAT_0050b3b0)[iVar11 * 0x19] = 0;
  (&DAT_0050b3d8)[iVar11 * 0x19] = 0x10;
  (&DAT_0050b3dc)[iVar11 * 0x19] = 0;
  (&DAT_0050b3e0)[iVar11 * 0x19] = 0;
  (&DAT_0050b3e4)[iVar11 * 0x19] = 0x78;
  (&DAT_0050b3e8)[iVar11 * 0x19] = 0x46;
  piVar7 = DAT_0051075c;
  DAT_00510760 = iVar13;
  (&DAT_0050b3c8)[iVar11 * 0x19] = (float)(int)(&DAT_0050b3b4)[iVar11 * 0x19];
  (&DAT_0050b3cc)[iVar11 * 0x19] = (float)(int)(&DAT_0050b3b8)[iVar11 * 0x19];
  *(float *)(iVar11 * 100 + 0x50b3c4) = (float)(int)(&DAT_0050b3b0)[iVar11 * 0x19] * _DAT_0043b39c;
  (&DAT_0050b3f0)[iVar11 * 0x19] = 0;
  (&DAT_0050b3f4)[iVar11 * 0x19] = 0;
  (&DAT_0050b3ec)[iVar11 * 0x19] = piVar7;
  iVar6 = 6;
  if (DAT_005101f8 != -1) {
    iVar8 = 0;
    iVar11 = DAT_005101f8;
    do {
      *(int *)((int)&DAT_004439f8 + iVar8) = iVar11 + -0x40;
      iVar6 = iVar6 + 1;
      *(int *)((int)&DAT_004439fc + iVar8) = *(int *)((int)&DAT_005101fc + iVar8) + -100;
      iVar11 = *(int *)((int)&DAT_00510200 + iVar8);
      iVar8 = iVar8 + 8;
    } while (iVar11 != -1);
  }
  iVar11 = iVar6 + -6;
  iVar6 = iVar6 + 1;
  DAT_00510768 = 0;
  piVar7 = &DAT_005101c8 + iVar6 * 2;
  DAT_00443a98 = iVar11;
  (&DAT_004439f8)[iVar11 * 2] = 0xffffffff;
  (&DAT_004439fc)[iVar11 * 2] = 0xffffffff;
  DAT_0051076c = 0;
  iVar11 = *piVar7;
  if (iVar11 != -1) {
    piVar4 = &DAT_0051062c;
    do {
      piVar4[-1] = iVar11;
      *piVar4 = piVar7[1];
      DAT_00510768 = DAT_00510768 + 1;
      piVar7 = piVar7 + 2;
      piVar4 = piVar4 + 2;
      iVar11 = *piVar7;
      iVar6 = iVar6 + 1;
    } while (iVar11 != -1);
  }
  iVar6 = iVar6 + 1;
  DAT_0050b398 = 0;
  iVar11 = (&DAT_005101c8)[iVar6 * 2];
  piVar7 = &DAT_005101c8 + iVar6 * 2;
  if (iVar11 != -1) {
    piVar4 = &DAT_005104ec;
    do {
      piVar4[-1] = iVar11;
      *piVar4 = piVar7[1];
      DAT_0050b398 = DAT_0050b398 + 1;
      piVar7 = piVar7 + 2;
      piVar4 = piVar4 + 2;
      iVar11 = *piVar7;
      iVar6 = iVar6 + 1;
    } while (iVar11 != -1);
  }
  iVar8 = DAT_0051062c;
  iVar11 = DAT_00510628;
  DAT_0050af74 = *(undefined4 *)(iVar6 * 8 + 0x5101d0);
  DAT_0050af70 = (&DAT_005101cc)[(iVar6 + 1) * 2];
  if (0 < DAT_0051c284) {
    fVar1 = (float)DAT_00510628;
    DAT_00510760 = iVar10 + 6;
    (&DAT_0050b3dc)[iVar13 * 0x19] = 0;
    (&DAT_0050b3e0)[iVar13 * 0x19] = 0;
    (&DAT_0050b3e4)[iVar13 * 0x19] = 0x65;
    (&DAT_0050b3e8)[iVar13 * 0x19] = 0x74;
    (&DAT_0050b3b4)[iVar13 * 0x19] = iVar11;
    piVar7 = DAT_00510738;
    (&DAT_0050b3b8)[iVar13 * 0x19] = iVar8;
    (&DAT_0050b3c8)[iVar13 * 0x19] = fVar1;
    (&DAT_0050b3cc)[iVar13 * 0x19] = (float)DAT_0051062c;
    (&DAT_0050b3d8)[iVar13 * 0x19] = 7;
    (&DAT_0050b3ec)[iVar13 * 0x19] = piVar7;
  }
  DAT_0050af64 = DAT_0051073c;
  _DAT_0050afd0 = 0;
  DAT_00510774 = 0;
  _DAT_0050afd4 = 0;
  DAT_0050af68 = DAT_00510740;
  _DAT_0050af6c = DAT_00510744;
  _DAT_0050afd8 = 0;
  DAT_0051c30c = 1;
  return;
}

