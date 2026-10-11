/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00423be0; function: UpdateSpudMazeMrBentleyNarrationSequence; body bytes: 835
 * callers: 1; callees: 4; success: True
 */


undefined4 UpdateSpudMazeMrBentleyNarrationSequence(void)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int local_10 [4];
  
  local_10[0] = 0x2c;
  local_10[3] = 0x2c;
  local_10[1] = 0x28;
  local_10[2] = 0x38;
  DAT_00510a00 = 3;
  DAT_00514510 = 0;
  switch(DAT_00445f00) {
  case 0:
    DAT_0051452c = DAT_0051452c + -1;
    if (DAT_0051452c < 1) {
      DAT_0051452c = 9;
      DAT_00445f10 = DAT_00445f10 + 1;
      if (7 < DAT_00445f10) {
        DAT_00445f10 = 0;
        iVar4 = AnyManagedSoundPlaying(DAT_0044ddd8);
        if (iVar4 == 0) {
          DAT_00445f10 = 0xc;
          DAT_00445f00 = DAT_00445f00 + 1;
          return 0;
        }
      }
    }
    break;
  case 1:
    iVar4 = DAT_0051452c + -1;
    if (iVar4 < 1) {
      iVar4 = 9;
      DAT_00445f04 = DAT_00445f04 + 5;
      DAT_00445f10 = DAT_00445f10 + 1;
      DAT_0051452c = 9;
      if ((0x14 < DAT_00445f10) && (DAT_00445f10 = 0xc, 299 < DAT_00445f04)) {
        DAT_00445f10 = 0x19;
        DAT_00445f00 = DAT_00445f00 + 1;
        return 0;
      }
    }
    goto LAB_00423d82;
  case 2:
    iVar4 = DAT_0051452c;
LAB_00423d82:
    DAT_0051452c = iVar4 + -1;
    if (DAT_0051452c < 1) {
      DAT_0051452c = 9;
      DAT_00445f10 = DAT_00445f10 + 1;
      if (0x27 < DAT_00445f10) {
        DAT_00445f10 = 0x19;
        iVar4 = DAT_00445f0c + -1;
        bVar1 = DAT_00445f0c < 1;
        DAT_00445f0c = iVar4;
        if (bVar1) {
          DAT_00445f00 = DAT_00445f00 + 1;
          bVar1 = 0 < DAT_005144c8;
          if (bVar1) {
            PlayManagedSoundById(DAT_0044ddd8,0x2ca,0x32,1);
          }
          else {
            PlayManagedSoundById(DAT_0044ddd8,0x2cb,0x32,1);
          }
          DAT_005140f8 = (uint)bVar1;
          DAT_00445f10 = local_10[DAT_005140f8];
          return 0;
        }
      }
    }
    break;
  case 3:
    DAT_0051452c = DAT_0051452c + -1;
    if (DAT_0051452c < 1) {
      DAT_00445f10 = DAT_00445f10 + 1;
      DAT_0051452c = 9;
      if (local_10[DAT_005140f8 + 2] <= DAT_00445f10) {
        DAT_00445f10 = local_10[DAT_005140f8];
      }
    }
    iVar4 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if (iVar4 == 0) {
      iVar4 = 1;
      uVar5 = 0x32;
      if (DAT_005144c8 < 1) {
        uVar2 = FUN_0042ffc4();
        uVar2 = uVar2 & 0x80000001;
        if ((int)uVar2 < 0) {
          uVar2 = (uVar2 - 1 | 0xfffffffe) + 1;
        }
        iVar3 = uVar2 + 0x2d6;
      }
      else {
        uVar2 = FUN_0042ffc4();
        uVar2 = uVar2 & 0x80000001;
        if ((int)uVar2 < 0) {
          uVar2 = (uVar2 - 1 | 0xfffffffe) + 1;
        }
        iVar3 = uVar2 + 0x2cc;
      }
      PlayManagedSoundById(DAT_0044ddd8,iVar3,uVar5,iVar4);
      DAT_00445f00 = DAT_00445f00 + 1;
      return 0;
    }
    break;
  case 4:
    DAT_0051452c = DAT_0051452c + -1;
    if (DAT_0051452c < 1) {
      DAT_00445f10 = DAT_00445f10 + 1;
      DAT_0051452c = 9;
      if (local_10[DAT_005140f8 + 2] <= DAT_00445f10) {
        DAT_00445f10 = local_10[DAT_005140f8];
      }
    }
    iVar4 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if (iVar4 == 0) {
      return 1;
    }
    break;
  case -1:
    DAT_00514528 = 1;
    DAT_0051452c = DAT_0051452c + -1;
    if (DAT_0051452c < 1) {
      DAT_0051452c = 9;
      DAT_00445f04 = DAT_00445f04 + 5;
      DAT_00445f10 = DAT_00445f10 + 1;
      if ((0x14 < DAT_00445f10) && (DAT_00445f10 = 0xc, 0x95 < DAT_00445f04)) {
        DAT_00445f00 = DAT_00445f00 + 1;
        DAT_00445f10 = 0;
        StopAllManagedSounds(DAT_0044ddd8);
        PlayManagedSoundById(DAT_0044ddd8,0x2c9,0x32,1);
      }
    }
  }
  return 0;
}

