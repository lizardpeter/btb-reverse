/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00425980; function: BuildSquirrelFrameRect; body bytes: 74
 * callers: 1; callees: 0; success: True
 */


void __cdecl BuildSquirrelFrameRect(int *param_1,int param_2)

{
  int iVar1;
  
  iVar1 = (param_2 / 9 + (param_2 % 9) * 4) * 0x59;
  *param_1 = iVar1;
  param_1[2] = iVar1 + 0x59;
  param_1[1] = 0;
  param_1[3] = 0x85;
  return;
}

