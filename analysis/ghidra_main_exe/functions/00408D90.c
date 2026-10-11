/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00408d90; function: InitializeBinkPlaybackSystem; body bytes: 117
 * callers: 1; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void InitializeBinkPlaybackSystem(void)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  
  piVar1 = *(int **)(DAT_0044de08 + 4);
  puVar3 = &DAT_004fbff0;
  for (iVar2 = 0x1f; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  DAT_004fbff0 = 0x7c;
  DAT_004fbff4 = 7;
  _DAT_004fbffc = 0x280;
  _DAT_004fbff8 = 0x1e0;
  _DAT_004fc058 = 0x40;
  (**(code **)(*piVar1 + 0x18))(piVar1,&DAT_004fbff0,&DAT_004fbfec,0);
  _BinkSetSoundSystem_8(_BinkOpenDirectSound_4_exref,0);
  DAT_004fbfe8 = 0;
  return;
}

