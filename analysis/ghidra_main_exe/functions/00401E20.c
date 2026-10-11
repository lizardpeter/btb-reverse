/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00401e20; function: CreateMainGameWindow; body bytes: 343
 * callers: 1; callees: 9; success: True
 */


undefined4 __cdecl
CreateMainGameWindow(HINSTANCE param_1,int param_2,undefined4 *param_3,undefined4 *param_4)

{
  ATOM AVar1;
  HWND pHVar2;
  HACCEL pHVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  WNDCLASSEXA WStack_30;
  
  pHVar2 = FindWindowA(s_WINNAME_0043e084,s_Bob_the_Builder_0043e08c);
  if (pHVar2 == (HWND)0x0) {
    WStack_30.cbSize = 0x30;
    WStack_30.lpszClassName = s_WINNAME_0043e084;
    WStack_30.lpfnWndProc = MainWindowProc;
    WStack_30.style = 3;
    WStack_30.hInstance = param_1;
    WStack_30.hIcon = LoadIconA(param_1,(LPCSTR)0x65);
    WStack_30.hIconSm = LoadIconA(param_1,(LPCSTR)0x65);
    WStack_30.hCursor = LoadCursorA((HINSTANCE)0x0,(LPCSTR)0x7f00);
    WStack_30.hbrBackground = (HBRUSH)0x6;
    WStack_30.lpszMenuName = (LPCSTR)0x66;
    WStack_30.cbClsExtra = 0;
    WStack_30.cbWndExtra = 0;
    AVar1 = RegisterClassExA(&WStack_30);
    if (AVar1 == 0) {
      return 0x80004005;
    }
    pHVar3 = LoadAcceleratorsA(param_1,(LPCSTR)0x67);
    iVar4 = GetSystemMetrics(0x20);
    iVar5 = GetSystemMetrics(0x21);
    iVar6 = GetSystemMetrics(0xf);
    iVar7 = GetSystemMetrics(4);
    pHVar2 = CreateWindowExA(0,s_WINNAME_0043e084,s_Bob_The_Builder_0043e074,0xce0000,-0x80000000,
                             -0x80000000,iVar4 * 2 + 0x280,iVar6 + iVar7 + 0x1e0 + iVar5 * 2,
                             (HWND)0x0,(HMENU)0x0,param_1,(LPVOID)0x0);
    if (pHVar2 == (HWND)0x0) {
      return 0x80004005;
    }
    ShowWindow(pHVar2,param_2);
    UpdateWindow(pHVar2);
    GetWindowRect(pHVar2,(LPRECT)&DAT_0044ddf8);
    *param_3 = pHVar2;
    *param_4 = pHVar3;
  }
  return 0;
}

