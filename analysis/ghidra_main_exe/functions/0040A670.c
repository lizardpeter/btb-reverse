/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040a670; function: InitializeDinoActivity; body bytes: 1556
 * callers: 1; callees: 7; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl InitializeDinoActivity(undefined4 param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  char *pcVar6;
  undefined4 *puVar7;
  int *local_604;
  undefined1 local_600 [8];
  int iStack_5f8;
  int iStack_5f4;
  char local_584 [4];
  char local_580 [4];
  char local_57c [4];
  char local_578 [4];
  undefined4 local_574 [28];
  char local_504 [4];
  char local_500 [4];
  char local_4fc [4];
  char local_4f8 [4];
  undefined4 local_4f4 [28];
  undefined4 local_484 [5];
  undefined4 local_46e [26];
  char local_404 [4];
  char local_400 [4];
  char local_3fc [4];
  char local_3f8 [4];
  char local_3f4;
  undefined4 local_3f3;
  undefined4 local_384 [5];
  undefined4 local_36e [26];
  char local_304 [4];
  char local_300 [4];
  char local_2fc [4];
  char local_2f8 [4];
  char local_2f4 [2];
  undefined4 local_2f2 [27];
  undefined4 local_284 [5];
  undefined4 local_26e [26];
  char local_204 [4];
  char local_200 [4];
  char local_1fc [4];
  char local_1f8 [4];
  char local_1f4;
  undefined4 local_1f3;
  undefined4 local_184 [5];
  undefined4 local_16e [26];
  CHAR local_104 [260];
  
  piVar5 = *(int **)(DAT_0044de08 + 4);
  DAT_004fc438 = DAT_0051c284 + param_2 * 3;
  DAT_004fc3f8 = (-(uint)(DAT_004fc438 != 2) & 0xb) + 9;
  DAT_004fc43c = param_2;
  puVar7 = &DAT_004fc084;
  for (iVar4 = 0x14; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  DAT_004fca84 = 0;
  DAT_00519940 = UnloadDinoActivityResources;
  DAT_004fc440 = 0;
  DAT_004fc2ac = LoadBitmapToDirectDrawSurface(piVar5,s_data_subgamedino_bob_bmp_0043f9ac,0,0);
  RegisterBitmapSurface(&DAT_004fc2ac,s_data_subgamedino_bob_bmp_0043f9ac);
  DAT_004fc434 = LoadBitmapToDirectDrawSurface(piVar5,s_data_subgamedino_ellis_bmp_0043f990,0,0);
  RegisterBitmapSurface(&DAT_004fc434,s_data_subgamedino_ellis_bmp_0043f990);
  MarkRegisteredSurfaceColorKeyed(0x4fc2ac);
  SetSurfaceTransparencyColorKey(DAT_004fc2ac,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x4fc434);
  SetSurfaceTransparencyColorKey(DAT_004fc434,0xff00ff);
  DAT_00508ad0 = LoadBitmapToDirectDrawSurface(piVar5,s_data_ui_printbar_bmp_0043f978,0,0);
  RegisterBitmapSurface(&DAT_00508ad0,s_data_ui_printbar_bmp_0043f978);
  MarkRegisteredSurfaceColorKeyed(0x508ad0);
  SetSurfaceTransparencyColorKey(DAT_00508ad0,0xff00ff);
  DAT_0050a5c4 = LoadBitmapToDirectDrawSurface
                           (piVar5,s_Data_SubGameFirework_certprint_b_0043f954,0,0);
  RegisterBitmapSurface(&DAT_0050a5c4,s_Data_SubGameFirework_certprint_b_0043f954);
  DAT_0050a5c8 = LoadBitmapToDirectDrawSurface
                           (piVar5,s_Data_SubGameFirework_certprintde_0043f92c,0,0);
  RegisterBitmapSurface(&DAT_0050a5c8,s_Data_SubGameFirework_certprintde_0043f92c);
  MarkRegisteredSurfaceColorKeyed(0x50a5c4);
  SetSurfaceTransparencyColorKey(DAT_0050a5c4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50a5c8);
  SetSurfaceTransparencyColorKey(DAT_0050a5c8,0xff00ff);
  LoadDinoLevelData();
  local_584[0] = s__Background_bmp_0043f91c[0];
  local_584[1] = s__Background_bmp_0043f91c[1];
  local_584[2] = s__Background_bmp_0043f91c[2];
  local_584[3] = s__Background_bmp_0043f91c[3];
  local_580[0] = s__Background_bmp_0043f91c[4];
  local_580[1] = s__Background_bmp_0043f91c[5];
  local_580[2] = s__Background_bmp_0043f91c[6];
  local_580[3] = s__Background_bmp_0043f91c[7];
  local_57c[0] = s__Background_bmp_0043f91c[8];
  local_57c[1] = s__Background_bmp_0043f91c[9];
  local_57c[2] = s__Background_bmp_0043f91c[10];
  local_57c[3] = s__Background_bmp_0043f91c[0xb];
  local_578[0] = s__Background_bmp_0043f91c[0xc];
  local_578[1] = s__Background_bmp_0043f91c[0xd];
  local_578[2] = s__Background_bmp_0043f91c[0xe];
  local_578[3] = s__Background_bmp_0043f91c[0xf];
  puVar7 = local_574;
  for (iVar4 = 0x1c; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  local_504[0] = s__SPRITE8BIT_bmp_0043f90c[0];
  local_504[1] = s__SPRITE8BIT_bmp_0043f90c[1];
  local_504[2] = s__SPRITE8BIT_bmp_0043f90c[2];
  local_504[3] = s__SPRITE8BIT_bmp_0043f90c[3];
  local_500[0] = s__SPRITE8BIT_bmp_0043f90c[4];
  local_500[1] = s__SPRITE8BIT_bmp_0043f90c[5];
  local_500[2] = s__SPRITE8BIT_bmp_0043f90c[6];
  local_500[3] = s__SPRITE8BIT_bmp_0043f90c[7];
  local_4f8[0] = s__SPRITE8BIT_bmp_0043f90c[0xc];
  local_4f8[1] = s__SPRITE8BIT_bmp_0043f90c[0xd];
  local_4f8[2] = s__SPRITE8BIT_bmp_0043f90c[0xe];
  local_4f8[3] = s__SPRITE8BIT_bmp_0043f90c[0xf];
  local_4fc[0] = s__SPRITE8BIT_bmp_0043f90c[8];
  local_4fc[1] = s__SPRITE8BIT_bmp_0043f90c[9];
  local_4fc[2] = s__SPRITE8BIT_bmp_0043f90c[10];
  local_4fc[3] = s__SPRITE8BIT_bmp_0043f90c[0xb];
  puVar7 = local_4f4;
  for (iVar4 = 0x1c; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  pcVar6 = s__Player_NORTHEAST_bmp_0043f8f4;
  puVar7 = local_484;
  for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  puVar7 = local_46e;
  for (iVar4 = 0x1a; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  local_404[0] = s__Player_EAST_bmp_0043f8e0[0];
  local_404[1] = s__Player_EAST_bmp_0043f8e0[1];
  local_404[2] = s__Player_EAST_bmp_0043f8e0[2];
  local_404[3] = s__Player_EAST_bmp_0043f8e0[3];
  local_400[0] = s__Player_EAST_bmp_0043f8e0[4];
  local_400[1] = s__Player_EAST_bmp_0043f8e0[5];
  local_400[2] = s__Player_EAST_bmp_0043f8e0[6];
  local_400[3] = s__Player_EAST_bmp_0043f8e0[7];
  local_3fc[0] = s__Player_EAST_bmp_0043f8e0[8];
  local_3fc[1] = s__Player_EAST_bmp_0043f8e0[9];
  local_3fc[2] = s__Player_EAST_bmp_0043f8e0[10];
  local_3fc[3] = s__Player_EAST_bmp_0043f8e0[0xb];
  local_3f8[0] = s__Player_EAST_bmp_0043f8e0[0xc];
  local_3f8[1] = s__Player_EAST_bmp_0043f8e0[0xd];
  local_3f8[2] = s__Player_EAST_bmp_0043f8e0[0xe];
  local_3f8[3] = s__Player_EAST_bmp_0043f8e0[0xf];
  local_3f4 = s__Player_EAST_bmp_0043f8e0[0x10];
  puVar7 = &local_3f3;
  for (iVar4 = 0x1b; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  *(undefined1 *)((int)puVar7 + 2) = 0;
  pcVar6 = s__Player_SOUTHEAST_bmp_0043f8c8;
  puVar7 = local_384;
  for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  puVar7 = local_36e;
  for (iVar4 = 0x1a; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  local_300[0] = s__Player_SOUTH_bmp_0043f8b4[4];
  local_300[1] = s__Player_SOUTH_bmp_0043f8b4[5];
  local_300[2] = s__Player_SOUTH_bmp_0043f8b4[6];
  local_300[3] = s__Player_SOUTH_bmp_0043f8b4[7];
  local_304[0] = s__Player_SOUTH_bmp_0043f8b4[0];
  local_304[1] = s__Player_SOUTH_bmp_0043f8b4[1];
  local_304[2] = s__Player_SOUTH_bmp_0043f8b4[2];
  local_304[3] = s__Player_SOUTH_bmp_0043f8b4[3];
  local_2f4[0] = s__Player_SOUTH_bmp_0043f8b4[0x10];
  local_2f4[1] = s__Player_SOUTH_bmp_0043f8b4[0x11];
  local_2f8[0] = s__Player_SOUTH_bmp_0043f8b4[0xc];
  local_2f8[1] = s__Player_SOUTH_bmp_0043f8b4[0xd];
  local_2f8[2] = s__Player_SOUTH_bmp_0043f8b4[0xe];
  local_2f8[3] = s__Player_SOUTH_bmp_0043f8b4[0xf];
  local_2fc[0] = s__Player_SOUTH_bmp_0043f8b4[8];
  local_2fc[1] = s__Player_SOUTH_bmp_0043f8b4[9];
  local_2fc[2] = s__Player_SOUTH_bmp_0043f8b4[10];
  local_2fc[3] = s__Player_SOUTH_bmp_0043f8b4[0xb];
  puVar7 = local_2f2;
  for (iVar4 = 0x1b; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  pcVar6 = s__Player_SOUTHWEST_bmp_0043f89c;
  puVar7 = local_284;
  for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  puVar7 = local_26e;
  for (iVar4 = 0x1a; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  local_204[0] = s__Player_WEST_bmp_0043f888[0];
  local_204[1] = s__Player_WEST_bmp_0043f888[1];
  local_204[2] = s__Player_WEST_bmp_0043f888[2];
  local_204[3] = s__Player_WEST_bmp_0043f888[3];
  local_200[0] = s__Player_WEST_bmp_0043f888[4];
  local_200[1] = s__Player_WEST_bmp_0043f888[5];
  local_200[2] = s__Player_WEST_bmp_0043f888[6];
  local_200[3] = s__Player_WEST_bmp_0043f888[7];
  local_1fc[0] = s__Player_WEST_bmp_0043f888[8];
  local_1fc[1] = s__Player_WEST_bmp_0043f888[9];
  local_1fc[2] = s__Player_WEST_bmp_0043f888[10];
  local_1fc[3] = s__Player_WEST_bmp_0043f888[0xb];
  local_1f4 = s__Player_WEST_bmp_0043f888[0x10];
  local_1f8[0] = s__Player_WEST_bmp_0043f888[0xc];
  local_1f8[1] = s__Player_WEST_bmp_0043f888[0xd];
  local_1f8[2] = s__Player_WEST_bmp_0043f888[0xe];
  local_1f8[3] = s__Player_WEST_bmp_0043f888[0xf];
  puVar7 = &local_1f3;
  for (iVar4 = 0x1b; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  *(undefined1 *)((int)puVar7 + 2) = 0;
  pcVar6 = s__Player_NORTHWEST_bmp_0043f870;
  puVar7 = local_184;
  for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = *(undefined2 *)pcVar6;
  puVar7 = local_16e;
  for (iVar4 = 0x1a; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  *(undefined2 *)puVar7 = 0;
  crt_sprintf(local_104,&DAT_0043e15c);
  DAT_004fc9e8 = LoadBitmapToDirectDrawSurface(piVar5,local_104,0,0);
  RegisterBitmapSurface(&DAT_004fc9e8,local_104);
  iVar4 = 0;
  if (0 < DAT_0043ee8c) {
    puVar7 = &DAT_004fc9ec;
    do {
      crt_sprintf(local_104,(byte *)s__s_piece_d_bmp_0043f860);
      piVar2 = LoadBitmapToDirectDrawSurface(piVar5,local_104,0,0);
      *puVar7 = piVar2;
      RegisterBitmapSurface(puVar7,local_104);
      MarkRegisteredSurfaceColorKeyed((int)puVar7);
      SetSurfaceTransparencyColorKey((int *)*puVar7,0xff00ff);
      iVar4 = iVar4 + 1;
      puVar7 = puVar7 + 1;
    } while (iVar4 < DAT_0043ee8c);
  }
  iVar4 = 0;
  if (0 < DAT_0043ee8c) {
    local_604 = &DAT_004fc9ec;
    piVar2 = &DAT_004fc470;
    piVar5 = &DAT_00510358;
    do {
      iVar1 = *piVar5;
      iVar3 = DAT_0043ee8c + iVar4;
      *piVar2 = iVar1;
      piVar2[-10] = (&DAT_005101c8)[iVar1 * 2];
      iVar1 = (&DAT_005101c8)[iVar3 * 2];
      iVar3 = (&DAT_005101cc)[iVar3 * 2];
      piVar2[-9] = (&DAT_005101cc)[*piVar2 * 2];
      piVar2[-8] = iVar1;
      piVar2[-7] = iVar3;
      local_604 = (int *)*local_604;
      piVar2[-6] = 0;
      piVar2[1] = (int)local_604;
      (**(code **)(*local_604 + 0x58))(local_604,local_600);
      (**(code **)(*(int *)piVar2[1] + 0x58))((int *)piVar2[1],&stack0xfffff9f8);
      piVar2[-1] = 1;
      piVar2[-5] = 0;
      piVar2[-4] = 0;
      piVar2[-3] = iStack_5f4;
      piVar2[-2] = iStack_5f8;
      local_604 = (int *)0xa;
      iVar4 = iVar4 + 1;
      piVar5 = piVar5 + 2;
      piVar2 = piVar2 + 0xc;
    } while (iVar4 < DAT_0043ee8c);
  }
  DAT_004fc418 = 0;
  DAT_004fc400 = (&DAT_005101c8)[DAT_0043ee8c * 4];
  DAT_004fc404 = (&DAT_005101cc)[DAT_0043ee8c * 4];
  _DAT_004fc408 = (float)DAT_004fc400;
  _DAT_004fc41c = 0;
  _DAT_004fc420 = 0;
  _DAT_004fc40c = (float)DAT_004fc404;
  DAT_004fc424 = 4;
  _DAT_004fc42c = 0x3f000000;
  DAT_004fc428 = 1;
  DAT_004fc3f0 = 0;
  _DAT_004fc410 = DAT_004fc400;
  _DAT_004fc414 = DAT_004fc404;
  ApplyGlobalGameVolume((DAT_00446ce0 + -100) * 0x2a);
  DAT_0051c30c = 1;
  DAT_004fca64 = 0;
  DAT_004fca68 = 0;
  return;
}

