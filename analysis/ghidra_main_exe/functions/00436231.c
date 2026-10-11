/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00436231; function: FUN_00436231; body bytes: 119
 * callers: 1; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_00436231(uint param_1,HANDLE param_2)

{
  int iVar1;
  DWORD nStdHandle;
  
  if (param_1 < DAT_0051da20) {
    iVar1 = (param_1 & 0x1f) * 8;
    if (*(int *)((&DAT_0051d920)[(int)param_1 >> 5] + iVar1) == -1) {
      if (DAT_00447594 == 1) {
        if (param_1 == 0) {
          nStdHandle = 0xfffffff6;
        }
        else if (param_1 == 1) {
          nStdHandle = 0xfffffff5;
        }
        else {
          if (param_1 != 2) goto LAB_00436287;
          nStdHandle = 0xfffffff4;
        }
        SetStdHandle(nStdHandle,param_2);
      }
LAB_00436287:
      *(HANDLE *)((&DAT_0051d920)[(int)param_1 >> 5] + iVar1) = param_2;
      return 0;
    }
  }
  DAT_0051c3cc = 0;
  _DAT_0051c3c8 = 9;
  return 0xffffffff;
}

