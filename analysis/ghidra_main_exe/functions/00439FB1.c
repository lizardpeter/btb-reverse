/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00439fb1; function: FUN_00439fb1; body bytes: 61
 * callers: 1; callees: 3; success: True
 */


void FUN_00439fb1(void)

{
  exception *this;
  int unaff_EBP;
  
  FUN_0043a228();
  *(exception **)(unaff_EBP + -0x10) = this;
  *(undefined ***)this = &PTR_FUN_0043bb74;
  *(undefined4 *)(unaff_EBP + -4) = 0;
  MsvcStringTidy(this + 0xc,'\x01');
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  exception::~exception(this);
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return;
}

