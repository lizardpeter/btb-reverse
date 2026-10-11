/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409390; function: ShutdownDirectSound8; body bytes: 44
 * callers: 2; callees: 2; success: True
 */


undefined4 ShutdownDirectSound8(void)

{
  int *piVar1;
  
  piVar1 = DAT_004fc174;
  if (DAT_004fc174 != (int *)0x0) {
    CSoundManager_Destructor(DAT_004fc174);
    FUN_0042fbdc((undefined *)piVar1);
    DAT_004fc174 = (int *)0x0;
  }
  return 1;
}

