/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043a00a; function: FUN_0043a00a; body bytes: 93
 * callers: 3; callees: 4; success: True
 */


undefined4 * FUN_0043a00a(void)

{
  undefined4 *this;
  undefined1 uVar1;
  int iVar2;
  undefined4 *this_00;
  int unaff_EBP;
  
  FUN_0043a228();
  iVar2 = *(int *)(unaff_EBP + 8);
  *(undefined4 **)(unaff_EBP + -0x10) = this_00;
  FUN_004303f7(this_00,iVar2);
  uVar1 = *(undefined1 *)(iVar2 + 0xc);
  *(undefined4 *)(unaff_EBP + -4) = 0;
  this = this_00 + 3;
  *(undefined1 *)this = uVar1;
  MsvcStringTidy(this,'\0');
  MsvcStringAssignSubstring(this,(void *)(iVar2 + 0xc),0,DAT_0043b334);
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  *this_00 = &PTR_FUN_0043bb74;
  return this_00;
}

