/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404f90; function: BitmapPrinterRenderToDC; body bytes: 425
 * callers: 0; callees: 6; success: True
 */


void __thiscall BitmapPrinterRenderToDC(int *param_1,undefined1 *param_2)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  char *pcVar7;
  undefined1 auStack_64 [4];
  char *pcStack_60;
  uint uStack_5c;
  undefined1 auStack_54 [4];
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined **appuStack_44 [3];
  undefined1 auStack_38 [16];
  undefined **appuStack_28 [3];
  undefined1 auStack_1c [8];
  void *pvStack_14;
  void *pvStack_c;
  undefined1 *puStack_8;
  int iStack_4;
  
  iVar2 = (int)param_2;
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_0043a3f0;
  pvStack_c = ExceptionList;
  if (param_1[1] != 0) {
    if (param_2 != (undefined1 *)0x0) {
      ExceptionList = &pvStack_c;
      (**(code **)(*param_1 + 0x24))(param_2);
      (**(code **)(*param_1 + 8))(iVar2);
      ExceptionList = pvStack_14;
      return;
    }
    auStack_64[0] = param_2._0_1_;
    ExceptionList = &pvStack_c;
    MsvcStringTidy(auStack_64,'\0');
    uVar4 = 0xffffffff;
    pcVar6 = s_Invalid_printer_DC__0043e1b0;
    do {
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      cVar1 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    uVar4 = ~uVar4 - 1;
    uVar3 = MsvcStringGrow(auStack_64,uVar4,'\x01');
    if ((char)uVar3 != '\0') {
      pcVar6 = s_Invalid_printer_DC__0043e1b0;
      pcVar7 = pcStack_60;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar7 = *(undefined4 *)pcVar6;
        pcVar6 = pcVar6 + 4;
        pcVar7 = pcVar7 + 4;
      }
      for (uVar5 = uVar4 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar7 = *pcVar6;
        pcVar6 = pcVar6 + 1;
        pcVar7 = pcVar7 + 1;
      }
      pcStack_60[uVar4] = '\0';
      uStack_5c = uVar4;
    }
    iStack_4 = 2;
    param_2 = &DAT_00482574;
    FUN_004303ba(appuStack_28,&param_2);
    iStack_4._0_1_ = 3;
    auStack_1c[0] = auStack_64[0];
    MsvcStringTidy(auStack_1c,'\0');
    MsvcStringAssignSubstring(auStack_1c,auStack_64,0,DAT_0043b334);
    appuStack_28[0] = &PTR_FUN_0043b328;
    iStack_4 = CONCAT31(iStack_4._1_3_,2);
                    /* WARNING: Subroutine does not return */
    __CxxThrowException_8(appuStack_28,&DAT_0043bdd0);
  }
  auStack_54[0] = param_2._0_1_;
  uVar4 = 0xffffffff;
  uStack_50 = 0;
  pcVar6 = s_No_bitmap_defined__0043e1c4;
  do {
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    cVar1 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar1 != '\0');
  uStack_4c = 0;
  uStack_48 = 0;
  ExceptionList = &pvStack_c;
  MsvcStringAssignBuffer(auStack_54,(undefined4 *)s_No_bitmap_defined__0043e1c4,~uVar4 - 1);
  iStack_4 = 0;
  param_2 = &DAT_00482574;
  FUN_004303ba(appuStack_44,&param_2);
  iStack_4._0_1_ = 1;
  auStack_38[0] = auStack_54[0];
  MsvcStringTidy(auStack_38,'\0');
  MsvcStringAssignSubstring(auStack_38,auStack_54,0,DAT_0043b334);
  appuStack_44[0] = &PTR_FUN_0043b328;
  iStack_4 = (uint)iStack_4._1_3_ << 8;
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(appuStack_44,&DAT_0043bdd0);
}

