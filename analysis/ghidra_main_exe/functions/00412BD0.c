/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00412bd0; function: BeginFireworksEditorAction; body bytes: 372
 * callers: 1; callees: 9; success: True
 */


void __cdecl BeginFireworksEditorAction(int param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  int local_4;
  
  iVar1 = param_1;
  if ((param_1 < 0) || (0xb < param_1)) {
    if (param_1 == 0xc) {
      DecodeSelectedFireworkGridCell(&param_1,&local_4);
      if ((-1 < (int)(&DAT_0050a678)[local_4 + param_1 * 6]) &&
         ((int)(&DAT_0050a678)[local_4 + param_1 * 6] < 0xc)) {
        RemoveFireworkGridItem(param_1,local_4);
        return;
      }
    }
    else {
      if (param_1 == 0x19) {
        DAT_0051c2d0 = 1;
        DAT_0051c310 = 3;
        DAT_0051c294 = LoadBitmapToDirectDrawSurface
                                 (*(int **)(DAT_0044de08 + 4),s_data_ui_DeleteFireworks_bmp_004435d8
                                  ,0,0);
        RegisterBitmapSurface(&DAT_0051c294,s_data_ui_DeleteFireworks_bmp_004435d8);
        MarkRegisteredSurfaceColorKeyed(0x51c294);
        SetSurfaceTransparencyColorKey(DAT_0051c294,0xff00ff);
        return;
      }
      if (param_1 == 0x1b) {
        DAT_0050a5bc = 0xe;
      }
    }
  }
  else {
    SetCursorSurface((int *)(&DAT_0050a4b8)[param_1]);
    if (iVar1 < 8) {
      if (DAT_0050ab20 == 0) {
        iVar4 = 2;
        uVar3 = 0x32;
        uVar2 = FUN_0042ffc4();
        PlayManagedSoundById(DAT_0044ddd8,(int)uVar2 % 5 + 0x35e,uVar3,iVar4);
        DAT_005093d8 = iVar1;
        DAT_0050ab20 = 1;
        return;
      }
    }
    else if (DAT_0050ab24 == 0) {
      iVar4 = 2;
      uVar3 = 0x32;
      uVar2 = FUN_0042ffc4();
      PlayManagedSoundById(DAT_0044ddd8,(int)uVar2 % 5 + 899,uVar3,iVar4);
      DAT_005093d8 = iVar1;
      DAT_0050ab24 = 1;
      return;
    }
  }
  return;
}

