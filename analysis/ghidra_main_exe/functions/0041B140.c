/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041b140; function: UpdateMazePlayerMovement; body bytes: 3635
 * callers: 1; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdateMazePlayerMovement(void)

{
  uint uVar1;
  float fVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  bool bVar9;
  float10 fVar10;
  longlong lVar11;
  longlong lVar12;
  int local_10;
  int local_c;
  float local_8;
  int local_4;
  
  _DAT_005109d8 = DAT_005109d0;
  local_c = 0;
  local_4 = 0;
  local_8 = 0.0;
  local_10 = -1;
  _DAT_005109dc = DAT_005109d4;
  if (DAT_005109e8 != 0) {
    _DAT_005109d8 = DAT_005109d0;
    _DAT_005109dc = DAT_005109d4;
    return;
  }
  ComputeMazePlayerInputDirection(&local_c,&local_4);
  lVar11 = __ftol();
  lVar12 = __ftol();
  iVar3 = DAT_005109e0 + DAT_00510d18 * 0x1e;
  fVar10 = DistanceBetweenIntegerPoints
                     ((int)lVar11,(int)lVar12,(&DAT_00510e18)[iVar3 * 8],(&DAT_00510e1c)[iVar3 * 8])
  ;
  if (((fVar10 < (float10)_DAT_0043b448) && ((float10)_DAT_00443d64 < fVar10)) &&
     (DAT_00512124 == 0)) {
    local_10 = DAT_005109e0;
  }
  if ((float10)_DAT_00443d64 < fVar10) {
    if ((float10)_DAT_0043b448 < fVar10) {
      DAT_00444230 = -1;
    }
    iVar4 = 0;
    iVar3 = DAT_00510d18;
    do {
      iVar6 = (&DAT_00510e28)[iVar4 + (DAT_005109e0 + iVar3 * 0x1e) * 8];
      if (iVar6 != -1) {
        iVar3 = iVar3 * 0x1e + iVar6;
        fVar10 = DistanceBetweenIntegerPoints
                           ((int)lVar11,(int)lVar12,(&DAT_00510e18)[iVar3 * 8],
                            (&DAT_00510e1c)[iVar3 * 8]);
        if (fVar10 < (float10)_DAT_00443d64) {
          local_10 = -1;
          DAT_00444230 = iVar6;
          DAT_005109e0 = iVar6;
          goto LAB_0041b304;
        }
        iVar3 = DAT_00510d18;
        if ((fVar10 < (float10)_DAT_0043b448) && (DAT_00512124 == 0)) {
          local_10 = iVar6;
        }
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 4);
  }
  else {
    if (DAT_00444230 == DAT_005109e0) {
      local_8 = 1.4013e-45;
    }
    else {
      DAT_00444230 = DAT_005109e0;
      local_8 = 0.0;
    }
LAB_0041b304:
    iVar3 = DAT_00510d18;
    if (DAT_005109e0 != -1) {
      iVar4 = (DAT_005109e0 + DAT_00510d18 * 0x1e) * 0x20;
      uVar1 = *(uint *)(&DAT_00510e24 + iVar4);
      if ((uVar1 & 0xf80) == 0) {
        DAT_005109f8 = *(uint *)(&DAT_00510e20 + iVar4);
      }
      else {
        DAT_005120e8 = *(undefined4 *)(&DAT_004441f8 + DAT_0051c284 * 4);
        if (DAT_005109cc == 0) {
          DAT_005107e8 = DAT_005120e8;
        }
        DAT_00510cf0 = DAT_00510d18;
        DAT_00444230 = -1;
        if ((uVar1 & 0x80) == 0) {
          if ((uVar1 & 0x100) == 0) {
            if ((uVar1 & 0x200) != 0) {
              bVar9 = DAT_00510d18 == 0;
              DAT_00510d18 = 1;
              if (bVar9) {
                DAT_005109e0 = 0;
                DAT_005109d0 = (float)_DAT_005111d8 + (float)_DAT_0043b488;
                DAT_005109d4 = (float)_DAT_005111dc;
              }
              else {
                DAT_005109e0 = 0xf;
                DAT_005109d0 = (float)_DAT_005113b8 - (float)_DAT_0043b488;
                DAT_005109d4 = (float)_DAT_005113bc;
              }
            }
          }
          else {
            DAT_005109e0 = 0;
            DAT_00510d18 = 2;
            DAT_005109d0 = (float)_DAT_00511598 + (float)_DAT_0043b488;
            DAT_005109d4 = (float)_DAT_0051159c;
          }
        }
        else {
          DAT_005109e0 = 0;
          DAT_00510d18 = 0;
          DAT_005109d0 = (float)DAT_00510e18 - (float)_DAT_0043b488;
          DAT_005109d4 = (float)DAT_00510e1c;
        }
        DAT_005109f8 = 10;
        DAT_00512128 = 1;
        iVar3 = DAT_00510d18;
        DAT_005120f8 = DAT_00510d18;
      }
    }
  }
  if (local_10 != -1) {
    if (local_c == 1) {
      if ((local_4 == 0) &&
         (local_10 = local_10 + iVar3 * 0x1e, *(int *)(&DAT_00510e2c + local_10 * 0x20) != -1)) {
        iVar4 = (&DAT_00510e1c)[local_10 * 8];
joined_r0x0041b4ea:
        fVar2 = DAT_005109d4 - (float)iVar4;
        if (fVar2 < _DAT_0043b2f0) {
          fVar2 = -fVar2;
        }
        if ((float)_DAT_0043b480 < fVar2) {
          if (DAT_005109d4 <= (float)iVar4) {
            DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
            DAT_00510a00 = 2;
          }
          else {
            DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
            DAT_00510a00 = 0;
          }
        }
      }
    }
    else if (local_c == -1) {
      if ((local_4 == 0) &&
         (local_10 = local_10 + iVar3 * 0x1e, *(int *)(&DAT_00510e34 + local_10 * 0x20) != -1)) {
        iVar4 = (&DAT_00510e1c)[local_10 * 8];
        goto joined_r0x0041b4ea;
      }
    }
    else if (local_c == 0) {
      if (local_4 == 1) {
        local_10 = local_10 + iVar3 * 0x1e;
        if ((&DAT_00510e28)[local_10 * 8] != -1) {
          iVar4 = (&DAT_00510e18)[local_10 * 8];
joined_r0x0041b5dd:
          fVar2 = DAT_005109d0 - (float)iVar4;
          if (fVar2 < _DAT_0043b2f0) {
            fVar2 = -fVar2;
          }
          if ((float)_DAT_0043b480 < fVar2) {
            if (DAT_005109d0 <= (float)iVar4) {
              DAT_005109d0 = _DAT_0044421c + DAT_005109d0;
              DAT_00510a00 = 1;
            }
            else {
              DAT_005109d0 = DAT_005109d0 - _DAT_0044421c;
              DAT_00510a00 = 3;
            }
          }
        }
      }
      else if ((local_4 == -1) &&
              ((local_10 = local_10 + iVar3 * 0x1e, (&DAT_00510e30)[local_10 * 8] != -1 ||
               (DAT_005109d4 < (float)(&DAT_00510e1c)[local_10 * 8])))) {
        iVar4 = (&DAT_00510e18)[local_10 * 8];
        goto joined_r0x0041b5dd;
      }
    }
  }
  uVar1 = DAT_005109f8;
  if (local_8 == 0.0) {
    if ((_DAT_00512148 & 2) == 0) {
      if (local_c == 1) {
        if ((DAT_005109f8 & 2) != 0) {
          DAT_005109d0 = _DAT_0044421c + DAT_005109d0;
          DAT_00510a00 = local_c;
          DAT_005109f8 = 10;
          _DAT_00512148 = 10;
        }
      }
      else if ((local_c == -1) && ((DAT_005109f8 & 8) != 0)) {
        DAT_005109d0 = DAT_005109d0 - _DAT_0044421c;
        DAT_00510a00 = 3;
        DAT_005109f8 = 10;
        _DAT_00512148 = 10;
      }
      if (local_4 == 1) {
        if ((DAT_005109f8 & 1) != 0) {
          iVar4 = iVar3 * 0x1e + DAT_005109e0;
          iVar6 = (&DAT_00510e28)[iVar4 * 8];
          if (((&DAT_00510e28)[iVar4 * 8] == -1) &&
             (iVar6 = DAT_005109e0, DAT_005109d4 <= (float)(&DAT_00510e1c)[iVar4 * 8])) {
            return;
          }
          iVar3 = iVar3 * 0x1e + iVar6;
          local_8 = (float)(&DAT_00510e1c)[iVar3 * 8];
          iVar3 = (&DAT_00510e18)[iVar3 * 8];
          if ((DAT_005109d4 < (float)(int)local_8) || (iVar7 = (int)local_8, iVar6 == -1)) {
            iVar3 = (&DAT_00510e18)[iVar4 * 8];
            iVar7 = (&DAT_00510e1c)[iVar4 * 8];
          }
          iVar4 = iVar3;
          iVar6 = iVar7;
          lVar11 = __ftol();
          iVar5 = (int)lVar11;
          lVar11 = __ftol();
          fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar5,iVar4,iVar6);
          if (fVar10 <= (float10)_DAT_0043b478) {
            DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
          }
          else {
            lVar11 = __ftol();
            iVar4 = (int)lVar11;
            lVar11 = __ftol();
            lVar11 = AngleBetweenIntegerPointsDegrees((int)lVar11,iVar4,iVar3,iVar7);
            local_8 = (float)lVar11;
            fVar10 = (float10)fcos((float10)(int)local_8 * (float10)_DAT_0043b384);
            DAT_005109d4 = (float)((float10)DAT_005109d4 - fVar10 * (float10)_DAT_0044421c);
            fVar10 = (float10)fsin((float10)(int)local_8 * (float10)_DAT_0043b384);
            DAT_005109d0 = (float)(fVar10 * (float10)_DAT_0044421c + (float10)DAT_005109d0);
          }
          DAT_005109f8 = 5;
          _DAT_00512148 = 5;
          DAT_00510a00 = 0;
          iVar3 = DAT_00510d18;
        }
      }
      else if ((local_4 == -1) && ((DAT_005109f8 & 4) != 0)) {
        iVar4 = iVar3 * 0x1e + DAT_005109e0;
        iVar6 = (&DAT_00510e30)[iVar4 * 8];
        if (((&DAT_00510e30)[iVar4 * 8] == -1) &&
           (iVar6 = DAT_005109e0, (float)(&DAT_00510e1c)[iVar4 * 8] <= DAT_005109d4)) {
          return;
        }
        iVar6 = iVar3 * 0x1e + iVar6;
        iVar3 = (&DAT_00510e1c)[iVar6 * 8];
        iVar4 = (&DAT_00510e18)[iVar6 * 8];
        iVar6 = iVar4;
        iVar7 = iVar3;
        lVar11 = __ftol();
        iVar5 = (int)lVar11;
        lVar11 = __ftol();
        fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar5,iVar6,iVar7);
        if (fVar10 <= (float10)_DAT_0043b478) {
          DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
        }
        else {
          lVar11 = __ftol();
          iVar6 = (int)lVar11;
          lVar11 = __ftol();
          lVar11 = AngleBetweenIntegerPointsDegrees((int)lVar11,iVar6,iVar4,iVar3);
          local_8 = (float)lVar11;
          fVar10 = (float10)fcos((float10)(int)local_8 * (float10)_DAT_0043b384);
          DAT_005109d4 = (float)((float10)DAT_005109d4 - fVar10 * (float10)_DAT_0044421c);
          fVar10 = (float10)fsin((float10)(int)local_8 * (float10)_DAT_0043b384);
          DAT_005109d0 = (float)(fVar10 * (float10)_DAT_0044421c + (float10)DAT_005109d0);
        }
        DAT_005109f8 = 5;
        _DAT_00512148 = 5;
        DAT_00510a00 = 2;
        iVar3 = DAT_00510d18;
      }
      goto LAB_0041bd09;
    }
    if (local_4 == 1) {
      if ((DAT_005109f8 & 1) != 0) {
        DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
        DAT_00510a00 = 0;
        _DAT_00512148 = 5;
        DAT_005109f8 = 5;
        uVar1 = DAT_005109f8;
      }
    }
    else if ((local_4 == -1) && ((DAT_005109f8 & 4) != 0)) {
      DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
      DAT_00510a00 = 2;
      _DAT_00512148 = 5;
      DAT_005109f8 = 5;
      uVar1 = DAT_005109f8;
    }
    goto joined_r0x0041bbf2;
  }
  if ((_DAT_00512148 & 1) == 0) {
    if (local_c == 1) {
      if ((DAT_005109f8 & 2) != 0) {
        DAT_005109d0 = _DAT_0044421c + DAT_005109d0;
        DAT_00510a00 = local_c;
        DAT_005109f8 = 10;
        _DAT_00512148 = 10;
      }
    }
    else if ((local_c == -1) && ((DAT_005109f8 & 8) != 0)) {
      DAT_005109d0 = DAT_005109d0 - _DAT_0044421c;
      DAT_00510a00 = 3;
      DAT_005109f8 = 10;
      _DAT_00512148 = 10;
    }
    if (local_4 == 1) {
      if ((DAT_005109f8 & 1) != 0) {
        DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
        DAT_00510a00 = 0;
        DAT_005109f8 = 5;
        _DAT_00512148 = 5;
      }
    }
    else if ((local_4 == -1) && ((DAT_005109f8 & 4) != 0)) {
      DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
      DAT_00510a00 = 2;
      DAT_005109f8 = 5;
      _DAT_00512148 = 5;
    }
    goto LAB_0041bd09;
  }
  if (local_4 == 1) {
    if ((DAT_005109f8 & 1) != 0) {
      iVar4 = (&DAT_00510e28)[(iVar3 * 0x1e + DAT_005109e0) * 8];
      if ((&DAT_00510e28)[(iVar3 * 0x1e + DAT_005109e0) * 8] == -1) {
        iVar4 = DAT_005109e0;
      }
      iVar4 = iVar3 * 0x1e + iVar4;
      iVar3 = (&DAT_00510e1c)[iVar4 * 8];
      iVar4 = (&DAT_00510e18)[iVar4 * 8];
      iVar6 = iVar4;
      iVar7 = iVar3;
      lVar11 = __ftol();
      iVar5 = (int)lVar11;
      lVar11 = __ftol();
      fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar5,iVar6,iVar7);
      if (fVar10 <= (float10)_DAT_0043b478) {
        DAT_005109d4 = DAT_005109d4 - _DAT_0044421c;
      }
      else {
        lVar11 = __ftol();
        iVar6 = (int)lVar11;
        lVar11 = __ftol();
        lVar11 = AngleBetweenIntegerPointsDegrees((int)lVar11,iVar6,iVar4,iVar3);
        local_8 = (float)lVar11;
        fVar10 = (float10)fcos((float10)(int)local_8 * (float10)_DAT_0043b384);
        DAT_005109d4 = (float)((float10)DAT_005109d4 - fVar10 * (float10)_DAT_0044421c);
        fVar10 = (float10)fsin((float10)(int)local_8 * (float10)_DAT_0043b384);
        DAT_005109d0 = (float)(fVar10 * (float10)_DAT_0044421c + (float10)DAT_005109d0);
      }
      DAT_00510a00 = 0;
LAB_0041bbe5:
      _DAT_00512148 = 5;
      DAT_005109f8 = 5;
      iVar3 = DAT_00510d18;
      uVar1 = 0;
    }
  }
  else if ((local_4 == -1) && ((DAT_005109f8 & 4) != 0)) {
    iVar4 = (&DAT_00510e30)[(iVar3 * 0x1e + DAT_005109e0) * 8];
    if ((&DAT_00510e30)[(iVar3 * 0x1e + DAT_005109e0) * 8] == -1) {
      iVar4 = DAT_005109e0;
    }
    iVar4 = iVar3 * 0x1e + iVar4;
    iVar3 = (&DAT_00510e1c)[iVar4 * 8];
    iVar4 = (&DAT_00510e18)[iVar4 * 8];
    iVar6 = iVar4;
    iVar7 = iVar3;
    lVar11 = __ftol();
    iVar5 = (int)lVar11;
    lVar11 = __ftol();
    fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar5,iVar6,iVar7);
    if (fVar10 <= (float10)_DAT_0043b478) {
      DAT_005109d4 = _DAT_0044421c + DAT_005109d4;
    }
    else {
      lVar11 = __ftol();
      iVar6 = (int)lVar11;
      lVar11 = __ftol();
      lVar11 = AngleBetweenIntegerPointsDegrees((int)lVar11,iVar6,iVar4,iVar3);
      local_8 = (float)lVar11;
      fVar10 = (float10)fcos((float10)(int)local_8 * (float10)_DAT_0043b384);
      DAT_005109d4 = (float)((float10)DAT_005109d4 - fVar10 * (float10)_DAT_0044421c);
      fVar10 = (float10)fsin((float10)(int)local_8 * (float10)_DAT_0043b384);
      DAT_005109d0 = (float)(fVar10 * (float10)_DAT_0044421c + (float10)DAT_005109d0);
    }
    DAT_00510a00 = 2;
    goto LAB_0041bbe5;
  }
