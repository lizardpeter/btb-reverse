/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004300be; function: FUN_004300be; body bytes: 134
 * callers: 1; callees: 6; success: True
 */


undefined4 __cdecl FUN_004300be(LPCSTR param_1)

{
  undefined1 uVar1;
  BOOL BVar2;
  DWORD DVar3;
  uint uVar4;
  undefined *puVar5;
  byte local_10c;
  byte local_10b;
  
  BVar2 = SetCurrentDirectoryA(param_1);
  if (BVar2 != 0) {
    DVar3 = GetCurrentDirectoryA(0x105,(LPSTR)&local_10c);
    if (DVar3 != 0) {
      if (((local_10c != 0x5c) && (local_10c != 0x2f)) || (local_10c != local_10b)) {
        param_1 = (LPCSTR)CONCAT31(param_1._1_3_,0x3d);
        uVar4 = FUN_004327e1((uint)local_10c);
        uVar1 = SUB41(param_1,0);
        param_1 = (LPCSTR)(uint)CONCAT12(0x3a,CONCAT11((char)uVar4,uVar1));
        BVar2 = SetEnvironmentVariableA((LPCSTR)&param_1,(LPCSTR)&local_10c);
        if (BVar2 == 0) goto LAB_00430132;
      }
      return 0;
    }
  }
LAB_00430132:
  puVar5 = (undefined *)GetLastError();
  FUN_0043277a(puVar5);
  return 0xffffffff;
}

