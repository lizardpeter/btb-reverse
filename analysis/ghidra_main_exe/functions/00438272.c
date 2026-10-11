/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00438272; function: FUN_00438272; body bytes: 137
 * callers: 1; callees: 2; success: True
 */


int __cdecl FUN_00438272(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  HMODULE hModule;
  int iVar1;
  
  iVar1 = 0;
  if (DAT_0051c66c == (FARPROC)0x0) {
    hModule = LoadLibraryA("user32.dll");
    if (hModule != (HMODULE)0x0) {
      DAT_0051c66c = GetProcAddress(hModule,"MessageBoxA");
      if (DAT_0051c66c != (FARPROC)0x0) {
        DAT_0051c670 = GetProcAddress(hModule,"GetActiveWindow");
        DAT_0051c674 = GetProcAddress(hModule,"GetLastActivePopup");
        goto LAB_004382c1;
      }
    }
    iVar1 = 0;
  }
  else {
LAB_004382c1:
    if (DAT_0051c670 != (FARPROC)0x0) {
      iVar1 = (*DAT_0051c670)();
      if ((iVar1 != 0) && (DAT_0051c674 != (FARPROC)0x0)) {
        iVar1 = (*DAT_0051c674)(iVar1);
      }
    }
    iVar1 = (*DAT_0051c66c)(iVar1,param_1,param_2,param_3);
  }
  return iVar1;
}