joined_r0x0041bbf2:
  if (local_c == 1) {
    if ((uVar1 & 2) != 0) {
      DAT_005109d0 = _DAT_0044421c + DAT_005109d0;
      DAT_00510a00 = 1;
      DAT_005109f8 = 10;
      _DAT_00512148 = 10;
    }
  }
  else if ((local_c == -1) && ((uVar1 & 8) != 0)) {
    DAT_005109d0 = DAT_005109d0 - _DAT_0044421c;
    DAT_00510a00 = 3;
    DAT_005109f8 = 10;
    _DAT_00512148 = 10;
  }
LAB_0041bd09:
  iVar3 = DAT_005109e0 + iVar3 * 0x1e;
  iVar4 = (&DAT_00510e1c)[iVar3 * 8];
  iVar3 = (&DAT_00510e18)[iVar3 * 8];
  lVar11 = __ftol();
  iVar6 = (int)lVar11;
  lVar11 = __ftol();
  fVar10 = DistanceBetweenIntegerPoints((int)lVar11,iVar6,iVar3,iVar4);
  iVar3 = DAT_00510d18;
  if (fVar10 < (float10)_DAT_0043b470) {
    if ((_DAT_00512148 & 1) == 0) {
      local_8 = (float)(&DAT_00510e1c)[(DAT_005109e0 + DAT_00510d18 * 0x1e) * 8];
      lVar11 = __ftol();
      iVar4 = (int)lVar11;
      if (iVar4 < 0) {
        iVar4 = -iVar4;
      }
      if (1 < iVar4) {
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
      local_8 = (float)(&DAT_00510e18)[(DAT_005109e0 + DAT_00510d18 * 0x1e) * 8];
      lVar11 = __ftol();
      iVar4 = (int)lVar11;
      if (iVar4 < 0) {
        iVar4 = -iVar4;
      }
      if (1 < iVar4) {
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
  piVar8 = &DAT_00510a18 + iVar3 * 0x14;
  local_8 = 5.60519e-45;
  do {
    iVar4 = *piVar8;
    if (iVar4 != -1) {
      if (piVar8[1] == 1) {
        iVar6 = (iVar4 + iVar3 * 4) * 0xc;
        iVar4 = *(int *)(&DAT_00510d20 + iVar6);
        iVar6 = *(int *)(&DAT_00510d24 + iVar6);
        lVar11 = __ftol();
        lVar12 = __ftol();
        fVar10 = DistanceBetweenIntegerPoints(iVar4,iVar6,(int)lVar11,(int)lVar12);
        if (fVar10 < (float10)_DAT_0043b460) {
LAB_0041bf33:
          piVar8[1] = 0;
          DAT_00510ba8 = DAT_00510ba8 + -1;
          DAT_005109e8 = 1;
          DAT_005109ec = 3;
        }
      }
      else if ((iVar4 != -1) && (piVar8[1] == 3)) {
        iVar4 = piVar8[3];
        iVar6 = piVar8[4];
        lVar11 = __ftol();
        lVar12 = __ftol();
        fVar10 = DistanceBetweenIntegerPoints(iVar4,iVar6,(int)lVar11,(int)lVar12);
        if (fVar10 < (float10)_DAT_0043b460) goto LAB_0041bf33;
      }
    }
    piVar8 = piVar8 + 5;
    local_8 = (float)((int)local_8 + -1);
    if (local_8 == 0.0) {
      return;
    }
  } while( true );
}

