/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00405309; function: Catch@00405309; body bytes: 26
 * callers: 0; callees: 3; success: True
 */


void Catch_00405309(void)

{
  HDC hdc;
  int unaff_EBP;
  
  hdc = *(HDC *)(unaff_EBP + 8);
  EndPage(hdc);
  EndDoc(hdc);
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(0,0);
}

