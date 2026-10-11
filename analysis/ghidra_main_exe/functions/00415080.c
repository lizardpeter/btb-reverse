/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00415080; function: UpdateGolfGameplay; body bytes: 1953
 * callers: 1; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int UpdateGolfGameplay(void)

{
  int iVar1;
  int iVar2;
  float fVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  float10 fVar8;
  longlong lVar9;
  undefined4 uVar10;
  float local_14;
  float local_10;
  int local_c [3];
  
  iVar6 = 0;
  if (DAT_0050ad9c == 0) {
    if (DAT_004fbfb4 != 0) {
      DAT_0050ad9c = 1;
      DAT_004437c8 = -1;
      DAT_0050ac1c = 0xffffffff;
      DAT_0050ac20 = 0xffffffff;
    }
    DAT_0050ada8 = 0x58 - DAT_004fbd24 / 7;
    if (DAT_0050ada8 < 0) {
      DAT_0050ada8 = 0;
    }
    else if (0x57 < DAT_0050ada8) {
      DAT_0050ada8 = 0x58;
    }
    if ((DAT_004fbe5c & 10) != 0) {
      iVar6 = DAT_0050ada8 + -1;
      if (-1 < iVar6) {
        DAT_0050ada8 = iVar6;
        return iVar6;
      }
      DAT_0050ada8 = 0;
      return iVar6;
    }
    if ((DAT_004fbe5c & 5) == 0) {
      return DAT_0050ada8;
    }
    iVar6 = DAT_0050ada8 + 1;
    if (iVar6 < 0x58) {
      DAT_0050ada8 = iVar6;
      return iVar6;
    }
    DAT_0050ada8 = 0x58;
    return iVar6;
  }
  if (DAT_0050ad9c == 1) {
    DAT_0050abf4 = DAT_0050abf4 + *(int *)(&DAT_0050adf0 + DAT_0051c284 * 4) * DAT_0050ae54;
    if (DAT_0050abf4 < 0x3e9) {
      if (-1 < DAT_0050abf4) goto LAB_00415194;
      DAT_0050abf4 = 0;
    }
    else {
      DAT_0050abf4 = 1000;
    }
    DAT_0050ae54 = -DAT_0050ae54;
LAB_00415194:
    if (DAT_004fbe54 == 0) {
      return DAT_0050ae54;
    }
    DAT_0050aefc = 0;
    DAT_0050ad9c = 2;
    return DAT_0050ae54;
  }
  if (DAT_0050ad9c == 2) {
    DAT_0050ae50 = 0;
    DAT_0050ae38 = 1;
    DAT_0050ae14 = DAT_0050abf4 / 3 + 500;
    DAT_0050abcc = ((DAT_0050abf4 / 2 + 500) * 0x10) / 1000;
    DAT_0050ac04 = DAT_0050abcc;
    lVar9 = __ftol();
    DAT_0050ab94 = (int)lVar9 << 1;
    DAT_0050ade8 = 0;
    DAT_0050ad9c = 99;
    return DAT_0050ab94;
  }
  if (DAT_0050ad9c == 99) {
    DAT_0050ade8 = DAT_0050ade8 + 1;
    if (DAT_0050ade8 < 9) {
      return DAT_0050ade8;
    }
    DAT_0050ade8 = 0;
    DAT_0050ad90 = DAT_0050ad90 + 1;
    if (DAT_0050ad90 < 8) {
      DAT_0050ade8 = 0;
      return DAT_0050ad90;
    }
    DAT_0050ad9c = 3;
    return DAT_0050ad90;
  }
  if (DAT_0050ad9c == 3) {
    DAT_0050ade8 = DAT_0050ade8 + 1;
    if (8 < DAT_0050ade8) {
      DAT_0050ad90 = DAT_0050ad90 + 1;
      DAT_0050ade8 = 0;
      if (DAT_0050adcc <= DAT_0050ad90) {
        DAT_0050ad90 = DAT_0050adcc + -1;
      }
    }
    if (DAT_0044de0c == 0) {
      uVar4 = DAT_0050af08 & 0x80000001;
      if ((int)uVar4 < 0) {
        uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
      }
      iVar6 = DAT_0050af08 + 1;
    }
    else {
      iVar6 = (int)DAT_0050af08 / 10;
      uVar4 = (int)DAT_0050af08 % 10;
    }
    if (uVar4 == 0) {
      DAT_0050af08 = DAT_0050af08 + 1;
      return iVar6;
    }
    DAT_0050af08 = DAT_0050af08 + 1;
    VectorFromDegreesAndMagnitude(&local_14,&local_10,DAT_0050ab94,DAT_0050ae14 / 0x50);
    _DAT_0050ac08 = _DAT_0050ac08 + local_14;
    _DAT_0050ac0c = _DAT_0050ac0c - local_10;
    lVar9 = __ftol();
    iVar6 = DAT_0050aefc;
    iVar1 = (int)lVar9;
    if (iVar1 < 10) {
      iVar1 = 10;
    }
    DAT_0050ae14 = DAT_0050ae14 - iVar1;
    if (DAT_0050ae14 < 0x14) {
      DAT_0050ae14 = 0;
      DAT_0050ad9c = DAT_0050ad9c + 1;
    }
    if ((0 < DAT_0050ae50) || (iVar1 = DAT_0050ac04, 0 < DAT_0050ac04)) {
      if (DAT_0050ae38 != 1) {
        iVar1 = DAT_0050ac04;
        if (8 < DAT_0050ac04) {
          iVar1 = 8;
        }
        DAT_0050ae50 = DAT_0050ae50 - iVar1;
        if (0 < DAT_0050ae50) {
          return iVar1;
        }
        lVar9 = __ftol();
        *(int *)(&DAT_0050ad20 + iVar6 * 8) = (int)lVar9;
        lVar9 = __ftol();
        *(int *)(&DAT_0050ad24 + iVar6 * 8) = (int)lVar9;
        DAT_0050aefc = iVar6 + 1;
        DAT_0050abcc = DAT_0050abcc - DAT_0050abcc / 3;
        DAT_0050ae50 = 0;
        DAT_0050ae38 = 1;
        DAT_0050ac04 = DAT_0050abcc;
        if (DAT_0050abcc < 3) {
          DAT_0050ac04 = 0;
        }
        goto LAB_004153fc;
      }
      iVar6 = DAT_0050ac04;
      if (8 < DAT_0050ac04) {
        iVar6 = 8;
      }
      DAT_0050ae50 = DAT_0050ae50 + iVar6;
      iVar1 = DAT_0050ac04 + -1;
      DAT_0050ac04 = iVar1;
      if (iVar1 == 0) {
        DAT_0050ae38 = -1;
        DAT_0050ac04 = DAT_0050abcc;
      }
    }
    if (DAT_0050ae50 != 0) {
      return iVar1;
    }
LAB_004153fc:
    iVar6 = 0;
    do {
      iVar2 = *(int *)((int)&DAT_0050adb8 + iVar6) + *(int *)((int)&DAT_0050abb8 + iVar6);
      iVar5 = *(int *)((int)&DAT_0050ac34 + iVar6) + *(int *)((int)&DAT_0050abb4 + iVar6);
      lVar9 = __ftol();
      iVar1 = (int)lVar9;
      lVar9 = __ftol();
      DistanceBetweenIntegerPoints((int)lVar9,iVar1,iVar5,iVar2);
      lVar9 = __ftol();
      iVar1 = (int)lVar9;
      if (iVar1 < 0xf) {
        DAT_0050ad9c = DAT_0050ad9c + 1;
        iVar1 = DAT_0050af00 + 1;
        DAT_0050ae14 = 0;
        DAT_0050af00 = iVar1;
      }
      iVar6 = iVar6 + 8;
    } while (iVar6 < 0x18);
    return iVar1;
  }
  if (DAT_0050ad9c == 4) {
    DAT_0050ad90 = 0;
    DAT_004437c8 = -1;
    do {
      iVar1 = (&DAT_0050abb8)[iVar6 * 2] + (&DAT_0050adb8)[iVar6 * 2];
      iVar5 = (&DAT_0050ac34)[iVar6 * 2] + (&DAT_0050abb4)[iVar6 * 2];
      lVar9 = __ftol();
      iVar2 = (int)lVar9;
      lVar9 = __ftol();
      fVar8 = DistanceBetweenIntegerPoints((int)lVar9,iVar2,iVar5,iVar1);
      if (fVar8 < (float10)_DAT_0043b3a8) {
        local_10 = (float)((&DAT_0050abb8)[iVar6 * 2] + (&DAT_0050adb8)[iVar6 * 2]);
        _DAT_0050ac08 = (float)(int)((&DAT_0050ac34)[iVar6 * 2] + (&DAT_0050abb4)[iVar6 * 2]);
        _DAT_0050ac0c = (float)(int)local_10;
        DAT_004437c8 = iVar6;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < 3);
    DAT_0050ad9c = DAT_0050ad9c + 1;
    return DAT_0050ad9c;
  }
  if (DAT_0050ad9c != 5) {
    if (DAT_0050ad9c != 6) {
      return DAT_0050ad9c;
    }
    DAT_0050abb0 = DAT_0050abb0 + -1;
    DAT_0050abf4 = 0;
    _DAT_0050ac08 = (float)DAT_0050ad80;
    _DAT_0050ac0c = (float)DAT_0050ad7c;
    DAT_0050ad9c = 0;
    DAT_0050ae54 = 1;
    return DAT_0050abb0;
  }
  if (DAT_004437c8 != -1) {
    DAT_0050af04 = DAT_0050af04 + 1 + DAT_004437c8 * 2;
  }
  if (DAT_0050abb0 == 1) {
    iVar6 = 1;
    uVar10 = 0x32;
    if (DAT_0050af04 < 5) {
      iVar1 = 0x87;
    }
    else {
      uVar4 = FUN_0042ffc4();
      uVar4 = uVar4 & 0x80000001;
      if ((int)uVar4 < 0) {
        uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
      }
      iVar1 = uVar4 + 0x85;
    }
    goto LAB_004157bd;
  }
  if (DAT_004437c8 != -1) {
    iVar6 = 1;
    uVar10 = 0x32;
    uVar4 = FUN_0042ffc4();
    iVar1 = (int)uVar4 % 0xc + 0x66;
    goto LAB_004157bd;
  }
  iVar6 = DAT_0050ad80;
  iVar1 = DAT_0050ad7c;
  lVar9 = __ftol();
  iVar2 = (int)lVar9;
  lVar9 = __ftol();
  DistanceBetweenIntegerPoints((int)lVar9,iVar2,iVar6,iVar1);
  if (DAT_0050ada8 < DAT_004437d8) {
    iVar6 = 0;
  }
  else {
    iVar6 = (DAT_0050ada8 < DAT_004437dc) + 1;
  }
  lVar9 = __ftol();
  iVar1 = (int)lVar9;
  lVar9 = __ftol();
  fVar8 = DistanceBetweenIntegerPoints
                    ((&DAT_0050ac34)[iVar6 * 2] + (&DAT_0050abb4)[iVar6 * 2],
                     (&DAT_0050adb8)[iVar6 * 2] + (&DAT_0050abb8)[iVar6 * 2],(int)lVar9,iVar1);
  local_10 = (float)((&DAT_0050ac34)[iVar6 * 2] + (&DAT_0050abb4)[iVar6 * 2]);
  fVar3 = 1.4013e-45;
  if (_DAT_0050ac08 <= (float)(int)local_10) {
    fVar3 = local_10;
  }
  local_c[0] = 0;
  local_c[1] = 0x3c;
  local_c[2] = 0x22;
  uVar4 = local_c[iVar6] - DAT_0050ada8 >> 0x1f;
  iVar1 = (local_c[iVar6] - DAT_0050ada8 ^ uVar4) - uVar4;
  if (iVar6 == 2) {
    if (6 < iVar1) {
LAB_00415750:
      uVar4 = FUN_0042ffc4();
      uVar4 = uVar4 & 0x80000001;
      bVar7 = uVar4 == 0;
      if ((int)uVar4 < 0) {
        bVar7 = (uVar4 - 1 | 0xfffffffe) == 0xffffffff;
      }
      iVar6 = 1;
      uVar10 = 0x32;
      if (bVar7) {
        iVar1 = 0x75;
      }
      else {
        iVar1 = 0x72;
      }
      goto LAB_004157bd;
    }
  }
  else if (((iVar6 == 0) || (iVar6 == 1)) && (10 < iVar1)) goto LAB_00415750;
  if ((float10)_DAT_0043b3a4 <= fVar8) {
    iVar6 = 1;
    uVar10 = 0x32;
    if (fVar3 == 1.4013e-45) {
      uVar4 = FUN_0042ffc4();
      uVar4 = uVar4 & 0x80000001;
      if ((int)uVar4 < 0) {
        uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
      }
      iVar1 = uVar4 + 0x83;
    }
    else {
      uVar4 = FUN_0042ffc4();
      uVar4 = uVar4 & 0x80000001;
      if ((int)uVar4 < 0) {
        uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
      }
      iVar1 = uVar4 + 0x81;
    }
  }
  else {
    iVar6 = 1;
    uVar10 = 0x32;
    uVar4 = FUN_0042ffc4();
    uVar4 = uVar4 & 0x80000003;
    if ((int)uVar4 < 0) {
      uVar4 = (uVar4 - 1 | 0xfffffffc) + 1;
    }
    iVar1 = uVar4 + 0x77;
  }
LAB_004157bd:
  PlayManagedSoundById(DAT_0044ddd8,iVar1,uVar10,iVar6);
  DAT_0050aefc = 0;
  DAT_0050ad9c = DAT_0050ad9c + 1;
  return DAT_0050ad9c;
}

