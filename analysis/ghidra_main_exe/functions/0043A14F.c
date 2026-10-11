/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043a14f; function: FUN_0043a14f; body bytes: 61
 * callers: 0; callees: 3; success: True
 */


void FUN_0043a14f(void)

{
  exception *this;
  int unaff_EBP;
  undefined4 *unaff_FS_OFFSET;
  
  FUN_0043a228();
  *(exception **)(unaff_EBP + -0x10) = this;
  *(undefined ***)this = &PTR_FUN_0043bb74;
  *(undefined4 *)(unaff_EBP + -4) = 0;
  MsvcStringTidy(this + 0xc,'\x01');
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  exception::~exception(this);
  *unaff_FS_OFFSET = *(undefined4 *)(unaff_EBP + -0xc);
  return;
}

