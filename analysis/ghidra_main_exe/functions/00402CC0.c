/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402cc0; function: StopSoundSlot; body bytes: 40
 * callers: 3; callees: 2; success: True
 */


void __thiscall StopSoundSlot(void *this,int param_1)

{
  *(undefined4 *)((int)this + param_1 * 4 + 0xc58) = 3;
  CSound_Stop(*(int *)((int)this + param_1 * 4));
  CSound_Reset(*(int *)((int)this + param_1 * 4));
  return;
}

