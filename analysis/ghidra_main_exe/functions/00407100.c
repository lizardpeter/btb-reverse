/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407100; function: LoadUiHotAreaReplacements; body bytes: 559
 * callers: 1; callees: 5; success: True
 */


void LoadUiHotAreaReplacements(void)

{
  byte bVar1;
  FILE *pFVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *extraout_ECX_01;
  void *pvVar6;
  int iVar7;
  byte *pbVar8;
  int *piVar9;
  bool bVar10;
  int local_7c;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  
  local_6c = 0;
  local_7c = 0;
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_loaddata_uiHotAreaReplace_txt_0043e9dc,&DAT_0043e070);
  pvVar6 = extraout_ECX;
  if (pFVar2 == (FILE *)0x0) {
    NoOpLegacyHook();
    pvVar6 = extraout_ECX_00;
  }
  crt_fscanf(pvVar6,(int *)pFVar2,&DAT_0043e054);
  local_70 = 0;
  iVar7 = 0;
  do {
    while( true ) {
      local_74 = 0;
      iVar3 = crt_fscanf(&local_74,(int *)pFVar2,&DAT_0043e8e4);
      if (iVar3 == -1) goto LAB_0040731e;
      if (local_74 != -1) break;
      iVar7 = iVar7 + 0x20;
      local_7c = 0;
      pvVar6 = (void *)0xffffffff;
      local_70 = iVar7;
      if (0x180 < iVar7) {
        NoOpLegacyHook();
        pvVar6 = extraout_ECX_01;
      }
      crt_fscanf(pvVar6,(int *)pFVar2,&DAT_0043e054);
    }
    iVar3 = (local_7c + iVar7) * 0x40c;
    *(int *)(&DAT_00494570 + iVar3) = local_74;
    piVar9 = (int *)(&DAT_00494600 + iVar3);
    local_68 = crt_fscanf(&DAT_00494574 + iVar3,(int *)pFVar2,
                          (byte *)s__d__d__d__s__d__d__d__s__d__d__s_0043e964);
    *(undefined4 *)(&DAT_0049482c + iVar3) = 0;
    *(undefined4 *)(&DAT_00494830 + iVar3) = 0;
    iVar7 = 3;
    do {
      if (*piVar9 != -1) {
        *(int *)(&DAT_00494830 + iVar3) = *(int *)(&DAT_00494830 + iVar3) + 1;
      }
      piVar9 = piVar9 + 1;
      iVar7 = iVar7 + -1;
    } while (iVar7 != 0);
    if (1 < *(int *)(&DAT_00494830 + iVar3)) {
      uVar4 = FUN_0042ffc4();
      *(int *)(&DAT_0049482c + iVar3) = (int)uVar4 % *(int *)(&DAT_00494830 + iVar3);
    }
    pbVar8 = &DAT_0043e95c;
    pbVar5 = &DAT_00494610 + iVar3;
    do {
      bVar1 = *pbVar5;
      bVar10 = bVar1 < *pbVar8;
      if (bVar1 != *pbVar8) {
LAB_004072b4:
        iVar7 = (1 - (uint)bVar10) - (uint)(bVar10 != 0);
        goto LAB_004072b9;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar5[1];
      bVar10 = bVar1 < pbVar8[1];
      if (bVar1 != pbVar8[1]) goto LAB_004072b4;
      pbVar5 = pbVar5 + 2;
      pbVar8 = pbVar8 + 2;
    } while (bVar1 != 0);
    iVar7 = 0;
LAB_004072b9:
    if (iVar7 == 0) {
      (&DAT_00494610)[iVar3] = 0;
    }
    pbVar8 = &DAT_0043e95c;
    pbVar5 = &DAT_00494718 + iVar3;
    do {
      bVar1 = *pbVar5;
      bVar10 = bVar1 < *pbVar8;
      if (bVar1 != *pbVar8) {
LAB_004072eb:
        iVar7 = (1 - (uint)bVar10) - (uint)(bVar10 != 0);
        goto LAB_004072f0;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar5[1];
      bVar10 = bVar1 < pbVar8[1];
      if (bVar1 != pbVar8[1]) goto LAB_004072eb;
      pbVar5 = pbVar5 + 2;
      pbVar8 = pbVar8 + 2;
    } while (bVar1 != 0);
    iVar7 = 0;
LAB_004072f0:
    if (iVar7 == 0) {
      (&DAT_00494718)[iVar3] = 0;
    }
    if (local_68 == -1) {
      local_6c = 1;
    }
    local_7c = local_7c + 1;
    iVar7 = local_70;
  } while (local_6c == 0);
LAB_0040731e:
  crt_fclose(pFVar2);
  return;
}

