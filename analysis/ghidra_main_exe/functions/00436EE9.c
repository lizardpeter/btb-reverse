/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00436ee9; function: FUN_00436ee9; body bytes: 41
 * callers: 1; callees: 0; success: True
 */


void FUN_00436ee9(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_0051c7e0;
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *(undefined1 *)puVar2 = 0;
  DAT_0051c6b8 = 0;
  DAT_0051c6cc = 0;
  DAT_0051c8e4 = 0;
  DAT_0051c6c0 = 0;
  DAT_0051c6c4 = 0;
  DAT_0051c6c8 = 0;
  return;
}

