/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00405350; function: BitmapPrinterSetTargetRect; body bytes: 47
 * callers: 0; callees: 0; success: True
 */


void __thiscall BitmapPrinterSetTargetRect(int param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  
  *(undefined4 *)(param_1 + 0x110) = *param_2;
  *(undefined4 *)(param_1 + 0x114) = param_2[1];
  *(undefined4 *)(param_1 + 0x118) = param_2[2];
  uVar1 = param_2[3];
  *(undefined4 *)(param_1 + 0x120) = 3;
  *(undefined4 *)(param_1 + 0x11c) = uVar1;
  return;
}

