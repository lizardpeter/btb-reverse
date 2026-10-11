/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00401f80; function: InitializeDisplayForGameWindow; body bytes: 30
 * callers: 1; callees: 1; success: True
 */


uint __cdecl InitializeDisplayForGameWindow(HWND param_1)

{
  uint uVar1;
  
  uVar1 = CreateOrResetDisplayManager(param_1,DAT_0044de0c);
  return uVar1 & (-1 < (int)uVar1) - 1;
}

