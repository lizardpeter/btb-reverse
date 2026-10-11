/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004373db; function: FUN_004373db; body bytes: 104
 * callers: 1; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

LPSTR __cdecl FUN_004373db(LPSTR param_1,WCHAR param_2)

{
  LPSTR pCVar1;
  
  pCVar1 = param_1;
  if (param_1 == (LPSTR)0x0) {
    return param_1;
  }
  if (DAT_0051c680 == 0) {
    if ((ushort)param_2 < 0x100) {
      *param_1 = (CHAR)param_2;
      return (LPSTR)0x1;
    }
  }
  else {
    param_1 = (LPSTR)0x0;
    pCVar1 = (LPSTR)WideCharToMultiByte(DAT_0051c690,0x220,&param_2,1,pCVar1,DAT_00449b10,
                                        (LPCSTR)0x0,(LPBOOL)&param_1);
    if ((pCVar1 != (LPSTR)0x0) && (param_1 == (LPSTR)0x0)) {
      return pCVar1;
    }
  }
  _DAT_0051c3c8 = 0x2a;
  return (LPSTR)0xffffffff;
}

