/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00432662; function: FUN_00432662; body bytes: 79
 * callers: 5; callees: 1; success: True
 */


void FUN_00432662(void)

{
  void *pvStack_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_0043b670;
  puStack_10 = &LAB_00435f38;
  pvStack_14 = ExceptionList;
  ExceptionList = &pvStack_14;
  if (PTR_FUN_004475b4 != (undefined *)0x0) {
    local_8 = 1;
    ExceptionList = &pvStack_14;
    (*(code *)PTR_FUN_004475b4)();
  }
  local_8 = 0xffffffff;
  FUN_0043260c();
  return;
}

