/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00410f70; function: PlayInputInterruptibleParkDesignerVoice; body bytes: 90
 * callers: 2; callees: 2; success: True
 */


void __cdecl PlayInputInterruptibleParkDesignerVoice(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = AnyManagedSoundPlaying(DAT_0044ddd8);
  if (((iVar1 != 0) && (param_2 != 0)) && (DAT_0050418c == -1)) {
    DAT_0050418c = param_1;
    DAT_00507b60 = param_2;
  }
  PlayManagedSoundById(DAT_0044ddd8,param_1,0x32,1);
  *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + param_1 + 0x280) * 4 + 0xd98) =
       1;
  return;
}

