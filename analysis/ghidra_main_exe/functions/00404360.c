/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404360; function: CSound_Play; body bytes: 134
 * callers: 14; callees: 4; success: True
 */


int __thiscall CSound_Play(void *this,undefined4 param_1,undefined4 param_2)

{
  int *piVar1;
  int iVar2;
  void *local_4;
  
  if (*(int *)((int)this + 4) == 0) {
    return -0x7ffbfe10;
  }
  local_4 = this;
  piVar1 = (int *)CSound_GetFreeBuffer((uint)this);
  if (piVar1 != (int *)0x0) {
    iVar2 = CSound_RestoreBuffer(piVar1,&local_4);
    if (-1 < iVar2) {
      if (local_4 != (void *)0x0) {
        iVar2 = CSound_FillBufferWithSound(this,piVar1);
        if (iVar2 < 0) {
          return iVar2;
        }
        CSound_Reset((int)this);
      }
      iVar2 = (**(code **)(*piVar1 + 0x30))(piVar1,0,param_1,param_2);
      (**(code **)(*piVar1 + 0x3c))(piVar1,DAT_00446ce4);
    }
    return iVar2;
  }
  return -0x7fffbffb;
}

