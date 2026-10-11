/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00411c80; function: InitializeFireworksActivity; body bytes: 2664
 * callers: 1; callees: 12; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeFireworksActivity(void)

{
  undefined4 uVar1;
  FILE *pFVar2;
  uint uVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  int *piVar7;
  undefined4 *puVar8;
  int iVar9;
  int *piVar10;
  int *piVar11;
  uint local_92c;
  FILE *local_928 [2];
  int *piStack_920;
  undefined4 local_91c;
  undefined4 local_918;
  int iStack_914;
  int iStack_910;
  undefined1 local_8a0;
  undefined4 local_89f;
  undefined1 local_820;
  undefined4 local_81f;
  undefined1 local_7a0;
  undefined4 local_79f;
  undefined1 local_720;
  undefined4 local_71f;
  undefined1 local_6a0;
  undefined4 local_69f;
  undefined1 local_620;
  undefined4 local_61f;
  undefined1 local_5a0;
  undefined4 local_59f;
  undefined1 local_520;
  undefined4 local_51f;
  undefined1 local_4a0;
  undefined4 local_49f;
  undefined1 local_420;
  undefined4 local_41f;
  char local_3a0 [4];
  char local_39c [4];
  char local_398;
  undefined4 local_397;
  int aiStack_320 [72];
  undefined4 auStack_200 [128];
  
  DAT_00446f38 = 0xffffffff;
  piVar7 = *(int **)(DAT_0044de08 + 4);
  DAT_00446ce8 = 4;
  DAT_00519940 = SaveFireworksLayoutAndUnloadResources;
  DAT_00442a08 = 0xa8;
  DAT_00442a10 = 0;
  DAT_00442a04 = 0x104;
  DAT_00442a0c = 0x104;
  *(undefined4 *)(&DAT_0051b5cc + DAT_00519934 * 400) = 1;
  DAT_0050ab20 = 0;
  DAT_0050ab24 = 0;
  DAT_00442a24 = 4;
  DAT_00442a28 = 4;
  DAT_0050ab28 = 0;
  DAT_0050ab2c = 0;
  DAT_0050ab30 = 0;
  DAT_0050ab34 = 0;
  _DAT_00509670 = 0;
  _DAT_00509674 = 0;
  DAT_0050ab60 = 0;
  DAT_0050ab64 = 0;
  LoadFireworksEditorResources();
  puVar5 = &DAT_0050a678;
  for (iVar6 = 0x12; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0xffffffff;
    puVar5 = puVar5 + 1;
  }
  puVar5 = &DAT_005123b8;
  for (iVar6 = 0x78; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0xffffffff;
    puVar5 = puVar5 + 1;
  }
  pFVar2 = (FILE *)crt_fopen(s_firedata1_txt_00442638 + DAT_00519934 * 0x32,&DAT_00441f40);
  if (pFVar2 != (FILE *)0x0) {
    local_92c = 0;
    do {
      iVar6 = 0;
      do {
        uVar3 = FUN_00430937((char *)local_928,4,1,(int *)pFVar2);
        if (uVar3 == 0) goto LAB_00411dbe;
        iVar4 = local_92c + iVar6;
        iVar6 = iVar6 + 1;
        (&DAT_0050a678)[iVar4] = local_928[0];
      } while (iVar6 < 6);
      local_92c = local_92c + 6;
    } while ((int)local_92c < 0x12);
LAB_00411dbe:
    crt_fclose(pFVar2);
  }
  local_8a0 = DAT_00482574;
  puVar5 = &DAT_004fc084;
  for (iVar6 = 0x14; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  puVar5 = &local_89f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_820 = local_8a0;
  puVar5 = &local_81f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_7a0 = local_8a0;
  puVar5 = &local_79f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_720 = local_8a0;
  puVar5 = &local_71f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_6a0 = local_8a0;
  puVar5 = &local_69f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_620 = local_8a0;
  puVar5 = &local_61f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_5a0 = local_8a0;
  puVar5 = &local_59f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_520 = local_8a0;
  puVar5 = &local_51f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_4a0 = local_8a0;
  puVar5 = &local_49f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_420 = local_8a0;
  puVar5 = &local_41f;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  local_398 = s_tune_wav_004435cc[8];
  local_3a0[0] = s_tune_wav_004435cc[0];
  local_3a0[1] = s_tune_wav_004435cc[1];
  local_3a0[2] = s_tune_wav_004435cc[2];
  local_3a0[3] = s_tune_wav_004435cc[3];
  local_39c[0] = s_tune_wav_004435cc[4];
  local_39c[1] = s_tune_wav_004435cc[5];
  local_39c[2] = s_tune_wav_004435cc[6];
  local_39c[3] = s_tune_wav_004435cc[7];
  puVar5 = &local_397;
  for (iVar6 = 0x1d; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  CSoundManager_Create
            (DAT_004fc174,&DAT_004fc0ac,local_3a0,0,DAT_0043b5a8,DAT_0043b5ac,DAT_0043b5b0,
             DAT_0043b5b4,1);
  DAT_00442a00 = 1;
  DAT_0050a5bc = 0;
  puVar5 = &DAT_0050a6d0;
  do {
    *puVar5 = 0xffffffff;
    puVar5 = puVar5 + 5;
  } while ((int)puVar5 < 0x50aab8);
  DAT_00509378 = 0;
  DAT_00508ad0 = LoadBitmapToDirectDrawSurface(piVar7,s_data_ui_printbar_bmp_0043f978,0,0);
  RegisterBitmapSurface(&DAT_00508ad0,s_data_ui_printbar_bmp_0043f978);
  MarkRegisteredSurfaceColorKeyed(0x508ad0);
  SetSurfaceTransparencyColorKey(DAT_00508ad0,0xff00ff);
  DAT_0050a4b8 = LoadBitmapToDirectDrawSurface
                           (piVar7,s_Data_SubGameFirework_Graph_red_a_0044359c,0,0);
  RegisterBitmapSurface(&DAT_0050a4b8,s_Data_SubGameFirework_Graph_red_a_0044359c);
  DAT_0050a4bc = LoadBitmapToDirectDrawSurface
                           (piVar7,s_Data_SubGameFirework_Graph_small_0044356c,0,0);
  RegisterBitmapSurface(&DAT_0050a4bc,s_Data_SubGameFirework_Graph_small_0044356c);
  _DAT_0050a4c0 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_mediu_0044353c,0,0);
  RegisterBitmapSurface(&DAT_0050a4c0,s_Data_SubGameFirework_Graph_mediu_0044353c);
  _DAT_0050a4c4 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_blue__0044350c,0,0);
  RegisterBitmapSurface(&DAT_0050a4c4,s_Data_SubGameFirework_Graph_blue__0044350c);
  _DAT_0050a4c8 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_red_l_004434e0,0,0);
  RegisterBitmapSurface(&DAT_0050a4c8,s_Data_SubGameFirework_Graph_red_l_004434e0);
  _DAT_0050a4cc =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_mediu_004434b0,0,0);
  RegisterBitmapSurface(&DAT_0050a4cc,s_Data_SubGameFirework_Graph_mediu_004434b0);
  _DAT_0050a4d0 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_small_00443480,0,0);
  RegisterBitmapSurface(&DAT_0050a4d0,s_Data_SubGameFirework_Graph_small_00443480);
  _DAT_0050a4d4 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_blue__00443450,0,0);
  RegisterBitmapSurface(&DAT_0050a4d4,s_Data_SubGameFirework_Graph_blue__00443450);
  _DAT_0050a4d8 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_red_c_00443420,0,0);
  RegisterBitmapSurface(&DAT_0050a4d8,s_Data_SubGameFirework_Graph_red_c_00443420);
  _DAT_0050a4dc =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_red_s_004433f0,0,0);
  RegisterBitmapSurface(&DAT_0050a4dc,s_Data_SubGameFirework_Graph_red_s_004433f0);
  _DAT_0050a4e0 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_green_004433c0,0,0);
  RegisterBitmapSurface(&DAT_0050a4e0,s_Data_SubGameFirework_Graph_green_004433c0);
  _DAT_0050a4e4 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_blue__00443390,0,0);
  RegisterBitmapSurface(&DAT_0050a4e4,s_Data_SubGameFirework_Graph_blue__00443390);
  DAT_0050a568 = LoadBitmapToDirectDrawSurface
                           (piVar7,s_Data_SubGameFirework_Graph_red_a_00443360,0,0);
  RegisterBitmapSurface(&DAT_0050a568,s_Data_SubGameFirework_Graph_red_a_00443360);
  _DAT_0050a56c =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_small_00443330,0,0);
  RegisterBitmapSurface(&DAT_0050a56c,s_Data_SubGameFirework_Graph_small_00443330);
  _DAT_0050a570 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_mediu_00443300,0,0);
  RegisterBitmapSurface(&DAT_0050a570,s_Data_SubGameFirework_Graph_mediu_00443300);
  _DAT_0050a574 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_blue__004432d0,0,0);
  RegisterBitmapSurface(&DAT_0050a574,s_Data_SubGameFirework_Graph_blue__004432d0);
  _DAT_0050a578 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_red_l_004432a4,0,0);
  RegisterBitmapSurface(&DAT_0050a578,s_Data_SubGameFirework_Graph_red_l_004432a4);
  _DAT_0050a57c =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_mediu_00443274,0,0);
  RegisterBitmapSurface(&DAT_0050a57c,s_Data_SubGameFirework_Graph_mediu_00443274);
  _DAT_0050a580 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_small_00443244,0,0);
  RegisterBitmapSurface(&DAT_0050a580,s_Data_SubGameFirework_Graph_small_00443244);
  _DAT_0050a584 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_green_00443214,0,0);
  RegisterBitmapSurface(&DAT_0050a584,s_Data_SubGameFirework_Graph_green_00443214);
  _DAT_0050a588 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_red_c_004431e4,0,0);
  RegisterBitmapSurface(&DAT_0050a588,s_Data_SubGameFirework_Graph_red_c_004431e4);
  _DAT_0050a58c =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_red_s_004431b4,0,0);
  RegisterBitmapSurface(&DAT_0050a58c,s_Data_SubGameFirework_Graph_red_s_004431b4);
  _DAT_0050a590 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_blue__00443184,0,0);
  RegisterBitmapSurface(&DAT_0050a590,s_Data_SubGameFirework_Graph_blue__00443184);
  _DAT_0050a594 =
       LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameFirework_Graph_blue__00443154,0,0);
  RegisterBitmapSurface(&DAT_0050a594,s_Data_SubGameFirework_Graph_blue__00443154);
  DAT_0050ab0c = LoadBitmapToDirectDrawSurface
                           (piVar7,s_Data_SubGameFirework_graph_Bk_01_0044312c,0,0);
  RegisterBitmapSurface(&DAT_0050ab0c,s_Data_SubGameFirework_graph_Bk_01_0044312c);
  DAT_0050ab10 = LoadBitmapToDirectDrawSurface
                           (piVar7,s_Data_SubGameFirework_graph_BK_01_00443104,0,0);
  RegisterBitmapSurface(&DAT_0050ab10,s_Data_SubGameFirework_graph_BK_01_00443104);
  iVar6 = 0;
  do {
    MarkRegisteredSurfaceColorKeyed((int)&DAT_0050a4b8 + iVar6);
    SetSurfaceTransparencyColorKey(*(int **)((int)&DAT_0050a4b8 + iVar6),0xff00ff);
    MarkRegisteredSurfaceColorKeyed((int)&DAT_0050a568 + iVar6);
    SetSurfaceTransparencyColorKey(*(int **)((int)&DAT_0050a568 + iVar6),0xff00ff);
    iVar6 = iVar6 + 4;
  } while (iVar6 < 0x30);
  local_92c = 0;
  puVar5 = &DAT_0050a4b8;
  piVar10 = &DAT_00509688;
  do {
    piVar10[4] = 100;
    *piVar10 = local_92c;
    piVar11 = (int *)*puVar5;
    piVar10[1] = 1;
    local_91c = 0x7c;
    local_918 = 6;
    piVar10[2] = (int)piVar11;
    (**(code **)(*piVar11 + 0x58))(piVar11,&local_91c);
    piVar10[-3] = 0;
    piVar10[-4] = 0;
    piVar10[-1] = iStack_914;
    piVar10[-2] = iStack_910;
    puVar5 = puVar5 + 1;
    local_92c = local_92c + 1;
    piVar10 = piVar10 + 9;
  } while ((int)puVar5 < 0x50a4e8);
  DAT_00508c0c = LoadBitmapToDirectDrawSurface(piVar7,s_Data_SubGameDYP_delcurs_bmp_00442500,0,0);
  RegisterBitmapSurface(&DAT_00508c0c,s_Data_SubGameDYP_delcurs_bmp_00442500);
  MarkRegisteredSurfaceColorKeyed(0x508c0c);
  SetSurfaceTransparencyColorKey(DAT_00508c0c,0xff00ff);
  local_92c = 0;
  local_928[0] = (FILE *)OpenGameDataFileWithCDFallback
                                   (s_data_subGameFirework_graph_firew_004430d8,&DAT_0043e070);
  if (local_928[0] != (FILE *)0x0) {
    piStack_920 = (int *)0x12;
    local_92c = 0x12;
    piVar7 = aiStack_320 + 2;
    do {
      crt_fscanf(local_928[0],(int *)local_928[0],(byte *)s__d__d__d__d__d__d__d__d_004430bc);
      piVar7[-2] = piVar7[-2] + -0x14;
      *piVar7 = *piVar7 + -0x14;
      piVar7[-1] = piVar7[-1] + -0x1e;
      piStack_920 = (int *)((int)piStack_920 + -1);
      piVar7[1] = piVar7[1] + -0x1e;
      piVar7 = piVar7 + 4;
    } while (piStack_920 != (int *)0x0);
    piStack_920 = (int *)0x0;
  }
  crt_fclose(local_928[0]);
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_data_subGameFirework_graph_firew_0044308c,&DAT_0043e070);
  if ((pFVar2 != (FILE *)0x0) && (local_92c < 0x1e)) {
    iVar6 = 0x1e - local_92c;
    piVar7 = aiStack_320 + local_92c * 4 + 2;
    do {
      crt_fscanf(piVar7 + -1,(int *)pFVar2,(byte *)s__d__d__d__d__d__d__d__d_004430bc);
      piVar7 = piVar7 + 4;
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
  }
  crt_fclose(pFVar2);
  local_928[0] = (FILE *)0x3;
  piVar7 = aiStack_320 + 1;
  piVar10 = &DAT_0050a6c4 + DAT_00509378 * 5;
  do {
    piVar11 = piVar10 + 0x1e;
    piStack_920 = piVar7 + 0x18;
    iVar6 = 6;
    do {
      piVar10[-1] = piVar7[-1];
      *piVar10 = *piVar7 + 0x1e;
      piVar10[1] = piVar7[1];
      piVar10[2] = piVar7[2] + 0x1e;
      piVar10[3] = 0xc;
      piVar10 = piVar10 + 5;
      iVar6 = iVar6 + -1;
      piVar7 = piVar7 + 4;
    } while (iVar6 != 0);
    local_928[0] = (FILE *)((int)&local_928[0][-1]._tmpfname + 3);
    piVar7 = piStack_920;
    piVar10 = piVar11;
  } while (local_928[0] != (FILE *)0x0);
  iVar4 = 0;
  iVar6 = DAT_00509378 + 0x1e;
  puVar5 = &DAT_0050a6c4 + (DAT_00509378 + 0x12) * 5;
  puVar8 = auStack_200 + 1;
  do {
    puVar5[-1] = puVar8[-1];
    *puVar5 = *puVar8;
    puVar5[1] = puVar8[1];
    puVar5[2] = puVar8[2];
    puVar5[3] = iVar4;
    puVar5 = puVar5 + 5;
    iVar4 = iVar4 + 1;
    puVar8 = puVar8 + 4;
  } while (iVar4 < 0xc);
  iVar4 = DAT_00509378 + 0x1f;
  (&DAT_0050a6c0)[iVar6 * 5] = DAT_004427d8;
  (&DAT_0050a6c4)[iVar6 * 5] = DAT_004427dc;
  (&DAT_0050a6c8)[iVar6 * 5] = DAT_004427e0;
  (&DAT_0050a6cc)[iVar6 * 5] = DAT_004427e4;
  uVar1 = DAT_004427e8;
  (&DAT_0050a6d0)[iVar6 * 5] = 0x1b;
  iVar9 = DAT_00509378 + 0x20;
  (&DAT_0050a6c0)[iVar4 * 5] = uVar1;
  (&DAT_0050a6c4)[iVar4 * 5] = DAT_004427ec;
  (&DAT_0050a6c8)[iVar4 * 5] = DAT_004427f0;
  (&DAT_0050a6cc)[iVar4 * 5] = DAT_004427f4;
  uVar1 = DAT_004427f8;
  (&DAT_0050a6d0)[iVar4 * 5] = 0x19;
  DAT_00509378 = DAT_00509378 + 0x21;
  (&DAT_0050a6c0)[iVar9 * 5] = uVar1;
  (&DAT_0050a6c4)[iVar9 * 5] = DAT_004427fc;
  (&DAT_0050a6c8)[iVar9 * 5] = DAT_00442800;
  iVar6 = DAT_00446ce0;
  (&DAT_0050a6cc)[iVar9 * 5] = DAT_00442804;
  (&DAT_0050a6d0)[iVar9 * 5] = 0x1a;
  ApplyGlobalGameVolume((iVar6 + -100) * 0x2a);
  DAT_0050ab70 = 0;
  DAT_0050ab74 = 0;
  DAT_0051c30c = 1;
  return;
}

