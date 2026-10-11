/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403b70; function: SetSurfaceTransparencyColorKey; body bytes: 60
 * callers: 23; callees: 2; success: True
 */


void __cdecl SetSurfaceTransparencyColorKey(int *param_1,COLORREF param_2)

{
  uint local_8;
  uint local_4;
  
  MarkRegisteredSurfaceColorKeyed((int)&param_1);
  local_8 = FUN_00403a70(param_1,param_2);
  local_4 = local_8;
  (**(code **)(*param_1 + 0x74))(param_1,8,&local_8);
  return;
}

