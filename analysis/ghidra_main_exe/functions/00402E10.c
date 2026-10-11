/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402e10; function: AcquireAndPlaySound; body bytes: 333
 * callers: 1; callees: 5; success: True
 */


int __thiscall AcquireAndPlaySound(void *this,int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  void *this_00;
  bool bVar2;
  int iVar3;
  undefined3 extraout_var;
  int *piVar4;
  int iVar5;
  int local_108;
  CHAR local_104 [260];
  
  iVar3 = 0;
  piVar4 = (int *)((int)this + 0xc58);
  do {
    if (*piVar4 == 0) {
      iVar5 = iVar3;
      if (iVar3 != -1) goto LAB_00402e8b;
      break;
    }
    iVar3 = iVar3 + 1;
    piVar4 = piVar4 + 1;
    iVar5 = -1;
  } while (iVar3 < 0x50);
  iVar3 = iVar5;
  local_108 = 0x65;
  iVar5 = 0;
  piVar4 = (int *)((int)this + 0xb18);
  do {
    bVar2 = IsSoundIdPlaying(this,piVar4[-0x276]);
    if ((CONCAT31(extraout_var,bVar2) == 0) && (*piVar4 < local_108)) {
      iVar3 = iVar5;
      local_108 = *piVar4;
    }
    iVar5 = iVar5 + 1;
    piVar4 = piVar4 + 1;
  } while (iVar5 < 0x50);
  if (iVar3 == -1) {
    return -1;
  }
  ReleaseSoundSlot(this,iVar3);
LAB_00402e8b:
  *(int *)((int)this + iVar3 * 4 + 0x140) = param_1;
  *(char *)(param_1 + 0x280 + (int)this) = (char)iVar3;
  *(undefined4 *)((int)this + iVar3 * 4 + 0xb18) = param_2;
  *(undefined4 *)((int)this + iVar3 * 4 + 0xd98) = 0;
  *(undefined4 *)((int)this + iVar3 * 4 + 0xed8) = param_3;
  crt_sprintf(local_104,(byte *)s_data_sound__s_0043e120);
  puVar1 = (undefined4 *)((int)this + iVar3 * 4);
  CSoundManager_Create
            (DAT_004fc174,puVar1,local_104,0,DAT_0043b5a8,DAT_0043b5ac,DAT_0043b5b0,DAT_0043b5b4,1);
  this_00 = (void *)*puVar1;
  if (this_00 != (void *)0x0) {
    CSound_Play(this_00,0,0);
    *(undefined4 *)((int)this + iVar3 * 4 + 0xc58) = 1;
  }
  return iVar3;
}

