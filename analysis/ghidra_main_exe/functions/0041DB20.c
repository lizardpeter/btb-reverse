/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041db20; function: InitializeBobsBandActivity; body bytes: 5477
 * callers: 1; callees: 11; success: True
 */


void InitializeBobsBandActivity(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  FILE *pFVar5;
  int *piVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  undefined4 *puVar10;
  int iVar11;
  undefined4 *puVar12;
  uint uVar13;
  undefined4 *puVar14;
  char *pcVar15;
  LPSTR pCVar16;
  LPCSTR pCVar17;
  int iVar18;
  char *pcVar19;
  int iVar20;
  undefined4 unaff_retaddr;
  int *piStack00000004;
  undefined4 uStack00000027;
  undefined4 uStack0000002b;
  undefined4 uStack0000002f;
  undefined4 uStack00000033;
  undefined2 uStack00000037;
  undefined1 uStack00000039;
  undefined4 uStack00000059;
  undefined4 uStack0000005d;
  undefined4 uStack00000061;
  undefined4 uStack00000065;
  undefined2 uStack00000069;
  undefined1 uStack0000006b;
  undefined4 uStack0000008a;
  undefined4 uStack0000008e;
  undefined4 uStack00000092;
  undefined4 uStack00000096;
  undefined4 uStack0000009a;
  undefined4 uStack000000bc;
  undefined4 uStack000000c0;
  undefined4 uStack000000c4;
  undefined4 uStack000000c8;
  undefined4 uStack000000cc;
  undefined4 uStack000000ef;
  undefined4 uStack000000f3;
  undefined4 uStack000000f7;
  undefined4 uStack000000fb;
  undefined2 uStack000000ff;
  undefined1 uStack00000101;
  undefined4 uStack00000121;
  undefined4 uStack00000125;
  undefined4 uStack00000129;
  undefined4 uStack0000012d;
  undefined2 uStack00000131;
  undefined1 uStack00000133;
  undefined4 uStack00000153;
  undefined4 uStack00000157;
  undefined4 uStack0000015b;
  undefined4 uStack0000015f;
  undefined2 uStack00000163;
  undefined1 uStack00000165;
  undefined4 uStack00000185;
  undefined4 uStack00000189;
  undefined4 uStack0000018d;
  undefined4 uStack00000191;
  undefined2 uStack00000195;
  undefined1 uStack00000197;
  undefined4 uStack000001b7;
  undefined4 uStack000001bb;
  undefined4 uStack000001bf;
  undefined4 uStack000001c3;
  undefined2 uStack000001c7;
  undefined1 uStack000001c9;
  undefined4 uStack000001e9;
  undefined4 uStack000001ed;
  undefined4 uStack000001f1;
  undefined4 in_stack_00000448;
  undefined4 in_stack_0000044c;
  
  FUN_00430f00();
  piVar6 = *(int **)(DAT_0044de08 + 4);
  LoadGrandOpeningMachineData();
  DAT_00519940 = SaveAndUnloadBobsBandActivity;
  DAT_00513f28 = 0;
  DAT_00513f18 = 0;
  DAT_00508c0c = LoadBitmapToDirectDrawSurface(piVar6,s_Data_SubGameDYP_delcurs_bmp_00442500,0,0);
  RegisterBitmapSurface(&DAT_00508c0c,s_Data_SubGameDYP_delcurs_bmp_00442500);
  MarkRegisteredSurfaceColorKeyed(0x508c0c);
  SetSurfaceTransparencyColorKey(DAT_00508c0c,0xff00ff);
  puVar14 = &DAT_00513f00;
  for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  puVar14 = &DAT_00512378;
  for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  puVar14 = &DAT_00512160;
  for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  puVar14 = &DAT_004fc084;
  for (iVar7 = 0x14; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_roley1_1_wav_00445990;
  puVar14 = (undefined4 *)&stack0x00000ebc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00000eda;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley2_1_wav_00445970;
  puVar14 = (undefined4 *)&stack0x00000f3c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00000f5a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_muck1_1_wav_00445950;
  pcVar19 = &stack0x00000fbc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000fd9;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_muck2_1_wav_00445930;
  pcVar19 = &stack0x0000103c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  DAT_00513f24 = 0;
  puVar14 = (undefined4 *)&stack0x00001059;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_lofty1_1_wav_00445910;
  puVar14 = (undefined4 *)&stack0x000010bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000010da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_lofty2_1_wav_004458f0;
  puVar14 = (undefined4 *)&stack0x0000113c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000115a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy1_1_wav_004458d0;
  puVar14 = (undefined4 *)&stack0x000011bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000011da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy2_1_wav_004458b0;
  puVar14 = (undefined4 *)&stack0x0000123c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000125a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop1_1_wav_00445890;
  puVar14 = (undefined4 *)&stack0x000012bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000012da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop2_1_wav_00445870;
  puVar14 = (undefined4 *)&stack0x0000133c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000135a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley1_2_wav_00445850;
  puVar14 = (undefined4 *)&stack0x000013bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000013da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley2_2_wav_00445830;
  puVar14 = (undefined4 *)&stack0x0000143c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000145a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_muck1_2_wav_00445810;
  pcVar19 = &stack0x000014bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x000014d9;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_muck2_2_wav_004457f0;
  pcVar19 = &stack0x0000153c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00001559;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_lofty1_2_wav_004457d0;
  puVar14 = (undefined4 *)&stack0x000015bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000015da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_lofty2_2_wav_004457b0;
  puVar14 = (undefined4 *)&stack0x0000163c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000165a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy1_2_wav_00445790;
  puVar14 = (undefined4 *)&stack0x000016bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000016da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy2_2_wav_00445770;
  puVar14 = (undefined4 *)&stack0x0000173c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000175a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop1_2_wav_00445750;
  puVar14 = (undefined4 *)&stack0x000017bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000017da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop2_2_wav_00445730;
  puVar14 = (undefined4 *)&stack0x0000183c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000185a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley1_3_wav_00445710;
  puVar14 = (undefined4 *)&stack0x000018bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000018da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley2_3_wav_004456f0;
  puVar14 = (undefined4 *)&stack0x0000193c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000195a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_muck1_3_wav_004456d0;
  pcVar19 = &stack0x000019bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x000019d9;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_muck2_3_wav_004456b0;
  pcVar19 = &stack0x00001a3c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00001a59;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_lofty1_3_wav_00445690;
  puVar14 = (undefined4 *)&stack0x00001abc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001ada;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_lofty2_3_wav_00445670;
  puVar14 = (undefined4 *)&stack0x00001b3c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001b5a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy1_3_wav_00445650;
  puVar14 = (undefined4 *)&stack0x00001bbc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001bda;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy2_3_wav_00445630;
  puVar14 = (undefined4 *)&stack0x00001c3c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001c5a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop1_3_wav_00445610;
  puVar14 = (undefined4 *)&stack0x00001cbc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001cda;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop2_3_wav_004455f0;
  puVar14 = (undefined4 *)&stack0x00001d3c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001d5a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley1_4_wav_004455d0;
  puVar14 = (undefined4 *)&stack0x00001dbc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001dda;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley2_4_wav_004455b0;
  puVar14 = (undefined4 *)&stack0x00001e3c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001e5a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_muck1_4_wav_00445590;
  pcVar19 = &stack0x00001ebc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00001ed9;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_muck2_4_wav_00445570;
  pcVar19 = &stack0x00001f3c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00001f59;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_lofty1_4_wav_00445550;
  puVar14 = (undefined4 *)&stack0x00001fbc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00001fda;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_lofty2_4_wav_00445530;
  puVar14 = (undefined4 *)&stack0x0000203c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000205a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy1_4_wav_00445510;
  puVar14 = (undefined4 *)&stack0x000020bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000020da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy2_4_wav_004454f0;
  puVar14 = (undefined4 *)&stack0x0000213c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000215a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop1_4_wav_004454d0;
  puVar14 = (undefined4 *)&stack0x000021bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000021da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop2_4_wav_004454b0;
  puVar14 = (undefined4 *)&stack0x0000223c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000225a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley1_5_wav_00445490;
  puVar14 = (undefined4 *)&stack0x000022bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000022da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_roley2_5_wav_00445470;
  puVar14 = (undefined4 *)&stack0x0000233c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000235a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_muck1_5_wav_00445450;
  pcVar19 = &stack0x000023bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x000023d9;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_muck2_5_wav_00445430;
  pcVar19 = &stack0x0000243c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00002459;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_lofty1_5_wav_00445410;
  puVar14 = (undefined4 *)&stack0x000024bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000024da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_lofty2_5_wav_004453f0;
  puVar14 = (undefined4 *)&stack0x0000253c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000255a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy1_5_wav_004453d0;
  puVar14 = (undefined4 *)&stack0x000025bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000025da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy2_5_wav_004453b0;
  puVar14 = (undefined4 *)&stack0x0000263c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000265a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop1_5_wav_00445390;
  puVar14 = (undefined4 *)&stack0x000026bc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000026da;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_scoop2_5_wav_00445370;
  puVar14 = (undefined4 *)&stack0x0000273c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x0000275a;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_bobmt_wav_00445354;
  puVar14 = (undefined4 *)&stack0x000027bc;
  for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  puVar14 = (undefined4 *)&stack0x000027d7;
  for (iVar7 = 0x19; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined1 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_Wendymt_wav_00445334;
  pcVar19 = &stack0x0000283c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00002859;
  for (iVar7 = 0x18; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_fpmt_wav_00445318;
  puVar14 = (undefined4 *)&stack0x000028bc;
  for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar12 = &DAT_004fc084;
  puVar14 = (undefined4 *)&stack0x000028d6;
  for (iVar7 = 0x19; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pCVar16 = &stack0x00000ebc;
  do {
    CSoundManager_Create
              (DAT_004fc174,puVar12,pCVar16,0,DAT_0043b5a8,DAT_0043b5ac,DAT_0043b5b0,DAT_0043b5b4,1)
    ;
    puVar12 = puVar12 + 1;
    pCVar16 = pCVar16 + 0x80;
  } while ((int)puVar12 < 0x4fc14c);
  CSoundManager_Create
            (DAT_004fc174,&DAT_004fc14c,&stack0x000027bc + DAT_0051c284 * 0x80,0,DAT_0043b5a8,
             DAT_0043b5ac,DAT_0043b5b0,DAT_0043b5b4,1);
  pcVar15 = s_Data_SubGameOpen_mpplayred_bmp_004452f8;
  puVar14 = (undefined4 *)&stack0x000001fc;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  pcVar15 = s_Data_SubGameOpen_mpplaydep_bmp_004452d8;
  puVar14 = (undefined4 *)&stack0x0000022e;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  pcVar15 = s_Data_SubGameOpen_mpstopred_bmp_004452b8;
  puVar14 = (undefined4 *)&stack0x00000260;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  pcVar15 = s_Data_SubGameOpen_mpstopdep_bmp_00445298;
  puVar14 = (undefined4 *)&stack0x00000292;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  pcVar15 = s_Data_SubGameOpen_mpclearallred_b_00445274;
  puVar14 = (undefined4 *)&stack0x000002c4;
  for (iVar7 = 8; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  pcVar15 = s_Data_SubGameOpen_mpclearalldep_b_00445250;
  puVar14 = (undefined4 *)&stack0x000002f6;
  for (iVar7 = 8; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  pcVar15 = s_Data_SubGameOpen_mpdeletered_bmp_0044522c;
  pcVar19 = &stack0x00000328;
  for (iVar7 = 8; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  pcVar15 = s_Data_SubGameOpen_mpdeletedep_bmp_00445208;
  pcVar19 = &stack0x0000035a;
  for (iVar7 = 8; iVar7 != 0; iVar7 = iVar7 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  uVar13 = 0;
  pCVar17 = &stack0x000001fc;
  do {
    uVar8 = uVar13 & 0x80000001;
    if ((int)uVar8 < 0) {
      uVar8 = (uVar8 - 1 | 0xfffffffe) + 1;
    }
    piVar4 = LoadBitmapToDirectDrawSurface(piVar6,pCVar17,0,0);
    (&DAT_0051276c)[uVar8 + ((int)uVar13 / 2) * 2] = piVar4;
    RegisterBitmapSurface(&DAT_0051276c + uVar8 + ((int)uVar13 / 2) * 2,pCVar17);
    uVar13 = uVar13 + 1;
    pCVar17 = pCVar17 + 0x32;
  } while ((int)uVar13 < 8);
  pcVar15 = s_Data_SubGameOpen_ROLEY1SEC_bmp_004451e8;
  puVar14 = (undefined4 *)&stack0x00000008;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  uStack00000027 = 0;
  uStack0000002b = 0;
  uStack0000002f = 0;
  uStack00000033 = 0;
  uStack00000037 = 0;
  uStack00000039 = 0;
  pcVar15 = s_Data_SubGameOpen_ROLEY2SEC_bmp_004451c8;
  puVar14 = (undefined4 *)&stack0x0000003a;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  uStack00000059 = 0;
  uStack0000005d = 0;
  uStack00000061 = 0;
  uStack00000065 = 0;
  uStack00000069 = 0;
  uStack0000006b = 0;
  pcVar15 = s_Data_SubGameOpen_MUCK1SEC_bmp_004451a8;
  puVar14 = (undefined4 *)&stack0x0000006c;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  uStack0000008a = 0;
  uStack0000008e = 0;
  uStack00000092 = 0;
  uStack00000096 = 0;
  uStack0000009a = 0;
  pcVar15 = s_Data_SubGameOpen_MUCK2SEC_bmp_00445188;
  puVar14 = (undefined4 *)&stack0x0000009e;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  uStack000000bc = 0;
  uStack000000c0 = 0;
  uStack000000c4 = 0;
  uStack000000c8 = 0;
  uStack000000cc = 0;
  pcVar15 = s_Data_SubGameOpen_lofty1sec_bmp_00445168;
  puVar14 = (undefined4 *)&stack0x000000d0;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  uStack000000ef = 0;
  uStack000000f3 = 0;
  uStack000000f7 = 0;
  uStack000000fb = 0;
  uStack000000ff = 0;
  uStack00000101 = 0;
  pcVar15 = s_Data_SubGameOpen_lofty2sec_bmp_00445148;
  puVar14 = (undefined4 *)&stack0x00000102;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  uStack00000121 = 0;
  uStack00000125 = 0;
  uStack00000129 = 0;
  uStack0000012d = 0;
  uStack00000131 = 0;
  uStack00000133 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy1sec_BMP_00445128;
  puVar14 = (undefined4 *)&stack0x00000134;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  uStack00000153 = 0;
  uStack00000157 = 0;
  uStack0000015b = 0;
  uStack0000015f = 0;
  uStack00000163 = 0;
  uStack00000165 = 0;
  pcVar15 = s_Data_SubGameOpen_dizzy2sec_BMP_00445108;
  puVar14 = (undefined4 *)&stack0x00000166;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  uStack00000185 = 0;
  uStack00000189 = 0;
  uStack0000018d = 0;
  uStack00000191 = 0;
  uStack00000195 = 0;
  uStack00000197 = 0;
  pcVar15 = s_Data_SubGameOpen_SCOOP1SEC_bmp_004450e8;
  puVar14 = (undefined4 *)&stack0x00000198;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  uStack000001b7 = 0;
  uStack000001bb = 0;
  uStack000001bf = 0;
  uStack000001c3 = 0;
  uStack000001c7 = 0;
  uStack000001c9 = 0;
  pcVar15 = s_Data_SubGameOpen_SCOOP2SEC_bmp_004450c8;
  puVar14 = (undefined4 *)&stack0x000001ca;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  *(char *)((int)puVar14 + 2) = pcVar15[2];
  uStack000001e9 = 0;
  puVar14 = &DAT_00512718;
  uStack000001ed = 0;
  pCVar17 = &stack0x00000008;
  uStack000001f1 = 0;
  do {
    piVar4 = LoadBitmapToDirectDrawSurface(piVar6,pCVar17,0,0);
    *puVar14 = piVar4;
    RegisterBitmapSurface(puVar14,pCVar17);
    MarkRegisteredSurfaceColorKeyed((int)puVar14);
    SetSurfaceTransparencyColorKey((int *)*puVar14,0xff00ff);
    puVar14 = puVar14 + 1;
    pCVar17 = pCVar17 + 0x32;
  } while ((int)puVar14 < 0x512740);
  pcVar15 = s_Data_SubGameOpen_BOBINSTAND_bmp_004450a8;
  puVar14 = (undefined4 *)&stack0x0000038c;
  for (iVar7 = 8; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x000003ac;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_wendyINSTAND_bm_00445084;
  puVar14 = (undefined4 *)&stack0x000003c8;
  for (iVar7 = 8; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x000003ea;
  for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_picklesINSTAND__00445060;
  puVar14 = (undefined4 *)&stack0x00000404;
  for (iVar7 = 9; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x00000428;
  for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  DAT_005125a8 = LoadBitmapToDirectDrawSurface(piVar6,&stack0x0000038c + DAT_0051c284 * 0x3c,0,0);
  RegisterBitmapSurface(&DAT_005125a8,&stack0x0000038c + DAT_0051c284 * 0x3c);
  MarkRegisteredSurfaceColorKeyed(0x5125a8);
  SetSurfaceTransparencyColorKey(DAT_005125a8,0xff00ff);
  puVar14 = &DAT_005123b8;
  for (iVar7 = 0x78; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar14 = 0xffffffff;
    puVar14 = puVar14 + 1;
  }
  DAT_00444be4 = 1;
  pFVar5 = (FILE *)crt_fopen(s_musicbob1_txt_00444384 + (DAT_00519934 + DAT_0051c284 * 5) * 0x80,
                             &DAT_00441f40);
  if (pFVar5 != (FILE *)0x0) {
    iVar7 = 0;
    do {
      iVar18 = 0;
      do {
        uVar13 = FUN_00430937(&stack0x00000000,4,1,(int *)pFVar5);
        if (uVar13 == 0) goto LAB_0041ea82;
        iVar11 = iVar18 + iVar7;
        iVar18 = iVar18 + 1;
        (&DAT_005123b8)[iVar11] = unaff_retaddr;
      } while (iVar18 < 0x18);
      iVar7 = iVar7 + 0x18;
    } while (iVar7 < 0x78);
LAB_0041ea82:
    crt_fclose(pFVar5);
  }
  iVar7 = 0;
  puVar14 = &DAT_005127a0;
  DAT_00512188 = 0;
  do {
    *puVar14 = 0xffffffff;
    puVar14 = puVar14 + 5;
  } while ((int)puVar14 < 0x513f10);
  DAT_00509378 = 0;
  DAT_00513f1c = LoadBitmapToDirectDrawSurface(piVar6,s_Data_SubGameOpen_music_01_bmp_00445040,0,0);
  RegisterBitmapSurface(&DAT_00513f1c,s_Data_SubGameOpen_music_01_bmp_00445040);
  DAT_00513f20 = LoadBitmapToDirectDrawSurface(piVar6,s_Data_SubGameOpen_toolbar_bmp_00445020,0,0);
  RegisterBitmapSurface(&DAT_00513f20,s_Data_SubGameOpen_toolbar_bmp_00445020);
  MarkRegisteredSurfaceColorKeyed(0x513f20);
  SetSurfaceTransparencyColorKey(DAT_00513f20,0xff00ff);
  pcVar15 = s_Data_SubGameOpen_sound5_bmp_00445004;
  puVar14 = (undefined4 *)&stack0x000004bc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x000004d8;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_sound6_bmp_00444fe8;
  puVar14 = (undefined4 *)&stack0x0000053c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x00000558;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_sound3_bmp_00444fcc;
  puVar14 = (undefined4 *)&stack0x000005bc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x000005d8;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_sound4_bmp_00444fb0;
  puVar14 = (undefined4 *)&stack0x0000063c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x00000658;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_sound7_bmp_00444f94;
  puVar14 = (undefined4 *)&stack0x000006bc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x000006d8;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_sound8_bmp_00444f78;
  puVar14 = (undefined4 *)&stack0x0000073c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x00000758;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_sound9_bmp_00444f5c;
  puVar14 = (undefined4 *)&stack0x000007bc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x000007d8;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_sound10_bmp_00444f3c;
  pcVar19 = &stack0x0000083c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000859;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_sound1_bmp_00444f20;
  puVar14 = (undefined4 *)&stack0x000008bc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x000008d8;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_sound2_bmp_00444f04;
  puVar14 = (undefined4 *)&stack0x0000093c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  puVar14 = (undefined4 *)&stack0x00000958;
  for (iVar18 = 0x19; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  pcVar15 = s_Data_SubGameOpen_hsound5_bmp_00444ee4;
  pcVar19 = &stack0x000009bc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x000009d9;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_hsound6_bmp_00444ec4;
  pcVar19 = &stack0x00000a3c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000a59;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_hsound3_bmp_00444ea4;
  pcVar19 = &stack0x00000abc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000ad9;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_hsound4_bmp_00444e84;
  pcVar19 = &stack0x00000b3c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000b59;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_hsound7_bmp_00444e64;
  pcVar19 = &stack0x00000bbc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000bd9;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_hsound8_bmp_00444e44;
  pcVar19 = &stack0x00000c3c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000c59;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_hsound9_bmp_00444e24;
  pcVar19 = &stack0x00000cbc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000cd9;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_hsound10_bmp_00444e04;
  puVar14 = (undefined4 *)&stack0x00000d3c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = *(undefined2 *)pcVar15;
  puVar14 = (undefined4 *)&stack0x00000d5a;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  pcVar15 = s_Data_SubGameOpen_hsound1_bmp_00444de4;
  pcVar19 = &stack0x00000dbc;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000dd9;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  pcVar15 = s_Data_SubGameOpen_hsound2_bmp_00444dc4;
  pcVar19 = &stack0x00000e3c;
  for (iVar18 = 7; iVar18 != 0; iVar18 = iVar18 + -1) {
    *(undefined4 *)pcVar19 = *(undefined4 *)pcVar15;
    pcVar15 = pcVar15 + 4;
    pcVar19 = pcVar19 + 4;
  }
  *pcVar19 = *pcVar15;
  puVar14 = (undefined4 *)&stack0x00000e59;
  for (iVar18 = 0x18; iVar18 != 0; iVar18 = iVar18 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  iVar18 = 0;
  do {
    puVar14 = (undefined4 *)((int)&DAT_00512390 + iVar7);
    piVar4 = LoadBitmapToDirectDrawSurface(piVar6,&stack0x000004bc + iVar18,0,0);
    *puVar14 = piVar4;
    RegisterBitmapSurface(puVar14,&stack0x000004bc + iVar18);
    MarkRegisteredSurfaceColorKeyed((int)puVar14);
    SetSurfaceTransparencyColorKey((int *)*puVar14,0xff00ff);
    puVar14 = (undefined4 *)((int)&DAT_00512744 + iVar7);
    piVar4 = LoadBitmapToDirectDrawSurface(piVar6,&stack0x000009bc + iVar18,0,0);
    *puVar14 = piVar4;
    RegisterBitmapSurface(puVar14,&stack0x000009bc + iVar18);
    MarkRegisteredSurfaceColorKeyed((int)puVar14);
    SetSurfaceTransparencyColorKey((int *)*puVar14,0xff00ff);
    iVar18 = iVar18 + 0x80;
    iVar7 = iVar7 + 4;
  } while (iVar18 < 0x500);
  iVar7 = 0;
  puVar14 = &DAT_005125c4;
  do {
    uVar1 = (&DAT_00444be8)[iVar7];
    puVar14[-1] = iVar7;
    *puVar14 = uVar1;
    puVar14[1] = (&DAT_00512390)[iVar7];
    puVar14[2] = (&DAT_00444c38)[iVar7];
    puVar14[3] = 100;
    puVar14 = puVar14 + 9;
    iVar7 = iVar7 + 1;
  } while ((int)puVar14 < 0x51272c);
  puVar14 = &DAT_005125c8;
  do {
    (**(code **)(*(int *)*puVar14 + 0x58))();
    puVar14[-5] = 0;
    puVar14[-6] = 0;
    puVar14[-3] = in_stack_00000448;
    puVar14[-4] = in_stack_0000044c;
    puVar14 = puVar14 + 9;
  } while ((int)puVar14 < 0x512730);
  iVar18 = 5;
  iVar7 = DAT_00444b08;
  piVar6 = &DAT_0051279c + DAT_00509378 * 5;
  do {
    iVar2 = DAT_00444b10;
    iVar20 = 0x18;
    iVar11 = DAT_00444b0c + iVar7;
    piStack00000004 = piVar6 + 0x78;
    iVar9 = DAT_00444b04;
    do {
      piVar6[-2] = iVar7;
      *piVar6 = iVar11;
      piVar6[-3] = iVar9;
      iVar9 = iVar9 + iVar2;
      piVar6[-1] = iVar9;
      piVar6[1] = 0xb;
      piVar6 = piVar6 + 5;
      iVar20 = iVar20 + -1;
    } while (iVar20 != 0);
    iVar18 = iVar18 + -1;
    iVar7 = iVar11;
    piVar6 = piStack00000004;
  } while (iVar18 != 0);
  iVar7 = 0;
  iVar18 = DAT_00509378 + 0x82;
  puVar14 = &DAT_0051279c + (DAT_00509378 + 0x78) * 5;
  puVar12 = &DAT_00444b20;
  do {
    puVar10 = puVar12 + 4;
    puVar14[-2] = puVar12[-2];
    *puVar14 = *puVar12;
    puVar14[-3] = puVar12[-3];
    puVar14[-1] = puVar12[-1];
    puVar14[1] = iVar7;
    uVar1 = DAT_00444bc8;
    puVar14 = puVar14 + 5;
    iVar7 = iVar7 + 1;
    puVar12 = puVar10;
  } while ((int)puVar10 < 0x444bc0);
  iVar11 = DAT_00509378 + 0x83;
  (&DAT_00512790)[iVar18 * 5] = DAT_00444bc4;
  uVar3 = DAT_00444bcc;
  (&DAT_00512794)[iVar18 * 5] = uVar1;
  uVar1 = DAT_00444bd0;
  (&DAT_00512798)[iVar18 * 5] = uVar3;
  uVar3 = DAT_00444bd4;
  (&DAT_0051279c)[iVar18 * 5] = uVar1;
  uVar1 = DAT_00444bd8;
  (&DAT_005127a0)[iVar18 * 5] = 0x16;
  DAT_00509378 = DAT_00509378 + 0x84;
  (&DAT_00512790)[iVar11 * 5] = uVar3;
  uVar3 = DAT_00444bdc;
  (&DAT_00512794)[iVar11 * 5] = uVar1;
  uVar1 = DAT_00444be0;
  (&DAT_00512798)[iVar11 * 5] = uVar3;
  iVar7 = DAT_00446ce0;
  (&DAT_0051279c)[iVar11 * 5] = uVar1;
  (&DAT_005127a0)[iVar11 * 5] = 0x17;
  ApplyGlobalGameVolume((iVar7 + -100) * 0x2a);
  DAT_0051c30c = 1;
  DAT_00512740 = DAT_0051c284 * 0xd + 0x1fe;
  return;
}

