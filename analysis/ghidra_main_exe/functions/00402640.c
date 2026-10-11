/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402640; function: FullscreenSystemKeyHook; body bytes: 153
 * callers: 0; callees: 2; success: True
 */


LRESULT FullscreenSystemKeyHook(int param_1,WPARAM param_2,int *param_3)

{
  int iVar1;
  ushort uVar2;
  LRESULT LVar3;
  bool bVar4;
  
  if (param_1 == 0) {
    switch(param_2) {
    case 0x100:
    case 0x101:
    case 0x104:
    case 0x105:
      iVar1 = *param_3;
      if ((((iVar1 != 0xd) || ((*(byte *)(param_3 + 2) & 0x20) == 0)) &&
          ((iVar1 != 0x73 || ((*(byte *)(param_3 + 2) & 0x20) == 0)))) &&
         ((iVar1 != 9 || ((*(byte *)(param_3 + 2) & 0x20) == 0)))) {
        bVar4 = iVar1 == 0x1b;
        if (bVar4) {
          if ((*(byte *)(param_3 + 2) & 0x20) != 0) {
            return 1;
          }
          bVar4 = true;
        }
        if (((!bVar4) || (uVar2 = GetKeyState(0x11), (uVar2 & 0x8000) == 0)) &&
           ((*param_3 != 0x2e ||
            (((*(byte *)(param_3 + 2) & 0x20) == 0 ||
             (uVar2 = GetKeyState(0x11), (uVar2 & 0x8000) == 0)))))) break;
      }
      return 1;
    }
  }
  LVar3 = CallNextHookEx((HHOOK)0x0,param_1,param_2,(LPARAM)param_3);
  return LVar3;
}

