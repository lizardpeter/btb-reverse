/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043260c; function: FUN_0043260c; body bytes: 79
 * callers: 3; callees: 1; success: True
 */


void FUN_0043260c(void)

{
  void *pvStack_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_0043b658;
  puStack_10 = &LAB_00435f38;
  pvStack_14 = ExceptionList;
  ExceptionList = &pvStack_14;
  if (DAT_0051c42c != (code *)0x0) {
    local_8 = 1;
    ExceptionList = &pvStack_14;
    (*DAT_0051c42c)();
  }
  local_8 = 0xffffffff;
  FUN_0043675d();
  return;
}

