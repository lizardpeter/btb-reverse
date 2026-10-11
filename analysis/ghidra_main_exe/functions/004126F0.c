/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004126f0; function: HitTestFireworksEditorRegions; body bytes: 1117
 * callers: 1; callees: 3; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 HitTestFireworksEditorRegions(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  int *local_4;
  
  if (DAT_0050a5bc == 1) {
    DAT_004fbd30 = DAT_004fbd30 + 10;
    DAT_004fbd24 = DAT_004fbd24 + 10;
  }
  iVar6 = 0;
  iVar8 = DAT_0050ab24;
  iVar4 = DAT_0050ab20;
  if (0 < DAT_00509378) {
    local_4 = &DAT_0050a6c8;
    do {
      iVar1 = local_4[2];
      if (iVar1 != -1) {
        iVar2 = 0xf;
        iVar5 = 0x32;
        if ((int)local_4 < 0x50a830) {
          if ((iVar4 == 0) && (iVar8 == 0)) {
            iVar2 = -0x10;
            goto LAB_00412772;
          }
        }
        else {
          iVar2 = 0;
LAB_00412772:
          iVar5 = 0;
        }
        if ((((local_4[-2] < iVar2 + DAT_004fbd24) && (iVar2 + DAT_004fbd24 < *local_4)) &&
            (local_4[-1] < DAT_004fbd30 + iVar5)) && (DAT_004fbd30 + iVar5 < local_4[1])) {
          if (DAT_0050a5bc == 0xd) {
LAB_004129ca:
            DAT_0050a5c0 = iVar6;
            return (&DAT_0050a6d0)[iVar6 * 5];
          }
          if ((int)local_4 < 0x50a7b8) {
            if (iVar4 == 1) {
              DAT_0050a5c0 = iVar6;
              return (&DAT_0050a6d0)[iVar6 * 5];
            }
            if (iVar8 == 1) {
              iVar8 = 2;
              uVar7 = 0x32;
              uVar3 = FUN_0042ffc4();
              uVar3 = uVar3 & 0x80000001;
              if ((int)uVar3 < 0) {
                uVar3 = (uVar3 - 1 | 0xfffffffe) + 1;
              }
              iVar4 = uVar3 + 0x392;
            }
            else {
              if ((&DAT_0050a678)[iVar6 % 6 + (iVar6 / 6) * 6] != -1) {
                iVar8 = iVar6 % 6 + (iVar6 / 6) * 6;
                DAT_005093d8 = (&DAT_0050a678)[iVar8];
                DAT_0050a5c0 = DAT_005093d8;
                SetCursorSurface((int *)(&DAT_0050a4b8)[DAT_005093d8]);
                if (iVar6 / 6 < 2) {
                  DAT_0050ab20 = 1;
                }
                else {
                  DAT_0050ab24 = 1;
                }
                DAT_005093d8 = (&DAT_0050a678)[iVar8];
                (&DAT_0050a678)[iVar8] = 0xffffffff;
                DAT_004fbd24 = (&DAT_0050a6c0)[iVar6 * 5];
                DAT_004fbd30 = (&DAT_0050a6c4)[iVar6 * 5] + -0x19;
                return 0xffffffff;
              }
              iVar8 = 2;
              uVar7 = 0x32;
              uVar3 = FUN_0042ffc4();
              iVar4 = (int)uVar3 % 3 + 0x367;
            }
LAB_00412867:
            PlayManagedSoundById(DAT_0044ddd8,iVar4,uVar7,iVar8);
            iVar8 = DAT_0050ab24;
            iVar4 = DAT_0050ab20;
          }
          else if ((int)local_4 < 0x50a830) {
            if (iVar8 == 1) goto LAB_004129ca;
            iVar8 = 2;
            uVar7 = 0x32;
            if (iVar4 == 1) {
              uVar3 = FUN_0042ffc4();
              uVar3 = uVar3 & 0x80000001;
              if ((int)uVar3 < 0) {
                uVar3 = (uVar3 - 1 | 0xfffffffe) + 1;
              }
              iVar4 = uVar3 + 0x36e;
              goto LAB_00412867;
            }
            uVar3 = FUN_0042ffc4();
            PlayManagedSoundById(DAT_0044ddd8,(int)uVar3 % 3 + 0x38b,uVar7,iVar8);
            iVar8 = DAT_0050ab24;
            iVar4 = DAT_0050ab20;
            if ((&DAT_0050a678)[iVar6 % 6 + (iVar6 / 6) * 6] != -1) {
              DAT_004fbd24 = (&DAT_0050a6c0)[iVar6 * 5];
              DAT_004fbd30 = (&DAT_0050a6c4)[iVar6 * 5] + -0x19;
              iVar8 = iVar6 % 6 + (iVar6 / 6) * 6;
              DAT_005093d8 = (&DAT_0050a678)[iVar8];
              DAT_0050a5c0 = DAT_005093d8;
              SetCursorSurface((int *)(&DAT_0050a4b8)[DAT_005093d8]);
              if (iVar6 / 6 < 2) {
                DAT_0050ab20 = 1;
              }
              else {
                DAT_0050ab24 = 1;
              }
              DAT_005093d8 = (&DAT_0050a678)[iVar8];
              (&DAT_0050a678)[iVar8] = 0xffffffff;
              return 0xffffffff;
            }
          }
          else if (iVar1 < 8) {
            if ((iVar4 == 0) && (iVar8 == 0)) {
              uVar7 = (&DAT_0050a6d0)[iVar6 * 5];
              _DAT_00509670 = uVar7;
LAB_00412aa9:
              DAT_0050a5c0 = iVar6;
              DAT_004fbd24 = (&DAT_0050a6c0)[iVar6 * 5] + -0x1e;
              DAT_004fbd30 = (&DAT_0050a6c4)[iVar6 * 5] + -0x14;
              return uVar7;
            }
          }
          else if (((iVar1 < 0xc) && (iVar8 == 0)) && (iVar4 == 0)) {
            uVar7 = (&DAT_0050a6d0)[iVar6 * 5];
            _DAT_00509674 = uVar7;
            goto LAB_00412aa9;
          }
        }
      }
      iVar6 = iVar6 + 1;
      local_4 = local_4 + 5;
    } while (iVar6 < DAT_00509378);
  }
  if (DAT_0050a5bc == 1) {
    DAT_004fbd30 = DAT_004fbd30 + -0x14;
    DAT_004fbd24 = DAT_004fbd24 + -0x14;
  }
  if (iVar4 == 1) {
    iVar4 = 2;
    uVar7 = 0x32;
    uVar3 = FUN_0042ffc4();
    iVar8 = (int)uVar3 % 3 + 0x36b;
  }
  else {
    if (iVar8 != 1) {
      return 0xffffffff;
    }
    iVar4 = 2;
    uVar7 = 0x32;
    uVar3 = FUN_0042ffc4();
    iVar8 = (int)uVar3 % 3 + 0x38f;
  }
  PlayManagedSoundById(DAT_0044ddd8,iVar8,uVar7,iVar4);
  return 0xffffffff;
}

