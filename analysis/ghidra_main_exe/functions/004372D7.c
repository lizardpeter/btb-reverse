/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004372d7; function: FUN_004372d7; body bytes: 154
 * callers: 5; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

DWORD __cdecl FUN_004372d7(uint param_1,LONG param_2,DWORD param_3)

{
  byte *pbVar1;
  HANDLE hFile;
  DWORD DVar2;
  undefined *puVar3;
  int iVar4;
  
  if (param_1 < DAT_0051da20) {
    iVar4 = (param_1 & 0x1f) * 8;
    if ((*(byte *)((&DAT_0051d920)[(int)param_1 >> 5] + 4 + iVar4) & 1) != 0) {
      hFile = (HANDLE)FUN_00436322(param_1);
      if (hFile == (HANDLE)0xffffffff) {
        _DAT_0051c3c8 = 9;
        return 0xffffffff;
      }
      DVar2 = SetFilePointer(hFile,param_2,(PLONG)0x0,param_3);
      if (DVar2 == 0xffffffff) {
        puVar3 = (undefined *)GetLastError();
      }
      else {
        puVar3 = (undefined *)0x0;
      }
      if (puVar3 != (undefined *)0x0) {
        FUN_0043277a(puVar3);
        return 0xffffffff;
      }
      pbVar1 = (byte *)((&DAT_0051d920)[(int)param_1 >> 5] + 4 + iVar4);
      *pbVar1 = *pbVar1 & 0xfd;
      return DVar2;
    }
  }
  DAT_0051c3cc = 0;
  _DAT_0051c3c8 = 9;
  return 0xffffffff;
}

