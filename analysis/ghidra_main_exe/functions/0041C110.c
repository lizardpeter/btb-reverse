/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041c110; function: DrawMazeActivity; body bytes: 1536
 * callers: 1; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DrawMazeActivity(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  longlong lVar8;
  undefined4 uVar9;
  int *piVar10;
  int iStack_20;
  int iStack_1c;
  int iStack_18;
  int iStack_14;
  int iStack_10;
  int iStack_c;
  int iStack_8;
  int iStack_4;
  
  piVar10 = *(int **)(DAT_0044de08 + 0xc);
  if (DAT_00512128 == 0) {
    (**(code **)(*piVar10 + 0x1c))(piVar10,0,0,(&DAT_00512100)[DAT_00510d18],0,0);
  }
  iVar3 = DAT_00510d18;
  iVar6 = 4;
  piVar7 = &DAT_00510a18 + DAT_00510d18 * 0x14;
  do {
    if ((*piVar7 == -1) || (piVar7[1] != 1)) {
      if (piVar7[1] == 3) {
        iVar1 = piVar7[4];
        iVar5 = *piVar10;
        iVar2 = piVar7[3];
        goto LAB_0041c1c2;
      }
    }
    else {
      iVar2 = (*piVar7 + iVar3 * 4) * 0xc;
      iVar5 = *piVar10;
      iVar1 = *(int *)(&DAT_00510d24 + iVar2);
      iVar2 = *(int *)(&DAT_00510d20 + iVar2);
LAB_0041c1c2:
      (**(code **)(iVar5 + 0x1c))(piVar10,iVar2 + -8 + DAT_005144d8,iVar1 + -8,DAT_00512114,0,1);
    }
    iVar5 = DAT_00510cf0;
    piVar7 = piVar7 + 5;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  if (DAT_00512128 == 0) {
    DAT_005107ec = 10;
    if ((_DAT_005109d8 == DAT_005109d0) && (_DAT_005109dc == DAT_005109d4)) {
      if ((DAT_005109e8 != 0) && (DAT_005109f0 = DAT_005109f0 + -1, DAT_005109f0 < 1)) {
        DAT_005109ec = DAT_005109ec + 1;
        if (7 < DAT_005109ec) {
          iVar3 = 1;
          DAT_005109e8 = 0;
          DAT_005109ec = 0;
          uVar9 = 10;
          if (DAT_00510cf8 < 6) {
            iVar6 = DAT_00510cf8 + 0x4d;
          }
          else {
            uVar4 = FUN_0042ffc4();
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            iVar6 = uVar4 + 0x51;
          }
          PlayManagedSoundById(DAT_0044ddd8,iVar6,uVar9,iVar3);
          DAT_00510cf8 = DAT_00510cf8 + 1;
        }
        DAT_005109f0 = 0x14;
      }
    }
    else if (DAT_005109e8 == 0) {
      DAT_005109f0 = DAT_005109f0 + -1;
      if (DAT_005109f0 < 1) {
        DAT_005109ec = DAT_005109ec + 1;
        if (DAT_005109f4 < DAT_005109ec) {
          DAT_005109ec = 0;
        }
        DAT_005109f0 = 10;
      }
    }
    else {
      DAT_005109f0 = DAT_005109f0 + -1;
      if (DAT_005109f0 < 1) {
        DAT_005109ec = DAT_005109ec + 1;
        if (7 < DAT_005109ec) {
          iVar3 = 1;
          DAT_005109e8 = 0;
          DAT_005109ec = 0;
          uVar9 = 10;
          if (DAT_00510cf8 < 6) {
            iVar6 = DAT_00510cf8 + 0x4d;
          }
          else {
            uVar4 = FUN_0042ffc4();
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            iVar6 = uVar4 + 0x51;
          }
          PlayManagedSoundById(DAT_0044ddd8,iVar6,uVar9,iVar3);
        }
        DAT_00510cf8 = DAT_00510cf8 + 1;
        DAT_005109f0 = DAT_005107ec;
      }
    }
    iVar3 = DAT_00510a00;
    if (DAT_00510a00 == 4) {
      iVar3 = 3;
    }
    iStack_c = iVar3 << 7;
    iStack_4 = (iVar3 + 1) * 0x80;
    iStack_10 = DAT_005109ec << 7;
    iStack_8 = (DAT_005109ec + 1) * 0x80;
    piVar10 = &iStack_10;
    lVar8 = __ftol();
    iVar3 = (int)lVar8;
    lVar8 = __ftol();
    BlitColorKeyedSurfaceClipped(DAT_0051210c,(int)lVar8,iVar3,piVar10);
    if ((1 < DAT_005109cc) && (DAT_00444220 == DAT_00510d18)) {
      iVar3 = DAT_005107a4;
      if (DAT_005107a4 == 4) {
        iVar3 = 3;
      }
      iStack_1c = iVar3 << 7;
      iStack_14 = (iVar3 + 1) * 0x80;
      iStack_20 = DAT_005107ac << 7;
      iVar3 = DAT_005107ac + 1;
      iStack_18 = iVar3 * 0x80;
      _DAT_00510798 = DAT_00510790;
      _DAT_0051079c = DAT_00510794;
      if ((0 < DAT_00510ba8) && (-1 < DAT_005109c8)) {
        if (DAT_005107a8 == 1) {
          DAT_005107b0 = DAT_005107b0 + -1;
          if (DAT_005107b0 < 1) {
            DAT_005107ac = iVar3;
            if (0xd < iVar3) {
              DAT_005107ac = 7;
            }
            DAT_005107b0 = DAT_005107ec;
          }
        }
        else {
          DAT_005107b0 = DAT_005107b0 + -1;
          if (DAT_005107b0 < 1) {
            DAT_005107ac = iVar3;
            if (6 < iVar3) {
              DAT_005107ac = 0;
            }
            DAT_005107b0 = DAT_005107ec;
          }
        }
      }
      piVar10 = &iStack_20;
      lVar8 = __ftol();
      iVar3 = (int)lVar8;
      lVar8 = __ftol();
      BlitColorKeyedSurfaceClipped(DAT_00512110,(int)lVar8,iVar3,piVar10);
    }
    iVar3 = DrawMazeTimer();
    _DAT_00510944 = 0x1a2;
    _DAT_005107f4 = 0x1a2;
    _DAT_0051094c = 0x1d3;
    _DAT_005107fc = 0x1d3;
    _DAT_005107f0 = (-(uint)(iVar3 != 0) & 0x7f) - 1;
    _DAT_00510940 = iVar3 * 0x14 + 0x7e;
    _DAT_005107f8 = iVar3 * 0x14 + _DAT_005107f0;
    _DAT_00510948 = _DAT_00510940 + DAT_00510ba8 * 0x14;
    return;
  }
  DAT_005144d8 = DAT_005144dc;
  if ((1 < DAT_005109cc) && (DAT_00444220 == DAT_00510cf0)) {
    iVar3 = DAT_005107a4;
    if (DAT_005107a4 == 4) {
      iVar3 = 3;
    }
    iStack_1c = iVar3 << 7;
    iStack_14 = (iVar3 + 1) * 0x80;
    iStack_20 = DAT_005107ac << 7;
    iVar3 = DAT_005107ac + 1;
    iStack_18 = iVar3 * 0x80;
    _DAT_00510798 = DAT_00510790;
    _DAT_0051079c = DAT_00510794;
    if ((0 < DAT_00510ba8) && (-1 < DAT_005109c8)) {
      if (DAT_005107a8 == 1) {
        DAT_005107b0 = DAT_005107b0 + -1;
        if (DAT_005107b0 < 1) {
          DAT_005107ac = iVar3;
          if (0xd < iVar3) {
            DAT_005107ac = 7;
          }
          DAT_005107b0 = DAT_005107ec;
        }
      }
      else {
        DAT_005107b0 = DAT_005107b0 + -1;
        if (DAT_005107b0 < 1) {
          DAT_005107ac = iVar3;
          if (6 < iVar3) {
            DAT_005107ac = 0;
          }
          DAT_005107b0 = DAT_005107ec;
        }
      }
    }
    piVar7 = &iStack_20;
    lVar8 = __ftol();
    iVar3 = (int)lVar8;
    lVar8 = __ftol();
    BlitColorKeyedSurfaceClipped(DAT_00512110,(int)lVar8,iVar3,piVar7);
  }
  iVar3 = 4;
  piVar7 = &DAT_00510a18 + iVar5 * 0x14;
  do {
    if ((*piVar7 == -1) || (piVar7[1] != 1)) {
      if (piVar7[1] == 3) {
        iVar1 = piVar7[4];
        iVar6 = *piVar10;
        iVar2 = piVar7[3];
        goto LAB_0041c382;
      }
    }
    else {
      iVar2 = (*piVar7 + iVar5 * 4) * 0xc;
      iVar6 = *piVar10;
      iVar1 = *(int *)(&DAT_00510d24 + iVar2);
      iVar2 = *(int *)(&DAT_00510d20 + iVar2);
LAB_0041c382:
      (**(code **)(iVar6 + 0x1c))(piVar10,iVar2 + -8 + DAT_005144d8,iVar1 + -8,DAT_00512114,0,1);
    }
    piVar7 = piVar7 + 5;
    iVar3 = iVar3 + -1;
    if (iVar3 == 0) {
      return;
    }
  } while( true );
}

