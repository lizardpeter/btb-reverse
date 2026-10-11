/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409730; function: ExportAndPrintGameImage; body bytes: 607
 * callers: 3; callees: 14; success: True
 */


undefined4 __cdecl ExportAndPrintGameImage(HGLOBAL param_1)

{
  UINT UVar1;
  LOGPALETTE *plpal;
  BYTE *pBVar2;
  BOOL BVar3;
  uint uVar4;
  int iVar5;
  BYTE *pBVar6;
  int *piVar7;
  LOGPALETTE *pLVar8;
  HGLOBAL *ppvVar9;
  tagPDA local_578;
  int aiStack_534 [74];
  RGBQUAD aRStack_40c [255];
  void *pvStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_0043a56b;
  pvStack_c = ExceptionList;
  DAT_00482570 = param_1;
  piVar7 = *(int **)(DAT_0044de08 + 4);
  ExceptionList = &pvStack_c;
  local_578.lStructSize = (DWORD)piVar7;
  ExportBackBufferToPrintBitmap();
  DAT_004fc29c = LoadImageA(DAT_0044ddf0,s_PrintMe_bmp_0043ee74,0,0,0,0x2010);
  GetObjectA(DAT_004fc29c,0x18,&DAT_004fc280);
  DAT_004fc298 = CreateCompatibleDC((HDC)0x0);
  if (DAT_004fc298 != (HDC)0x0) {
    DAT_004fc2a0 = SelectObject(DAT_004fc298,DAT_004fc29c);
    UVar1 = GetDIBColorTable(DAT_004fc298,0,0x100,aRStack_40c);
    if (UVar1 != 0) {
      uVar4 = UVar1 * 4 + 4;
      plpal = (LOGPALETTE *)operator_new(uVar4);
      pLVar8 = plpal;
      for (uVar4 = uVar4 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
        pLVar8->palVersion = 0;
        pLVar8->palNumEntries = 0;
        pLVar8 = (LOGPALETTE *)pLVar8->palPalEntry;
      }
      for (iVar5 = 0; iVar5 != 0; iVar5 = iVar5 + -1) {
        *(undefined1 *)&pLVar8->palVersion = 0;
        pLVar8 = (LOGPALETTE *)((int)&pLVar8->palVersion + 1);
      }
      plpal->palVersion = 0x300;
      plpal->palNumEntries = 0x100;
      if (UVar1 != 0) {
        pBVar6 = &plpal->palPalEntry[0].peGreen;
        pBVar2 = &aRStack_40c[0].rgbGreen;
        do {
          ((PALETTEENTRY *)(pBVar6 + -1))->peRed = pBVar2[1];
          *pBVar6 = *pBVar2;
          pBVar6[1] = ((RGBQUAD *)(pBVar2 + -1))->rgbBlue;
          pBVar6 = pBVar6 + 4;
          UVar1 = UVar1 - 1;
          pBVar2 = pBVar2 + 4;
        } while (UVar1 != 0);
      }
      DAT_004fc2a4 = CreatePalette(plpal);
      FUN_0042fbdc((undefined *)plpal);
      piVar7 = (int *)local_578.lStructSize;
    }
  }
  local_578.hwndOwner = (HWND)0x42;
  ppvVar9 = &local_578.hDevMode;
  for (iVar5 = 0xf; iVar5 != 0; iVar5 = iVar5 + -1) {
    *ppvVar9 = (HGLOBAL)0x0;
    ppvVar9 = ppvVar9 + 1;
  }
  *(undefined2 *)ppvVar9 = 0;
  local_578.hDevMode = param_1;
  local_578.hInstance._2_2_ = 1;
  local_578.nMinPage = 0;
  local_578.nMaxPage = 1;
  local_578.nCopies = 1;
  local_578.hInstance._0_2_ = 0;
  local_578.nFromPage = 0x10c;
  local_578.nToPage = 0x10;
  local_578.hDevNames = (HGLOBAL)0x0;
  local_578.hDC = (HDC)0x0;
  (**(code **)(*piVar7 + 0x28))(piVar7);
  BVar3 = PrintDlgA(&local_578);
  if (BVar3 != 0) {
    BitmapPrinterConstructor(aiStack_534);
    puStack_8 = (undefined1 *)0x0;
    DAT_004fc29c = SelectObject(DAT_004fc298,DAT_004fc2a0);
    (**(code **)(aiStack_534[0] + 0xc))(DAT_004fc29c,DAT_004fc2a4);
    DAT_004fc2a0 = SelectObject(DAT_004fc298,DAT_004fc29c);
    BitmapPrinterSetScaleMode((void *)((int)&local_578.hPrintTemplate + 2),(undefined1 *)0x2);
    (**(code **)(local_578._60_4_ + 0x14))(s_Bob_the_Builder___Bob_Builds_a_P_0043e0c0);
    (**(code **)(local_578._56_4_ + 4))(local_578.hwndOwner);
    if (local_578.hDevMode != (HGLOBAL)0x0) {
      GlobalFree(local_578.hDevMode);
    }
    if (local_578.hDevNames != (HGLOBAL)0x0) {
      GlobalFree(local_578.hDevNames);
    }
    puStack_8 = (undefined1 *)0xffffffff;
    BitmapPrinterDestructor(aiStack_534);
  }
  ExceptionList = pvStack_10;
  return 1;
}

