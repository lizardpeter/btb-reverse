/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404dd0; function: BitmapPrinterConstructor; body bytes: 58
 * callers: 1; callees: 0; success: True
 */


undefined4 * __fastcall BitmapPrinterConstructor(undefined4 *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[0x48] = 3;
  *param_1 = &PTR_BitmapPrinterRenderToDC_0043b2f4;
  puVar2 = param_1 + 3;
  for (iVar1 = 0x41; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  param_1[0x44] = 0;
  param_1[0x45] = 0;
  param_1[0x46] = 0;
  param_1[0x47] = 0;
  return param_1;
}

