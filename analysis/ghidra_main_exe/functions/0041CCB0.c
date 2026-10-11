/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041ccb0; function: UpdateMazeSpudNPC; body bytes: 2042
 * callers: 1; callees: 7; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdateMazeSpudNPC(void)

{
  void *this;
  int *piVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int *extraout_EDX;
  int iVar5;
  float10 fVar6;
  longlong lVar7;
  undefined4 uVar8;
  int iVar9;
  int local_20 [8];
  
  if ((DAT_00444220 == DAT_00510d18) || ((int)DAT_005109cc < 2)) {
    switch(DAT_005109cc) {
    case (int *)0x0:
      if (*(int *)(&DAT_004441f8 + DAT_0051c284 * 4) == -1) {
        return;
      }
      DAT_005107e8 = DAT_005107e8 + -1;
      if (DAT_005107e8 < 1) {
        DAT_005109cc = (int *)0x1;
      }
      break;
    case (int *)0x1:
      DAT_00444220 = DAT_00510d18;
      DAT_00512154 = 0;
      if (DAT_00510d18 == 1) {
        uVar2 = FUN_0042ffc4();
        uVar2 = uVar2 & 0x80000001;
        if ((int)uVar2 < 0) {
          uVar2 = (uVar2 - 1 | 0xfffffffe) + 1;
        }
        DAT_005107a0 = (int *)(-(uint)(uVar2 != 1) & 0xf);
      }
      else {
        DAT_005107a0 = (int *)0x0;
      }
      iVar5 = 0;
      DAT_005107a4 = 1;
      DAT_005107a8 = 0;
      DAT_005107ac = 0;
      _DAT_005107b4 = 7;
      iVar9 = (int)DAT_005107a0 + DAT_00510d18 * 0x1e;
      DAT_00510790 = (float)(int)(&DAT_00510e18)[iVar9 * 8];
      DAT_00510794 = (float)(int)(&DAT_00510e1c)[iVar9 * 8];
      DAT_005107b0 = *(int *)(&DAT_00444204 + DAT_0051c284 * 4) / 2;
      piVar1 = &DAT_00510a1c + DAT_00510d18 * 0x14;
      iVar9 = 4;
      do {
        if ((-1 < piVar1[-1]) && (*piVar1 == 1)) {
          iVar5 = iVar5 + 1;
        }
        piVar1 = piVar1 + 5;
        iVar9 = iVar9 + -1;
      } while (iVar9 != 0);
      if (iVar5 == 0) {
        DAT_005107e8 = *(int *)(&DAT_004441f8 + DAT_0051c284 * 4);
        DAT_005109cc = (int *)0x0;
      }
      else {
        PlayManagedSoundById(DAT_0044ddd8,0x58,10,1);
        uVar2 = FUN_0042ffc4();
        iVar3 = 0;
        iVar9 = 0;
        piVar1 = &DAT_00510a1c + DAT_00510d18 * 0x14;
        do {
          if ((-1 < piVar1[-1]) && (*piVar1 == 1)) {
            DAT_00510e10 = iVar9;
            if (iVar3 == (int)uVar2 % iVar5) break;
            iVar3 = iVar3 + 1;
          }
          iVar9 = iVar9 + 1;
          piVar1 = piVar1 + 5;
          DAT_00510e10 = (int)uVar2 % iVar5;
        } while (iVar9 < 4);
        FindMazeShortestPath
                  (*(undefined4 *)
                    (&DAT_00510d28 +
                    ((&DAT_00510a18)[(DAT_00510d18 * 4 + DAT_00510e10) * 5] + DAT_00510d18 * 4) *
                    0xc),DAT_005107a0,DAT_005107a0,
                   *(undefined4 *)
                    (&DAT_00510d28 +
                    ((&DAT_00510a18)[(DAT_00510d18 * 4 + DAT_00510e10) * 5] + DAT_00510d18 * 4) *
                    0xc));
        DAT_005107a4 = 0;
        DAT_005107c4 = (int *)0x0;
        DAT_005109cc = (int *)0x2;
        piVar1 = &DAT_00510e28 + (DAT_00510988 + DAT_00510d18 * 0x1e) * 8;
        do {
          if (*piVar1 == DAT_0051098c) break;
          DAT_005107a4 = DAT_005107a4 + 1;
          piVar1 = piVar1 + 1;
        } while (DAT_005107a4 < 4);
      }
      break;
    case (int *)0x2:
      MoveMazeActorTowardNode
                ((&DAT_0051098c)[(int)DAT_005107c4],&DAT_00510790,
                 *(int *)(&DAT_00444204 + DAT_0051c284 * 4));
      lVar7 = DistanceFromActorToCurrentScreenNode((&DAT_0051098c)[(int)DAT_005107c4]);
      this = DAT_0044ddd8;
      if ((int)lVar7 < 3) {
        DAT_005107c4 = (int *)((int)DAT_005107c4 + 1);
        DAT_005107a4 = 0;
        DAT_005107a0 = (int *)(&DAT_00510988)[(int)DAT_005107c4];
        piVar1 = &DAT_00510e28 + ((int)DAT_005107a0 + DAT_00510d18 * 0x1e) * 8;
        do {
          if (*piVar1 == (&DAT_0051098c)[(int)DAT_005107c4]) break;
          DAT_005107a4 = DAT_005107a4 + 1;
          piVar1 = piVar1 + 1;
        } while (DAT_005107a4 < 4);
        if (DAT_00510d0c <= (int)DAT_005107c4) {
          iVar9 = DAT_00510e10 + DAT_00510d18 * 4;
          if ((&DAT_00510a1c)[iVar9 * 5] == 1) {
            DAT_005109cc = (int *)0x4;
            (&DAT_00510a1c)[iVar9 * 5] = 2;
            PlayManagedSoundById(this,0x59,10,1);
            DAT_005107ac = DAT_005107ac + 7;
          }
          else {
            DAT_005109cc = (int *)0x6;
          }
        }
      }
      break;
    case (int *)0x3:
      DAT_005109cc = (int *)((int)DAT_005109cc + 1);
      DAT_00512154 = 0;
      break;
    case (int *)0x4:
      iVar9 = 0;
      DAT_00512154 = 0;
      local_20[0] = 0;
      local_20[1] = 1;
      local_20[2] = 2;
      local_20[3] = 3;
      local_20[4] = 0;
      local_20[5] = 1;
      local_20[6] = 2;
      local_20[7] = 3;
      uVar2 = FUN_0042ffc4();
      uVar2 = uVar2 & 0x80000003;
      if ((int)uVar2 < 0) {
        uVar2 = (uVar2 - 1 | 0xfffffffc) + 1;
      }
      iVar5 = DAT_005107a4 + 2;
      if (3 < DAT_005107a4 + 2) {
        iVar5 = DAT_005107a4 + -2;
      }
      DAT_005107a4 = iVar5;
      piVar1 = local_20 + uVar2;
      do {
        if (DAT_005107a4 != *piVar1) {
          iVar5 = (&DAT_00510e28)[*piVar1 + ((int)DAT_005107a0 + DAT_00510d18 * 0x1e) * 8];
          if (iVar5 != DAT_005109e0) {
            iVar3 = DAT_00510d18 * 0x1e + DAT_005109e0;
            if ((((iVar5 != (&DAT_00510e28)[iVar3 * 8]) &&
                 (iVar5 != *(int *)(&DAT_00510e2c + iVar3 * 0x20))) &&
                (iVar5 != (&DAT_00510e30)[iVar3 * 8])) &&
               ((iVar5 != *(int *)(&DAT_00510e34 + iVar3 * 0x20) && (iVar5 != -1)))) break;
          }
        }
        iVar9 = iVar9 + 1;
        piVar1 = piVar1 + 1;
      } while (iVar9 < 4);
      if (iVar9 == 4) {
        iVar9 = 0;
        piVar1 = local_20 + uVar2;
        iVar5 = (DAT_00510d18 * 0x1e + (int)DAT_005107a0) * 8;
        do {
          iVar3 = (&DAT_00510e28)[iVar5 + *piVar1];
          if (iVar3 != DAT_005109e0) {
            iVar4 = DAT_005109e0 + DAT_00510d18 * 0x1e;
            if (((iVar3 != (&DAT_00510e28)[iVar4 * 8]) &&
                (iVar3 != *(int *)(&DAT_00510e2c + iVar4 * 0x20))) &&
               ((iVar3 != (&DAT_00510e30)[iVar4 * 8] &&
                ((iVar3 != *(int *)(&DAT_00510e34 + iVar4 * 0x20) && (iVar3 != -1)))))) break;
          }
          iVar9 = iVar9 + 1;
          piVar1 = piVar1 + 1;
        } while (iVar9 < 4);
        if (iVar9 == 4) {
          iVar9 = 0;
          piVar1 = local_20 + uVar2;
          do {
            if ((&DAT_00510e28)[iVar5 + *piVar1] != -1) break;
            iVar9 = iVar9 + 1;
            piVar1 = piVar1 + 1;
          } while (iVar9 < 4);
        }
      }
      DAT_00444238 = 999999;
      DAT_005109cc = (int *)0x5;
      DAT_005107a4 = local_20[uVar2 + iVar9];
      DAT_005107a8 = 1;
      DAT_005107c4 = (int *)(&DAT_00510e28)
                            [DAT_005107a4 + ((int)DAT_005107a0 + DAT_00510d18 * 0x1e) * 8];
      DAT_005107b0 = *(int *)(&DAT_00444210 + DAT_0051c284 * 4) / 2;
      break;
    case (int *)0x5:
      MoveMazeActorTowardNode((int)DAT_005107c4,&DAT_00510790,(DAT_0051c284 == 2) + 1);
      lVar7 = DistanceFromActorToCurrentScreenNode((int)DAT_005107c4);
      iVar9 = (int)lVar7;
      if (iVar9 < DAT_00444238) {
        DAT_00444238 = iVar9;
      }
      if (iVar9 < 3) {
        DAT_005107a0 = DAT_005107c4;
        DAT_005109cc = (int *)((-(uint)(DAT_00512154 != 0) & 2) + 4);
        uVar2 = FUN_0042ffc4();
        if ((int)uVar2 % 0x1e == 0) {
          iVar9 = 1;
          uVar8 = 10;
          uVar2 = FUN_0042ffc4();
          uVar2 = uVar2 & 0x80000003;
          if ((int)uVar2 < 0) {
            uVar2 = (uVar2 - 1 | 0xfffffffc) + 1;
          }
          PlayManagedSoundById(DAT_0044ddd8,uVar2 + 0x5a,uVar8,iVar9);
        }
      }
      lVar7 = __ftol();
      iVar9 = (int)lVar7;
      lVar7 = __ftol();
      iVar5 = (int)lVar7;
      lVar7 = __ftol();
      iVar3 = (int)lVar7;
      lVar7 = __ftol();
      fVar6 = DistanceBetweenIntegerPoints((int)lVar7,iVar3,iVar5,iVar9);
      if ((fVar6 < (float10)_DAT_0043b49c) && (DAT_00512154 == 0)) {
        iVar9 = 1;
        uVar8 = 10;
        DAT_00512154 = 1;
        DAT_005107a8 = 0;
        uVar2 = FUN_0042ffc4();
        uVar2 = uVar2 & 0x80000001;
        if ((int)uVar2 < 0) {
          uVar2 = (uVar2 - 1 | 0xfffffffe) + 1;
        }
        PlayManagedSoundById(DAT_0044ddd8,uVar2 + 0x5e,uVar8,iVar9);
        iVar9 = DAT_00510e10 + DAT_00510d18 * 4;
        iVar5 = iVar9 * 0x14;
        (&DAT_00510a1c)[iVar9 * 5] = 3;
        lVar7 = __ftol();
        *(int *)(&DAT_00510a24 + iVar5) = (int)lVar7;
        lVar7 = __ftol();
        *(int *)(&DAT_00510a28 + iVar5) = (int)lVar7;
      }
      break;
    case (int *)0x6:
      DAT_00512154 = 0;
      if (DAT_00510d18 == 1) {
        uVar2 = FUN_0042ffc4();
        uVar2 = uVar2 & 0x80000001;
        if ((int)uVar2 < 0) {
          uVar2 = (uVar2 - 1 | 0xfffffffe) + 1;
        }
        uVar2 = -(uint)(uVar2 != 1) & 0xf;
        piVar1 = extraout_EDX;
      }
      else {
        uVar2 = 0;
        piVar1 = DAT_005109cc;
      }
      FindMazeShortestPath(DAT_005107a0,piVar1,DAT_005107a0,uVar2);
      DAT_005107a4 = 0;
      DAT_005107c4 = (int *)0x0;
      piVar1 = &DAT_00510e28 + (DAT_00510988 + DAT_00510d18 * 0x1e) * 8;
      do {
        if (*piVar1 == DAT_0051098c) break;
        DAT_005107a4 = DAT_005107a4 + 1;
        piVar1 = piVar1 + 1;
      } while (DAT_005107a4 < 4);
      DAT_005109cc = (int *)0x7;
      DAT_005107a8 = 0;
      DAT_005107b0 = *(int *)(&DAT_00444204 + DAT_0051c284 * 4) / 2;
      break;
    case (int *)0x7:
      MoveMazeActorTowardNode
                ((&DAT_0051098c)[(int)DAT_005107c4],&DAT_00510790,
                 *(int *)(&DAT_00444204 + DAT_0051c284 * 4));
      lVar7 = DistanceFromActorToCurrentScreenNode((&DAT_0051098c)[(int)DAT_005107c4]);
      if ((int)lVar7 < 3) {
        DAT_005107c4 = (int *)((int)DAT_005107c4 + 1);
        DAT_005107a4 = 0;
        DAT_005107a0 = (int *)(&DAT_00510988)[(int)DAT_005107c4];
        piVar1 = &DAT_00510e28 + ((int)DAT_005107a0 + DAT_00510d18 * 0x1e) * 8;
        do {
          if (*piVar1 == (&DAT_0051098c)[(int)DAT_005107c4]) break;
          DAT_005107a4 = DAT_005107a4 + 1;
          piVar1 = piVar1 + 1;
        } while (DAT_005107a4 < 4);
        if (DAT_00510d0c <= (int)DAT_005107c4) {
          DAT_005109cc = (int *)0x0;
          DAT_005107e8 = 500;
        }
      }
    }
    _DAT_00510798 = DAT_00510790;
    _DAT_0051079c = DAT_00510794;
  }
  else {
    DAT_005120e8 = DAT_005120e8 + -1;
    if (DAT_005120e8 < 0) {
      DAT_005109cc = (int *)0x1;
      (&DAT_00510a1c)[(DAT_00510e10 + DAT_00444220 * 4) * 5] = 1;
      return;
    }
  }
  return;
}

