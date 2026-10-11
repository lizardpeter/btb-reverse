/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402f60; function: ReapFinishedSounds; body bytes: 81
 * callers: 1; callees: 2; success: True
 */


void __fastcall ReapFinishedSounds(void *param_1)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  
  iVar2 = 0;
  piVar3 = (int *)((int)param_1 + 0xd98);
  do {
    if (piVar3[-0x50] == 2) {
      if ((*piVar3 == 0) || ((DAT_004fbe54 == 0 && (DAT_004fbd50 == 0)))) {
        uVar1 = CSound_IsSoundPlaying(piVar3[-0x366]);
        if (uVar1 != 0) goto LAB_00402fa4;
      }
      StopSoundSlot(param_1,iVar2);
    }
LAB_00402fa4:
    iVar2 = iVar2 + 1;
    piVar3 = piVar3 + 1;
    if (0x4f < iVar2) {
      return;
    }
  } while( true );
}

