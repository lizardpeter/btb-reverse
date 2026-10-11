/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402700; function: WinMain; body bytes: 1004
 * callers: 1; callees: 40; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 WinMain(HINSTANCE param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined *puVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  DWORD DVar4;
  int iVar5;
  uint uVar6;
  void *pvVar7;
  BOOL BVar8;
  HWND pHStack_94;
  HKEY local_90;
  HACCEL pHStack_8c;
  DWORD local_88;
  DWORD local_84;
  tagMSG tStack_80;
  BYTE local_64 [100];
  
  DAT_0044ddf0 = param_1;
  RegOpenKeyExA((HKEY)0x80000002,s_Software_0043e0f4,0,0x20019,&local_90);
  RegOpenKeyExA(local_90,s_BBC_Multimedia_0043e0e4,0,0x20019,&local_90);
  RegOpenKeyExA(local_90,s_Bob_the_Builder___Bob_Builds_a_P_0043e0c0,0,0x20019,&local_90);
  RegOpenKeyExA(local_90,s_1_0_0_0043e0b8,0,0x20019,&local_90);
  local_88 = 100;
  RegQueryValueExA(local_90,s_Install_Path_0043e0a8,(LPDWORD)0x0,&local_84,local_64,&local_88);
  RegCloseKey(local_90);
  FUN_004300be((LPCSTR)local_64);
  FUN_00406cb0();
  LoadHelpInfo();
  SystemParametersInfoA(0x61,1,(PVOID)0x0,0);
  DAT_0044ddb0 = 0;
  DAT_00519940 = 0;
  uVar3 = FUN_0042ffe2((int *)0x0);
  FUN_0042ffba(uVar3);
  LoadGlobalDataTables();
  DVar4 = GetTickCount();
  FUN_0042ffba(DVar4);
  iVar5 = CreateMainGameWindow(DAT_0044ddf0,param_4,&pHStack_94,&pHStack_8c);
  if ((-1 < iVar5) && (uVar6 = InitializeDisplayForGameWindow(pHStack_94), -1 < (int)uVar6)) {
    _DAT_0044ddcc = DAT_0044ddf0;
    DAT_0044ddd0 = pHStack_94;
    DAT_0044ddd4 = timeGetTime();
    FUN_004092d0(pHStack_94);
    pvVar7 = operator_new(0x7738);
    if (pvVar7 == (void *)0x0) {
      DAT_0044ddd8 = (undefined *)0x0;
    }
    else {
      DAT_0044ddd8 = (undefined *)SoundManagerConstructor((int)pvVar7);
    }
    iVar5 = InitializeDirectInput();
    if (iVar5 < 0) {
      DestroyWindow(pHStack_94);
      return 0;
    }
    InitializeBinkPlaybackSystem();
    if (DAT_0044de0c == 0) {
      DAT_0044ddc8 = SetWindowsHookExA(0xd,FullscreenSystemKeyHook,DAT_0044ddf0,0);
    }
    while( true ) {
      while (BVar8 = PeekMessageA(&tStack_80,(HWND)0x0,0,0,0), BVar8 != 0) {
        BVar8 = GetMessageA(&tStack_80,(HWND)0x0,0,0);
        if (BVar8 == 0) {
          return tStack_80.wParam;
        }
        iVar5 = TranslateAcceleratorA(pHStack_94,pHStack_8c,&tStack_80);
        if (iVar5 == 0) {
          TranslateMessage(&tStack_80);
          DispatchMessageA(&tStack_80);
        }
      }
      if (DAT_004fbd00 == 1) break;
      if (DAT_0044ddb0 == 0) {
LAB_00402a3c:
        if (DAT_0044de10 == 0) goto LAB_00402a26;
        RunActiveGameFrame(pHStack_94);
      }
      else {
        if (DAT_0044ddb0 == 1) {
          LoadCreditsSlideshow();
          DAT_0044ddb0 = DAT_0044ddb0 + 1;
        }
        if (DAT_0044ddb0 == 2) {
          UpdateCreditsSlideshow();
        }
        if (DAT_0044ddb0 < 3) goto LAB_00402a3c;
        RunMainGameFlow(pHStack_94);
        DestroyDisplayManager();
        if (DAT_004fbd00 == 1) {
          MessageBoxA((HWND)0x0,s_CD_Removed_0043e298,s_Error_0043e680,0x10);
        }
        PostQuitMessage(0);
        ReleaseInputAndCursorResources();
        ShutdownDirectSound8();
        puVar1 = DAT_0044ddd8;
        if (DAT_0044ddd8 != (undefined *)0x0) {
          NoOpLegacyHook();
          FUN_0042fbdc(puVar1);
          DAT_0044ddd8 = (undefined *)0x0;
        }
        puVar2 = DAT_0044de08;
        if (DAT_0044de08 != (undefined4 *)0x0) {
          DisplayManagerDestructor(DAT_0044de08);
          FUN_0042fbdc((undefined *)puVar2);
          DAT_0044de08 = (undefined4 *)0x0;
        }
        if (DAT_0044de0c == 0) {
          UnhookWindowsHookEx(DAT_0044ddc8);
        }
        DAT_0044de10 = 0;
LAB_00402a26:
        WaitMessage();
        DAT_0044ddd4 = timeGetTime();
      }
    }
    PostQuitMessage(0);
    ReleaseInputAndCursorResources();
    ShutdownDirectSound8();
    puVar1 = DAT_0044ddd8;
    if (DAT_0044ddd8 != (undefined *)0x0) {
      NoOpLegacyHook();
      FUN_0042fbdc(puVar1);
      DAT_0044ddd8 = (undefined *)0x0;
    }
    puVar2 = DAT_0044de08;
    if (DAT_0044de08 != (undefined4 *)0x0) {
      DisplayManagerDestructor(DAT_0044de08);
      FUN_0042fbdc((undefined *)puVar2);
      DAT_0044de08 = (undefined4 *)0x0;
    }
    if (DAT_0044de0c == 0) {
      UnhookWindowsHookEx(DAT_0044ddc8);
    }
    DAT_0044de10 = 0;
    MessageBoxA((HWND)0x0,s_CD_Removed_0043e298,s_Error_0043e680,0x10);
  }
  return 0;
}

