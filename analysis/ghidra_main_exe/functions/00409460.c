/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409460; function: ExportBackBufferToPrintBitmap; body bytes: 720
 * callers: 1; callees: 5; success: True
 */


void ExportBackBufferToPrintBitmap(void)

{
  ushort uVar1;
  int *piVar2;
  int *piVar3;
  uint uVar4;
  char *pcVar5;
  int iVar6;
  int iVar7;
  ushort *puVar8;
  ushort *puVar9;
  char cVar10;
  int iVar11;
  uint uVar12;
  char cVar13;
  char cVar14;
  undefined4 *puVar15;
  byte bVar16;
  byte bVar17;
  byte bVar18;
  int iStack_6c;
  FILE *pFStack_58;
  int *piStack_54;
  char acStack_50 [2];
  int iStack_4e;
  undefined2 uStack_4a;
  undefined2 uStack_48;
  undefined2 uStack_46;
  undefined2 local_44;
  undefined2 uStack_42;
  int *local_40;
  int iStack_3c;
  int iStack_38;
  undefined2 uStack_34;
  ushort uStack_32;
  undefined2 uStack_30;
  undefined2 uStack_2e;
  int iStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined1 uStack_18;
  undefined1 uStack_17;
  undefined1 uStack_16;
  undefined1 uStack_15;
  
  piVar2 = *(int **)(DAT_0044de08 + 0xc);
  local_40 = piVar2;
  piVar3 = (int *)crt_fopen(s_PrintMe_bmp_0043ee74,&DAT_0043ee80);
  puVar15 = &DAT_004fc178;
  for (iVar7 = 0x1f; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar15 = 0;
    puVar15 = puVar15 + 1;
  }
  DAT_004fc178 = 0x7c;
  local_44 = SUB42(piVar3,0);
  uStack_42 = (undefined2)((uint)piVar3 >> 0x10);
  (**(code **)(*piVar2 + 100))(piVar2,0,&DAT_004fc178,0x11,0);
  acStack_50[0] = 'B';
  acStack_50[1] = 'M';
  iStack_4e = (DAT_004fc180 * DAT_004fc184 + 0x12) * 3;
  uStack_48 = 0;
  uStack_4a = 0;
  uStack_46 = 0x36;
  local_44 = 0;
  FUN_00430a52(acStack_50,0xe,1,piVar3);
  iVar7 = DAT_004fc19c;
  iStack_38 = DAT_004fc180;
  iStack_2c = DAT_004fc180 * DAT_004fc184;
  local_40 = (int *)0x28;
  iStack_3c = DAT_004fc184;
  uStack_34 = 1;
  uStack_32 = 0x18;
  uStack_30 = 0;
  uStack_2e = 0;
  uStack_28 = 0;
  uStack_24 = 0;
  uStack_20 = 0;
  uStack_1c = 0;
  uStack_18 = 0;
  uStack_17 = 0;
  uStack_16 = 0;
  uStack_15 = 0;
  FUN_00430a52((char *)&local_40,0x2c,1,piVar3);
  bVar17 = 0;
  for (uVar4 = DAT_004fc1d0; (uVar4 & 1) == 0; uVar4 = (int)uVar4 >> 1) {
    bVar17 = bVar17 + 1;
  }
  cVar14 = '\0';
  for (uVar4 = DAT_004fc1d0 >> (bVar17 & 0x1f); (uVar4 & 1) != 0; uVar4 = (int)uVar4 >> 1) {
    cVar14 = cVar14 + '\x01';
  }
  bVar16 = 0;
  for (uVar4 = DAT_004fc1d4; (uVar4 & 1) == 0; uVar4 = (int)uVar4 >> 1) {
    bVar16 = bVar16 + 1;
  }
  cVar13 = '\0';
  for (uVar4 = DAT_004fc1d4 >> (bVar16 & 0x1f); (uVar4 & 1) != 0; uVar4 = (int)uVar4 >> 1) {
    cVar13 = cVar13 + '\x01';
  }
  bVar18 = 0;
  for (uVar4 = DAT_004fc1d8; (uVar4 & 1) == 0; uVar4 = (int)uVar4 >> 1) {
    bVar18 = bVar18 + 1;
  }
  cVar10 = '\0';
  for (uVar4 = DAT_004fc1d8 >> (bVar18 & 0x1f); (uVar4 & 1) != 0; uVar4 = (int)uVar4 >> 1) {
    cVar10 = cVar10 + '\x01';
  }
  uVar12 = (uint)uStack_32 * DAT_004fc180 * DAT_004fc184 >> 3;
  pcVar5 = (char *)operator_new(uVar12);
  puVar8 = (ushort *)((iStack_38 + -1) * DAT_004fc188 + iVar7);
  uVar4 = DAT_004fc188 >> 1;
  if (-1 < iStack_38 + -1) {
    iStack_6c = iStack_38;
    iVar7 = 0;
    do {
      iVar11 = 0;
      iVar6 = iVar7;
      puVar9 = puVar8;
      if (0 < iStack_3c) {
        do {
          uVar1 = *puVar9;
          pcVar5[iVar7] = (char)(uVar1 >> (bVar16 & 0x1f)) << (8U - cVar13 & 0x1f);
          pcVar5[iVar7 + 1] = (char)(uVar1 >> (bVar17 & 0x1f)) << (8U - cVar14 & 0x1f);
          iVar6 = iVar7 + 3;
          iVar11 = iVar11 + 1;
          pcVar5[iVar7 + 2] = (char)(uVar1 >> (bVar18 & 0x1f)) << (8U - cVar10 & 0x1f);
          iVar7 = iVar6;
          puVar9 = puVar9 + 1;
        } while (iVar11 < iStack_3c);
      }
      puVar8 = puVar8 + -uVar4;
      iStack_6c = iStack_6c + -1;
      iVar7 = iVar6;
    } while (iStack_6c != 0);
  }
  FUN_00430a52(pcVar5,uVar12,1,(int *)pFStack_58);
  FUN_0042fbdc(pcVar5);
  crt_fclose(pFStack_58);
  (**(code **)(*piStack_54 + 0x80))(piStack_54,0);
  return;
}

