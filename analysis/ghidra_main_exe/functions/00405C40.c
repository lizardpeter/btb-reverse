/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00405c40; function: BitmapPrinterImportDDB; body bytes: 664
 * callers: 0; callees: 14; success: True
 */


void __thiscall BitmapPrinterImportDDB(int param_1,HBITMAP param_2,HPALETTE param_3)

{
  char cVar1;
  int iVar2;
  undefined4 *puVar3;
  HDC hdc;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  char *pcVar7;
  undefined4 *puVar8;
  char *pcVar9;
  undefined1 uStack_81;
  undefined1 *puStack_80;
  undefined1 auStack_7c [4];
  char *pcStack_78;
  uint uStack_74;
  undefined1 auStack_6c [4];
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined1 auStack_5c [4];
  int iStack_58;
  UINT UStack_54;
  undefined **appuStack_44 [3];
  undefined1 auStack_38 [16];
  undefined **appuStack_28 [3];
  undefined1 auStack_1c [16];
  void *pvStack_c;
  undefined1 *puStack_8;
  int iStack_4;
  
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_0043a4e0;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  iVar2 = GetObjectA(param_2,0x18,auStack_5c);
  if (iVar2 != 0x18) {
    auStack_7c[0] = uStack_81;
    MsvcStringTidy(auStack_7c,'\0');
    uVar5 = 0xffffffff;
    pcVar7 = s_Error_getting_DDB_info__0043e248;
    do {
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar7 + 1;
    } while (cVar1 != '\0');
    uVar5 = ~uVar5 - 1;
    uVar4 = MsvcStringGrow(auStack_7c,uVar5,'\x01');
    if ((char)uVar4 != '\0') {
      pcVar7 = s_Error_getting_DDB_info__0043e248;
      pcVar9 = pcStack_78;
      for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar7;
        pcVar7 = pcVar7 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar6 = uVar5 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
        *pcVar9 = *pcVar7;
        pcVar7 = pcVar7 + 1;
        pcVar9 = pcVar9 + 1;
      }
      pcStack_78[uVar5] = '\0';
      uStack_74 = uVar5;
    }
    iStack_4 = 2;
    puStack_80 = &DAT_00482574;
    FUN_004303ba(appuStack_28,&puStack_80);
    iStack_4._0_1_ = 3;
    auStack_1c[0] = auStack_7c[0];
    MsvcStringTidy(auStack_1c,'\0');
    MsvcStringAssignSubstring(auStack_1c,auStack_7c,0,DAT_0043b334);
    appuStack_28[0] = &PTR_FUN_0043b328;
    iStack_4 = CONCAT31(iStack_4._1_3_,2);
                    /* WARNING: Subroutine does not return */
    __CxxThrowException_8(appuStack_28,&DAT_0043bdd0);
  }
  uVar5 = UStack_54 * (iStack_58 * 0x18 + 0x1f >> 3 & 0xfffffffcU) + 0x28;
  FUN_0042fbdc(*(undefined **)(param_1 + 4));
  puVar3 = (undefined4 *)operator_new(uVar5);
  *(undefined4 **)(param_1 + 4) = puVar3;
  *(undefined4 **)(param_1 + 8) = puVar3 + 10;
  puVar8 = puVar3;
  for (uVar5 = uVar5 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *puVar8 = 0;
    puVar8 = puVar8 + 1;
  }
  for (iVar2 = 0; iVar2 != 0; iVar2 = iVar2 + -1) {
    *(undefined1 *)puVar8 = 0;
    puVar8 = (undefined4 *)((int)puVar8 + 1);
  }
  *puVar3 = 0x28;
  puVar3[1] = iStack_58;
  puVar3[2] = UStack_54;
  *(undefined2 *)((int)puVar3 + 0xe) = 0x18;
  *(undefined2 *)(puVar3 + 3) = 1;
  puVar3[4] = 0;
  hdc = GetDC((HWND)0x0);
  if (param_3 != (HPALETTE)0x0) {
    param_3 = SelectPalette(hdc,param_3,1);
    RealizePalette(hdc);
  }
  iVar2 = GetDIBits(hdc,param_2,0,UStack_54,*(LPVOID *)(param_1 + 8),*(LPBITMAPINFO *)(param_1 + 4),
                    0);
  if (param_3 != (HPALETTE)0x0) {
    SelectPalette(hdc,param_3,1);
  }
  ReleaseDC((HWND)0x0,hdc);
  if (iVar2 != 0) {
    ExceptionList = pvStack_c;
    return;
  }
  uVar5 = 0xffffffff;
  pcVar7 = s_Error_getting_DDB_bits__0043e260;
  do {
    if (uVar5 == 0) break;
    uVar5 = uVar5 - 1;
    cVar1 = *pcVar7;
    pcVar7 = pcVar7 + 1;
  } while (cVar1 != '\0');
  auStack_6c[0] = uStack_81;
  uStack_68 = 0;
  uStack_64 = 0;
  uStack_60 = 0;
  MsvcStringAssignBuffer(auStack_6c,(undefined4 *)s_Error_getting_DDB_bits__0043e260,~uVar5 - 1);
  iStack_4 = 0;
  puStack_80 = &DAT_00482574;
  FUN_004303ba(appuStack_44,&puStack_80);
  auStack_38[0] = auStack_6c[0];
  iStack_4._0_1_ = 1;
  MsvcStringTidy(auStack_38,'\0');
  MsvcStringAssignSubstring(auStack_38,auStack_6c,0,DAT_0043b334);
  appuStack_44[0] = &PTR_FUN_0043b328;
  iStack_4 = (uint)iStack_4._1_3_ << 8;
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(appuStack_44,&DAT_0043bdd0);
}

