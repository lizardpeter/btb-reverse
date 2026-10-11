/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402c90; function: StopAllManagedSounds; body bytes: 48
 * callers: 25; callees: 2; success: True
 */


void __fastcall StopAllManagedSounds(void *param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  int iVar2;
  int *piVar3;
  
  iVar2 = 0;
  piVar3 = (int *)((int)param_1 + 0x140);
  do {
    bVar1 = IsSoundIdPlaying(param_1,*piVar3);
    if (CONCAT31(extraout_var,bVar1) != 0) {
      StopSoundSlot(param_1,iVar2);
    }
    iVar2 = iVar2 + 1;
    piVar3 = piVar3 + 1;
  } while (iVar2 < 0x50);
  return;
}

