/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004227f0; function: DrawAndUpdateSpudMazeScene; body bytes: 1486
 * callers: 1; callees: 8; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DrawAndUpdateSpudMazeScene(void)

{
  int *piVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  int *piVar10;
  longlong lVar11;
  undefined8 uVar12;
  WORD *pWVar13;
  int aiStack_40 [3];
  undefined4 uStack_34;
  int aiStack_30 [2];
  _SYSTEMTIME _Stack_28;
  int iStack_18;
  undefined4 uStack_14;
  
  iVar7 = DAT_00510cf0;
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  if (DAT_00512128 == 0) {
    (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,(&DAT_005144e4)[DAT_00510d18],0,0);
  }
  else {
    iVar6 = 0;
    if (0 < (int)(&DAT_004459cc)[DAT_00510cf0]) {
      iVar9 = DAT_00510cf0 * 0x2c;
      piVar5 = (int *)(&DAT_00513f40 + DAT_00510cf0 * 0x58);
      do {
        if (0 < *(int *)((int)&DAT_00514110 + iVar9)) {
          if (*(int *)((int)&DAT_00514110 + iVar9) < 0x1e) {
            BlitColorKeyedSurfaceClipped
                      (*(int **)((int)&DAT_005141ec + iVar9),*piVar5 + DAT_005144dc,piVar5[1],
                       (int *)0x0);
          }
          iVar4 = *(int *)((int)&DAT_00514110 + iVar9) + 1;
          *(int *)((int)&DAT_00514110 + iVar9) = iVar4;
          if (10 < iVar4) {
            *(undefined4 *)((int)&DAT_00514110 + iVar9) = 1;
          }
        }
        iVar6 = iVar6 + 1;
        piVar5 = piVar5 + 2;
        iVar9 = iVar9 + 4;
      } while (iVar6 < (int)(&DAT_004459cc)[iVar7]);
    }
  }
  iVar6 = 0;
  piVar5 = &DAT_004459cc + DAT_00510d18;
  iVar7 = DAT_00510d18;
  if (0 < (int)(&DAT_004459cc)[DAT_00510d18]) {
    piVar10 = (int *)(&DAT_00513f40 + DAT_00510d18 * 0x58);
    iVar9 = DAT_00510d18 * 0x2c;
    do {
      if (0 < *(int *)((int)&DAT_00514110 + iVar9)) {
        if (*(int *)((int)&DAT_00514110 + iVar9) < 0x1e) {
          BlitColorKeyedSurfaceClipped
                    (*(int **)((int)&DAT_005141ec + iVar9),DAT_005144d8 + *piVar10,piVar10[1],
                     (int *)0x0);
          iVar7 = DAT_00510d18;
        }
        iVar4 = *(int *)((int)&DAT_00514110 + iVar9) + 1;
        *(int *)((int)&DAT_00514110 + iVar9) = iVar4;
        if (10 < iVar4) {
          *(undefined4 *)((int)&DAT_00514110 + iVar9) = 1;
        }
      }
      iVar6 = iVar6 + 1;
      piVar10 = piVar10 + 2;
      iVar9 = iVar9 + 4;
    } while (iVar6 < *piVar5);
  }
  DAT_005107ec = 10;
  if ((1 < DAT_005109cc) && (DAT_00513f3c == iVar7)) {
    if ((DAT_005107a4 == 4) || ((DAT_005107a4 != 0 && (DAT_005107a4 != 1)))) {
      _Stack_28.wSecond = 0x80;
      _Stack_28.wMilliseconds = 0;
      uStack_14 = 0x100;
    }
    else {
      _Stack_28.wSecond = 0;
      _Stack_28.wMilliseconds = 0;
      uStack_14 = 0x80;
    }
    _Stack_28._8_4_ = DAT_005107ac * 0x9c;
    iStack_18 = _Stack_28._8_4_ + 0x9c;
    _DAT_00510798 = DAT_00510790;
    _DAT_0051079c = DAT_00510794;
    UpdatePilchardAnimation();
    pWVar13 = &_Stack_28.wHour;
    lVar11 = __ftol();
    iVar7 = (int)lVar11;
    lVar11 = __ftol();
    BlitColorKeyedSurfaceClipped(DAT_005144fc,(int)lVar11,iVar7,(int *)pWVar13);
  }
  uVar3 = DAT_00510a00;
  if (DAT_00510a00 == 4) {
    uVar3 = 3;
  }
  uVar2 = -(uint)(DAT_00445ef4 != 0) & 0x1e;
  if ((DAT_00514510 != 0) || (uVar3 < 4)) {
    switch(uVar3) {
    default:
      aiStack_40[1] = 0xa6;
      uStack_34 = 0xf9;
      aiStack_40[0] = DAT_005109ec;
      break;
    case 1:
      aiStack_40[1] = 0;
      uStack_34 = 0x53;
      aiStack_40[0] = uVar2 + DAT_005109ec;
      break;
    case 3:
      aiStack_40[1] = 0x53;
      uStack_34 = 0xa6;
      aiStack_40[0] = uVar2 + DAT_005109ec;
    }
    aiStack_40[0] = aiStack_40[0] * 0x53;
    aiStack_40[2] = aiStack_40[0] + 0x53;
  }
  piVar5 = aiStack_40;
  lVar11 = __ftol();
  iVar7 = (int)lVar11;
  lVar11 = __ftol();
  BlitColorKeyedSurfaceClipped(DAT_005144f8,(int)lVar11,iVar7,piVar5);
  UpdateSpudMazeBobAnimation();
  if ((DAT_00445ef4 == 0) && (DAT_00514490 == DAT_00510d18)) {
    if (DAT_00514520 != 0) {
      if (DAT_00514524 < 100) {
        DAT_00514524 = DAT_00514524 + 2;
      }
      DAT_005144c0 = DAT_005144c0 + DAT_00514524 / 10;
      if (0x171 < DAT_005144c0) {
        DAT_00514520 = 0;
      }
    }
    BlitColorKeyedSurfaceClipped
              (DAT_00514494,DAT_005144bc + DAT_005144d8,DAT_005144c0 + -0x1e,(int *)0x0);
    if (0x22b < DAT_005144bc) {
      DAT_005144bc = 0x22b;
    }
    if (0x172 < DAT_005144c0) {
      DAT_005144c0 = 0x172;
    }
    lVar11 = __ftol();
    uVar3 = (int)(uint)lVar11 >> 0x1f;
    if (((int)(((uint)lVar11 ^ uVar3) - uVar3) < 0x14) && (_DAT_0043b4c8 < DAT_005109d4)) {
      DAT_00445ef4 = 1;
    }
  }
  MaybePlayRandomSpudVoice();
  if (DAT_00514528 == 1) {
    aiStack_30[0] = DAT_00445f10 * 0x4c;
    _Stack_28._0_4_ = aiStack_30[0] + 0x4c;
    aiStack_30[1] = 0;
    _Stack_28.wDayOfWeek = 0x50;
    _Stack_28.wDay = 0;
    BlitColorKeyedSurfaceClipped(DAT_005144b8,DAT_00445f04,DAT_00445f08,aiStack_30);
  }
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_005144c4,0,1);
  GetSystemTime(&_Stack_28);
  SystemTimeToFileTime(&_Stack_28,(LPFILETIME)&DAT_005120f0);
  uVar12 = Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
  DAT_005109c8 = (DAT_00510950 + DAT_00510d08) - (int)uVar12;
  if (1 < DAT_005120fc - DAT_005109c8) {
    DAT_00510d08 = DAT_00510d08 + (DAT_005120fc - DAT_005109c8);
    uVar12 = Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
    DAT_005109c8 = (DAT_00510d08 + DAT_00510950) - (int)uVar12;
  }
  aiStack_40[1] = 0x19;
  DAT_005120fc = DAT_005109c8;
  lVar11 = __ftol();
  aiStack_40[0] = (int)lVar11;
  piVar5 = (int *)&stack0xffffffb8;
  (**(code **)(*piVar1 + 0x1c))(piVar1,0x1a6,0x1ae,DAT_005144d0,piVar5,1);
  iVar6 = 0;
  iVar9 = 0;
  iVar7 = 0;
  if (0 < DAT_00513f38) {
    do {
      if (0xe6 < iVar6) {
        iVar6 = 0;
        iVar9 = 0x14;
      }
      uVar8 = DAT_00514508;
      if (DAT_00513f38 - DAT_005144c8 <= iVar7) {
        uVar8 = DAT_00514504;
      }
      (**(code **)(*piVar5 + 0x1c))
                (piVar5,(DAT_00513f38 / 2) * -0x14 + 0xf4 + iVar6,iVar9 + 0x1ad,uVar8,0,1);
      iVar6 = iVar6 + 0x14;
      iVar7 = iVar7 + 1;
    } while (iVar7 < DAT_00513f38);
  }
  return;
}

