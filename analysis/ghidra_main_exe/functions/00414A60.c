/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00414a60; function: DrawGolfActivity; body bytes: 1566
 * callers: 1; callees: 3; success: True
 */


/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DrawGolfActivity(void)

{
  int *piVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  longlong lVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  int *piStack_78;
  int *piStack_74;
  int iStack_70;
  undefined *puStack_6c;
  undefined *apuStack_68 [2];
  int aiStack_58 [5];
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  aiStack_58[4] = 1;
  aiStack_58[3] = 0;
  aiStack_58[2] = DAT_0050aedc;
  aiStack_58[1] = 0;
  aiStack_58[0] = 0;
  (**(code **)(*piVar1 + 0x1c))();
  iVar5 = DAT_0050af04 % 10;
  if (0 < DAT_0050af04 / 10) {
    apuStack_68[1] = &DAT_004437e8 + (DAT_0050af04 / 10) * 0x10;
    apuStack_68[0] = DAT_0050adec;
    puStack_6c = (undefined *)0x1aa;
    iStack_70 = 0x1fe;
    piStack_78 = (int *)0x414acd;
    piStack_74 = piVar1;
    (**(code **)(*piVar1 + 0x1c))();
  }
  apuStack_68[0] = &DAT_004437e8 + iVar5 * 0x10;
  apuStack_68[1] = (undefined *)0x1;
  puStack_6c = DAT_0050adec;
  iStack_70 = 0x1aa;
  piStack_74 = (int *)0x217;
  piStack_78 = piVar1;
  (**(code **)(*piVar1 + 0x1c))();
  iVar5 = 0;
  iVar6 = 0x175;
  if (0 < DAT_0050abb0) {
    do {
      (**(code **)(*piVar1 + 0x1c))(piVar1,iVar6,0x1b5,DAT_0050aee8,0);
      iVar6 = iVar6 + 0x14;
      iVar5 = iVar5 + 1;
    } while (iVar5 < DAT_0050abb0);
  }
  VectorFromDegreesAndMagnitude
            ((float *)(apuStack_68 + 1),(float *)apuStack_68,DAT_0050ada8 + -2,0x8c);
  if ((DAT_0050ad9c == 0) && (0 < DAT_0050abb0)) {
    iVar5 = *piVar1;
    uVar9 = 0;
    uVar8 = DAT_0050aef8;
    lVar7 = __ftol();
    uVar2 = (undefined4)lVar7;
    lVar7 = __ftol();
    (**(code **)(iVar5 + 0x1c))(piVar1,(int)lVar7,uVar2,uVar8,uVar9);
  }
  if ((_DAT_0043b39c < _DAT_0050ac0c) && (0 < DAT_0050abb0)) {
    iVar5 = *piVar1;
    uVar9 = 0;
    uVar8 = DAT_0050aef0;
    lVar7 = __ftol();
    uVar2 = (undefined4)lVar7;
    lVar7 = __ftol();
    (**(code **)(iVar5 + 0x1c))(piVar1,(int)lVar7,uVar2,uVar8,uVar9);
    if (0 < DAT_0050abb0) {
      iVar5 = *piVar1;
      uVar9 = 0;
      uVar8 = DAT_0050aee4;
      lVar7 = __ftol();
      uVar2 = (undefined4)lVar7;
      lVar7 = __ftol();
      (**(code **)(iVar5 + 0x1c))(piVar1,(int)lVar7,uVar2,uVar8,uVar9);
    }
  }
  aiStack_58[0] = DAT_00443618 + DAT_00443618 * DAT_0050ad90;
  lVar7 = __ftol();
  aiStack_58[1] = DAT_0044361c + (int)lVar7 * DAT_0044361c;
  (**(code **)(*piVar1 + 0x1c))(piVar1,DAT_0050ada0,DAT_0050ada4,DAT_0050aee0,&stack0xffffffa0);
  if (DAT_004437c8 < 0) {
    iVar5 = 0;
    do {
      iVar6 = *(int *)((int)&DAT_0050abd0 + iVar5) + -1;
      *(int *)((int)&DAT_0050abd0 + iVar5) = iVar6;
      if (iVar6 < 1) {
        iVar6 = *(int *)((int)&DAT_0050ad84 + iVar5) + 1;
        *(undefined4 *)((int)&DAT_0050abd0 + iVar5) = 5;
        *(int *)((int)&DAT_0050ad84 + iVar5) = iVar6;
        if (*(int *)((int)&DAT_004437cc + iVar5) <= iVar6) {
          *(undefined4 *)((int)&DAT_0050ad84 + iVar5) = 0;
        }
      }
      iVar5 = iVar5 + 4;
    } while (iVar5 < 8);
  }
  else {
    iVar5 = 0;
    apuStack_68[0] = (undefined *)0x15;
    apuStack_68[1] = (undefined *)0x15;
    aiStack_58[0] = 0x2d;
    aiStack_58[1] = 0x29;
    aiStack_58[2] = 0x5a;
    aiStack_58[3] = 0x58;
    iVar6 = 0;
    do {
      iVar4 = *(int *)((int)&DAT_0050abd0 + iVar6) + -1;
      *(int *)((int)&DAT_0050abd0 + iVar6) = iVar4;
      if (iVar4 < 1) {
        iVar4 = *(int *)((int)&DAT_0050ad84 + iVar6) + 1;
        *(undefined4 *)((int)&DAT_0050abd0 + iVar6) = 5;
        *(int *)((int)&DAT_0050ad84 + iVar6) = iVar4;
        if (iVar4 == *(int *)((int)&DAT_004437cc + iVar6)) {
          if (*(int *)((int)&DAT_0050ac1c + iVar6) == -1) {
            uVar3 = FUN_0042ffc4();
            uVar3 = uVar3 & 0x80000001;
            if ((int)uVar3 < 0) {
              uVar3 = (uVar3 - 1 | 0xfffffffe) + 1;
            }
            *(uint *)((int)&DAT_0050ac1c + iVar6) = uVar3;
            *(undefined **)((int)&DAT_0050ad84 + iVar6) = apuStack_68[iVar5 + uVar3];
          }
          else {
            *(undefined4 *)((int)&DAT_0050ad84 + iVar6) = 0;
          }
        }
        if ((-1 < *(int *)((int)&DAT_0050ac1c + iVar6)) &&
           (aiStack_58[iVar5 + *(int *)((int)&DAT_0050ac1c + iVar6)] <=
            *(int *)((int)&DAT_0050ad84 + iVar6))) {
          *(undefined4 *)((int)&DAT_0050ad84 + iVar6) = 0;
          *(undefined4 *)((int)&DAT_0050ac1c + iVar6) = 0xfffffffe;
        }
      }
      iVar5 = iVar5 + 2;
      iVar6 = iVar6 + 4;
    } while (iVar5 < 4);
  }
  iVar5 = 0;
  do {
    if (iVar5 == 0) {
      piStack_78 = (int *)(DAT_0050ad84 * DAT_0050ad70 + 1);
      iStack_70 = DAT_0050ad70 + -1 + (int)piStack_78;
      puStack_6c = (undefined *)DAT_0050ae08;
    }
    else {
      piStack_78 = (int *)((&DAT_0050ad84)[iVar5] * (&DAT_0050ad70)[iVar5]);
      puStack_6c = (undefined *)(&DAT_0050ae08)[iVar5];
      iStack_70 = (&DAT_0050ad70)[iVar5] + (int)piStack_78;
    }
    piStack_74 = (int *)0x0;
    iVar6 = (**(code **)(*piVar1 + 0x1c))
                      (piVar1,(&DAT_0050ab98)[iVar5],(&DAT_0050aba4)[iVar5],(&DAT_0050adfc)[iVar5],
                       &piStack_78,1);
    if (iVar6 == -0x7789fe3e) {
      (**(code **)(*(int *)(&DAT_0050adfc)[iVar5] + 0x6c))((int *)(&DAT_0050adfc)[iVar5]);
      (**(code **)(*piVar1 + 0x1c))
                (piVar1,(&DAT_0050ab98)[iVar5],(&DAT_0050aba4)[iVar5],(&DAT_0050adfc)[iVar5],
                 &stack0xffffff84,1);
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 2);
  iVar5 = 0;
  do {
    if (iVar5 == DAT_004437c8) {
      piStack_74 = (int *)0x0;
      if (iVar5 == 0) {
        puStack_6c = (undefined *)0xa5;
        piStack_78 = (int *)(DAT_0050ae18 * 0xc6);
        iStack_70 = (int)piStack_78 + 0xc6;
        DAT_0050aed0 = DAT_0050aed0 + 1;
        if (3 < DAT_0050aed0) {
          DAT_0050ae18 = DAT_0050ae18 + 1;
          DAT_0050aed0 = 0;
          if (0x12 < DAT_0050ae18) {
            DAT_0050ae18 = 0;
          }
        }
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,DAT_0050abb4 + -0x23,DAT_0050abb8 + 2,DAT_0050addc,&piStack_78,1);
      }
      else {
        puStack_6c = (undefined *)(&DAT_0050abe8)[iVar5];
        piStack_78 = (int *)((&DAT_0050abdc)[iVar5] * (&DAT_0050ae18)[iVar5]);
        iStack_70 = (&DAT_0050abdc)[iVar5] + (int)piStack_78;
        iVar6 = (&DAT_0050aed0)[iVar5];
        (&DAT_0050aed0)[iVar5] = iVar6 + 1;
        if (3 < iVar6 + 1) {
          iVar6 = (&DAT_0050ae18)[iVar5] + 1;
          (&DAT_0050aed0)[iVar5] = 0;
          (&DAT_0050ae18)[iVar5] = iVar6;
          if ((&DAT_0050ac24)[iVar5] <= iVar6) {
            (&DAT_0050ae18)[iVar5] = 0;
          }
        }
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,(&DAT_0050abb4)[iVar5 * 2],(&DAT_0050abb8)[iVar5 * 2],
                   (&DAT_0050ab88)[iVar5],&piStack_78,1);
      }
    }
    else {
      if ((iVar5 == 0) && (DAT_0050abf8 = DAT_0050abf8 + 1, 3 < DAT_0050abf8)) {
        DAT_0050ae24 = DAT_0050ae24 + 1;
        DAT_0050abf8 = 0;
        if (DAT_0050ac24 <= DAT_0050ae24) {
          DAT_0050ae24 = 0;
        }
      }
      puStack_6c = (undefined *)(&DAT_0050abe8)[iVar5];
      piStack_78 = (int *)((&DAT_0050ae24)[iVar5] * (&DAT_0050abdc)[iVar5]);
      iStack_70 = (&DAT_0050abdc)[iVar5] + (int)piStack_78;
      piStack_74 = (int *)0x0;
      (**(code **)(*piVar1 + 0x1c))
                (piVar1,(&DAT_0050abb4)[iVar5 * 2],(&DAT_0050abb8)[iVar5 * 2],(&DAT_0050ab88)[iVar5]
                 ,&piStack_78,1);
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 3);
  if (DAT_0050ad9c != 0) {
    piStack_78 = (int *)0x0;
    iStack_70 = (DAT_0050abf4 * 0x101) / 1000;
    piStack_74 = (int *)0x0;
    puStack_6c = (undefined *)0x35;
    (**(code **)(*piVar1 + 0x1c))(piVar1,0x53,0x1a0,DAT_0050aef4,&piStack_78,1);
  }
  return;
}

