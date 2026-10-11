/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004273f0; function: UpdateSquirrelPlacementInteraction; body bytes: 1524
 * callers: 1; callees: 4; success: True
 */


void UpdateSquirrelPlacementInteraction(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  undefined4 uVar7;
  int iVar8;
  int local_30 [12];
  
  if (DAT_005150e4 == 0) {
    if (DAT_005150e8 == 0) {
      if (6 < DAT_00514fb4) {
        if (DAT_005150d0 < DAT_005150cc) {
          DAT_005150d8 = 1;
          DAT_00514ed8 = 1;
          DAT_005150dc = 0;
        }
        else {
          DAT_005150e0 = 1;
          StopAllManagedSounds(DAT_0044ddd8);
          iVar8 = 1;
          uVar7 = 0x32;
          uVar3 = FUN_0042ffc4();
          uVar3 = uVar3 & 0x80000003;
          if ((int)uVar3 < 0) {
            uVar3 = (uVar3 - 1 | 0xfffffffc) + 1;
          }
          PlayManagedSoundById(DAT_0044ddd8,uVar3 + 0x298,uVar7,iVar8);
        }
      }
      if (((DAT_004fbe54 != 0) && (DAT_00514fb4 < 6)) && (iVar8 = 0, 0 < DAT_00514f50)) {
        piVar6 = &DAT_00514ff0;
        piVar5 = &DAT_004467a0;
        do {
          if (((piVar5[-1] < DAT_004fbd24) && (DAT_004fbd24 < piVar5[-1] + 0x59)) &&
             ((*piVar5 < DAT_004fbd30 && (DAT_004fbd30 < *piVar5 + 0x85)))) {
            DAT_004469a8 = 0;
            DAT_00514ef8 = iVar8;
            if (*piVar6 == 1) {
              DAT_005150e4 = 1;
            }
            else {
              DAT_005150e4 = 2;
            }
          }
          iVar8 = iVar8 + 1;
          piVar5 = piVar5 + 2;
          piVar6 = piVar6 + 1;
        } while (iVar8 < DAT_00514f50);
        return;
      }
    }
    return;
  }
  if ((DAT_005150e4 == 1) || (DAT_005150e4 == 2)) {
    iVar8 = StepSquirrelPlacementActorTowardTarget
                      ((&DAT_0044679c)[DAT_00514ef8 * 2],(&DAT_004467a0)[DAT_00514ef8 * 2] + 0x85);
    if (iVar8 == 0) {
      return;
    }
    DAT_004469a4 = DAT_00514ef8;
    DAT_005150e4 = DAT_005150e4 + 2;
    return;
  }
  if ((DAT_005150e4 != 3) && (DAT_005150e4 != 4)) {
    if (DAT_005150e4 == 5) {
      iVar8 = StepSquirrelPlacementActorTowardTarget(DAT_00446994,DAT_00446998);
      if (iVar8 == 0) {
        return;
      }
      DAT_005150e4 = 0;
      return;
    }
    if (DAT_005150e4 == 6) {
      iVar8 = StepSquirrelPlacementActorTowardTarget(DAT_00446994,DAT_00446998);
      if (iVar8 != 0) {
        DAT_005150e4 = 7;
      }
      DAT_004469a8 = 2;
      return;
    }
    if (DAT_005150e4 == 7) {
      iVar4 = StepSquirrelPlacementActorTowardTarget
                        ((&DAT_00446784)[*(int *)(&DAT_00515078 + DAT_005150d0 * 4) * 2],
                         (&DAT_00446788)[*(int *)(&DAT_00515078 + DAT_005150d0 * 4) * 2] + 0x85);
      iVar8 = DAT_00514ef8;
      if (iVar4 == 0) {
        return;
      }
      DAT_005150e4 = 8;
      (&DAT_00515088)[DAT_00514ef8] = DAT_00514efc;
      DAT_004469a4 = iVar8;
      return;
    }
    if (DAT_005150e4 != 8) {
      return;
    }
    DAT_004469a8 = 2;
    iVar8 = StepSquirrelPlacementActorTowardTarget
                      ((&DAT_0044679c)[DAT_00514ef8 * 2],(&DAT_004467a0)[DAT_00514ef8 * 2] + 0x85);
    if (iVar8 == 0) {
      return;
    }
    DAT_005150e4 = 5;
    DAT_004469a4 = 0xffffffff;
    return;
  }
  iVar8 = StepSquirrelPlacementActorTowardTarget
                    ((&DAT_00446784)[*(int *)(&DAT_00515078 + DAT_005150d0 * 4) * 2],
                     (&DAT_00446788)[*(int *)(&DAT_00515078 + DAT_005150d0 * 4) * 2] + 0x85);
  if (iVar8 == 0) {
    return;
  }
  if (DAT_005150e4 == 3) {
    DAT_004469a8 = 1;
    if (DAT_00514fb4 < 5) {
      DAT_005150e8 = 1;
    }
    DAT_004469a4 = 0xffffffff;
    iVar8 = *(int *)(&DAT_00515078 + DAT_005150d0 * 4);
    iVar1 = DAT_005150d0 * 4;
    iVar4 = iVar8 + DAT_005150d0 * 4;
    iVar2 = (&DAT_00515088)[DAT_00514ef8];
    if ((&DAT_00514f64)[iVar4] == (iVar2 % 9) % 3) {
      (&DAT_00514f64)[iVar4] = 0xffffffff;
    }
    (&DAT_00514fbc)[iVar4] = iVar2;
    if ((iVar8 < 4) || (DAT_00514fb4 < 5)) {
      iVar8 = iVar8 + 1;
      *(int *)(&DAT_00515078 + iVar1) = iVar8;
      if ((iVar8 == 1) || ((iVar8 == 2 || (iVar4 = DAT_00515004 + 1, iVar8 == 3)))) {
        DAT_00515004 = DAT_00515004 + 2;
        iVar4 = DAT_00515004;
      }
    }
    else {
      iVar4 = DAT_00515004;
      if (DAT_005150d0 < DAT_005150cc) {
        DAT_005150d8 = 1;
        DAT_00514ed8 = 1;
        DAT_005150dc = 0;
      }
      else {
        DAT_005150e0 = 1;
      }
    }
    DAT_00515004 = iVar4;
    local_30[5] = 0x279;
    local_30[6] = 0x27a;
    local_30[7] = 0x27b;
    local_30[8] = 0x27c;
    local_30[9] = 0x290;
    local_30[10] = 0x291;
    local_30[0xb] = 0x292;
    uVar3 = FUN_0042ffc4();
    DAT_00515120 = (int)uVar3 % 7;
    iVar8 = local_30[DAT_00515120 + 5];
    PlayManagedSoundById(DAT_0044ddd8,iVar8,0x32,1);
    if (DAT_00515120 < 4) {
      uVar3 = FUN_0042ffc4();
      DAT_00514f08 = (int)uVar3 % 3;
      uVar3 = FUN_0042ffc4();
      DAT_00514f60 = (int)uVar3 % 10 + 10;
      DAT_00514fec = iVar8;
    }
    (&DAT_00515088)[DAT_00514ef8] = 0xffffffff;
    DAT_005150e4 = DAT_005150e4 + 2;
    return;
  }
  iVar4 = DAT_005150d0 * 4;
  uVar7 = (&DAT_00515088)[DAT_00514ef8];
  local_30[0] = 0x27d;
  iVar8 = *(int *)(&DAT_00515078 + iVar4);
  DAT_004469a4 = 0xffffffff;
  DAT_00514efc = uVar7;
  (&DAT_00515088)[DAT_00514ef8] = 0xffffffff;
  (&DAT_00514fbc)[iVar8 + iVar4] = uVar7;
  local_30[1] = 0x27e;
  local_30[5] = 0x285;
  local_30[6] = 0x286;
  local_30[7] = 0x288;
  local_30[8] = 0x289;
  local_30[2] = 0x28a;
  local_30[3] = 0x28b;
  local_30[4] = 0x28c;
  uVar3 = FUN_0042ffc4();
  if ((int)uVar3 % 5 == 0) {
    uVar3 = FUN_0042ffc4();
  }
  else {
    if (DAT_00514f00 < (int)(&DAT_00514eb8)[DAT_00514ef8 * 2]) {
      uVar3 = FUN_0042ffc4();
      iVar8 = local_30[(int)uVar3 % 3 + 2];
      goto LAB_0042793d;
    }
    if ((int)(&DAT_00514eb8)[DAT_00514ef8 * 2] < DAT_00514f00) {
      uVar3 = FUN_0042ffc4();
      uVar3 = uVar3 & 0x80000003;
      if ((int)uVar3 < 0) {
        uVar3 = (uVar3 - 1 | 0xfffffffc) + 1;
      }
      iVar8 = local_30[uVar3 + 5];
      goto LAB_0042793d;
    }
    uVar3 = FUN_0042ffc4();
  }
  uVar3 = uVar3 & 0x80000001;
  if ((int)uVar3 < 0) {
    uVar3 = (uVar3 - 1 | 0xfffffffe) + 1;
  }
  iVar8 = local_30[uVar3];
LAB_0042793d:
  PlayManagedSoundById(DAT_0044ddd8,iVar8,0x32,1);
  if ((iVar8 == 0x27d) || (iVar8 == 0x27e)) {
    uVar3 = FUN_0042ffc4();
    DAT_00514f08 = (int)uVar3 % 3;
    uVar3 = FUN_0042ffc4();
    DAT_00514f60 = (int)uVar3 % 10 + 10;
    DAT_00514fec = iVar8;
  }
  DAT_005150e4 = DAT_005150e4 + 2;
  return;
}

