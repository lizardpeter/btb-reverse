/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00405140; function: BitmapPrinterPrintDocument; body bytes: 457
 * callers: 0; callees: 10; success: True
 */


void __thiscall BitmapPrinterPrintDocument(void *this,HDC param_1)

{
  char cVar1;
  HDC hdc;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  char *pcVar7;
  undefined **local_80 [3];
  undefined1 local_74 [16];
  undefined **local_64 [3];
  undefined1 local_58 [16];
  DOCINFOA local_48;
  undefined1 local_34 [4];
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined1 local_24 [4];
  char *local_20;
  uint local_1c;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  hdc = param_1;
  local_8 = 0xffffffff;
  puStack_c = &LAB_0043a420;
  local_10 = ExceptionList;
  local_14 = &stack0xffffff74;
  if (*(int *)((int)this + 4) != 0) {
    if (param_1 != (HDC)0x0) {
      local_48.lpszOutput = (LPCSTR)0x0;
      local_48.lpszDocName = (LPCSTR)((int)this + 0xc);
      local_48.lpszDatatype = (LPCSTR)0x0;
      local_48.cbSize = 0x14;
      local_48.fwType = 0;
      ExceptionList = &local_10;
      iVar3 = StartDocA(param_1,&local_48);
      if (0 < iVar3) {
        iVar3 = StartPage(hdc);
        if (0 < iVar3) {
                    /* WARNING: Load size is inaccurate */
          local_8 = 4;
          (*(code *)**this)(hdc);
          EndPage(hdc);
        }
        EndDoc(hdc);
      }
      ExceptionList = local_10;
      return;
    }
    local_24[0] = param_1._3_1_;
    ExceptionList = &local_10;
    local_14 = &stack0xffffff74;
    MsvcStringTidy(local_24,'\0');
    uVar4 = 0xffffffff;
    pcVar6 = s_Invalid_printer_DC__0043e1b0;
    do {
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      cVar1 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    uVar4 = ~uVar4 - 1;
    uVar2 = MsvcStringGrow(local_24,uVar4,'\x01');
    if ((char)uVar2 != '\0') {
      pcVar6 = s_Invalid_printer_DC__0043e1b0;
      pcVar7 = local_20;
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
      local_20[uVar4] = '\0';
      local_1c = uVar4;
    }
    local_8 = 2;
    param_1 = (HDC)&DAT_00482574;
    FUN_004303ba(local_80,&param_1);
    local_8._0_1_ = 3;
    local_74[0] = local_24[0];
    MsvcStringTidy(local_74,'\0');
    MsvcStringAssignSubstring(local_74,local_24,0,DAT_0043b334);
    local_80[0] = &PTR_FUN_0043b328;
    local_8 = CONCAT31(local_8._1_3_,2);
                    /* WARNING: Subroutine does not return */
    __CxxThrowException_8(local_80,&DAT_0043bdd0);
  }
  local_34[0] = param_1._3_1_;
  uVar4 = 0xffffffff;
  local_30 = 0;
  pcVar6 = s_No_bitmap_defined__0043e1c4;
  do {
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    cVar1 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar1 != '\0');
  local_2c = 0;
  local_28 = 0;
  ExceptionList = &local_10;
  local_14 = &stack0xffffff74;
  MsvcStringAssignBuffer(local_34,(undefined4 *)s_No_bitmap_defined__0043e1c4,~uVar4 - 1);
  local_8 = 0;
  param_1 = (HDC)&DAT_00482574;
  FUN_004303ba(local_64,&param_1);
  local_8._0_1_ = 1;
  local_58[0] = local_34[0];
  MsvcStringTidy(local_58,'\0');
  MsvcStringAssignSubstring(local_58,local_34,0,DAT_0043b334);
  local_64[0] = &PTR_FUN_0043b328;
  local_8 = (uint)local_8._1_3_ << 8;
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(local_64,&DAT_0043bdd0);
}

