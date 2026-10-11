/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403590; function: UpdateDisplayDestinationRect; body bytes: 88
 * callers: 3; callees: 4; success: True
 */


undefined4 __fastcall UpdateDisplayDestinationRect(int param_1)

{
  int yBottom;
  int xRight;
  
  if (*(int *)(param_1 + 0x28) != 0) {
    GetClientRect(*(HWND *)(param_1 + 0x14),(LPRECT)(param_1 + 0x18));
    ClientToScreen(*(HWND *)(param_1 + 0x14),(LPPOINT)(param_1 + 0x18));
    ClientToScreen(*(HWND *)(param_1 + 0x14),(LPPOINT)(param_1 + 0x20));
    return 0;
  }
  yBottom = GetSystemMetrics(1);
  xRight = GetSystemMetrics(0);
  SetRect((LPRECT)(param_1 + 0x18),0,0,xRight,yBottom);
  return 0;
}

