/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00401fa0; function: CreateOrResetDisplayManager; body bytes: 253
 * callers: 3; callees: 11; success: True
 */


int __cdecl CreateOrResetDisplayManager(HWND param_1,int param_2)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  HINSTANCE hInstance;
  HMENU hMenu;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puVar1 = DAT_0044de08;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0043a36b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  if (DAT_0044de08 != (undefined4 *)0x0) {
    ExceptionList = &local_c;
    DisplayManagerDestructor(DAT_0044de08);
    FUN_0042fbdc((undefined *)puVar1);
    DAT_0044de08 = (undefined4 *)0x0;
  }
  puVar1 = (undefined4 *)operator_new(0x30);
  local_4 = 0;
  if (puVar1 == (undefined4 *)0x0) {
    DAT_0044de08 = (undefined4 *)0x0;
  }
  else {
    DAT_0044de08 = (undefined4 *)DisplayManagerConstructor(puVar1);
  }
  local_4 = 0xffffffff;
  if (param_2 == 0) {
    iVar2 = InitializeFullscreenDisplay(DAT_0044de08);
    if (iVar2 < 0) {
      ExceptionList = local_c;
      return iVar2;
    }
  }
  else {
    iVar2 = InitializeWindowedDisplay(DAT_0044de08);
    if (iVar2 < 0) {
      ExceptionList = local_c;
      return iVar2;
    }
    uVar3 = GetWindowLongA(param_1,-0x10);
    SetWindowLongA(param_1,-0x10,uVar3 | 0x80000);
    hInstance = (HINSTANCE)GetWindowLongA(param_1,-6);
    hMenu = LoadMenuA(hInstance,(LPCSTR)0x66);
    SetMenu(param_1,hMenu);
  }
  ClearBackBuffer(DAT_0044de08,0);
  ExceptionList = local_c;
  return 0;
}

