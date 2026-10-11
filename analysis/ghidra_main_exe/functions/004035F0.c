/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004035f0; function: CopyBitmapToSurface; body bytes: 225
 * callers: 1; callees: 6; success: True
 */


int __cdecl CopyBitmapToSurface(int *param_1,HANDLE param_2)

{
  HDC hdc;
  int iVar1;
  HDC__ *hdc_00;
  undefined1 auStack_98 [16];
  int iStack_88;
  int iStack_84;
  HDC__ HStack_80;
  undefined4 uStack_7c;
  int iStack_8;
  int iStack_4;
  
  if ((param_2 != (HANDLE)0x0) && (param_1 != (int *)0x0)) {
    (**(code **)(*param_1 + 0x6c))(param_1);
    hdc = CreateCompatibleDC((HDC)0x0);
    if (hdc == (HDC)0x0) {
      OutputDebugStringA(s_createcompatible_dc_failed_0043e130);
    }
    SelectObject(hdc,param_2);
    GetObjectA(param_2,0x18,auStack_98);
    hdc_00 = &HStack_80;
    HStack_80.unused = 0x7c;
    uStack_7c = 6;
    (**(code **)(*param_1 + 0x58))(param_1);
    iVar1 = (**(code **)(*param_1 + 0x44))(param_1,&stack0xffffff5c);
    if (iVar1 == 0) {
      BitBlt(hdc_00,0,0,iStack_84,iStack_88,hdc,iStack_8,iStack_4,0xcc0020);
      (**(code **)(*param_1 + 0x68))(param_1,hdc_00);
    }
    DeleteDC(hdc);
    return iVar1;
  }
  return -0x7fffbffb;
}

