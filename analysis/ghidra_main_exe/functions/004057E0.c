/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004057e0; function: BitmapPrinterStretchDIBToPrinter; body bytes: 1105
 * callers: 0; callees: 11; success: True
 */


void __thiscall BitmapPrinterStretchDIBToPrinter(int param_1,HDC param_2)

{
  char cVar1;
  int SrcWidth;
  BOOL BVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  DWORD *pDVar8;
  char *pcVar9;
  undefined1 uStack_101;
  undefined1 *puStack_100;
  undefined1 auStack_fc [4];
  char *pcStack_f8;
  uint uStack_f4;
  undefined1 *puStack_ec;
  undefined **appuStack_e8 [3];
  undefined1 auStack_dc [16];
  undefined1 auStack_cc [4];
  char *pcStack_c8;
  uint uStack_c4;
  undefined **appuStack_bc [3];
  undefined1 auStack_b0 [16];
  _OSVERSIONINFOA _Stack_a0;
  void *pvStack_c;
  undefined1 *puStack_8;
  int iStack_4;
  
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_0043a4a8;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  BVar2 = IsRectEmpty((RECT *)(param_1 + 0x110));
  if (BVar2 == 0) {
    uVar4 = GetDeviceCaps(param_2,0x26);
    puStack_100 = (undefined1 *)(uVar4 & 0x100);
    if (puStack_100 == (undefined1 *)0x0) {
      iVar6 = *(int *)(param_1 + 4);
      if ((iVar6 == 0) || (*(int *)(param_1 + 8) == 0)) {
        auStack_cc[0] = uStack_101;
        MsvcStringTidy(auStack_cc,'\0');
        uVar4 = 0xffffffff;
        pcVar7 = s_Error_getting_DIB_info__0043e1e8;
        do {
          if (uVar4 == 0) break;
          uVar4 = uVar4 - 1;
          cVar1 = *pcVar7;
          pcVar7 = pcVar7 + 1;
        } while (cVar1 != '\0');
        uVar4 = ~uVar4 - 1;
        uVar3 = MsvcStringGrow(auStack_cc,uVar4,'\x01');
        if ((char)uVar3 != '\0') {
          pcVar7 = s_Error_getting_DIB_info__0043e1e8;
          pcVar9 = pcStack_c8;
          for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
            *(undefined4 *)pcVar9 = *(undefined4 *)pcVar7;
            pcVar7 = pcVar7 + 4;
            pcVar9 = pcVar9 + 4;
          }
          for (uVar5 = uVar4 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
            *pcVar9 = *pcVar7;
            pcVar7 = pcVar7 + 1;
            pcVar9 = pcVar9 + 1;
          }
          pcStack_c8[uVar4] = '\0';
          uStack_c4 = uVar4;
        }
        iStack_4 = 6;
        puStack_100 = &DAT_00482574;
        FUN_004303ba(appuStack_bc,&puStack_100);
        iStack_4._0_1_ = 7;
        auStack_b0[0] = auStack_cc[0];
        MsvcStringTidy(auStack_b0,'\0');
        MsvcStringAssignSubstring(auStack_b0,auStack_cc,0,DAT_0043b334);
        appuStack_bc[0] = &PTR_FUN_0043b328;
        iStack_4 = CONCAT31(iStack_4._1_3_,6);
                    /* WARNING: Subroutine does not return */
        __CxxThrowException_8(appuStack_bc,&DAT_0043bdd0);
      }
      SrcWidth = *(int *)(iVar6 + 4);
      puStack_100 = *(undefined1 **)(iVar6 + 8);
      puStack_ec = (undefined1 *)0x3;
      Escape(param_2,8,4,(LPCSTR)&puStack_ec,(LPVOID)0x0);
      _Stack_a0.dwOSVersionInfoSize = 0x94;
      pDVar8 = &_Stack_a0.dwMajorVersion;
      for (iVar6 = 0x24; iVar6 != 0; iVar6 = iVar6 + -1) {
        *pDVar8 = 0;
        pDVar8 = pDVar8 + 1;
      }
      GetVersionExA(&_Stack_a0);
      SetStretchBltMode(param_2,3);
      iVar6 = StretchDIBits(param_2,*(int *)(param_1 + 0x110),*(int *)(param_1 + 0x114),
                            *(int *)(param_1 + 0x118) - *(int *)(param_1 + 0x110),
                            *(int *)(param_1 + 0x11c) - *(int *)(param_1 + 0x114),0,0,SrcWidth,
                            (int)puStack_100,*(void **)(param_1 + 8),*(BITMAPINFO **)(param_1 + 4),0
                            ,0xcc0020);
      puStack_100 = (undefined1 *)(uint)(iVar6 != -1);
      if (puStack_100 != (undefined1 *)0x0) {
        ExceptionList = pvStack_c;
        return;
      }
      auStack_fc[0] = uStack_101;
      MsvcStringTidy(auStack_fc,'\0');
      uVar4 = 0xffffffff;
      pcVar7 = s_Error_printing_DIB__0043e200;
      do {
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        cVar1 = *pcVar7;
        pcVar7 = pcVar7 + 1;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4 - 1;
      uVar3 = MsvcStringGrow(auStack_fc,uVar4,'\x01');
      if ((char)uVar3 != '\0') {
        pcVar7 = s_Error_printing_DIB__0043e200;
        pcVar9 = pcStack_f8;
        for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
          *(undefined4 *)pcVar9 = *(undefined4 *)pcVar7;
          pcVar7 = pcVar7 + 4;
          pcVar9 = pcVar9 + 4;
        }
        for (uVar5 = uVar4 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
          *pcVar9 = *pcVar7;
          pcVar7 = pcVar7 + 1;
          pcVar9 = pcVar9 + 1;
        }
        pcStack_f8[uVar4] = '\0';
        uStack_f4 = uVar4;
      }
      iStack_4 = 4;
      puStack_100 = &DAT_00482574;
      FUN_004303ba(appuStack_e8,&puStack_100);
      iStack_4._0_1_ = 5;
      auStack_dc[0] = auStack_fc[0];
      MsvcStringTidy(auStack_dc,'\0');
      MsvcStringAssignSubstring(auStack_dc,auStack_fc,0,DAT_0043b334);
      appuStack_e8[0] = &PTR_FUN_0043b328;
      iStack_4 = CONCAT31(iStack_4._1_3_,4);
                    /* WARNING: Subroutine does not return */
      __CxxThrowException_8(appuStack_e8,&DAT_0043bdd0);
    }
    auStack_fc[0] = uStack_101;
    MsvcStringTidy(auStack_fc,'\0');
    uVar4 = 0xffffffff;
    pcVar7 = s_Unsupported_printer__0043e214;
    do {
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar7 + 1;
    } while (cVar1 != '\0');
    uVar4 = ~uVar4 - 1;
    uVar3 = MsvcStringGrow(auStack_fc,uVar4,'\x01');
    if ((char)uVar3 != '\0') {
      pcVar7 = s_Unsupported_printer__0043e214;
      pcVar9 = pcStack_f8;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar7;
        pcVar7 = pcVar7 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar5 = uVar4 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar9 = *pcVar7;
        pcVar7 = pcVar7 + 1;
        pcVar9 = pcVar9 + 1;
      }
      pcStack_f8[uVar4] = '\0';
      uStack_f4 = uVar4;
    }
    iStack_4 = 2;
    puStack_ec = &DAT_00482574;
    FUN_004303ba(appuStack_e8,&puStack_ec);
    iStack_4._0_1_ = 3;
    auStack_dc[0] = auStack_fc[0];
    MsvcStringTidy(auStack_dc,'\0');
    MsvcStringAssignSubstring(auStack_dc,auStack_fc,0,DAT_0043b334);
    appuStack_e8[0] = &PTR_FUN_0043b328;
    iStack_4 = CONCAT31(iStack_4._1_3_,2);
                    /* WARNING: Subroutine does not return */
    __CxxThrowException_8(appuStack_e8,&DAT_0043bdd0);
  }
  auStack_fc[0] = uStack_101;
  MsvcStringTidy(auStack_fc,'\0');
  uVar4 = 0xffffffff;
  pcVar7 = s_Invalid_target_rectangle__0043e22c;
  do {
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    cVar1 = *pcVar7;
    pcVar7 = pcVar7 + 1;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4 - 1;
  uVar3 = MsvcStringGrow(auStack_fc,uVar4,'\x01');
  if ((char)uVar3 != '\0') {
    pcVar7 = s_Invalid_target_rectangle__0043e22c;
    pcVar9 = pcStack_f8;
    for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
      *(undefined4 *)pcVar9 = *(undefined4 *)pcVar7;
      pcVar7 = pcVar7 + 4;
      pcVar9 = pcVar9 + 4;
    }
    for (uVar5 = uVar4 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar9 = *pcVar7;
      pcVar7 = pcVar7 + 1;
      pcVar9 = pcVar9 + 1;
    }
    pcStack_f8[uVar4] = '\0';
    uStack_f4 = uVar4;
  }
  iStack_4 = 0;
  puStack_ec = &DAT_00482574;
  FUN_004303ba(appuStack_e8,&puStack_ec);
  iStack_4._0_1_ = 1;
  auStack_dc[0] = auStack_fc[0];
  MsvcStringTidy(auStack_dc,'\0');
  MsvcStringAssignSubstring(auStack_dc,auStack_fc,0,DAT_0043b334);
  appuStack_e8[0] = &PTR_FUN_0043b328;
  iStack_4 = (uint)iStack_4._1_3_ << 8;
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(appuStack_e8,&DAT_0043bdd0);
}

