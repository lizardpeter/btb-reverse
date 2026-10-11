/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041a730; function: InitializeMazeActivity; body bytes: 1886
 * callers: 1; callees: 11; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeMazeActivity(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  _SYSTEMTIME local_20;
  _SYSTEMTIME local_10;
  
  piVar1 = *(int **)(DAT_0044de08 + 4);
  DAT_005144d8 = 0;
  DAT_00519940 = UnloadMazeActivityResources;
  DAT_005107d0 = 0;
  LoadMazeData();
  uVar2 = DAT_0043b5a8;
  puVar3 = &DAT_004fc084;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  CSoundManager_Create
            (DAT_004fc174,&DAT_004fc088,s_Data_SubGameMaze_lofty_engine_wa_00444360,0,uVar2,
             DAT_0043b5ac,DAT_0043b5b0,DAT_0043b5b4,1);
  DAT_0050afc8 = DAT_004fbd24;
  DAT_0050afcc = DAT_004fbd30;
  DAT_005109cc = 0;
  puVar3 = &DAT_00510a1c;
  do {
    iVar6 = 4;
    do {
      puVar3[-1] = 0xffffffff;
      *puVar3 = 1;
      puVar3 = puVar3 + 5;
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
  } while ((int)puVar3 < 0x510b0c);
  ApplyGlobalGameVolume((DAT_00446ce0 + -100) * 0x2a);
  puVar3 = &DAT_00512100;
  pcVar7 = s_Data_SubGameMaze_LEFT_MAZE_bmp_00443d68;
  do {
    piVar4 = LoadBitmapToDirectDrawSurface(piVar1,pcVar7,0,0);
    *puVar3 = piVar4;
    RegisterBitmapSurface(puVar3,pcVar7);
    pcVar7 = pcVar7 + 0x104;
    puVar3 = puVar3 + 1;
  } while ((int)pcVar7 < 0x444074);
  DAT_0051210c = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameMaze_player_bmp_00444344,0,0);
  RegisterBitmapSurface(&DAT_0051210c,s_Data_SubGameMaze_player_bmp_00444344);
  DAT_00512110 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameMaze_spud_bmp_00444328,0,0);
  RegisterBitmapSurface(&DAT_00512110,s_Data_SubGameMaze_spud_bmp_00444328);
  DAT_00512120 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameMaze_numerals_bmp_00444308,0,0);
  RegisterBitmapSurface(&DAT_00512120,s_Data_SubGameMaze_numerals_bmp_00444308);
  DAT_00512114 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameMaze_box_bmp_004442ec,0,0);
  RegisterBitmapSurface(&DAT_00512114,s_Data_SubGameMaze_box_bmp_004442ec);
  DAT_00512118 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameMaze_boxghost_bmp_004442cc,0,0);
  RegisterBitmapSurface(&DAT_00512118,s_Data_SubGameMaze_boxghost_bmp_004442cc);
  DAT_0051211c = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameMaze_smallbox_bmp_004442ac,0,0);
  RegisterBitmapSurface(&DAT_0051211c,s_Data_SubGameMaze_smallbox_bmp_004442ac);
  DAT_005144d0 = LoadBitmapToDirectDrawSurface(piVar1,s_Data_SubGameSpudMaze_timer_bmp_0044428c,0,0)
  ;
  RegisterBitmapSurface(&DAT_005144d0,s_Data_SubGameSpudMaze_timer_bmp_0044428c);
  MarkRegisteredSurfaceColorKeyed(0x51210c);
  SetSurfaceTransparencyColorKey(DAT_0051210c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x512110);
  SetSurfaceTransparencyColorKey(DAT_00512110,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x512114);
  SetSurfaceTransparencyColorKey(DAT_00512114,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x512118);
  SetSurfaceTransparencyColorKey(DAT_00512118,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51211c);
  SetSurfaceTransparencyColorKey(DAT_0051211c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x512120);
  SetSurfaceTransparencyColorKey(DAT_00512120,0);
  MarkRegisteredSurfaceColorKeyed(0x5144d0);
  SetSurfaceTransparencyColorKey(DAT_005144d0,0xff00ff);
  DAT_00510d18 = 1;
  DAT_005109d0 = 0x43770000;
  DAT_005109d4 = 0x43858000;
  DAT_005109e0 = 8;
  _DAT_005109e4 = 4;
  DAT_005109e8 = 0;
  DAT_005109ec = 0;
  DAT_005109f0 = 0;
  DAT_005109f4 = 2;
  DAT_005109f8 = 10;
  puVar3 = &DAT_00510a18;
  do {
    iVar6 = 4;
    do {
      *puVar3 = 0xffffffff;
      puVar3 = puVar3 + 5;
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
  } while ((int)puVar3 < 0x510b08);
  DAT_005109c8 = 300;
  if (DAT_0051c284 == 0) {
    DAT_00510ba8 = DAT_00444224;
    DAT_00510d08 = DAT_005109bc;
    uVar5 = FUN_0042ffc4();
    DAT_00510a18 = uVar5 & 0x80000003;
    if ((int)DAT_00510a18 < 0) {
      DAT_00510a18 = (DAT_00510a18 - 1 | 0xfffffffc) + 1;
    }
    uVar5 = FUN_0042ffc4();
    DAT_00510a68 = uVar5 & 0x80000003;
    if ((int)DAT_00510a68 < 0) {
      DAT_00510a68 = (DAT_00510a68 - 1 | 0xfffffffc) + 1;
    }
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a7c = uVar5 & 0x80000003;
      if ((int)DAT_00510a7c < 0) {
        DAT_00510a7c = (DAT_00510a7c - 1 | 0xfffffffc) + 1;
      }
    } while (DAT_00510a7c == DAT_00510a68);
    uVar5 = FUN_0042ffc4();
    DAT_00510ab8 = uVar5 & 0x80000003;
    if ((int)DAT_00510ab8 < 0) {
      DAT_00510ab8 = (DAT_00510ab8 - 1 | 0xfffffffc) + 1;
    }
  }
  else if (DAT_0051c284 == 1) {
    DAT_00510d08 = DAT_005109c0;
    uVar5 = FUN_0042ffc4();
    DAT_00510a18 = uVar5 & 0x80000003;
    if ((int)DAT_00510a18 < 0) {
      DAT_00510a18 = (DAT_00510a18 - 1 | 0xfffffffc) + 1;
    }
    DAT_00510ba8 = (&DAT_00444224)[DAT_0051c284];
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a2c = uVar5 & 0x80000003;
      if ((int)DAT_00510a2c < 0) {
        DAT_00510a2c = (DAT_00510a2c - 1 | 0xfffffffc) + 1;
      }
    } while (DAT_00510a2c == DAT_00510a18);
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a40 = uVar5 & 0x80000003;
      if ((int)DAT_00510a40 < 0) {
        DAT_00510a40 = (DAT_00510a40 - 1 | 0xfffffffc) + 1;
      }
    } while ((DAT_00510a40 == DAT_00510a18) || (DAT_00510a40 == DAT_00510a2c));
    uVar5 = FUN_0042ffc4();
    DAT_00510a68 = uVar5 & 0x80000003;
    if ((int)DAT_00510a68 < 0) {
      DAT_00510a68 = (DAT_00510a68 - 1 | 0xfffffffc) + 1;
    }
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a7c = uVar5 & 0x80000003;
      if ((int)DAT_00510a7c < 0) {
        DAT_00510a7c = (DAT_00510a7c - 1 | 0xfffffffc) + 1;
      }
    } while (DAT_00510a7c == DAT_00510a68);
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a90 = uVar5 & 0x80000003;
      if ((int)DAT_00510a90 < 0) {
        DAT_00510a90 = (DAT_00510a90 - 1 | 0xfffffffc) + 1;
      }
    } while ((DAT_00510a90 == DAT_00510a68) || (DAT_00510a90 == DAT_00510a7c));
    uVar5 = FUN_0042ffc4();
    DAT_00510ab8 = uVar5 & 0x80000003;
    if ((int)DAT_00510ab8 < 0) {
      DAT_00510ab8 = (DAT_00510ab8 - 1 | 0xfffffffc) + 1;
    }
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510acc = uVar5 & 0x80000003;
      if ((int)DAT_00510acc < 0) {
        DAT_00510acc = (DAT_00510acc - 1 | 0xfffffffc) + 1;
      }
    } while (DAT_00510acc == DAT_00510ab8);
  }
  else {
    DAT_00510d08 = DAT_005109c4;
    uVar5 = FUN_0042ffc4();
    DAT_00510a18 = uVar5 & 0x80000003;
    if ((int)DAT_00510a18 < 0) {
      DAT_00510a18 = (DAT_00510a18 - 1 | 0xfffffffc) + 1;
    }
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a2c = uVar5 & 0x80000003;
      if ((int)DAT_00510a2c < 0) {
        DAT_00510a2c = (DAT_00510a2c - 1 | 0xfffffffc) + 1;
      }
    } while (DAT_00510a2c == DAT_00510a18);
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a40 = uVar5 & 0x80000003;
      if ((int)DAT_00510a40 < 0) {
        DAT_00510a40 = (DAT_00510a40 - 1 | 0xfffffffc) + 1;
      }
    } while ((DAT_00510a40 == DAT_00510a18) || (DAT_00510a40 == DAT_00510a2c));
    do {
      _DAT_00510a54 = FUN_0042ffc4();
      _DAT_00510a54 = _DAT_00510a54 & 0x80000003;
      if ((int)_DAT_00510a54 < 0) {
        _DAT_00510a54 = (_DAT_00510a54 - 1 | 0xfffffffc) + 1;
      }
    } while (((_DAT_00510a54 == DAT_00510a18) || (_DAT_00510a54 == DAT_00510a2c)) ||
            (_DAT_00510a54 == DAT_00510a40));
    uVar5 = FUN_0042ffc4();
    DAT_00510a68 = uVar5 & 0x80000003;
    if ((int)DAT_00510a68 < 0) {
      DAT_00510a68 = (DAT_00510a68 - 1 | 0xfffffffc) + 1;
    }
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a7c = uVar5 & 0x80000003;
      if ((int)DAT_00510a7c < 0) {
        DAT_00510a7c = (DAT_00510a7c - 1 | 0xfffffffc) + 1;
      }
    } while (DAT_00510a7c == DAT_00510a68);
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510a90 = uVar5 & 0x80000003;
      if ((int)DAT_00510a90 < 0) {
        DAT_00510a90 = (DAT_00510a90 - 1 | 0xfffffffc) + 1;
      }
    } while ((DAT_00510a90 == DAT_00510a68) || (DAT_00510a90 == DAT_00510a7c));
    do {
      _DAT_00510aa4 = FUN_0042ffc4();
      _DAT_00510aa4 = _DAT_00510aa4 & 0x80000003;
      if ((int)_DAT_00510aa4 < 0) {
        _DAT_00510aa4 = (_DAT_00510aa4 - 1 | 0xfffffffc) + 1;
      }
    } while (((_DAT_00510aa4 == DAT_00510a68) || (_DAT_00510aa4 == DAT_00510a7c)) ||
            (_DAT_00510aa4 == DAT_00510a90));
    uVar5 = FUN_0042ffc4();
    DAT_00510ab8 = uVar5 & 0x80000003;
    if ((int)DAT_00510ab8 < 0) {
      DAT_00510ab8 = (DAT_00510ab8 - 1 | 0xfffffffc) + 1;
    }
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510acc = uVar5 & 0x80000003;
      if ((int)DAT_00510acc < 0) {
        DAT_00510acc = (DAT_00510acc - 1 | 0xfffffffc) + 1;
      }
    } while (DAT_00510acc == DAT_00510ab8);
    do {
      uVar5 = FUN_0042ffc4();
      DAT_00510ae0 = uVar5 & 0x80000003;
      if ((int)DAT_00510ae0 < 0) {
        DAT_00510ae0 = (DAT_00510ae0 - 1 | 0xfffffffc) + 1;
      }
    } while ((DAT_00510ae0 == DAT_00510ab8) || (DAT_00510ae0 == DAT_00510acc));
    do {
      _DAT_00510af4 = FUN_0042ffc4();
      _DAT_00510af4 = _DAT_00510af4 & 0x80000003;
      if ((int)_DAT_00510af4 < 0) {
        _DAT_00510af4 = (_DAT_00510af4 - 1 | 0xfffffffc) + 1;
      }
    } while (((_DAT_00510af4 == DAT_00510ab8) || (_DAT_00510af4 == DAT_00510acc)) ||
            (_DAT_00510af4 == DAT_00510ae0));
    DAT_00510ba8 = (&DAT_00444224)[DAT_0051c284];
  }
  DAT_00510cf8 = 0;
  DAT_005107e8 = *(undefined4 *)(&DAT_004441f8 + DAT_0051c284 * 4);
  DAT_00444220 = 1;
  DAT_00512124 = 0;
  GetSystemTime(&local_10);
  GetSystemTime(&local_20);
  SystemTimeToFileTime(&local_10,(LPFILETIME)&DAT_00510d10);
  SystemTimeToFileTime(&local_20,(LPFILETIME)&DAT_005120f0);
  Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
  GetSystemTime(&local_20);
  SystemTimeToFileTime(&local_20,(LPFILETIME)&DAT_005120f0);
  Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
  DAT_00510950 = 0;
  DAT_0051c30c = 1;
  DAT_0051212c = 1;
  DAT_005109c8 = 300;
  _DAT_00510a08 = DAT_00510d08;
  DAT_005120fc = DAT_00510d08;
  return;
}

