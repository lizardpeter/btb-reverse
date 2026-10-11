/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00433961; function: FUN_00433961; body bytes: 502
 * callers: 3; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int __cdecl FUN_00433961(uint param_1,char *param_2,char *param_3)

{
  int *piVar1;
  byte *pbVar2;
  char cVar3;
  byte bVar4;
  BOOL BVar5;
  undefined *puVar6;
  DWORD DVar7;
  int iVar8;
  char *pcVar9;
  DWORD local_10;
  char *local_c;
  char local_5;
  
  if (param_1 < DAT_0051da20) {
    iVar8 = (param_1 & 0x1f) * 8;
    piVar1 = &DAT_0051d920 + ((int)param_1 >> 5);
    bVar4 = *(byte *)((&DAT_0051d920)[(int)param_1 >> 5] + iVar8 + 4);
    if ((bVar4 & 1) != 0) {
      local_c = (char *)0x0;
      if ((param_3 == (char *)0x0) || ((bVar4 & 2) != 0)) {
        return 0;
      }
      pcVar9 = param_2;
      if (((bVar4 & 0x48) != 0) &&
         (cVar3 = *(char *)((&DAT_0051d920)[(int)param_1 >> 5] + iVar8 + 5), cVar3 != '\n')) {
        param_3 = param_3 + -1;
        *param_2 = cVar3;
        pcVar9 = param_2 + 1;
        local_c = (char *)0x1;
        *(undefined1 *)(*piVar1 + 5 + iVar8) = 10;
      }
      BVar5 = ReadFile(*(HANDLE *)(*piVar1 + iVar8),pcVar9,(DWORD)param_3,&local_10,
                       (LPOVERLAPPED)0x0);
      if (BVar5 == 0) {
        puVar6 = (undefined *)GetLastError();
        if (puVar6 == (undefined *)0x5) {
          _DAT_0051c3c8 = 9;
          DAT_0051c3cc = 5;
          return -1;
        }
        if (puVar6 != (undefined *)0x6d) {
          FUN_0043277a(puVar6);
          return -1;
        }
        return 0;
      }
      bVar4 = *(byte *)(*piVar1 + 4 + iVar8);
      if ((bVar4 & 0x80) == 0) {
        return (int)local_c + local_10;
      }
      if ((local_10 == 0) || (*param_2 != '\n')) {
        bVar4 = bVar4 & 0xfb;
      }
      else {
        bVar4 = bVar4 | 4;
      }
      *(byte *)(*piVar1 + 4 + iVar8) = bVar4;
      param_3 = param_2;
      local_c = param_2 + (int)local_c + local_10;
      pcVar9 = param_2;
      if (param_2 < local_c) {
        do {
          cVar3 = *param_3;
          if (cVar3 == '\x1a') {
            pbVar2 = (byte *)(*piVar1 + 4 + iVar8);
            bVar4 = *pbVar2;
            if ((bVar4 & 0x40) == 0) {
              *pbVar2 = bVar4 | 2;
            }
            break;
          }
          if (cVar3 == '\r') {
            if (param_3 < local_c + -1) {
              if (param_3[1] == '\n') {
                param_3 = param_3 + 2;
                goto LAB_00433af7;
              }
              *pcVar9 = '\r';
              pcVar9 = pcVar9 + 1;
              param_3 = param_3 + 1;
            }
            else {
              param_3 = param_3 + 1;
              BVar5 = ReadFile(*(HANDLE *)(*piVar1 + iVar8),&local_5,1,&local_10,(LPOVERLAPPED)0x0);
              if (((BVar5 == 0) && (DVar7 = GetLastError(), DVar7 != 0)) || (local_10 == 0)) {
LAB_00433b11:
                *pcVar9 = '\r';
LAB_00433b14:
                pcVar9 = pcVar9 + 1;
              }
              else if ((*(byte *)(*piVar1 + 4 + iVar8) & 0x48) == 0) {
                if ((pcVar9 == param_2) && (local_5 == '\n')) {
LAB_00433af7:
                  *pcVar9 = '\n';
                  goto LAB_00433b14;
                }
                FUN_004372d7(param_1,-1,1);
                if (local_5 != '\n') goto LAB_00433b11;
              }
              else {
                if (local_5 == '\n') goto LAB_00433af7;
                *pcVar9 = '\r';
                pcVar9 = pcVar9 + 1;
                *(char *)(*piVar1 + 5 + iVar8) = local_5;
              }
            }
          }
          else {
            *pcVar9 = cVar3;
            pcVar9 = pcVar9 + 1;
            param_3 = param_3 + 1;
          }
        } while (param_3 < local_c);
      }
      return (int)pcVar9 - (int)param_2;
    }
  }
  DAT_0051c3cc = 0;
  _DAT_0051c3c8 = 9;
  return -1;
}

