/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004390cc; function: FUN_004390cc; body bytes: 326
 * callers: 1; callees: 8; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int __cdecl FUN_004390cc(undefined *param_1,int param_2)

{
  DWORD DVar1;
  DWORD DVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  HANDLE hFile;
  BOOL BVar6;
  int iVar7;
  uint uVar8;
  char local_1004 [4060];
  undefined4 uStackY_28;
  
  FUN_00430f00();
  iVar7 = 0;
  if ((param_1 < DAT_0051da20) &&
     ((*(byte *)((&DAT_0051d920)[(int)param_1 >> 5] + 4 + ((uint)param_1 & 0x1f) * 8) & 1) != 0)) {
    DVar1 = FUN_004372d7((uint)param_1,0,1);
    if ((DVar1 != 0xffffffff) && (DVar2 = FUN_004372d7((uint)param_1,0,2), DVar2 != 0xffffffff)) {
      uVar8 = param_2 - DVar2;
      if ((int)uVar8 < 1) {
        if ((int)uVar8 < 0) {
          FUN_004372d7((uint)param_1,param_2,0);
          hFile = (HANDLE)FUN_00436322((uint)param_1);
          BVar6 = SetEndOfFile(hFile);
          iVar7 = (BVar6 != 0) - 1;
          if (iVar7 == -1) {
            _DAT_0051c3c8 = 0xd;
            DAT_0051c3cc = GetLastError();
          }
        }
      }
      else {
        _memset(local_1004,0,0x1000);
        uStackY_28 = 0x43915c;
        iVar3 = FUN_00439627((uint)param_1,0x8000);
        do {
          uVar4 = 0x1000;
          if ((int)uVar8 < 0x1000) {
            uVar4 = uVar8;
          }
          iVar5 = FUN_0043407d(param_1,local_1004,uVar4);
          if (iVar5 == -1) {
            if (DAT_0051c3cc == 5) {
              _DAT_0051c3c8 = 0xd;
            }
            iVar7 = -1;
            break;
          }
          uVar8 = uVar8 - iVar5;
        } while (0 < (int)uVar8);
        FUN_00439627((uint)param_1,iVar3);
      }
      FUN_004372d7((uint)param_1,DVar1,0);
      return iVar7;
    }
  }
  else {
    _DAT_0051c3c8 = 9;
  }
  return -1;
}

