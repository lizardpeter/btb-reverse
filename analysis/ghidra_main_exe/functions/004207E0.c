/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004207e0; function: InitializeSpudMazeActivity; body bytes: 2492
 * callers: 1; callees: 11; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeSpudMazeActivity(void)

{
  undefined4 *puVar1;
  int *piVar2;
  undefined4 *puVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  int local_fc;
  int *local_f4;
  uint local_f0;
  int local_ec;
  undefined4 *local_e8;
  int local_e4 [3];
  _SYSTEMTIME _Stack_d8;
  _SYSTEMTIME _Stack_c8;
  CHAR local_b8 [60];
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  
  DAT_0051c32c = 1;
  piVar2 = *(int **)(DAT_0044de08 + 4);
  DAT_005144d8 = 0;
  DAT_005144dc = 0;
  DAT_00514528 = 0;
  DAT_00514530 = 0;
  DAT_005109c8 = 300;
  DAT_00445f04 = 0xffffff9c;
  DAT_00445f08 = 300;
  DAT_00445f0c = 2;
  DAT_0051452c = 0;
  DAT_00445f10 = 0xc;
  DAT_00445ef4 = 1;
  DAT_00445f00 = 0xffffffff;
  DAT_00514510 = 0;
  _DAT_005144cc = 0;
  DAT_00514520 = 0;
  DAT_00514524 = 0;
  DAT_00519940 = UnloadSpudMazeActivityResources;
  DAT_005107d0 = 0;
  LoadSpudMazeData();
  DAT_0050afcc = DAT_004fbd30;
  puVar3 = &DAT_004fc084;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  DAT_0050afc8 = DAT_004fbd24;
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
  } while ((int)puVar3 < 0x510bac);
  ApplyGlobalGameVolume((DAT_00446ce0 + -100) * 0x2a);
  puVar3 = &DAT_005144e4;
  pcVar7 = s_Data_SubGameSpudMaze_screen1_bmp_004459e0;
  do {
    piVar4 = LoadBitmapToDirectDrawSurface(piVar2,pcVar7,0,0);
    *puVar3 = piVar4;
    RegisterBitmapSurface(puVar3,pcVar7);
    pcVar7 = pcVar7 + 0x104;
    puVar3 = puVar3 + 1;
  } while ((int)pcVar7 < 0x445ef4);
  DAT_005144f8 = LoadBitmapToDirectDrawSurface
                           (piVar2,s_Data_SubGameSpudMaze_repair_bmp_00446044,0,0);
  RegisterBitmapSurface(&DAT_005144f8,s_Data_SubGameSpudMaze_repair_bmp_00446044);
  DAT_005144fc = LoadBitmapToDirectDrawSurface
                           (piVar2,s_Data_SubGameSpudMaze_pilchard_bm_00446020,0,0);
  RegisterBitmapSurface(&DAT_005144fc,s_Data_SubGameSpudMaze_pilchard_bm_00446020);
  DAT_0051450c = LoadBitmapToDirectDrawSurface(piVar2,s_Data_SubGameMaze_numerals_bmp_00444308,0,0);
  RegisterBitmapSurface(&DAT_0051450c,s_Data_SubGameMaze_numerals_bmp_00444308);
  DAT_00514500 = LoadBitmapToDirectDrawSurface(piVar2,s_Data_SubGameMaze_box_bmp_004442ec,0,0);
  RegisterBitmapSurface(&DAT_00514500,s_Data_SubGameMaze_box_bmp_004442ec);
  DAT_00514504 = LoadBitmapToDirectDrawSurface
                           (piVar2,s_Data_SubGameSpudMaze_toolbarhamm_00445ff4,0,0);
  RegisterBitmapSurface(&DAT_00514504,s_Data_SubGameSpudMaze_toolbarhamm_00445ff4);
  DAT_00514508 = LoadBitmapToDirectDrawSurface
                           (piVar2,s_Data_SubGameSpudMaze_toolbarhamm_00445fcc,0,0);
  RegisterBitmapSurface(&DAT_00514508,s_Data_SubGameSpudMaze_toolbarhamm_00445fcc);
  DAT_00514494 = LoadBitmapToDirectDrawSurface
                           (piVar2,s_Data_SubGameSpudMaze_hammer_bmp_00445fac,0,0);
  RegisterBitmapSurface(&DAT_00514494,s_Data_SubGameSpudMaze_hammer_bmp_00445fac);
  DAT_005144d0 = LoadBitmapToDirectDrawSurface(piVar2,s_Data_SubGameSpudMaze_timer_bmp_0044428c,0,0)
  ;
  RegisterBitmapSurface(&DAT_005144d0,s_Data_SubGameSpudMaze_timer_bmp_0044428c);
  DAT_005144b8 = LoadBitmapToDirectDrawSurface
                           (piVar2,s_Data_SubGameSpudMaze_bentley_bmp_00445f88,0,0);
  RegisterBitmapSurface(&DAT_005144b8,s_Data_SubGameSpudMaze_bentley_bmp_00445f88);
  DAT_005144c4 = LoadBitmapToDirectDrawSurface
                           (piVar2,s_Data_SubGameSpudMaze_screenborde_00445f60,0,0);
  RegisterBitmapSurface(&DAT_005144c4,s_Data_SubGameSpudMaze_screenborde_00445f60);
  MarkRegisteredSurfaceColorKeyed(0x5144f8);
  SetSurfaceTransparencyColorKey(DAT_005144f8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5144fc);
  SetSurfaceTransparencyColorKey(DAT_005144fc,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x514500);
  SetSurfaceTransparencyColorKey(DAT_00514500,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x514504);
  SetSurfaceTransparencyColorKey(DAT_00514504,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x514508);
  SetSurfaceTransparencyColorKey(DAT_00514508,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x51450c);
  SetSurfaceTransparencyColorKey(DAT_0051450c,0);
  MarkRegisteredSurfaceColorKeyed(0x514494);
  SetSurfaceTransparencyColorKey(DAT_00514494,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5144d0);
  SetSurfaceTransparencyColorKey(DAT_005144d0,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5144b8);
  SetSurfaceTransparencyColorKey(DAT_005144b8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x5144c4);
  SetSurfaceTransparencyColorKey(DAT_005144c4,0xff00ff);
  puVar3 = &DAT_00514110;
  for (iVar6 = 0x37; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  DAT_005144c8 = 0;
  local_f0 = FUN_0042ffc4();
  local_f0 = local_f0 & 0x80000003;
  if ((int)local_f0 < 0) {
    local_f0 = (local_f0 - 1 | 0xfffffffc) + 1;
  }
  local_ec = 0;
  local_e8 = &DAT_005142dc;
  local_f4 = &DAT_004459cc;
  do {
    iVar6 = 0;
    if (0 < *local_f4) {
      local_e4[0] = 8;
      local_e4[1] = 5;
      local_e4[2] = 3;
      local_fc = local_ec;
      puVar3 = local_e8;
      do {
        iVar6 = iVar6 + 1;
        crt_sprintf(local_b8,(byte *)s_Data_SubGameSpudMaze__d__d_bmp_00445f40);
        puVar1 = (undefined4 *)((int)&DAT_005141ec + local_fc);
        piVar4 = LoadBitmapToDirectDrawSurface(piVar2,local_b8,0,0);
        *puVar1 = piVar4;
        RegisterBitmapSurface(puVar1,local_b8);
        MarkRegisteredSurfaceColorKeyed((int)puVar1);
        SetSurfaceTransparencyColorKey((int *)*puVar1,0xff00ff);
        local_7c = 0x7c;
        local_78 = 6;
        (**(code **)(*(int *)*puVar1 + 0x58))((int *)*puVar1,&local_7c);
        puVar3[-1] = uStack_70;
        *puVar3 = uStack_74;
        if ((int)local_f0 % local_e4[DAT_0051c284] == 0) {
          DAT_005144c8 = DAT_005144c8 + 1;
          *(undefined4 *)((int)&DAT_00514110 + local_fc) = 1;
        }
        local_f0 = local_f0 + 1;
        puVar3 = puVar3 + 2;
        local_fc = local_fc + 4;
      } while (iVar6 < *local_f4);
    }
    local_f4 = local_f4 + 1;
    local_e8 = local_e8 + 0x16;
    local_ec = local_ec + 0x2c;
  } while ((int)local_f4 < 0x4459e0);
  DAT_005109d0 = (float)_DAT_00510e58;
  DAT_005109e0 = 2;
  DAT_005109d4 = (float)_DAT_00510e5c;
  DAT_005109f4 = 2;
  DAT_00513f38 = DAT_005144c8;
  DAT_00510d18 = 0;
  _DAT_005109e4 = 1;
  DAT_00510a00 = 1;
  DAT_005109e8 = 0;
  DAT_005109ec = 0;
  DAT_005109f0 = 0;
  DAT_005109f8 = 3;
  puVar3 = &DAT_00510a18;
  do {
    iVar6 = 4;
    do {
      *puVar3 = 0xffffffff;
      puVar3 = puVar3 + 5;
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
  } while ((int)puVar3 < 0x510ba8);
  if (DAT_0051c284 == 0) {
    DAT_00510ba8 = DAT_00444224;
    DAT_00510d08 = DAT_005109bc;
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
  _DAT_005140fc = DAT_00510d08;
  GetSystemTime(&_Stack_c8);
  GetSystemTime(&_Stack_d8);
  SystemTimeToFileTime(&_Stack_c8,(LPFILETIME)&DAT_00510d10);
  SystemTimeToFileTime(&_Stack_d8,(LPFILETIME)&DAT_005120f0);
  Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
  DAT_00510cf8 = 0;
  DAT_00512124 = 0;
  DAT_00510950 = 0;
  DAT_00513f3c = 1;
  DAT_0051c30c = 1;
  DAT_005107e8 = *(undefined4 *)(&DAT_004441f8 + DAT_0051c284 * 4);
  _DAT_00510a08 = DAT_00510d08;
  DAT_005120fc = DAT_00510d08;
  return;
}

