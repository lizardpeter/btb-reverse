/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004280a0; function: StopActivityMusic; body bytes: 42
 * callers: 8; callees: 1; success: True
 */


void StopActivityMusic(void)

{
  if (DAT_0051c2b4 != (undefined4 *)0x0) {
    CSound_Stop((int)DAT_0051c2b4);
    if (DAT_0051c2b4 != (undefined4 *)0x0) {
      (**(code **)*DAT_0051c2b4)(1);
    }
    DAT_0051c2b4 = (undefined4 *)0x0;
  }
  return;
}

