/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004145f0; function: InitializeGolfActivity; body bytes: 1077
 * callers: 1; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeGolfActivity(void)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  LPCSTR pCVar7;
  
  piVar1 = *(int **)(DAT_0044de08 + 4);
  iVar6 = 0;
  iVar4 = 0;
  DAT_004437c8 = 0xffffffff;
  DAT_0050ad84 = 0;
  DAT_0050ad90 = 0;
  DAT_0050ad88 = 0;
  DAT_0050af00 = 0;
  DAT_0050af04 = 0;
  iVar2 = 5;
  _DAT_0050ad8c = 0;
  do {
    *(int *)((int)&DAT_0050abd0 + iVar4) = iVar2;
    iVar2 = iVar2 + 2;
    iVar4 = iVar4 + 4;
  } while (iVar2 < 0xb);
  DAT_0050ac1c = 0xffffffff;
  DAT_0050ac20 = 0xffffffff;
  LoadGolfData();
  _DAT_0050ac08 = (float)DAT_0050ad80;
  DAT_0050aed0 = 0;
  _DAT_0050ac0c = (float)DAT_0050ad7c;
  DAT_0050aed4 = 0;
  DAT_0050abf8 = 0;
  DAT_0050ae18 = 0;
  _DAT_0050aed8 = 0;
  _DAT_0050abfc = 0;
  DAT_0050ae1c = 0;
  DAT_0050ae24 = 0;
  _DAT_0050ac00 = 0;
  _DAT_0050ae20 = 0;
  _DAT_0050ae28 = 0;
  DAT_00519940 = UnloadGolfActivityResources;
  DAT_0050abb0 = 5;
  DAT_0050ae50 = 0;
  DAT_0050ad9c = 0;
  DAT_0050abf4 = 0;
  DAT_0050ae54 = 1;
  _DAT_0050ae2c = 0;
  DAT_0050ada0 = DAT_0050ae44;
  DAT_0050ada4 = DAT_0050ae40;
  DAT_0050ada8 = 0x3c;
  DAT_0050aedc = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameGolf_golfbg_bmp_0044396c,0,0);
  RegisterBitmapSurface(&DAT_0050aedc,s_Data_SubGameGolf_golfbg_bmp_0044396c);
  iVar2 = 0;
  do {
    piVar3 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameGolf_flag_bmp_00443620 + iVar6,0,0);
    *(undefined4 *)((int)&DAT_0050ab88 + iVar2) = piVar3;
    RegisterBitmapSurface
              ((undefined4 *)((int)&DAT_0050ab88 + iVar2),
               s_Data_SubGameGolf_flag_bmp_00443620 + iVar6);
    piVar3 = LoadBitmapToDirectDrawSurface
                       (piVar1,s_Data_SubGameGolf_flag01_bmp_004436f4 + iVar6,0,0);
    *(undefined4 *)((int)&DAT_0050addc + iVar2) = piVar3;
    RegisterBitmapSurface
              ((undefined4 *)((int)&DAT_0050addc + iVar2),
               s_Data_SubGameGolf_flag01_bmp_004436f4 + iVar6);
    iVar6 = iVar6 + 0x46;
    iVar2 = iVar2 + 4;
  } while (iVar6 < 0xd2);
  iVar2 = 0;
  do {
    MarkRegisteredSurfaceColorKeyed((int)&DAT_0050ab88 + iVar2);
    SetSurfaceTransparencyColorKey(*(int **)((int)&DAT_0050ab88 + iVar2),0xff00ff);
    MarkRegisteredSurfaceColorKeyed((int)&DAT_0050addc + iVar2);
    SetSurfaceTransparencyColorKey(*(int **)((int)&DAT_0050addc + iVar2),0xff00ff);
    iVar2 = iVar2 + 4;
  } while (iVar2 < 0xc);
  DAT_0050aee0 = LoadBitmapToDirectDrawSurface(piVar1,&DAT_0050ae64,0,0);
  RegisterBitmapSurface(&DAT_0050aee0,&DAT_0050ae64);
  DAT_0050aee4 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameGolf_golfballstop_bm_00443948,0,0);
  RegisterBitmapSurface(&DAT_0050aee4,s_Data_SubGameGolf_golfballstop_bm_00443948);
  DAT_0050aee8 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameGolf_toolbar_ball_bm_00443924,0,0);
  RegisterBitmapSurface(&DAT_0050aee8,s_Data_SubGameGolf_toolbar_ball_bm_00443924);
  DAT_0050aeec = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameGolf_golfballgo_bmp_00443904,0,0);
  RegisterBitmapSurface(&DAT_0050aeec,s_Data_SubGameGolf_golfballgo_bmp_00443904);
  DAT_0050aef0 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameGolf_shadow_bmp_004438e8,0,0);
  RegisterBitmapSurface(&DAT_0050aef0,s_Data_SubGameGolf_shadow_bmp_004438e8);
  DAT_0050aef4 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameGolf_powerbar_bmp_004438c8,0,0);
  RegisterBitmapSurface(&DAT_0050aef4,s_Data_SubGameGolf_powerbar_bmp_004438c8);
  DAT_0050aef8 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameGolf_marker_bmp_004438ac,0,0);
  RegisterBitmapSurface(&DAT_0050aef8,s_Data_SubGameGolf_marker_bmp_004438ac);
  DAT_0050adec = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameGolf_golfnumbers_bmp_00443888,0,0);
  RegisterBitmapSurface(&DAT_0050adec,s_Data_SubGameGolf_golfnumbers_bmp_00443888);
  puVar5 = &DAT_0050adfc;
  pCVar7 = &DAT_0050ac4c;
  do {
    piVar3 = LoadBitmapToDirectDrawSurface(piVar1,pCVar7,0,0);
    *puVar5 = piVar3;
    RegisterBitmapSurface(puVar5,pCVar7);
    pCVar7 = pCVar7 + 0x46;
    puVar5 = puVar5 + 1;
  } while ((int)pCVar7 < 0x50ad1e);
  MarkRegisteredSurfaceColorKeyed(0x50aedc);
  SetSurfaceTransparencyColorKey(DAT_0050aedc,0xff00ff);
  puVar5 = &DAT_0050adfc;
  do {
    MarkRegisteredSurfaceColorKeyed((int)puVar5);
    SetSurfaceTransparencyColorKey((int *)*puVar5,0xff00ff);
    puVar5 = puVar5 + 1;
  } while ((int)puVar5 < 0x50ae08);
  MarkRegisteredSurfaceColorKeyed(0x50aee0);
  SetSurfaceTransparencyColorKey(DAT_0050aee0,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50aee4);
  SetSurfaceTransparencyColorKey(DAT_0050aee4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50aee8);
  SetSurfaceTransparencyColorKey(DAT_0050aee8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50aef0);
  SetSurfaceTransparencyColorKey(DAT_0050aef0,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50aef4);
  SetSurfaceTransparencyColorKey(DAT_0050aef4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50aef8);
  SetSurfaceTransparencyColorKey(DAT_0050aef8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50adec);
  SetSurfaceTransparencyColorKey(DAT_0050adec,0xff00ff);
  DAT_0051c30c = 1;
  DAT_004437e0 = 1;
  return;
}

