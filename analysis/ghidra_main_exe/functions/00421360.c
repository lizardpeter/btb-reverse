/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00421360; function: UpdateSpudMazeBobMovement; body bytes: 4246
 * callers: 1; callees: 7; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdateSpudMazeBobMovement(void)

{
  float fVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  float10 fVar10;
  longlong lVar11;
  longlong lVar12;
  undefined4 uVar13;
  int local_14;
  int *local_10;
  int local_c;
  float local_8;
  int *local_4;
  
  _DAT_005109d8 = DAT_005109d0;
  piVar7 = (int *)0xffffffff;
  local_c = 0;
  local_14 = 0;
  local_8 = 0.0;
  local_10 = (int *)0xffffffff;
  _DAT_005109dc = DAT_005109d4;
  if (DAT_005109e8 != 0) {
    _DAT_005109d8 = DAT_005109d0;
    _DAT_005109dc = DAT_005109d4;
    return;
  }
  ComputeSpudMazeBobInputDirection(&local_c,&local_14);
  if ((local_c != 0) || (DAT_00514518 = 0, local_14 != 0)) {
    DAT_00514518 = 1;
  }
  lVar11 = __ftol();
  lVar12 = __ftol();
  iVar9 = DAT_005109e0 + DAT_00510d18 * 0x1e;
  fVar10 = DistanceBetweenIntegerPoints
                     ((int)lVar11,(int)lVar12,(&DAT_00510e18)[iVar9 * 8],(&DAT_00510e1c)[iVar9 * 8])
  ;
  if (((fVar10 < (float10)_DAT_0043b4c0) && ((float10)_DAT_00443d64 < fVar10)) &&
     (DAT_00512124 == 0)) {
    local_10 = (int *)DAT_005109e0;
    piVar7 = (int *)DAT_005109e0;
  }
  if (DAT_00514510 != 0) {
    return;
  }
  if ((float10)_DAT_00443d64 < fVar10) {
    if ((float10)_DAT_0043b458 < fVar10) {
      DAT_00444230 = -1;
    }
    iVar2 = 0;
    iVar9 = DAT_00510d18;
    do {
      iVar6 = (&DAT_00510e28)[iVar2 + (DAT_005109e0 + iVar9 * 0x1e) * 8];
      if (iVar6 != -1) {
        iVar9 = iVar9 * 0x1e + iVar6;
        fVar10 = DistanceBetweenIntegerPoints
                           ((int)lVar11,(int)lVar12,(&DAT_00510e18)[iVar9 * 8],
                            (&DAT_00510e1c)[iVar9 * 8]);
        if (fVar10 < (float10)_DAT_00443d64) {
          local_10 = (int *)0xffffffff;
          piVar7 = (int *)0xffffffff;
          DAT_00444230 = iVar6;
          DAT_005109e0 = iVar6;
          goto LAB_0042155e;
        }
        iVar9 = DAT_00510d18;
        if ((fVar10 < (float10)_DAT_0043b4c0) && (DAT_00512124 == 0)) {
          local_10 = (int *)iVar6;
        }
      }
      iVar2 = iVar2 + 1;
      piVar7 = local_10;
    } while (iVar2 < 4);
  }
  else {
    if (DAT_00444230 == DAT_005109e0) {
      local_8 = 1.4013e-45;
    }
    else {
      local_8 = 0.0;
      DAT_00444230 = DAT_005109e0;
    }
LAB_0042155e:
    iVar9 = DAT_00510d18;
    if (DAT_005109e0 != -1) {
      iVar2 = (DAT_005109e0 + DAT_00510d18 * 0x1e) * 0x20;
      uVar4 = *(uint *)(&DAT_00510e24 + iVar2);
      if ((uVar4 & 0xf80) != 0) {
        DAT_005120e8 = *(undefined4 *)(&DAT_004441f8 + DAT_0051c284 * 4);
        if (DAT_005109cc == 0) {
          DAT_005107e8 = DAT_005120e8;
        }
        DAT_00510cf0 = DAT_00510d18;
        DAT_00444230 = 0xffffffff;
        if (((uVar4 & 0x80) == 0) && ((uVar4 & 0x400) == 0)) {
          if ((uVar4 & 0x900) == 0) goto LAB_00421642;
          DAT_00510d18 = DAT_00510d18 + 1;
          DAT_005109e0 = DAT_005109e0 + 2;
          DAT_005144b4 = 2;
          iVar9 = DAT_005109e0 + DAT_00510d18 * 0x1e;
          DAT_005109d0 = (float)(int)(&DAT_00510e18)[iVar9 * 8] + (float)_DAT_0043b4b8;
        }
        else {
          DAT_00510d18 = DAT_00510d18 + -1;
          DAT_005109e0 = DAT_005109e0 + -2;
          DAT_005144b4 = 1;
          iVar9 = DAT_005109e0 + DAT_00510d18 * 0x1e;
          DAT_005109d0 = (float)(int)(&DAT_00510e18)[iVar9 * 8] - (float)_DAT_0043b4b8;
        }
        DAT_005109d4 = (float)(int)(&DAT_00510e1c)[iVar9 * 8];
LAB_00421642:
        DAT_00512128 = 1;
        DAT_005109f8 = 10;
        DAT_005120f8 = DAT_00510d18;
        ResetPilchardSpawnDelay();
        return;
      }
      DAT_005109f8 = *(uint *)(&DAT_00510e20 + iVar2);
    }
  }
  if (piVar7 != (int *)0xffffffff) {
    if (local_c == 1) {
      if ((local_14 == 0) &&
         (iVar2 = (int)piVar7 + iVar9 * 0x1e, *(int *)(&DAT_00510e2c + iVar2 * 0x20) != -1)) {
        fVar1 = DAT_005109d4 - (float)(int)(&DAT_00510e1c)[iVar2 * 8];
        if (fVar1 < _DAT_0043b2f0) {
          fVar1 = -fVar1;
        }
        if ((float)_DAT_0043b480 < fVar1) {
          if (DAT_005109d4 <= (float)(int)(&DAT_00510e1c)[iVar2 * 8]) {
LAB_00421733:
            DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
            DAT_00514518 = DAT_00514518 + 1;
            DAT_00510a00 = 2;
          }
          else {
            DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
            DAT_00510a00 = 0;
            DAT_00514518 = DAT_00514518 + 1;
          }
        }
      }
    }
    else if (local_c == -1) {
      if ((local_14 == 0) &&
         (iVar2 = (int)piVar7 + iVar9 * 0x1e, *(int *)(&DAT_00510e34 + iVar2 * 0x20) != -1)) {
        fVar1 = DAT_005109d4 - (float)(int)(&DAT_00510e1c)[iVar2 * 8];
        if (fVar1 < _DAT_0043b2f0) {
          fVar1 = -fVar1;
        }
        if ((float)_DAT_0043b480 < fVar1) {
          if (DAT_005109d4 <= (float)(int)(&DAT_00510e1c)[iVar2 * 8]) goto LAB_00421733;
          DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
          DAT_00514518 = DAT_00514518 + 1;
          DAT_00510a00 = 0;
        }
      }
    }
    else if (local_c == 0) {
      if (local_14 == 1) {
        iVar2 = (int)piVar7 + iVar9 * 0x1e;
        if ((&DAT_00510e28)[iVar2 * 8] != -1) {
          fVar1 = DAT_005109d0 - (float)(int)(&DAT_00510e18)[iVar2 * 8];
          if (fVar1 < _DAT_0043b2f0) {
            fVar1 = -fVar1;
          }
          if ((float)_DAT_0043b480 < fVar1) {
            if (DAT_005109d0 <= (float)(int)(&DAT_00510e18)[iVar2 * 8]) goto LAB_004218e0;
joined_r0x004218c0:
            if (DAT_0051451c != 0) {
              DAT_005109d0 = DAT_005109d0 - _DAT_0044421c;
            }
            DAT_00510a00 = 3;
          }
        }
      }
      else if ((local_14 == -1) &&
              (iVar2 = (int)piVar7 + iVar9 * 0x1e, (&DAT_00510e30)[iVar2 * 8] != -1)) {
        fVar1 = DAT_005109d0 - (float)(int)(&DAT_00510e18)[iVar2 * 8];
        if (fVar1 < _DAT_0043b2f0) {
          fVar1 = -fVar1;
        }
        if ((float)_DAT_0043b480 < fVar1) {
          if ((float)(int)(&DAT_00510e18)[iVar2 * 8] < DAT_005109d0) goto joined_r0x004218c0;
LAB_004218e0:
          if (DAT_0051451c != 0) {
            DAT_005109d0 = _DAT_0044421c + DAT_005109d0;
          }
          DAT_00510a00 = 1;
        }
      }
    }
  }
  uVar4 = DAT_005109f8;
  if (local_8 == 0.0) {
    if ((_DAT_00514538 & 2) == 0) {
      if (local_c == 1) {
        if ((DAT_005109f8 & 2) != 0) {
          if (DAT_0051451c != 0) {
            DAT_005109d0 = _DAT_0044421c + DAT_005109d0;
          }
          DAT_00510a00 = 1;
          DAT_00514518 = DAT_00514518 + 1;
          _DAT_00514538 = 10;
          DAT_005109f8 = 10;
        }
      }
      else if ((local_c == -1) && ((DAT_005109f8 & 8) != 0)) {
        _DAT_00514538 = 10;
        DAT_005109f8 = 10;
        if (DAT_0051451c != 0) {
          DAT_005109d0 = DAT_005109d0 - _DAT_0044421c;
        }
        DAT_00510a00 = 3;
        DAT_00514518 = DAT_00514518 + 1;
      }
      if (local_14 == 1) {
        if ((DAT_005109f8 & 1) != 0) {
          iVar2 = iVar9 * 0x1e + DAT_005109e0;
          iVar6 = (&DAT_00510e28)[iVar2 * 8];
          if ((&DAT_00510e28)[iVar2 * 8] == -1) {
            iVar6 = DAT_005109e0;
          }
          iVar9 = iVar9 * 0x1e + iVar6;
          local_8 = (float)(&DAT_00510e1c)[iVar9 * 8];
          iVar9 = (&DAT_00510e18)[iVar9 * 8];
          if ((DAT_005109d4 < (float)(int)local_8) || (iVar8 = (int)local_8, iVar6 == -1)) {
            iVar9 = (&DAT_00510e18)[iVar2 * 8];
            iVar8 = (&DAT_00510e1c)[iVar2 * 8];
          }
          iVar2 = iVar9;
          iVar6 = iVar8;
          lVar11 = __ftol();
          iVar3 = (int)lVar11;
          lVar11 = __ftol();
          fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar3,iVar2,iVar6);
          if (fVar10 <= (float10)_DAT_0043b478) {
            if (DAT_0051451c != 0) {
              DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
            }
          }
          else if (DAT_0051451c != 0) {
            lVar11 = __ftol();
            iVar2 = (int)lVar11;
            lVar11 = __ftol();
            lVar11 = AngleBetweenIntegerPointsDegrees((int)lVar11,iVar2,iVar9,iVar8);
            local_8 = (float)lVar11;
            fVar10 = (float10)fcos((float10)(int)local_8 * (float10)_DAT_0043b384);
            DAT_005109d4 = (float)((float10)DAT_005109d4 - fVar10 * (float10)_DAT_0044421c);
            fVar10 = (float10)fsin((float10)(int)local_8 * (float10)_DAT_0043b384);
            DAT_005109d0 = (float)(fVar10 * (float10)_DAT_0044421c + (float10)DAT_005109d0);
          }
          DAT_00514518 = DAT_00514518 + 1;
          DAT_00510a00 = 0;
          DAT_005109f8 = 5;
          _DAT_00514538 = 5;
          iVar9 = DAT_00510d18;
        }
      }
      else if ((local_14 == -1) && ((DAT_005109f8 & 4) != 0)) {
        iVar2 = (&DAT_00510e30)[(iVar9 * 0x1e + DAT_005109e0) * 8];
        if ((&DAT_00510e30)[(iVar9 * 0x1e + DAT_005109e0) * 8] == -1) {
          iVar2 = DAT_005109e0;
        }
        iVar2 = iVar9 * 0x1e + iVar2;
        iVar9 = (&DAT_00510e1c)[iVar2 * 8];
        iVar2 = (&DAT_00510e18)[iVar2 * 8];
        iVar6 = iVar2;
        iVar8 = iVar9;
        lVar11 = __ftol();
        iVar3 = (int)lVar11;
        lVar11 = __ftol();
        fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar3,iVar6,iVar8);
        if (fVar10 <= (float10)_DAT_0043b478) {
          if (DAT_0051451c != 0) {
            DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
          }
        }
        else if (DAT_0051451c != 0) {
          lVar11 = __ftol();
          iVar6 = (int)lVar11;
          lVar11 = __ftol();
          lVar11 = AngleBetweenIntegerPointsDegrees((int)lVar11,iVar6,iVar2,iVar9);
          local_8 = (float)lVar11;
          fVar10 = (float10)fcos((float10)(int)local_8 * (float10)_DAT_0043b384);
          DAT_005109d4 = (float)((float10)DAT_005109d4 - fVar10 * (float10)_DAT_0044421c);
          fVar10 = (float10)fsin((float10)(int)local_8 * (float10)_DAT_0043b384);
          DAT_005109d0 = (float)(fVar10 * (float10)_DAT_0044421c + (float10)DAT_005109d0);
        }
        DAT_00514518 = DAT_00514518 + 1;
        DAT_00510a00 = 2;
        DAT_005109f8 = 5;
        _DAT_00514538 = 5;
        iVar9 = DAT_00510d18;
      }
      goto LAB_004220d0;
    }
    if (local_14 == 1) {
      if ((DAT_005109f8 & 1) != 0) {
        _DAT_00514538 = 5;
        DAT_005109f8 = 5;
        if (DAT_0051451c != 0) {
          DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
        }
        DAT_00510a00 = 0;
        DAT_00514518 = DAT_00514518 + 1;
        uVar4 = DAT_005109f8;
      }
    }
    else if ((local_14 == -1) && ((DAT_005109f8 & 4) != 0)) {
      _DAT_00514538 = 5;
      DAT_005109f8 = 5;
      if (DAT_0051451c != 0) {
        DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
      }
      DAT_00510a00 = 2;
      DAT_00514518 = DAT_00514518 + 1;
      uVar4 = DAT_005109f8;
    }
    goto joined_r0x00421f5d;
  }
  if ((_DAT_00514538 & 1) == 0) {
    if (local_c == 1) {
      if ((DAT_005109f8 & 2) != 0) {
        if (DAT_0051451c != 0) {
          DAT_005109d0 = _DAT_0044421c + DAT_005109d0;
        }
        DAT_00510a00 = 1;
        DAT_00514518 = DAT_00514518 + 1;
        _DAT_00514538 = 10;
        DAT_005109f8 = 10;
      }
    }
    else if ((local_c == -1) && ((DAT_005109f8 & 8) != 0)) {
      _DAT_00514538 = 10;
      DAT_005109f8 = 10;
      if (DAT_0051451c != 0) {
        DAT_005109d0 = DAT_005109d0 - _DAT_0044421c;
      }
      DAT_00510a00 = 3;
      DAT_00514518 = DAT_00514518 + 1;
    }
    if (local_14 == 1) {
      if ((DAT_005109f8 & 1) != 0) {
        DAT_005109f8 = 5;
        _DAT_00514538 = 5;
        if (DAT_0051451c != 0) {
          DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
        }
        DAT_00510a00 = 0;
        DAT_00514518 = DAT_00514518 + 1;
      }
    }
    else if ((local_14 == -1) && ((DAT_005109f8 & 4) != 0)) {
      DAT_005109f8 = 5;
      _DAT_00514538 = 5;
      if (DAT_0051451c != 0) {
        DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
      }
      DAT_00510a00 = 2;
      DAT_00514518 = DAT_00514518 + 1;
    }
    goto LAB_004220d0;
  }
  if (local_14 == 1) {
    if ((DAT_005109f8 & 1) != 0) {
      if ((&DAT_00510e28)[(iVar9 * 0x1e + DAT_005109e0) * 8] == -1) {
        return;
      }
      iVar2 = iVar9 * 0x1e + (&DAT_00510e28)[(iVar9 * 0x1e + DAT_005109e0) * 8];
      iVar9 = (&DAT_00510e1c)[iVar2 * 8];
      iVar2 = (&DAT_00510e18)[iVar2 * 8];
      iVar6 = iVar2;
      iVar8 = iVar9;
      lVar11 = __ftol();
      iVar3 = (int)lVar11;
      lVar11 = __ftol();
      fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar3,iVar6,iVar8);
      if (fVar10 <= (float10)_DAT_0043b478) {
        if (DAT_0051451c != 0) {
          DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
        }
      }
      else if (DAT_0051451c != 0) {
        lVar11 = __ftol();
        iVar6 = (int)lVar11;
        lVar11 = __ftol();
        lVar11 = AngleBetweenIntegerPointsDegrees((int)lVar11,iVar6,iVar2,iVar9);
        local_8 = (float)lVar11;
        fVar10 = (float10)fcos((float10)(int)local_8 * (float10)_DAT_0043b384);
        DAT_005109d4 = (float)((float10)DAT_005109d4 - fVar10 * (float10)_DAT_0044421c);
        fVar10 = (float10)fsin((float10)(int)local_8 * (float10)_DAT_0043b384);
        DAT_005109d0 = (float)(fVar10 * (float10)_DAT_0044421c + (float10)DAT_005109d0);
      }
      DAT_00510a00 = 0;
LAB_00421f50:
      DAT_00514518 = DAT_00514518 + 1;
      _DAT_00514538 = 5;
      DAT_005109f8 = 5;
      iVar9 = DAT_00510d18;
      uVar4 = 0;
    }
  }
  else if ((local_14 == -1) && ((DAT_005109f8 & 4) != 0)) {
    if ((&DAT_00510e30)[(iVar9 * 0x1e + DAT_005109e0) * 8] == -1) {
      return;
    }
    iVar2 = iVar9 * 0x1e + (&DAT_00510e30)[(iVar9 * 0x1e + DAT_005109e0) * 8];
    iVar9 = (&DAT_00510e1c)[iVar2 * 8];
    iVar2 = (&DAT_00510e18)[iVar2 * 8];
    iVar6 = iVar2;
    iVar8 = iVar9;
    lVar11 = __ftol();
    iVar3 = (int)lVar11;
    lVar11 = __ftol();
    fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar3,iVar6,iVar8);
    if (fVar10 <= (float10)_DAT_0043b478) {
      if (DAT_0051451c != 0) {
        DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
      }
    }
    else if (DAT_0051451c != 0) {
      lVar11 = __ftol();
      iVar6 = (int)lVar11;
      lVar11 = __ftol();
      lVar11 = AngleBetweenIntegerPointsDegrees((int)lVar11,iVar6,iVar2,iVar9);
      local_8 = (float)lVar11;
      fVar10 = (float10)fcos((float10)(int)local_8 * (float10)_DAT_0043b384);
      DAT_005109d4 = (float)((float10)DAT_005109d4 - fVar10 * (float10)_DAT_0044421c);
      fVar10 = (float10)fsin((float10)(int)local_8 * (float10)_DAT_0043b384);
      DAT_005109d0 = (float)(fVar10 * (float10)_DAT_0044421c + (float10)DAT_005109d0);
    }
    DAT_00510a00 = 2;
    goto LAB_00421f50;
  }
