/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043646f; function: FUN_0043646f; body bytes: 200
 * callers: 1; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __cdecl FUN_0043646f(LPWSTR param_1,byte *param_2,uint param_3)

{
  byte bVar1;
  int iVar2;
  
  if ((param_2 != (byte *)0x0) && (param_3 != 0)) {
    bVar1 = *param_2;
    if (bVar1 != 0) {
      if (DAT_0051c680 == 0) {
        if (param_1 != (LPWSTR)0x0) {
          *param_1 = (ushort)bVar1;
        }
        return 1;
      }
      if ((PTR_DAT_00449b1c[(uint)bVar1 * 2 + 1] & 0x80) == 0) {
        iVar2 = MultiByteToWideChar(DAT_0051c690,9,(LPCSTR)param_2,1,param_1,
                                    (uint)(param_1 != (LPWSTR)0x0));
        if (iVar2 != 0) {
          return 1;
        }
      }
      else {
        if (1 < (int)DAT_00449b10) {
          if ((int)param_3 < (int)DAT_00449b10) {
            _DAT_0051c3c8 = 0x2a;
            return 0xffffffff;
          }
          iVar2 = MultiByteToWideChar(DAT_0051c690,9,(LPCSTR)param_2,DAT_00449b10,param_1,
                                      (uint)(param_1 != (LPWSTR)0x0));
          if (iVar2 != 0) {
            return DAT_00449b10;
          }
        }
        if ((DAT_00449b10 <= param_3) && (param_2[1] != 0)) {
          return DAT_00449b10;
        }
      }
      _DAT_0051c3c8 = 0x2a;
      return 0xffffffff;
    }
    if (param_1 != (LPWSTR)0x0) {
      *param_1 = L'\0';
    }
  }
  return 0;
}

