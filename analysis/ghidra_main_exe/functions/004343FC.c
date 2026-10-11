/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004343fc; function: FUN_004343fc; body bytes: 72
 * callers: 1; callees: 1; success: True
 */


undefined4 __cdecl FUN_004343fc(undefined4 param_1)

{
  DAT_0051da38 = HeapAlloc(DAT_0051da40,0,0x140);
  if (DAT_0051da38 == (LPVOID)0x0) {
    return 0;
  }
  DAT_0051da30 = 0;
  DAT_0051da34 = 0;
  DAT_0051da2c = DAT_0051da38;
  DAT_0051da3c = param_1;
  DAT_0051da24 = 0x10;
  return 1;
}