joined_r0x00421f5d:
  if (local_c == 1) {
    if ((uVar4 & 2) != 0) {
      if (DAT_0051451c != 0) {
        DAT_005109d0 = _DAT_0044421c + DAT_005109d0;
      }
      DAT_00510a00 = 1;
      DAT_00514518 = DAT_00514518 + 1;
      DAT_005109f8 = 10;
      _DAT_00514538 = 10;
    }
  }
  else if ((local_c == -1) && ((uVar4 & 8) != 0)) {
    DAT_005109f8 = 10;
    _DAT_00514538 = 10;
    if (DAT_0051451c != 0) {
      DAT_005109d0 = DAT_005109d0 - _DAT_0044421c;
    }
    DAT_00510a00 = 3;
    DAT_00514518 = DAT_00514518 + 1;
  }
LAB_004220d0:
  iVar9 = DAT_005109e0 + iVar9 * 0x1e;
  iVar2 = (&DAT_00510e1c)[iVar9 * 8];
  iVar9 = (&DAT_00510e18)[iVar9 * 8];
  lVar11 = __ftol();
  iVar6 = (int)lVar11;
  lVar11 = __ftol();
  fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar6,iVar9,iVar2);
  iVar9 = DAT_00510d18;
  if (fVar10 < (float10)_DAT_0043b470) {
    if ((_DAT_00514538 & 1) == 0) {
      local_8 = (float)(int)(&DAT_00510e1c)[(DAT_005109e0 + DAT_00510d18 * 0x1e) * 8];
      lVar11 = __ftol();
      iVar2 = (int)lVar11;
      if (iVar2 < 0) {
        iVar2 = -iVar2;
      }
      if (1 < iVar2) {
        if (local_8 <= DAT_005109d4) {
          if (local_8 < DAT_005109d4) {
            DAT_005109d4 = DAT_005109d4 - (float)_DAT_0043b468;
          }
        }
        else {
          DAT_005109d4 = DAT_005109d4 + (float)_DAT_0043b468;
        }
      }
    }
    else {
      local_8 = (float)(int)(&DAT_00510e18)[(DAT_005109e0 + DAT_00510d18 * 0x1e) * 8];
      lVar11 = __ftol();
      iVar2 = (int)lVar11;
      if (iVar2 < 0) {
        iVar2 = -iVar2;
      }
      if (1 < iVar2) {
        if (local_8 <= DAT_005109d0) {
          if (local_8 < DAT_005109d0) {
            DAT_005109d0 = DAT_005109d0 - (float)_DAT_0043b468;
          }
        }
        else {
          DAT_005109d0 = DAT_005109d0 + (float)_DAT_0043b468;
        }
      }
    }
  }
  local_4 = &DAT_004459cc + iVar9;
  local_8 = 0.0;
  if (0 < (int)(&DAT_004459cc)[iVar9]) {
    local_10 = &DAT_00514110 + iVar9 * 0xb;
    iVar9 = iVar9 * 0x58;
    do {
      if (0 < *local_10) {
        iVar2 = *(int *)((int)&DAT_005142dc + iVar9);
        iVar8 = *(int *)((int)&DAT_005142d8 + iVar9) / 2 + *(int *)(&DAT_00513f40 + iVar9);
        iVar6 = *(int *)(&DAT_00513f44 + iVar9);
        lVar11 = __ftol();
        lVar12 = __ftol();
        fVar10 = DistanceBetweenIntegerPoints(iVar8,iVar2 + 0x14 + iVar6,(int)lVar11,(int)lVar12);
        if (((fVar10 < (float10)_DAT_0043b4b0) &&
            (uVar4 = iVar8 - (int)lVar11, uVar5 = (int)uVar4 >> 0x1f,
            (int)((uVar4 ^ uVar5) - uVar5) < 0xf)) &&
           ((DAT_004fbe54 != 0 || ((DAT_004fbe5c & 0x10) != 0)))) {
          if (DAT_00445ef4 == 0) {
            iVar2 = 1;
            uVar13 = 0x32;
            uVar4 = FUN_0042ffc4();
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            iVar6 = uVar4 + 0x2c5;
          }
          else {
            DAT_005109ec = 9;
            DAT_00514510 = 1;
            DAT_00514514 = 5;
            *local_10 = 0;
            uVar4 = FUN_0042ffc4();
            iVar6 = (int)uVar4 % 7 + 0x2be;
            if (iVar6 == DAT_0051453c) {
              do {
                uVar4 = FUN_0042ffc4();
                iVar6 = (int)uVar4 % 7 + 0x2be;
              } while (iVar6 == DAT_0051453c);
            }
            iVar2 = 1;
            uVar13 = 0x32;
          }
          PlayManagedSoundById(DAT_0044ddd8,iVar6,uVar13,iVar2);
        }
      }
      local_10 = local_10 + 1;
      local_8 = (float)((int)local_8 + 1);
      iVar9 = iVar9 + 8;
    } while ((int)local_8 < *local_4);
  }
  DAT_0051451c = 0;
  DAT_00514518 = (uint)(1 < (int)DAT_00514518);
  return;
}

