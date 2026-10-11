/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402cf0; function: PlayManagedSoundById; body bytes: 278
 * callers: 47; callees: 4; success: True
 */


undefined4 __thiscall PlayManagedSoundById(void *this,int param_1,undefined4 param_2,int param_3)

{
  bool bVar1;
  undefined3 extraout_var;
  int iVar2;
  int iVar3;
  int *piVar4;
  
  iVar3 = 0;
  if (param_1 == 0) {
    param_1 = 0;
  }
  if (*(char *)(param_1 + 0x6cc + (int)this) == '\0') {
    return 0;
  }
  iVar2 = (int)*(char *)(param_1 + 0x280 + (int)this);
  if ((iVar2 != -1) && (*(int *)((int)this + iVar2 * 4 + 0xc58) == 2)) {
    return 2;
  }
  if (0 < param_3) {
    piVar4 = (int *)((int)this + 0xed8);
    do {
      if ((piVar4[-0x3b6] != 0) &&
         (bVar1 = IsSoundIdPlaying(this,piVar4[-0x366]), CONCAT31(extraout_var,bVar1) != 0)) {
        if (*piVar4 == 1) {
          return 0;
        }
        if ((*piVar4 == 2) && ((param_3 == 1 || (param_3 == 2)))) {
          StopSoundSlot(this,iVar3);
        }
      }
      iVar3 = iVar3 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar3 < 0x50);
  }
  if (iVar2 == -1) {
    iVar2 = AcquireAndPlaySound(this,param_1,param_2,param_3);
  }
  iVar3 = *(int *)((int)this + iVar2 * 4 + 0xc58);
  if (iVar3 == 1) {
    *(undefined4 *)((int)this + iVar2 * 4 + 0xc58) = 2;
    CSound_Play(*(void **)((int)this + iVar2 * 4),0,0);
    return 1;
  }
  if (iVar3 == 3) {
    CSound_Play(*(void **)((int)this + iVar2 * 4),0,0);
    *(undefined4 *)((int)this + iVar2 * 4 + 0xc58) = 2;
  }
  return 1;
}

