/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00439f23; function: FUN_00439f23; body bytes: 100
 * callers: 2; callees: 4; success: True
 */


undefined4 * FUN_00439f23(void)

{
  undefined4 *this;
  undefined1 *puVar1;
  undefined4 *this_00;
  int unaff_EBP;
  
  FUN_0043a228();
  *(undefined4 **)(unaff_EBP + -0x14) = this_00;
  *(undefined1 **)(unaff_EBP + -0x10) = &DAT_00482574;
  FUN_004303ba(this_00,(undefined4 *)(unaff_EBP + -0x10));
  puVar1 = *(undefined1 **)(unaff_EBP + 8);
  *(undefined4 *)(unaff_EBP + -4) = 0;
  this = this_00 + 3;
  *(undefined1 *)this = *puVar1;
  MsvcStringTidy(this,'\0');
  MsvcStringAssignSubstring(this,puVar1,0,DAT_0043b334);
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  *this_00 = &PTR_FUN_0043bb74;
  return this_00;
}

