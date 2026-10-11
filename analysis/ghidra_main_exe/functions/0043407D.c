/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043407d; function: FUN_0043407d; body bytes: 429
 * callers: 4; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int __cdecl FUN_0043407d(undefined *param_1,char *param_2,uint param_3)

{
  int *piVar1;
  char *pcVar2;
  byte bVar3;
  char cVar4;
  char *pcVar5;
  BOOL BVar6;
  int iVar7;
  char local_418 [1028];
  int local_14;
  DWORD local_10;
  DWORD local_c;
  char *local_8;
  
  if (param_1 < DAT_0051da20) {
    piVar1 = &DAT_0051d920 + ((int)param_1 >> 5);
    iVar7 = ((uint)param_1 & 0x1f) * 8;
    bVar3 = *(byte *)(*piVar1 + 4 + iVar7);
    if ((bVar3 & 1) != 0) {
      local_c = 0;
      local_14 = 0;
      if (param_3 == 0) {
        return 0;
      }
      if ((bVar3 & 0x20) != 0) {
        FUN_004372d7((uint)param_1,0,2);
      }
      if ((*(byte *)((undefined4 *)(*piVar1 + iVar7) + 1) & 0x80) == 0) {
        BVar6 = WriteFile(*(HANDLE *)(*piVar1 + iVar7),param_2,param_3,&local_10,(LPOVERLAPPED)0x0);
        if (BVar6 == 0) {
          param_1 = (undefined *)GetLastError();
        }
        else {
          local_c = local_10;
          param_1 = (undefined *)0x0;
        }
LAB_00434176:
        if (local_c != 0) {
          return local_c - local_14;
        }
        if (param_1 != (undefined *)0x0) {
          if (param_1 == (undefined *)0x5) {
            _DAT_0051c3c8 = 9;
            DAT_0051c3cc = 5;
            return -1;
          }
          FUN_0043277a(param_1);
          return -1;
        }
      }
      else {
        local_8 = param_2;
        param_1 = (undefined *)0x0;
        if (param_3 != 0) {
          do {
            pcVar5 = local_418;
            do {
              if (param_3 <= (uint)((int)local_8 - (int)param_2)) break;
              pcVar2 = local_8 + 1;
              cVar4 = *local_8;
              local_8 = pcVar2;
              if (cVar4 == '\n') {
                local_14 = local_14 + 1;
                *pcVar5 = '\r';
                pcVar5 = pcVar5 + 1;
              }
              *pcVar5 = cVar4;
              pcVar5 = pcVar5 + 1;
            } while ((int)pcVar5 - (int)local_418 < 0x400);
            BVar6 = WriteFile(*(HANDLE *)(*piVar1 + iVar7),local_418,(int)pcVar5 - (int)local_418,
                              &local_10,(LPOVERLAPPED)0x0);
            if (BVar6 == 0) {
              param_1 = (undefined *)GetLastError();
              goto LAB_00434176;
            }
            local_c = local_c + local_10;
            if (((int)local_10 < (int)pcVar5 - (int)local_418) ||
               (param_3 <= (uint)((int)local_8 - (int)param_2))) goto LAB_00434176;
          } while( true );
        }
      }
      if (((*(byte *)(*piVar1 + 4 + iVar7) & 0x40) != 0) && (*param_2 == '\x1a')) {
        return 0;
      }
      _DAT_0051c3c8 = 0x1c;
      DAT_0051c3cc = 0;
      return -1;
    }
  }
  DAT_0051c3cc = 0;
  _DAT_0051c3c8 = 9;
  return -1;
}

