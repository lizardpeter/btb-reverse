/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004092d0; function: FUN_004092d0; body bytes: 183
 * callers: 1; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_004092d0(HWND param_1)

{
  undefined4 *puVar1;
  int iVar2;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0043a54b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar1 = (undefined4 *)operator_new(4);
  local_4 = 0;
  if (puVar1 == (undefined4 *)0x0) {
    DAT_004fc174 = (int *)0x0;
  }
  else {
    DAT_004fc174 = (int *)CSoundManager_Constructor(puVar1);
  }
  local_4 = 0xffffffff;
  iVar2 = CSoundManager_Initialize(DAT_004fc174);
  if (iVar2 < 0) {
    EndDialog(param_1,3);
    ExceptionList = local_c;
    return 0;
  }
  _DAT_004fc080 = 0;
  DAT_00446ce4 = (DAT_00446ce0 + -100) * 0x2a;
  ExceptionList = local_c;
  return 0;
}

