/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00422760; function: MaybePlayRandomSpudVoice; body bytes: 140
 * callers: 1; callees: 3; success: True
 */


int MaybePlayRandomSpudVoice(void)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  undefined3 extraout_var;
  
  iVar3 = DAT_00514528;
  if (DAT_00514528 < 1) {
    uVar2 = FUN_0042ffc4();
    iVar3 = (int)uVar2 / 3000;
    if ((int)uVar2 % 3000 == 0) {
      iVar3 = 0x2d2;
      do {
        bVar1 = IsSoundIdPlaying(DAT_0044ddd8,iVar3);
        if (CONCAT31(extraout_var,bVar1) != 0) {
          return CONCAT31(extraout_var,bVar1);
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 < 0x2d6);
      uVar2 = FUN_0042ffc4();
      uVar2 = uVar2 & 0x80000003;
      if ((int)uVar2 < 0) {
        uVar2 = (uVar2 - 1 | 0xfffffffc) + 1;
      }
      iVar3 = uVar2 + 0x2d2;
      if (iVar3 == DAT_00514540) {
        do {
          uVar2 = FUN_0042ffc4();
          uVar2 = uVar2 & 0x80000003;
          if ((int)uVar2 < 0) {
            uVar2 = (uVar2 - 1 | 0xfffffffc) + 1;
          }
          iVar3 = uVar2 + 0x2d2;
        } while (iVar3 == DAT_00514540);
      }
      iVar3 = PlayManagedSoundById(DAT_0044ddd8,iVar3,0x32,1);
    }
  }
  return iVar3;
}

