/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00405490; function: BitmapPrinterComputeTargetRect; body bytes: 554
 * callers: 0; callees: 3; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall BitmapPrinterComputeTargetRect(int param_1,HDC param_2)

{
  int iVar1;
  int iVar2;
  HDC hdc;
  int iVar3;
  longlong lVar4;
  longlong lVar5;
  
  iVar1 = GetDeviceCaps(param_2,8);
  iVar2 = GetDeviceCaps(param_2,10);
  GetDeviceCaps(param_2,0x58);
  GetDeviceCaps(param_2,0x5a);
  iVar3 = *(int *)(param_1 + 0x120);
  if (iVar3 == 0) {
    *(undefined4 *)(param_1 + 0x114) = 0;
    *(undefined4 *)(param_1 + 0x110) = 0;
    *(int *)(param_1 + 0x118) = iVar1;
    lVar4 = __ftol();
    *(int *)(param_1 + 0x11c) = (int)lVar4;
  }
  else {
    if (iVar3 == 1) {
      *(undefined4 *)(param_1 + 0x114) = 0;
      *(undefined4 *)(param_1 + 0x110) = 0;
      *(int *)(param_1 + 0x11c) = iVar2;
      lVar4 = __ftol();
      *(int *)(param_1 + 0x118) = (int)lVar4;
      return;
    }
    if (iVar3 == 2) {
      hdc = GetDC(DAT_00482570);
      GetDeviceCaps(hdc,0x58);
      GetDeviceCaps(hdc,0x5a);
      GetDeviceCaps(param_2,0x58);
      GetDeviceCaps(param_2,0x5a);
      iVar3 = GetDeviceCaps(param_2,8);
      iVar1 = GetDeviceCaps(param_2,10);
      lVar4 = __ftol();
      lVar5 = __ftol();
      *(int *)(param_1 + 0x114) = iVar1 / 2 + (int)lVar4 / 2;
      *(int *)(param_1 + 0x110) = iVar3 / 2 + (int)lVar5 / 2;
      lVar4 = __ftol();
      *(int *)(param_1 + 0x118) = (int)lVar4;
      lVar4 = __ftol();
      *(int *)(param_1 + 0x11c) = (int)lVar4;
      return;
    }
  }
  return;
}

