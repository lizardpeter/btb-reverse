/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402bc0; function: ReleaseSoundSlot; body bytes: 82
 * callers: 1; callees: 0; success: True
 */


void __thiscall ReleaseSoundSlot(void *this,int param_1)

{
  undefined4 *puVar1;
  
  puVar1 = *(undefined4 **)((int)this + param_1 * 4);
  if (puVar1 != (undefined4 *)0x0) {
    (**(code **)*puVar1)(1);
    *(undefined4 *)((int)this + param_1 * 4) = 0;
  }
  *(undefined4 *)((int)this + param_1 * 4) = 0;
  *(undefined4 *)((int)this + param_1 * 4 + 0xb18) = 0xffffffff;
  *(undefined4 *)((int)this + param_1 * 4 + 0xc58) = 0;
  *(undefined1 *)((int)this + *(int *)((int)this + param_1 * 4 + 0x140) + 0x280) = 0xff;
  *(undefined4 *)((int)this + param_1 * 4 + 0x140) = 0xffffffff;
  return;
}

