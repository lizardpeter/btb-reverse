/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00430fea; function: entry; body bytes: 235
 * callers: 0; callees: 15; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void entry(void)

{
  DWORD DVar1;
  int iVar2;
  byte *pbVar3;
  uint uVar4;
  HMODULE pHVar5;
  UINT UVar6;
  undefined4 uVar7;
  _STARTUPINFOA local_60;
  undefined1 *local_1c;
  _EXCEPTION_POINTERS *local_18;
  void *pvStack_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &DAT_0043b600;
  puStack_10 = &LAB_00435f38;
  pvStack_14 = ExceptionList;
  local_1c = &stack0xffffff88;
  ExceptionList = &pvStack_14;
  DVar1 = GetVersion();
  _DAT_0051c3e0 = DVar1 >> 8 & 0xff;
  _DAT_0051c3dc = DVar1 & 0xff;
  _DAT_0051c3d8 = _DAT_0051c3dc * 0x100 + _DAT_0051c3e0;
  _DAT_0051c3d4 = DVar1 >> 0x10;
  iVar2 = FUN_0043439f(0);
  if (iVar2 == 0) {
    FUN_00431105((undefined *)0x1c);
  }
  local_8 = 0;
  FUN_00435d83();
  DAT_0051da48 = GetCommandLineA();
  DAT_0051c414 = FUN_00435c51();
  FUN_00435a04();
  FUN_0043594b();
  FUN_00430835();
  local_60.dwFlags = 0;
  GetStartupInfoA(&local_60);
  pbVar3 = FUN_004358f3();
  if ((local_60.dwFlags & 1) == 0) {
    uVar4 = 10;
  }
  else {
    uVar4 = (uint)local_60.wShowWindow;
  }
  uVar7 = 0;
  pHVar5 = GetModuleHandleA((LPCSTR)0x0);
  UVar6 = WinMain(pHVar5,uVar7,pbVar3,uVar4);
  FUN_00430862(UVar6);
  FUN_0043576f(local_18->ExceptionRecord->ExceptionCode,local_18);
  return;
}

