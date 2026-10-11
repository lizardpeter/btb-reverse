/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00401000; function: PlayHelpVoice; body bytes: 39
 * callers: 2; callees: 2; success: True
 */


void __cdecl PlayHelpVoice(int param_1,int param_2)

{
  if (param_2 == 1) {
    StopAllManagedSounds(DAT_0044ddd8);
  }
  PlayManagedSoundById(DAT_0044ddd8,param_1,0x32,2);
  return;
}

