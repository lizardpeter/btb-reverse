/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00431129; function: FUN_00431129; body bytes: 179
 * callers: 2; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_00431129(uint param_1)

{
  int iVar1;
  int iVar2;
  HANDLE hObject;
  BOOL BVar3;
  undefined *puVar4;
  int iVar5;
  
  if (DAT_0051da20 <= param_1) {
    DAT_0051c3cc = 0;
    _DAT_0051c3c8 = 9;
    return 0xffffffff;
  }
  iVar5 = (param_1 & 0x1f) * 8;
  if ((*(byte *)((&DAT_0051d920)[(int)param_1 >> 5] + 4 + iVar5) & 1) == 0) {
    _DAT_0051c3c8 = 9;
    DAT_0051c3cc = 0;
    return 0xffffffff;
  }
  iVar1 = FUN_00436322(param_1);
  if (iVar1 != -1) {
    if ((param_1 == 1) || (param_1 == 2)) {
      iVar1 = FUN_00436322(2);
      iVar2 = FUN_00436322(1);
      if (iVar2 == iVar1) goto LAB_004311a2;
    }
    hObject = (HANDLE)FUN_00436322(param_1);
    BVar3 = CloseHandle(hObject);
    if (BVar3 == 0) {
      puVar4 = (undefined *)GetLastError();
      goto LAB_004311a4;
    }
  }
LAB_004311a2:
  puVar4 = (undefined *)0x0;
LAB_004311a4:
  FUN_004362a8(param_1);
  *(undefined1 *)((&DAT_0051d920)[(int)param_1 >> 5] + 4 + iVar5) = 0;
  if (puVar4 == (undefined *)0x0) {
    return 0;
  }
  FUN_0043277a(puVar4);
  return 0xffffffff;
}

