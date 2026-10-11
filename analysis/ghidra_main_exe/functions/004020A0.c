/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004020a0; function: DestroyDisplayManager; body bytes: 39
 * callers: 1; callees: 2; success: True
 */


void DestroyDisplayManager(void)

{
  undefined4 *puVar1;
  
  puVar1 = DAT_0044de08;
  if (DAT_0044de08 != (undefined4 *)0x0) {
    DisplayManagerDestructor(DAT_0044de08);
    FUN_0042fbdc((undefined *)puVar1);
    DAT_0044de08 = (undefined4 *)0x0;
  }
  return;
}

