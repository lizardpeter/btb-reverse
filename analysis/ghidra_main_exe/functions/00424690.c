/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00424690; function: UpdateAndDrawSpudSkatePlayback; body bytes: 1514
 * callers: 1; callees: 9; success: True
 */


void UpdateAndDrawSpudSkatePlayback(void)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  undefined *puVar5;
  undefined4 *puVar6;
  undefined *puVar7;
  undefined4 uVar8;
  int *piVar9;
  undefined4 uVar10;
  
  piVar2 = *(int **)(DAT_0044de08 + 0xc);
  uVar10 = 0;
  (**(code **)(*piVar2 + 0x1c))(piVar2,0,0,DAT_00514ea0,0,0);
  puVar7 = *(undefined **)(DAT_00514b2c + 0xc);
  iVar4 = DAT_00514eb0 % 10;
  if (0 < DAT_00514eb0 / 10) {
    (**(code **)(*piVar2 + 0x1c))
              (piVar2,0x1c2,0x19f,DAT_005148b4,&DAT_00446578 + (DAT_00514eb0 / 10) * 0x10,1);
  }
  puVar5 = &DAT_00446578 + iVar4 * 0x10;
  piVar9 = (int *)0x1;
  (**(code **)(*piVar2 + 0x1c))(piVar2,0x1db,0x19f,DAT_005148b4);
  if (DAT_00514eac == 1) {
    if (DAT_005148b8 == 0) {
      StopAllManagedSounds(DAT_0044ddd8);
      if (DAT_00514eb0 < 10) {
        iVar4 = 1;
        uVar8 = 0x32;
        uVar1 = FUN_0042ffc4();
        uVar1 = uVar1 & 0x80000001;
        if ((int)uVar1 < 0) {
          uVar1 = (uVar1 - 1 | 0xfffffffe) + 1;
        }
        PlayManagedSoundById(DAT_0044ddd8,uVar1 + 0x305,uVar8,iVar4);
      }
      iVar4 = 1;
      uVar8 = 0x32;
      if (DAT_00514eb0 < 0x14) {
        uVar1 = FUN_0042ffc4();
        PlayManagedSoundById(DAT_0044ddd8,(int)uVar1 % 3 + 0x307,uVar8,iVar4);
        iVar4 = DAT_00519934 * 400;
        *(undefined4 *)(&DAT_0051b5c0 + iVar4) = 1;
        iVar4 = *(int *)(&DAT_0051b5bc + iVar4);
      }
      else {
        uVar1 = FUN_0042ffc4();
        uVar1 = uVar1 & 0x80000001;
        if ((int)uVar1 < 0) {
          uVar1 = (uVar1 - 1 | 0xfffffffe) + 1;
        }
        PlayManagedSoundById(DAT_0044ddd8,uVar1 + 0x30a,uVar8,iVar4);
        iVar4 = DAT_00519934 * 400;
        *(undefined4 *)(&DAT_0051b5c0 + iVar4) = 1;
        iVar4 = *(int *)(&DAT_0051b5bc + iVar4);
      }
      if (iVar4 == 1) {
        DAT_00446f38 = 2;
      }
      DAT_005148b8 = -1;
    }
    if ((uint)DAT_00514b3c[2] <= (uint)DAT_00514b3c[3]) {
      (**(code **)(*piVar2 + 0x1c))(piVar2,0x14,0x14,DAT_00514e9c,0,0);
      iVar4 = AnyManagedSoundPlaying(DAT_0044ddd8);
      if (iVar4 != 0) {
        return;
      }
      DAT_00514eac = 2;
      return;
    }
    DecodeAndBlitBinkFrameNoAdvance(uVar10,DAT_00514b3c,DAT_00514e9c);
    _BinkNextFrame_4(DAT_00514b3c);
    iVar4 = _BinkWait_4(DAT_00514b3c);
    while (iVar4 != 0) {
      iVar4 = _BinkWait_4(DAT_00514b3c);
    }
    (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))
              (*(int **)(DAT_0044de08 + 0xc),0x14,0x14,DAT_00514e9c,0,0);
    return;
  }
  iVar4 = 0;
  piVar2 = &DAT_00514b6c;
  do {
    if ((int)puVar7 <= *piVar2) {
      iVar4 = iVar4 + -1;
      break;
    }
    iVar4 = iVar4 + 1;
    piVar2 = piVar2 + 4;
  } while (iVar4 < DAT_00446568);
  if ((-1 < iVar4) && (puVar7 == (undefined *)(&DAT_00514b40)[iVar4])) {
    do {
      uVar1 = FUN_0042ffc4();
    } while ((&DAT_00514634)[(int)uVar1 % 5 + (DAT_00514ea4 + iVar4 * 4) * 5] == -1);
    PlayManagedSoundById
              (DAT_0044ddd8,(&DAT_00514634)[(int)uVar1 % 5 + (DAT_00514ea4 + iVar4 * 4) * 5],0x32,1)
    ;
    DAT_00514eb0 = DAT_00514eb0 + DAT_00514ea4;
  }
  if ((-1 < DAT_0044656c) && (puVar7 == *(undefined **)(&DAT_00514558 + DAT_0044656c * 4))) {
    DAT_00514ea4 = 0;
    DAT_0044656c = -1;
  }
  if (((DAT_004fbe54 != 0) || ((DAT_004fbe5c & 0x10) != 0)) && (DAT_0044656c == -1)) {
    iVar4 = 0;
    piVar2 = &DAT_00514b6c;
    do {
      if (((int)puVar7 < piVar2[3]) && (*piVar2 <= (int)puVar7)) {
        if (iVar4 != -1) {
          iVar3 = 3;
          piVar2 = &DAT_00514b78 + iVar4 * 4;
          goto LAB_004249c8;
        }
        break;
      }
      iVar4 = iVar4 + 1;
      piVar2 = piVar2 + 4;
    } while (iVar4 < DAT_00446568);
  }
  goto LAB_004249dd;
  while( true ) {
    iVar3 = iVar3 + -1;
    piVar2 = piVar2 + -1;
    if (iVar3 < 0) break;
LAB_00424c3c:
    if (*piVar2 <= (int)puVar7) break;
  }
  goto LAB_00424c48;
  while( true ) {
    iVar3 = iVar3 + -1;
    piVar2 = piVar2 + -1;
    if (iVar3 < 0) break;
LAB_004249c8:
    DAT_0044656c = iVar4;
    if (*piVar2 <= (int)puVar7) {
      DAT_00514ea4 = iVar3 + 1;
      break;
    }
  }
