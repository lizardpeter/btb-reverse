/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00428120; function: UpdateWalkthroughMovie; body bytes: 175
 * callers: 1; callees: 4; success: True
 */


void __cdecl UpdateWalkthroughMovie(undefined4 param_1)

{
  int iVar1;
  
  if ((((0x31 < DAT_004fbd24) && (DAT_004fbd24 < 0x16d)) && (0x8c < DAT_004fbd30)) &&
     (DAT_004fbd30 < 0x15e)) {
    iVar1 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if (iVar1 == 0) {
      PlayManagedSoundById(DAT_0044ddd8,DAT_00446fd8,0x5a,1);
      *(undefined4 *)
       ((int)DAT_0044ddd8 + *(char *)(DAT_00446fd8 + 0x280 + (int)DAT_0044ddd8) * 4 + 0xd98) = 1;
    }
  }
  DAT_0051bca8 = 1;
  DecodeAndBlitBinkFrame(param_1,DAT_0051bd2c,DAT_0051be3c);
  if ((uint)DAT_0051bd2c[2] <= (uint)DAT_0051bd2c[3]) {
    RestartBinkMovie(DAT_0051bd2c);
  }
  return;
}

