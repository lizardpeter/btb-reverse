/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004267b0; function: DrawAndUpdateSquirrelActivityScene; body bytes: 1918
 * callers: 1; callees: 5; success: True
 */


void DrawAndUpdateSquirrelActivityScene(void)

{
  bool bVar1;
  int iVar2;
  undefined3 extraout_var;
  uint uVar3;
  int iVar4;
  int *unaff_EBP;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  int *piVar9;
  int iStack_34;
  int *local_20;
  int local_10 [3];
  undefined4 local_4;
  
  local_20 = *(int **)(DAT_0044de08 + 0xc);
  iVar5 = 0;
  if (DAT_005150d8 != 0) {
    iVar5 = (int)((ulonglong)((longlong)DAT_005150f0 * -0x66666667) >> 0x20);
    iVar5 = (iVar5 >> 1) - (iVar5 >> 0x1f);
  }
  local_10[1] = 0;
  local_10[0] = (DAT_004467d4 - DAT_004467bc) * DAT_005150d0;
  local_10[2] = local_10[0] + 0x26c;
  local_4 = 0x1e0;
  if (DAT_005150d8 == 0) {
    iStack_34 = 0;
    (**(code **)(*local_20 + 0x1c))(local_20,0,0,DAT_0051509c,local_10);
  }
  iStack_34 = 1;
  local_10[1] = 0;
  local_4 = 0x2c;
  local_10[0] = DAT_005150f4 * 0x280;
  local_10[2] = local_10[0] + 0x280;
  piVar9 = local_10;
  (**(code **)(*local_20 + 0x1c))(local_20,0,0x1a1,DAT_005150c0);
  if (DAT_005150d8 != 0) {
    DAT_00514f58 = DAT_00514f58 + 1;
    if (DAT_00514f58 < 3) {
LAB_0042696f:
      if (0 < DAT_00514ed8) goto LAB_0042697d;
    }
    else {
      DAT_00514f58 = 0;
      if (DAT_00514ed8 == 0) {
        iVar2 = DAT_0044699c + -0x15;
        DAT_005150dc = 0x15;
        DAT_00515000 = 0;
        iVar4 = DAT_0044699c + 7;
        iVar6 = iVar4 >> 0x1f;
        DAT_0044699c = iVar2;
        if (iVar4 / 0x15 + iVar6 == iVar6) goto LAB_00426a36;
        DAT_00514ed8 = 1;
        DAT_005150dc = 0x15;
LAB_004268eb:
        DAT_005150dc = DAT_005150dc + 1;
        if (DAT_005150dc < 0x29) goto LAB_0042697d;
        DAT_005150dc = 0x15;
        DAT_00514ed8 = 2;
LAB_00426917:
        DAT_005150dc = DAT_005150dc + 1;
        if (DAT_005150dc < 0x2a) goto LAB_0042697d;
        DAT_00515000 = DAT_00515000 + 1;
        if (0 < DAT_00515000) {
          DAT_00514ed8 = 3;
          goto LAB_00426957;
        }
        DAT_005150dc = 0x15;
LAB_004269b3:
        unaff_EBP = (int *)((DAT_005150dc + -0x15) * 0x1cf);
        local_20 = (int *)((int)unaff_EBP + 0x1cf);
      }
      else {
        if (DAT_00514ed8 == 1) goto LAB_004268eb;
        if (DAT_00514ed8 == 2) goto LAB_00426917;
        if (DAT_00514ed8 != 3) goto LAB_0042696f;
LAB_00426957:
        DAT_005150dc = DAT_005150dc + 1;
        if (0x3b < DAT_005150dc) {
          DAT_00514ed8 = 4;
        }
LAB_0042697d:
        if (DAT_005150dc < 0x15) {
          unaff_EBP = (int *)(DAT_005150dc * 0x1cf);
          local_20 = (int *)((int)unaff_EBP + 0x1cf);
        }
        else {
          if (DAT_005150dc < 0x29) goto LAB_004269b3;
          unaff_EBP = (int *)((DAT_005150dc + -0x29) * 0x1cf);
          local_20 = (int *)((int)unaff_EBP + 0x1cf);
        }
      }
      BlitColorKeyedSurfaceClipped(DAT_005150c4,0x6e,0,(int *)&stack0xffffffd8);
    }
LAB_00426a36:
    if ((DAT_005150d8 != 0) && (DAT_00514ed8 != 0)) goto LAB_00426beb;
  }
  BlitColorKeyedSurfaceClipped(DAT_005150b4,DAT_00446a24 + iVar5,DAT_00446a28,(int *)0x0);
  BlitColorKeyedSurfaceClipped
            (DAT_005150ac,DAT_00446a2c + iVar5,DAT_00446a30,(int *)&stack0xffffffd8);
  if (DAT_00514fec != -1) {
    bVar1 = IsSoundIdPlaying(DAT_0044ddd8,DAT_00514fec);
    if (CONCAT31(extraout_var,bVar1) == 0) {
      DAT_00514fec = -1;
    }
    DAT_00514f60 = DAT_00514f60 + -1;
    if (DAT_00514f60 < 1) {
      uVar3 = FUN_0042ffc4();
      iVar4 = DAT_00514f08;
      DAT_00514f60 = (int)uVar3 % 10 + 10;
      do {
        uVar3 = FUN_0042ffc4();
        DAT_00514f08 = (int)uVar3 % 3;
      } while (DAT_00514f08 == iVar4);
    }
  }
  BlitColorKeyedSurfaceClipped
            (DAT_005150b0,DAT_00446a34 + iVar5,DAT_00446a38,(int *)&stack0xffffffd8);
  unaff_EBP = (int *)(((DAT_0044699c + 0x1c) / 0x15) * 0xdb);
  local_20 = (int *)((int)unaff_EBP + 0xdb);
  BlitColorKeyedSurfaceClipped(DAT_005150b8,DAT_00446a3c,DAT_00446a40,(int *)&stack0xffffffd8);
LAB_00426beb:
  iVar4 = *(int *)(&DAT_00515078 + DAT_005150d0 * 4);
  if ((DAT_005150e4 == 6) || (DAT_005150e4 == 7)) {
    iVar4 = iVar4 + 1;
  }
  iVar6 = 0;
  if (0 < iVar4) {
    piVar8 = &DAT_00446784;
    do {
      BuildSquirrelFrameRect((int *)&stack0xffffffd8,(&DAT_00514fbc)[iVar6 + DAT_005150d0 * 4]);
      BlitColorKeyedSurfaceClipped(DAT_005150a0,*piVar8 + iVar5,piVar8[1],(int *)&stack0xffffffd8);
      iVar6 = iVar6 + 1;
      piVar8 = piVar8 + 2;
    } while (iVar6 < iVar4);
  }
  iVar4 = 0;
  iStack_34 = 0x12;
  piVar8 = &DAT_004467bc;
  do {
    BuildSquirrelFrameRect
              ((int *)&stack0xffffffd8,(&iStack_34)[(&DAT_00514f0c)[iVar4 + DAT_005150d0 * 4]]);
    BlitColorKeyedSurfaceClipped(DAT_005150a0,*piVar8 + iVar5,piVar8[1],(int *)&stack0xffffffd8);
    piVar8 = piVar8 + 2;
    iVar4 = iVar4 + 1;
  } while ((int)piVar8 < 0x4467dc);
  if (DAT_005150d0 == DAT_0051c284) {
    iStack_34 = 0x50;
    BlitColorKeyedSurfaceClipped
              (DAT_005150c8,DAT_004467d4,
               (&iStack_34)[(&DAT_00514f18)[DAT_005150d0 * 4]] + DAT_004467d8,(int *)0x0);
  }
  if (DAT_005150d8 != 0) {
    iVar4 = 1;
    iStack_34 = 0x12;
    piVar8 = &DAT_004467c4;
    do {
      BuildSquirrelFrameRect
                ((int *)&stack0xffffffd8,
                 (&iStack_34)[*(int *)(&DAT_00514f1c + (iVar4 + DAT_005150d0 * 4) * 4)]);
      BlitColorKeyedSurfaceClipped
                (DAT_005150a0,(*piVar8 - DAT_004467bc) + DAT_004467d4 + iVar5,piVar8[1],
                 (int *)&stack0xffffffd8);
      piVar8 = piVar8 + 2;
      iVar4 = iVar4 + 1;
    } while ((int)piVar8 < 0x4467dc);
  }
  UpdateAndDrawSquirrelRunAssembly();
  if ((DAT_005150d8 == 0) || (DAT_00514ed8 == 0)) {
    unaff_EBP = (int *)0x0;
    local_20 = (int *)0x2f;
    (**(code **)(*piVar9 + 0x1c))
              (piVar9,DAT_0044699c + 0x1c + iVar5,0,DAT_005150a8,&stack0xffffffd8,1);
  }
  iVar5 = 0;
  if (0 < DAT_00514f50) {
    piVar7 = &DAT_0044679c;
    piVar8 = &DAT_00515088;
    do {
      if (*piVar8 != -1) {
        BuildSquirrelFrameRect((int *)&stack0xffffffd8,*piVar8);
        if (iVar5 == DAT_004469a4) {
          (**(code **)(*piVar9 + 0x1c))
                    (piVar9,DAT_0044699c,DAT_004469a0 + -0x85,DAT_005150a4,&stack0xffffffd8,1);
        }
        else {
          iVar6 = DAT_005150ec + *piVar7;
          iVar4 = iVar6 + 0x59;
          if ((0 < iVar4) && (iVar6 < 0)) {
            unaff_EBP = (int *)((int)unaff_EBP - iVar6);
            iVar6 = 0;
          }
          if (0x27f < iVar4) {
            local_20 = (int *)((int)local_20 + (0x280 - iVar4));
          }
          if (local_20 != unaff_EBP && -1 < (int)local_20 - (int)unaff_EBP) {
            (**(code **)(*piVar9 + 0x1c))(piVar9,iVar6,piVar7[1],DAT_005150a4,&stack0xffffffd8,1);
          }
        }
      }
      iVar5 = iVar5 + 1;
      piVar8 = piVar8 + 1;
      piVar7 = piVar7 + 2;
    } while (iVar5 < DAT_00514f50);
  }
  (**(code **)(*piVar9 + 0x1c))(piVar9,0,0,DAT_00515068,0,1);
  (**(code **)(*piVar9 + 0x1c))(piVar9,0,0x14,DAT_0051506c,0,1);
  (**(code **)(*piVar9 + 0x1c))(piVar9,0x26c,0x14,DAT_00515070,0,1);
  (**(code **)(*piVar9 + 0x1c))(piVar9,0x14,400,DAT_00515074,0,1);
  return;
}

