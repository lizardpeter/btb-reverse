/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00405ee0; function: BitmapPrinterImportDIBSection; body bytes: 556
 * callers: 0; callees: 11; success: True
 */


void __thiscall BitmapPrinterImportDIBSection(int *param_1,HANDLE param_2)

{
  char cVar1;
  int iVar2;
  undefined4 *puVar3;
  HDC hdc;
  HGDIOBJ h;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  UINT UVar7;
  undefined4 *puVar8;
  char *pcVar9;
  char *pcVar10;
  undefined1 *puStack_98;
  undefined1 uStack_91;
  undefined1 auStack_90 [4];
  char *pcStack_8c;
  uint uStack_88;
  undefined4 uStack_84;
  UINT UStack_80;
  undefined **appuStack_7c [3];
  undefined1 auStack_70 [4];
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined1 auStack_60 [4];
  int iStack_5c;
  int iStack_58;
  ushort uStack_4e;
  undefined2 uStack_4c;
  undefined2 uStack_4a;
  undefined4 auStack_48 [4];
  int iStack_38;
  undefined1 *puStack_28;
  void *pvStack_c;
  undefined1 *puStack_8;
  int iStack_4;
  
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_0043a503;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  iVar2 = GetObjectA(param_2,0x54,auStack_60);
  if (iVar2 == 0x54) {
    puStack_98 = puStack_28;
    iVar2 = (uint)uStack_4e * iStack_5c + 0x1f;
    if (((short)puStack_28 == 0) && (uStack_4e < 9)) {
      puStack_98 = (undefined1 *)(1 << ((byte)uStack_4e & 0x1f));
    }
    else if (iStack_38 != 0) {
      (**(code **)(*param_1 + 0x2c))(param_2,0);
      ExceptionList = pvStack_c;
      return;
    }
    UVar7 = (UINT)(short)puStack_98;
    uVar6 = iStack_58 * (CONCAT31((int3)(iVar2 >> 0xb),(char)(iVar2 >> 3)) & 0xfffffffc);
    UStack_80 = UVar7;
    FUN_0042fbdc((undefined *)param_1[1]);
    puVar3 = (undefined4 *)operator_new(uVar6 + 0x28 + UVar7 * 4);
    param_1[1] = (int)puVar3;
    param_1[2] = (int)(puVar3 + UVar7 + 10);
    puVar8 = auStack_48;
    for (iVar2 = 10; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar3 = *puVar8;
      puVar8 = puVar8 + 1;
      puVar3 = puVar3 + 1;
    }
    if ((0 < (short)puStack_98) && (iStack_38 != 3)) {
      hdc = CreateCompatibleDC((HDC)0x0);
      h = SelectObject(hdc,param_2);
      GetDIBColorTable(hdc,0,UStack_80,(RGBQUAD *)(param_1[1] + 0x28));
      SelectObject(hdc,h);
      DeleteDC(hdc);
    }
    puVar8 = (undefined4 *)CONCAT22(uStack_4a,uStack_4c);
    puVar3 = (undefined4 *)param_1[2];
    for (uVar6 = uVar6 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *puVar3 = *puVar8;
      puVar8 = puVar8 + 1;
      puVar3 = puVar3 + 1;
    }
    for (iVar2 = 0; iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined1 *)puVar3 = *(undefined1 *)puVar8;
      puVar8 = (undefined4 *)((int)puVar8 + 1);
      puVar3 = (undefined4 *)((int)puVar3 + 1);
    }
    ExceptionList = pvStack_c;
    return;
  }
  auStack_90[0] = uStack_91;
  uVar6 = 0xffffffff;
  pcVar9 = s_Error_getting_DIB_section_info__0043e278;
  do {
    if (uVar6 == 0) break;
    uVar6 = uVar6 - 1;
    cVar1 = *pcVar9;
    pcVar9 = pcVar9 + 1;
  } while (cVar1 != '\0');
  uVar6 = ~uVar6 - 1;
  pcStack_8c = (char *)0x0;
  uStack_88 = 0;
  uStack_84 = 0;
  uVar4 = MsvcStringGrow(auStack_90,uVar6,'\x01');
  if ((char)uVar4 != '\0') {
    pcVar9 = s_Error_getting_DIB_section_info__0043e278;
    pcVar10 = pcStack_8c;
    for (uVar5 = uVar6 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
      *(undefined4 *)pcVar10 = *(undefined4 *)pcVar9;
      pcVar9 = pcVar9 + 4;
      pcVar10 = pcVar10 + 4;
    }
    for (uVar5 = uVar6 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar10 = *pcVar9;
      pcVar9 = pcVar9 + 1;
      pcVar10 = pcVar10 + 1;
    }
    pcStack_8c[uVar6] = '\0';
    uStack_88 = uVar6;
  }
  iStack_4 = 0;
  puStack_98 = &DAT_00482574;
  FUN_004303ba(appuStack_7c,&puStack_98);
  iStack_4._0_1_ = 1;
  auStack_70[0] = auStack_90[0];
  uStack_6c = 0;
  uStack_68 = 0;
  uStack_64 = 0;
  MsvcStringAssignSubstring(auStack_70,auStack_90,0,DAT_0043b334);
  appuStack_7c[0] = &PTR_FUN_0043b328;
  iStack_4 = (uint)iStack_4._1_3_ << 8;
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(appuStack_7c,&DAT_0043bdd0);
}