LAB_004249dd:
  if (*(int *)(DAT_00514b2c + 0xc) == DAT_00446574) {
    DAT_00514ea8 = DAT_00514ea8 + 1;
    if (1 < DAT_00514ea8) {
      DAT_00514eac = 1;
      DecodeAndBlitBinkFrameNoAdvance(uVar10,DAT_00514b3c,DAT_00514e9c);
      _BinkNextFrame_4(DAT_00514b3c);
      iVar4 = _BinkWait_4(DAT_00514b3c);
      while (iVar4 != 0) {
        iVar4 = _BinkWait_4(DAT_00514b3c);
      }
      (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))
                (*(int **)(DAT_0044de08 + 0xc),0x14,0x14,DAT_00514e9c,0,0);
      return;
    }
    do {
      uVar1 = FUN_0042ffc4();
    } while (*(int *)(&DAT_00514864 + ((int)uVar1 % 5 + DAT_00514ea4 * 5) * 4) == -1);
    PlayManagedSoundById
              (DAT_0044ddd8,*(int *)(&DAT_00514864 + ((int)uVar1 % 5 + DAT_00514ea4 * 5) * 4),0x32,1
              );
    DAT_00514eb0 = DAT_00514eb0 + DAT_00514ea4;
  }
  iVar4 = 0;
  do {
    if (*(uint *)((&DAT_00514b2c)[iVar4] + 8) <= *(uint *)((&DAT_00514b2c)[iVar4] + 0xc)) {
      iVar4 = 0;
      puVar6 = &DAT_005148c0;
      puVar7 = puVar5;
      do {
        _BinkGoto_12((&DAT_00514b2c)[iVar4],1,0);
        SeekAndDecodeBinkToExactFrame((&DAT_00514b2c)[iVar4],DAT_00446570);
        DecodeAndBlitBinkFrameNoAdvance(uVar10,(int *)(&DAT_00514b2c)[iVar4],(&DAT_00514e8c)[iVar4])
        ;
        _BinkNextFrame_4((&DAT_00514b2c)[iVar4]);
        if (puVar6 == (undefined4 *)&DAT_00514a34) {
          (**(code **)(*piVar9 + 0x1c))(piVar9,0x14,0x14,DAT_00514e98,0,0);
          return;
        }
        puVar6 = puVar6 + 0x1f;
        iVar4 = iVar4 + 1;
        puVar5 = puVar7;
      } while ((int)puVar6 < 0x514ab0);
    }
    DecodeAndBlitBinkFrameNoAdvance(uVar10,(int *)(&DAT_00514b2c)[iVar4],(&DAT_00514e8c)[iVar4]);
    _BinkNextFrame_4((&DAT_00514b2c)[iVar4]);
    if (iVar4 == DAT_00514ea4) {
      iVar3 = _BinkWait_4((&DAT_00514b2c)[iVar4]);
      puVar7 = puVar5;
      while (iVar3 != 0) {
        iVar3 = _BinkWait_4((&DAT_00514b2c)[iVar4]);
      }
      (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))
                (*(int **)(DAT_0044de08 + 0xc),0x14,0x14,(&DAT_00514e8c)[iVar4],0,0);
      puVar5 = puVar7;
    }
    iVar4 = iVar4 + 1;
  } while (iVar4 < 4);
  iVar4 = 0;
  piVar2 = &DAT_00514b6c;
  do {
    if (((int)puVar7 < piVar2[3]) && (*piVar2 <= (int)puVar7)) {
      if (-1 < iVar4) {
        iVar3 = 3;
        piVar2 = &DAT_00514b78 + iVar4 * 4;
        goto LAB_00424c3c;
      }
      break;
    }
    iVar4 = iVar4 + 1;
    piVar2 = piVar2 + 4;
  } while (iVar4 < DAT_00446568);
LAB_00424c48:
  if (0 < DAT_00514ea4) {
    (**(code **)(*piVar9 + 0x1c))
              (piVar9,0x10b,0x19f,*(undefined4 *)(&DAT_00514b5c + DAT_00514ea4 * 4),0,1);
  }
  return;
}

