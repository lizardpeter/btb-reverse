/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00423630; function: UpdateSpudMazePilchard; body bytes: 1412
 * callers: 1; callees: 6; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall UpdateSpudMazePilchard(undefined4 param_1,int *param_2)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  longlong lVar6;
  int local_20 [8];
  
  if ((DAT_00513f3c != DAT_00510d18) && (1 < DAT_005109cc)) {
    DAT_005120e8 = DAT_005120e8 + -1;
    if (DAT_005120e8 < 0) {
      DAT_005109cc = 1;
    }
    DAT_0051454c = 0;
    return;
  }
  switch(DAT_005109cc) {
  case 0:
    DAT_0051454c = 0;
    DAT_005107e8 = DAT_005107e8 + -1;
    if (DAT_005107e8 < 1) {
      DAT_005109cc = 1;
    }
    break;
  case 1:
    DAT_00513f3c = DAT_00510d18;
    DAT_00514530 = 0;
    DAT_0051454c = 0;
    if (DAT_00510d18 == 4) {
      DAT_005107a0 = 2;
    }
    else if (DAT_00510d18 == 0) {
      DAT_005107a0 = 0;
    }
    else {
      uVar1 = FUN_0042ffc4();
      DAT_005107a0 = uVar1 & 0x80000003;
      if ((int)DAT_005107a0 < 0) {
        DAT_005107a0 = (DAT_005107a0 - 1 | 0xfffffffc) + 1;
      }
    }
    DAT_005107a4 = 1;
    DAT_005107a8 = 0;
    DAT_005107ac = 0;
    _DAT_005107b4 = 7;
    iVar3 = DAT_005107a0 + DAT_00510d18 * 0x1e;
    DAT_00510790 = (float)(int)(&DAT_00510e18)[iVar3 * 8];
    DAT_00510794 = (float)(int)(&DAT_00510e1c)[iVar3 * 8];
    DAT_005107b0 = *(int *)(&DAT_00444204 + DAT_0051c284 * 4) / 2;
    BuildShortestSpudMazeGraphPath
              (DAT_005107a0,(int *)(*(int *)(&DAT_00444204 + DAT_0051c284 * 4) >> 0x1f),DAT_005107a0
               ,DAT_005109e0);
    DAT_005107a4 = 0;
    DAT_005107c4 = 0;
    DAT_005109cc = 2;
    piVar2 = &DAT_00510e28 + (DAT_00510988 + DAT_00510d18 * 0x1e) * 8;
    do {
      if (*piVar2 == DAT_0051098c) break;
      DAT_005107a4 = DAT_005107a4 + 1;
      piVar2 = piVar2 + 1;
    } while (DAT_005107a4 < 4);
    iVar3 = 0;
    uVar4 = 0x32;
    uVar1 = FUN_0042ffc4();
    uVar1 = uVar1 & 0x80000001;
    if ((int)uVar1 < 0) {
      uVar1 = (uVar1 - 1 | 0xfffffffe) + 1;
    }
    PlayManagedSoundById(DAT_0044ddd8,uVar1 + 0x2ce,uVar4,iVar3);
    break;
  case 2:
    iVar3 = HandlePilchardCollisionAndHammerDrop();
    if (iVar3 != 0) {
      DAT_0051454c = 1;
      return;
    }
    MovePilchardTowardGraphNode
              ((&DAT_0051098c)[DAT_005107c4],&DAT_00510790,
               *(int *)(&DAT_00444204 + DAT_0051c284 * 4));
    lVar6 = DistanceFromActorToCurrentScreenNode((&DAT_0051098c)[DAT_005107c4]);
    if ((int)lVar6 < 3) {
      DAT_005107c4 = DAT_005107c4 + 1;
      DAT_005107a4 = 0;
      DAT_005107a0 = (&DAT_00510988)[DAT_005107c4];
      piVar2 = &DAT_00510e28 + (DAT_005107a0 + DAT_00510d18 * 0x1e) * 8;
      do {
        if (*piVar2 == (&DAT_0051098c)[DAT_005107c4]) break;
        DAT_005107a4 = DAT_005107a4 + 1;
        piVar2 = piVar2 + 1;
      } while (DAT_005107a4 < 4);
      if (DAT_00510d0c <= (int)DAT_005107c4) {
        DAT_005109cc = 4;
      }
    }
    break;
  case 3:
    DAT_005109cc = DAT_005109cc + 1;
    DAT_0051454c = 0;
    break;
  case 4:
    DAT_0051454c = 0;
    local_20[0] = 0;
    local_20[1] = 1;
    local_20[2] = 2;
    local_20[3] = 3;
    local_20[4] = 0;
    local_20[5] = 1;
    local_20[6] = 2;
    local_20[7] = 3;
    uVar1 = FUN_0042ffc4();
    uVar1 = uVar1 & 0x80000003;
    if ((int)uVar1 < 0) {
      uVar1 = (uVar1 - 1 | 0xfffffffc) + 1;
    }
    iVar3 = DAT_005107a4 + 2;
    if (3 < iVar3) {
      iVar3 = DAT_005107a4 + -2;
    }
    iVar5 = 0;
    piVar2 = local_20 + uVar1;
    do {
      if ((iVar3 != *piVar2) &&
         ((&DAT_00510e28)[*piVar2 + (DAT_005107a0 + DAT_00510d18 * 0x1e) * 8] != -1)) break;
      iVar5 = iVar5 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar5 < 4);
    if (iVar5 == 4) {
      iVar5 = 0;
      piVar2 = local_20 + uVar1;
      do {
        if ((&DAT_00510e28)[*piVar2 + (DAT_005107a0 + DAT_00510d18 * 0x1e) * 8] != -1) break;
        iVar5 = iVar5 + 1;
        piVar2 = piVar2 + 1;
      } while (iVar5 < 4);
    }
    DAT_00445f1c = 999999;
    DAT_005109cc = 5;
    DAT_005107a4 = local_20[uVar1 + iVar5];
    DAT_005107a8 = 1;
    DAT_005107c4 = (&DAT_00510e28)[DAT_005107a4 + (DAT_005107a0 + DAT_00510d18 * 0x1e) * 8];
    DAT_005107b0 = *(int *)(&DAT_00444210 + DAT_0051c284 * 4) / 2;
    break;
  case 5:
    MovePilchardTowardGraphNode
              (DAT_005107c4,&DAT_00510790,*(int *)(&DAT_00444210 + DAT_0051c284 * 4));
    lVar6 = DistanceFromActorToCurrentScreenNode(DAT_005107c4);
    iVar3 = (int)lVar6;
    if (iVar3 < DAT_00445f1c) {
      DAT_00445f1c = iVar3;
    }
    if (iVar3 < 3) {
      DAT_005107a0 = DAT_005107c4;
      DAT_005109cc = (-(uint)(DAT_0051454c != 0) & 2) + 4;
    }
    iVar3 = HandlePilchardCollisionAndHammerDrop();
    if (iVar3 != 0) {
      DAT_0051454c = 1;
      return;
    }
    break;
  case 6:
    DAT_0051454c = 0;
    if (DAT_00510d18 == 4) {
      uVar4 = 2;
    }
    else {
      uVar4 = 0;
    }
    BuildShortestSpudMazeGraphPath(DAT_005109cc,param_2,DAT_005107a0,uVar4);
    DAT_005107a4 = 0;
    DAT_005107c4 = 0;
    piVar2 = &DAT_00510e28 + (DAT_00510988 + DAT_00510d18 * 0x1e) * 8;
    do {
      if (*piVar2 == DAT_0051098c) break;
      DAT_005107a4 = DAT_005107a4 + 1;
      piVar2 = piVar2 + 1;
    } while (DAT_005107a4 < 4);
    DAT_005109cc = 7;
    DAT_005107a8 = 0;
    DAT_005107b0 = *(int *)(&DAT_00444204 + DAT_0051c284 * 4) / 2;
    break;
  case 7:
    MovePilchardTowardGraphNode
              ((&DAT_0051098c)[DAT_005107c4],&DAT_00510790,
               *(int *)(&DAT_00444204 + DAT_0051c284 * 4));
    lVar6 = DistanceFromActorToCurrentScreenNode((&DAT_0051098c)[DAT_005107c4]);
    if ((int)lVar6 < 3) {
      DAT_005107c4 = DAT_005107c4 + 1;
      DAT_005107a4 = 0;
      DAT_005107a0 = (&DAT_00510988)[DAT_005107c4];
      piVar2 = &DAT_00510e28 + (DAT_005107a0 + DAT_00510d18 * 0x1e) * 8;
      do {
        if (*piVar2 == (&DAT_0051098c)[DAT_005107c4]) break;
        DAT_005107a4 = DAT_005107a4 + 1;
        piVar2 = piVar2 + 1;
      } while (DAT_005107a4 < 4);
      if (DAT_00510d0c <= (int)DAT_005107c4) {
        DAT_005109cc = 0;
        DAT_005107e8 = 500;
      }
      HandlePilchardCollisionAndHammerDrop();
    }
  }
  _DAT_00510798 = DAT_00510790;
  _DAT_0051079c = DAT_00510794;
  return;
}

