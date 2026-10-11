/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00408460; function: FUN_00408460; body bytes: 67
 * callers: 1; callees: 4; success: True
 */


void __cdecl FUN_00408460(UINT param_1,LPWORD param_2)

{
  HKL dwhkl;
  BOOL BVar1;
  UINT uVirtKey;
  
  dwhkl = GetKeyboardLayout(0);
  BVar1 = GetKeyboardState(&DAT_004fbd54);
  if (BVar1 == 0) {
    return;
  }
  uVirtKey = MapVirtualKeyExA(param_1,1,dwhkl);
  ToAsciiEx(uVirtKey,param_1,&DAT_004fbd54,param_2,0,dwhkl);
  return;
}

