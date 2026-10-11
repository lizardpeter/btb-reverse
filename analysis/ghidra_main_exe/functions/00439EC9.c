/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00439ec9; function: FUN_00439ec9; body bytes: 89
 * callers: 4; callees: 6; success: True
 */


void FUN_00439ec9(void)

{
  size_t sVar1;
  int unaff_EBP;
  
  FUN_0043a228();
  *(undefined1 *)(unaff_EBP + -0x20) = *(undefined1 *)(unaff_EBP + -0xd);
  MsvcStringTidy((void *)(unaff_EBP + -0x20),'\0');
  sVar1 = _strlen("string too long");
  MsvcStringAssignBuffer((void *)(unaff_EBP + -0x20),(undefined4 *)"string too long",sVar1);
  *(undefined4 *)(unaff_EBP + -4) = 0;
  FUN_00439f23();
  *(undefined ***)(unaff_EBP + -0x3c) = &PTR_LAB_0043bb54;
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(unaff_EBP + -0x3c,&DAT_0043c0e8);
}

