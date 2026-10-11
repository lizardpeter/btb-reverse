/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00437bfa; function: FUN_00437bfa; body bytes: 697
 * callers: 1; callees: 11; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * __cdecl FUN_00437bfa(LPCSTR param_1,uint param_2,uint param_3,uint param_4)

{
  byte *pbVar1;
  uint uVar2;
  undefined *puVar3;
  HANDLE hFile;
  DWORD DVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  _SECURITY_ATTRIBUTES local_20;
  DWORD local_14;
  DWORD local_10;
  DWORD local_c;
  byte local_5;
  
  bVar7 = (param_2 & 0x80) == 0;
  local_20.nLength = 0xc;
  local_20.lpSecurityDescriptor = (LPVOID)0x0;
  if (bVar7) {
    local_5 = 0;
  }
  else {
    local_5 = 0x10;
  }
  local_20.bInheritHandle = (BOOL)bVar7;
  if (((param_2 & 0x8000) == 0) && (((param_2 & 0x4000) != 0 || (DAT_0051c6b0 != 0x8000)))) {
    local_5 = local_5 | 0x80;
  }
  uVar2 = param_2 & 3;
  if (uVar2 == 0) {
    local_10 = 0x80000000;
  }
  else if (uVar2 == 1) {
    local_10 = 0x40000000;
  }
  else {
    if (uVar2 != 2) {
      _DAT_0051c3c8 = 0x16;
      DAT_0051c3cc = 0;
      return (undefined *)0xffffffff;
    }
    local_10 = 0xc0000000;
  }
  if (param_3 == 0x10) {
    local_14 = 0;
  }
  else if (param_3 == 0x20) {
    local_14 = 1;
  }
  else if (param_3 == 0x30) {
    local_14 = 2;
  }
  else {
    if (param_3 != 0x40) {
      _DAT_0051c3c8 = 0x16;
      DAT_0051c3cc = 0;
      return (undefined *)0xffffffff;
    }
    local_14 = 3;
  }
  uVar2 = param_2 & 0x700;
  if (uVar2 < 0x401) {
    if ((uVar2 == 0x400) || (uVar2 == 0)) {
      local_c = 3;
    }
    else if (uVar2 == 0x100) {
      local_c = 4;
    }
    else {
      if (uVar2 == 0x200) goto LAB_00437d18;
      if (uVar2 != 0x300) {
        _DAT_0051c3c8 = 0x16;
        DAT_0051c3cc = 0;
        return (undefined *)0xffffffff;
      }
      local_c = 2;
    }
  }
  else {
    if (uVar2 != 0x500) {
      if (uVar2 == 0x600) {
LAB_00437d18:
        local_c = 5;
        goto LAB_00437d28;
      }
      if (uVar2 != 0x700) {
        _DAT_0051c3c8 = 0x16;
        DAT_0051c3cc = 0;
        return (undefined *)0xffffffff;
      }
    }
    local_c = 1;
  }
LAB_00437d28:
  uVar2 = 0x80;
  if (((param_2 & 0x100) != 0) && ((~DAT_0051c3d0 & param_4 & 0x80) == 0)) {
    uVar2 = 1;
  }
  if ((param_2 & 0x40) != 0) {
    uVar2 = uVar2 | 0x4000000;
    local_10 = CONCAT13(local_10._3_1_,0x10000);
  }
  if ((param_2 & 0x1000) != 0) {
    uVar2 = uVar2 | 0x100;
  }
  if ((param_2 & 0x20) == 0) {
    if ((param_2 & 0x10) != 0) {
      uVar2 = uVar2 | 0x10000000;
    }
  }
  else {
    uVar2 = uVar2 | 0x8000000;
  }
  puVar3 = (undefined *)FUN_0043619c();
  if (puVar3 == (undefined *)0xffffffff) {
    DAT_0051c3cc = 0;
    _DAT_0051c3c8 = 0x18;
  }
  else {
    hFile = CreateFileA(param_1,local_10,local_14,&local_20,local_c,uVar2,(HANDLE)0x0);
    if (hFile != (HANDLE)0xffffffff) {
      DVar4 = GetFileType(hFile);
      if (DVar4 != 0) {
        if (DVar4 == 2) {
          local_5 = local_5 | 0x40;
        }
        else if (DVar4 == 3) {
          local_5 = local_5 | 8;
        }
        FUN_00436231((uint)puVar3,hFile);
        iVar6 = ((uint)puVar3 & 0x1f) * 8;
        param_1._3_1_ = local_5 & 0x48;
        *(byte *)((&DAT_0051d920)[(int)puVar3 >> 5] + 4 + iVar6) = local_5 | 1;
        if ((((local_5 & 0x48) == 0) && ((local_5 & 0x80) != 0)) && ((param_2 & 2) != 0)) {
          local_14 = FUN_004372d7((uint)puVar3,-1,2);
          if (local_14 == 0xffffffff) {
            if (DAT_0051c3cc != 0x83) {
LAB_00437e89:
              FUN_00431129((uint)puVar3);
              return (undefined *)0xffffffff;
            }
          }
          else {
            param_3 = param_3 & 0xffffff;
            iVar5 = FUN_00433961((uint)puVar3,(char *)((int)&param_3 + 3),(char *)0x1);
            if ((((iVar5 == 0) && (param_3._3_1_ == '\x1a')) &&
                (iVar5 = FUN_004390cc(puVar3,local_14), iVar5 == -1)) ||
               (DVar4 = FUN_004372d7((uint)puVar3,0,0), DVar4 == 0xffffffff)) goto LAB_00437e89;
          }
        }
        if (param_1._3_1_ != 0) {
          return puVar3;
        }
        if ((param_2 & 8) != 0) {
          pbVar1 = (byte *)((&DAT_0051d920)[(int)puVar3 >> 5] + 4 + iVar6);
          *pbVar1 = *pbVar1 | 0x20;
          return puVar3;
        }
        return puVar3;
      }
      CloseHandle(hFile);
    }
    puVar3 = (undefined *)GetLastError();
    FUN_0043277a(puVar3);
  }
  return (undefined *)0xffffffff;
}

