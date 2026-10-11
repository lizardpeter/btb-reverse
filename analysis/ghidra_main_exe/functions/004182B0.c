/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004182b0; function: UpdateHerdingActivity; body bytes: 4317
 * callers: 1; callees: 15; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 UpdateHerdingActivity(void)

{
  int iVar1;
  float fVar2;
  uint uVar3;
  uint uVar4;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined3 extraout_var_01;
  undefined4 *puVar5;
  int *piVar6;
  undefined3 extraout_var_02;
  int iVar7;
  int *piVar8;
  int iVar9;
  int iVar10;
  bool bVar11;
  float10 fVar12;
  float10 fVar13;
  longlong lVar14;
  longlong lVar15;
  undefined4 uVar16;
  uint local_4c;
  int local_40;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18 [6];
  
  EnableInputProcessing();
  if ((DAT_0044ddb0 != 0) || (DAT_0051c2e8 != 0)) {
    UnloadHerdingActivityResources();
    if (DAT_0051c2e8 != 0) {
      DAT_0051c2e8 = 0;
      DAT_0044de14 = 4;
    }
    if (DAT_00446f38 != -1) {
      DAT_0044de14 = 0x40;
    }
    return 1;
  }
  DAT_0050b3c0 = DAT_0050b3b8;
  DAT_0050b3bc = DAT_0050b3b4;
  iVar10 = (DAT_0050b3b4 - (DAT_0050b3dc - DAT_0050b3e4) / 2) - DAT_0051071c;
  iVar9 = ((DAT_0050b3e8 - DAT_0050b3e0) / 2 - DAT_00510720) + DAT_0050b3b8;
  DAT_0050b3d0 = 0;
  local_4c = 0;
  if (DAT_00443a9c == 1) {
    if (_DAT_004fbe5c != 0) {
      DAT_00443a9c = 0;
    }
    fVar12 = DistanceBetweenIntegerPoints(DAT_004fbd24,DAT_004fbd30,iVar10,iVar9);
    if (fVar12 <= (float10)_DAT_0043b448) {
      DAT_0050b3d0 = 0;
    }
    else {
      DAT_0050b3f4 = 0x41700000;
      lVar14 = AngleBetweenIntegerPointsDegrees(iVar10,iVar9,DAT_004fbd24,DAT_004fbd30);
      DAT_0050b3b0 = ((int)lVar14 + 0x16) / 0x2d;
      if (7 < (int)DAT_0050b3b0) {
        DAT_0050b3b0 = DAT_0050b3b0 - 8;
      }
      fVar12 = (float10)(int)lVar14 * (float10)_DAT_0043b384;
      if (DAT_0051078c != 0) {
        fVar13 = (float10)fsin(fVar12);
        DAT_0050b3c8 = (float)(fVar13 * (float10)_DAT_0043b444 + (float10)DAT_0050b3c8);
        fVar12 = (float10)fcos(fVar12);
        DAT_0050b3cc = (float)((float10)DAT_0050b3cc - fVar12 * (float10)_DAT_0043b444);
        lVar14 = __ftol();
        DAT_0050b3b4 = (int)lVar14;
        lVar14 = __ftol();
        DAT_0050b3b8 = (int)lVar14;
      }
      DAT_0050b3d0 = DAT_0050b3d0 | 2;
      DAT_0051078c = 1;
    }
  }
  else {
    if ((DAT_004fbd24 != DAT_0050afc8) || (DAT_004fbd30 != DAT_0050afcc)) {
      DAT_00443a9c = 1;
    }
    DAT_0050afc8 = DAT_004fbd24;
    DAT_0050afcc = DAT_004fbd30;
    if ((_DAT_004fbe5c & 2) == 0) {
      if ((_DAT_004fbe5c & 1) != 0) {
        DAT_0050b3d0 = 8;
        if (DAT_0051078c != 0) {
          DAT_0050b3c8 = DAT_0050b3c8 - _DAT_0043b444;
        }
        goto LAB_004184f4;
      }
    }
    else {
      DAT_0050b3d0 = 2;
      if (DAT_0051078c != 0) {
        DAT_0050b3c8 = DAT_0050b3c8 + _DAT_0043b444;
      }
LAB_004184f4:
      local_4c = 1;
    }
    if ((_DAT_004fbe5c & 8) == 0) {
      if ((_DAT_004fbe5c & 4) != 0) {
        DAT_0050b3d0 = DAT_0050b3d0 | 1;
        if (DAT_0051078c != 0) {
          DAT_0050b3cc = DAT_0050b3cc - _DAT_0043b444;
        }
        goto LAB_00418545;
      }
    }
    else {
      DAT_0050b3d0 = DAT_0050b3d0 | 4;
      if (DAT_0051078c != 0) {
        DAT_0050b3cc = DAT_0050b3cc + _DAT_0043b444;
      }
LAB_00418545:
      local_4c = local_4c + 1;
    }
    DAT_0051078c = 1;
  }
  lVar14 = __ftol();
  local_2c = (int)lVar14;
  local_34 = local_2c + -0x14;
  lVar14 = __ftol();
  lVar15 = __ftol();
  local_38 = (int)lVar14 + 0x12;
  local_30 = (int)lVar15 + -0x12;
  iVar9 = 0;
  local_18[0] = 0x1e;
  local_18[1] = 100;
  local_18[2] = 0;
  local_18[3] = 0;
  local_18[4] = 0;
  local_18[5] = 0;
  do {
    if ((&DAT_00443aa4)[iVar9] != 2) {
      iVar10 = (&DAT_00443ab0)[iVar9 * 2];
      iVar7 = (&DAT_00443ab4)[iVar9 * 2];
      local_40 = 0x28;
      if (iVar9 == 2) {
        local_40 = 0x32;
        iVar10 = 0xf0;
        iVar7 = 0x19e;
      }
      fVar12 = DistanceBetweenIntegerPoints
                         (local_18[iVar9 * 2] + DAT_0050b3b4,local_18[iVar9 * 2 + 1] + DAT_0050b3b8,
                          iVar10,iVar7);
      if ((fVar12 < (float10)local_40) && (DAT_00510718 != iVar9)) {
        uVar3 = FUN_0042ffc4();
        uVar3 = uVar3 & 0x80000001;
        bVar11 = uVar3 == 0;
        if ((int)uVar3 < 0) {
          bVar11 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff;
        }
        if (bVar11) {
          iVar10 = iVar9 + 0x261;
        }
        else {
          iVar10 = iVar9 + 0x246;
        }
        PlayManagedSoundById(DAT_0044ddd8,iVar10,0x5a,1);
        if (DAT_00510718 != -1) {
          (&DAT_00443aa4)[DAT_00510718] = 1;
          piVar8 = &DAT_0050af14;
          do {
            if (*piVar8 != -1) {
              iVar10 = *piVar8 * 100;
              *(undefined4 *)(&DAT_0050b400 + iVar10) = 200;
              uVar3 = FUN_0042ffc4();
              uVar4 = FUN_0042ffc4();
              *(int *)(&DAT_0050b404 + iVar10) = (int)uVar3 % 400 + 0x11e;
              *piVar8 = -1;
              *(int *)(&DAT_0050b408 + iVar10) = (int)uVar4 % 400 + 0x1c2;
            }
            piVar8 = piVar8 + 1;
          } while ((int)piVar8 < 0x50af64);
        }
        DAT_00510718 = iVar9;
        (&DAT_00443aa4)[iVar9] = 0xffffffff;
      }
    }
    iVar9 = iVar9 + 1;
  } while (iVar9 < 3);
  iVar9 = 1;
  if (1 < DAT_00510760) {
    piVar8 = &DAT_0050b41c;
    do {
      if ((3 < piVar8[8]) && (piVar8[8] < 7)) {
        local_24 = *piVar8;
        local_28 = piVar8[-1];
        local_1c = (piVar8[0xc] - piVar8[10]) + local_24;
        local_20 = (piVar8[0xb] - piVar8[9]) + local_28;
        iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
        if (iVar10 != 0) {
          DAT_0050b3c8 = (float)DAT_0050b3bc;
          DAT_0050b3cc = (float)DAT_0050b3c0;
        }
      }
      if (piVar8[8] == 0xd) {
        local_28 = piVar8[-1];
        local_1c = (piVar8[0xc] - piVar8[10]) + *piVar8;
        local_24 = *piVar8 + 0x4b;
        local_20 = (piVar8[0xb] - piVar8[9]) + -0x46 + local_28;
        iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
        fVar2 = DAT_0050b3c8;
        if (iVar10 != 0) {
          DAT_0050b3c8 = (float)DAT_0050b3bc;
          lVar14 = __ftol();
          local_2c = (int)lVar14;
          local_34 = local_2c + -0x14;
          lVar14 = __ftol();
          lVar15 = __ftol();
          local_38 = (int)lVar14 + 0x12;
          local_30 = (int)lVar15 + -0x12;
          iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
          if (iVar10 != 0) {
            DAT_0050b3cc = (float)DAT_0050b3c0;
            DAT_0050b3c8 = fVar2;
            lVar14 = __ftol();
            local_2c = (int)lVar14;
            local_34 = local_2c + -0x14;
            lVar14 = __ftol();
            lVar15 = __ftol();
            local_30 = (int)lVar15 + -0x12;
            local_38 = (int)lVar14 + 0x12;
            iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
            if (iVar10 != 0) {
              DAT_0050b3c8 = (float)DAT_0050b3bc;
            }
          }
        }
      }
      if (piVar8[8] == 0xc) {
        local_1c = (piVar8[0xc] - piVar8[10]) + -0x14 + *piVar8;
        local_24 = *piVar8 + 0x4b;
        local_28 = piVar8[-1] + -0x28;
        local_20 = (piVar8[0xb] - piVar8[9]) + -0x14 + piVar8[-1];
        iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
        fVar2 = DAT_0050b3c8;
        if (iVar10 != 0) {
          DAT_0050b3c8 = (float)DAT_0050b3bc;
          lVar14 = __ftol();
          local_2c = (int)lVar14;
          local_34 = local_2c + -0x14;
          lVar14 = __ftol();
          lVar15 = __ftol();
          local_38 = (int)lVar14 + 0x12;
          local_30 = (int)lVar15 + -0x12;
          iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
          if (iVar10 != 0) {
            DAT_0050b3cc = (float)DAT_0050b3c0;
            DAT_0050b3c8 = fVar2;
            lVar14 = __ftol();
            local_2c = (int)lVar14;
            local_34 = local_2c + -0x14;
            lVar14 = __ftol();
            lVar15 = __ftol();
            local_30 = (int)lVar15 + -0x12;
            local_38 = (int)lVar14 + 0x12;
            iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
            if (iVar10 != 0) {
              DAT_0050b3c8 = (float)DAT_0050b3bc;
            }
          }
        }
      }
      if (piVar8[8] == 0xe) {
        local_24 = *piVar8 + 0x1e;
        local_1c = (piVar8[0xc] - piVar8[10]) + local_24;
        local_28 = piVar8[-1] + -0x1e;
        local_20 = (piVar8[0xb] - piVar8[9]) + 10 + local_28;
        iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
        fVar2 = DAT_0050b3c8;
        if (iVar10 != 0) {
          DAT_0050b3c8 = (float)DAT_0050b3bc;
          lVar14 = __ftol();
          local_2c = (int)lVar14;
          local_34 = local_2c + -0x14;
          lVar14 = __ftol();
          lVar15 = __ftol();
          local_30 = (int)lVar15 + -0x12;
          local_38 = (int)lVar14 + 0x12;
          iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
          if (iVar10 != 0) {
            DAT_0050b3cc = (float)DAT_0050b3c0;
            DAT_0050b3c8 = fVar2;
            lVar14 = __ftol();
            local_2c = (int)lVar14;
            local_34 = local_2c + -0x14;
            lVar14 = __ftol();
            lVar15 = __ftol();
            local_38 = (int)lVar14 + 0x12;
            local_30 = (int)lVar15 + -0x12;
            iVar10 = AnyRectCornerInsideRect(&local_38,&local_28);
            if (iVar10 != 0) {
              DAT_0050b3c8 = (float)DAT_0050b3bc;
            }
          }
        }
      }
      iVar9 = iVar9 + 1;
      piVar8 = piVar8 + 0x19;
    } while (iVar9 < DAT_00510760);
  }
  lVar14 = __ftol();
  lVar15 = __ftol();
  bVar11 = PointInPolygon(&DAT_004439f8,DAT_00443a98,(int)lVar14,(int)lVar15);
  if (CONCAT31(extraout_var,bVar11) != 0) {
    if ((local_4c < 2) && (DAT_00443a9c == 0)) {
LAB_00418ccc:
      DAT_0050b3c8 = (float)DAT_0050b3bc;
    }
    else {
      bVar11 = PointInPolygon(&DAT_004439f8,DAT_00443a98,DAT_0050b3bc,(int)lVar15);
      if (CONCAT31(extraout_var_00,bVar11) == 0) {
        DAT_0050b3c8 = (float)DAT_0050b3bc;
        goto LAB_00418ce4;
      }
      lVar14 = __ftol();
      bVar11 = PointInPolygon(&DAT_004439f8,DAT_00443a98,(int)lVar14,DAT_0050b3c0);
      if (CONCAT31(extraout_var_01,bVar11) != 0) goto LAB_00418ccc;
    }
    DAT_0050b3cc = (float)DAT_0050b3c0;
  }
LAB_00418ce4:
  iVar9 = DAT_00510718;
  uVar3 = DAT_0050b3d0;
  if (DAT_0050b3d0 != 0) {
    iVar10 = DAT_0050b3d4;
    if (DAT_00510718 < 0) {
      if ((9 < DAT_0050b3ac) || (DAT_0050b3ac < 0)) {
        DAT_0050b3ac = 0;
        iVar10 = 0x32;
      }
    }
    else if ((0x1d < DAT_0050b3ac) || (DAT_0050b3ac < 0x14)) {
      DAT_0050b3ac = 0x14;
      iVar10 = 0x32;
    }
    iVar7 = DAT_0050b3ac;
    if (DAT_00443a9c == 0) {
      iVar1 = -10;
    }
    else {
      lVar14 = __ftol();
      iVar1 = -(int)lVar14;
    }
    DAT_0050b3d4 = iVar10 + iVar1;
    if (DAT_0050b3d4 < 1) {
      DAT_0050b3ac = iVar7 + 1;
      DAT_0050b3d4 = 0x32;
      DAT_0051078c = 1;
      if (iVar9 < 0) {
        if (9 < DAT_0050b3ac) {
          DAT_0050b3ac = 0;
        }
      }
      else if (0x1d < DAT_0050b3ac) {
        DAT_0050b3ac = 0x14;
      }
    }
    if (DAT_00443a9c == 0) {
      if ((uVar3 & 2) == 0) {
        if ((uVar3 & 8) == 0) {
          if ((uVar3 & 1) == 0) {
            if ((uVar3 & 4) != 0) {
              DAT_0050b3b0 = 4;
            }
          }
          else {
            DAT_0050b3b0 = 0;
          }
        }
        else if ((uVar3 & 1) == 0) {
          DAT_0050b3b0 = 6 - ((uVar3 & 4) != 0);
        }
        else {
          DAT_0050b3b0 = 7;
        }
      }
      else if ((uVar3 & 1) == 0) {
        DAT_0050b3b0 = (uint)(byte)(((byte)uVar3 & 4 | 8) >> 2);
      }
      else {
        DAT_0050b3b0 = 1;
      }
    }
    goto LAB_00418ece;
  }
  if (DAT_00510718 == -1) {
    iVar9 = 0xc;
    if ((0xb < DAT_0050b3ac) && (DAT_0050b3ac < 0x10)) {
      DAT_0050b3d4 = DAT_0050b3d4 + -10;
      if (DAT_0050b3d4 < 1) {
        DAT_0050b3ac = DAT_0050b3ac + 1;
        DAT_0050b3d4 = 100;
        DAT_0051078c = 1;
        if (0xf < DAT_0050b3ac) {
          DAT_0050b3ac = 0xc;
        }
      }
      goto LAB_00418ece;
    }
  }
  else {
    iVar9 = 0x21;
    if ((0x20 < DAT_0050b3ac) && (DAT_0050b3ac < 0x27)) {
      DAT_0050b3d4 = DAT_0050b3d4 + -10;
      if (0 < DAT_0050b3d4) goto LAB_00418ece;
      DAT_0050b3ac = DAT_0050b3ac + 1;
      DAT_0050b3d4 = 100;
      DAT_0051078c = 1;
      if (DAT_0050b3ac < 0x27) goto LAB_00418ece;
    }
  }
  DAT_0050b3ac = iVar9;
LAB_00418ece:
  lVar14 = __ftol();
  DAT_0050b3b4 = (int)lVar14;
  lVar14 = __ftol();
  iVar9 = DAT_00510760;
  DAT_0050b3b8 = (int)lVar14;
  if (0 < DAT_00510760) {
    puVar5 = &DAT_0050b3f0;
    iVar10 = DAT_00510760;
    do {
      *puVar5 = 0;
      puVar5 = puVar5 + 0x19;
      iVar10 = iVar10 + -1;
    } while (iVar10 != 0);
  }
  iVar10 = 0;
  DAT_00510770 = 0;
  if (0 < iVar9) {
    piVar8 = &DAT_0050b3ac;
    do {
      if (piVar8[0xb] == 7) {
        fVar12 = DistanceBetweenIntegerPoints
                           ((&DAT_00510628)[DAT_0051076c * 2],(&DAT_0051062c)[DAT_0051076c * 2],
                            piVar8[2],piVar8[3]);
        iVar9 = DAT_00510768;
        iVar7 = DAT_0051076c;
        if (fVar12 < (float10)_DAT_0043b378) {
          iVar7 = DAT_0051076c + 1;
          DAT_0051076c = iVar7;
          piVar8[0x12] = (int)((float)piVar8[0x12] * (float)_DAT_0043b338);
          if (iVar9 <= iVar7) {
            DAT_0051076c = 0;
            iVar7 = 0;
          }
        }
        lVar14 = AngleBetweenIntegerPointsDegrees
                           (piVar8[2],piVar8[3],(&DAT_00510628)[iVar7 * 2],
                            (&DAT_0051062c)[iVar7 * 2]);
        iVar9 = ((int)lVar14 + 0x16) / 0x2d;
        piVar8[1] = iVar9;
        if (7 < iVar9) {
          piVar8[1] = iVar9 + -8;
        }
        fVar12 = (float10)(int)lVar14 * (float10)_DAT_0043b384;
        fVar13 = (float10)fsin(fVar12);
        fVar12 = (float10)fcos(fVar12);
        if ((DAT_00443b20 != 0) && ((*piVar8 == 3 || (*piVar8 == 4)))) {
          piVar8[7] = (int)((float)piVar8[0x12] * _DAT_0043b378 * (float)fVar13 + (float)piVar8[7]);
          piVar8[8] = (int)((float)piVar8[8] - (float)piVar8[0x12] * _DAT_0043b378 * (float)fVar12);
          lVar14 = __ftol();
          piVar8[2] = (int)lVar14;
          lVar14 = __ftol();
          piVar8[3] = (int)lVar14;
          piVar8[0x11] = 1;
          DAT_00443b20 = 0;
        }
        if ((float)piVar8[0x12] < _DAT_0043b440) {
          piVar8[0x12] = (int)((float)piVar8[0x12] + _DAT_0043b434);
        }
        iVar9 = piVar8[10];
        piVar8[10] = iVar9 + -5;
        if (iVar9 + -5 < 1) {
          piVar8[10] = 0x19;
          iVar9 = *piVar8;
          DAT_00443b20 = 1;
          *piVar8 = iVar9 + 1;
          if (6 < iVar9 + 1) {
            *piVar8 = 0;
          }
        }
      }
      if ((0 < piVar8[0xb]) && (piVar8[0xb] < 4)) {
        bVar11 = true;
        piVar6 = &DAT_0050af14;
        do {
          if (*piVar6 == iVar10) {
            bVar11 = false;
            break;
          }
          piVar6 = piVar6 + 1;
        } while ((int)piVar6 < 0x50af64);
        if ((piVar8[0x13] == 0) && (bVar11)) {
          piVar8[4] = piVar8[2];
          piVar8[5] = piVar8[3];
          iVar9 = UpdateHerdingAnimal(iVar10);
          if (iVar9 == 0) {
            if (7 < piVar8[1]) {
              piVar8[1] = piVar8[1] + -8;
            }
            if (piVar8[0x11] == 0) {
              if ((float)piVar8[0x12] < _DAT_0043b43c) {
                piVar8[0x12] = (int)((float)piVar8[0x12] + _DAT_0043b434);
              }
              piVar8[7] = (int)(*(float *)(&DAT_004439b4 + piVar8[1] * 4) * (float)piVar8[0x12] +
                               (float)piVar8[7]);
              piVar8[8] = (int)(*(float *)(&DAT_004439d4 + piVar8[1] * 4) * (float)piVar8[0x12] +
                               (float)piVar8[8]);
              lVar14 = __ftol();
              piVar8[2] = (int)lVar14;
              lVar14 = __ftol();
              piVar8[3] = (int)lVar14;
            }
            bVar11 = PointInPolygon(&DAT_004439f8,DAT_00443a98,piVar8[2],piVar8[3]);
            if (CONCAT31(extraout_var_02,bVar11) != 0) {
              piVar8[2] = piVar8[4];
              piVar8[3] = piVar8[5];
              lVar14 = AngleBetweenIntegerPointsDegrees
                                 ((piVar8[0xc] - piVar8[0xe]) / 2 + piVar8[2],
                                  (piVar8[0xf] - piVar8[0xd]) / 2 + piVar8[5],DAT_0050af74,
                                  DAT_0050af70);
              fVar12 = (float10)(int)lVar14 * (float10)_DAT_0043b384;
              fVar13 = (float10)fsin(fVar12);
              piVar8[1] = ((int)lVar14 + 0x16) / 0x2d;
              piVar8[7] = (int)(float)(fVar13 * (float10)_DAT_0043b430 + (float10)(float)piVar8[7]);
              fVar12 = (float10)fcos(fVar12);
              piVar8[8] = (int)(float)((float10)(float)piVar8[8] - fVar12 * (float10)_DAT_0043b430);
              lVar14 = __ftol();
              piVar8[2] = (int)lVar14;
              lVar14 = __ftol();
              piVar8[3] = (int)lVar14;
              piVar8[0x12] = 0;
            }
          }
        }
        else {
          UpdateHerdingAnimal(iVar10);
        }
      }
      iVar10 = iVar10 + 1;
      piVar8 = piVar8 + 0x19;
    } while (iVar10 < DAT_00510760);
  }
  DrawHerdingActivity();
  if (DAT_00510764 < 1) {
    if (DAT_00510774 == 0) {
      iVar9 = AnyManagedSoundPlaying(DAT_0044ddd8);
      if (iVar9 == 0) {
        iVar9 = 1;
        uVar16 = 0x5a;
        uVar3 = FUN_0042ffc4();
        uVar3 = uVar3 & 0x80000001;
        if ((int)uVar3 < 0) {
          uVar3 = (uVar3 - 1 | 0xfffffffe) + 1;
        }
        PlayManagedSoundById(DAT_0044ddd8,uVar3 + 599,uVar16,iVar9);
        DAT_00510774 = 1;
      }
    }
    else if ((DAT_00510774 == 1) && (iVar9 = AnyManagedSoundPlaying(DAT_0044ddd8), iVar9 == 0)) {
      PreparePlayAgainTransition();
      DAT_0044de14 = 0x3c;
      DAT_0051b418 = 0xe;
      DAT_00446ce8 = 0xffffffff;
      DAT_00446f38 = 0;
      UnloadHerdingActivityResources();
      DAT_0051c2fc = 1;
      *(undefined4 *)(&DAT_0051b598 + DAT_00519934 * 400) = 1;
    }
  }
  if (DAT_0051c30c == 1) {
    CSound_Stop(DAT_0051c2b8);
    PlayActivityMusicByIndex(6);
    DAT_0051c30c = 0;
    PlayManagedSoundById(DAT_0044ddd8,0x245,0x5a,1);
    *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x4c5) * 4 + 0xd98) = 1;
  }
  return 1;
}

