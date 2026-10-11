/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00423470; function: HandlePilchardCollisionAndHammerDrop; body bytes: 410
 * callers: 1; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 HandlePilchardCollisionAndHammerDrop(void)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  int iVar5;
  float10 fVar6;
  longlong lVar7;
  undefined4 uVar8;
  
  if (0 < DAT_00514530) {
    DAT_00514530 = DAT_00514530 + -1;
  }
  lVar7 = __ftol();
  iVar5 = (int)lVar7;
  lVar7 = __ftol();
  iVar2 = (int)lVar7;
  lVar7 = __ftol();
  iVar3 = (int)lVar7;
  lVar7 = __ftol();
  fVar6 = DistanceBetweenIntegerPoints((int)lVar7,iVar3,iVar2,iVar5);
  if ((fVar6 < (float10)_DAT_0043b49c) && (DAT_00514530 < 1)) {
    if ((DAT_00445ef4 != 0) && ((DAT_00514510 == 0 && (DAT_005109d4 < _DAT_0043b4e8)))) {
      lVar7 = __ftol();
      DAT_005144bc = (undefined4)lVar7;
      lVar7 = __ftol();
      DAT_005144c0 = (undefined4)lVar7;
      DAT_00514520 = 1;
      DAT_00445ef4 = 0;
      DAT_00514530 = 500;
      DAT_00514490 = DAT_00510d18;
      uVar4 = FUN_0042ffc4();
      iVar5 = (int)uVar4 % 6 + 0x2b8;
      if (iVar5 == DAT_00514548) {
        do {
          uVar4 = FUN_0042ffc4();
          iVar5 = (int)uVar4 % 6 + 0x2b8;
        } while (iVar5 == DAT_00514548);
      }
      PlayManagedSoundById(DAT_0044ddd8,iVar5,0x32,1);
      bVar1 = IsSoundIdPlaying(DAT_0044ddd8,0x2d0);
      if (CONCAT31(extraout_var,bVar1) == 0) {
        bVar1 = IsSoundIdPlaying(DAT_0044ddd8,0x2d1);
        iVar5 = CONCAT31(extraout_var_00,bVar1);
        if (iVar5 == 0) {
          uVar8 = 0x32;
          uVar4 = FUN_0042ffc4();
          uVar4 = uVar4 & 0x80000001;
          if ((int)uVar4 < 0) {
            uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
          }
          PlayManagedSoundById(DAT_0044ddd8,uVar4 + 0x2d0,uVar8,iVar5);
        }
      }
    }
    return 1;
  }
  return 0;
}

