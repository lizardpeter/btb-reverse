/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042cfd0; function: PreparePlayAgainTransition; body bytes: 135
 * callers: 10; callees: 3; success: True
 */


void PreparePlayAgainTransition(void)

{
  if (DAT_0051c2b4 != (undefined4 *)0x0) {
    CSound_Stop((int)DAT_0051c2b4);
    if (DAT_0051c2b4 != (undefined4 *)0x0) {
      (**(code **)*DAT_0051c2b4)(1);
      DAT_0051c2b4 = (undefined4 *)0x0;
    }
  }
  (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,*(undefined4 *)(DAT_0044de08 + 0xc),0,0);
  DAT_0051c300 = 1;
  StopAllManagedSounds(DAT_0044ddd8);
  PlayManagedSoundById(DAT_0044ddd8,0x23d,0x32,1);
  *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x4bd) * 4 + 0xd98) = 1;
  return;
}

