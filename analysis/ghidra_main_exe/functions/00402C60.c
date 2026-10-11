/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402c60; function: IsSoundIdPlaying; body bytes: 44
 * callers: 12; callees: 1; success: True
 */


bool __thiscall IsSoundIdPlaying(void *this,int param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  
  cVar1 = *(char *)(param_1 + 0x280 + (int)this);
  if ((cVar1 != -1) && (iVar2 = *(int *)((int)this + cVar1 * 4), iVar2 != 0)) {
    uVar3 = CSound_IsSoundPlaying(iVar2);
    return uVar3 != 0;
  }
  return false;
}

