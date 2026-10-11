/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00405330; function: BitmapPrinterSetDocumentName; body bytes: 23
 * callers: 0; callees: 1; success: True
 */


void __thiscall BitmapPrinterSetDocumentName(int param_1,LPCSTR param_2)

{
  lstrcpynA((LPSTR)(param_1 + 0xc),param_2,0x104);
  return;
}

