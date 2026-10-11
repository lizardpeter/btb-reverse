/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00416a30; function: UpdateHerdingAnimal; body bytes: 6141
 * callers: 1; callees: 8; success: True
 */


/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl UpdateHerdingAnimal(int param_1)

{
  float fVar1;
  int iVar2;
  int *piVar3;
  bool bVar4;
  int *piVar5;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  uint uVar6;
  undefined3 extraout_var_01;
  undefined3 extraout_var_02;
  undefined3 extraout_var_03;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  int iVar12;
  int iVar13;
  int *piVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int *piVar18;
  bool bVar19;
  float10 fVar20;
  float10 fVar21;
  float10 extraout_ST0;
  unkbyte10 extraout_ST0_00;
  float10 extraout_ST0_01;
  longlong lVar22;
  longlong lVar23;
  undefined4 uVar24;
  int iVar25;
  int *local_64;
  float local_50;
  int local_48;
  int *local_44;
  int local_40;
  int local_3c;
  int local_38 [7];
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar2 = param_1;
  iVar12 = DAT_0050b3b4;
  local_38[3] = 3;
  local_38[4] = 7;
  iVar15 = param_1 * 100;
  local_38[5] = 9;
  local_38[0] = 6;
  iVar7 = (&DAT_0050b3d8)[param_1 * 0x19];
  iVar10 = (int)((&DAT_0050b3dc)[param_1 * 0x19] - (&DAT_0050b3e4)[param_1 * 0x19]) / 2 +
           (&DAT_0050b3b4)[param_1 * 0x19];
  local_38[1] = 10;
  iVar13 = ((&DAT_0050b3e8)[param_1 * 0x19] - (&DAT_0050b3e0)[param_1 * 0x19]) / 2 +
           (&DAT_0050b3b8)[param_1 * 0x19];
  iVar16 = iVar7 + -1;
  iVar17 = (DAT_0050b3e8 - DAT_0050b3e0) + -0x14 + DAT_0050b3b8;
  local_38[2] = 0xc;
  if (iVar16 == 0) {
    local_40 = 2;
  }
  else {
    local_40 = iVar16;
    if (iVar16 == 2) {
      local_40 = 0;
    }
  }
  fVar20 = DistanceBetweenIntegerPoints(iVar10,iVar13,DAT_0050b3b4,iVar17);
  local_50 = (float)fVar20;
  bVar19 = false;
  if (0 < *(int *)(&DAT_0050b400 + iVar15)) {
    *(int *)(&DAT_0050b400 + iVar15) = *(int *)(&DAT_0050b400 + iVar15) + -1;
  }
  iVar25 = (&DAT_0050b3f8)[param_1 * 0x19];
  if (iVar25 != 0) {
    iVar10 = (&DAT_0050b3b4)[param_1 * 0x19] - (&DAT_00443b08)[iVar16 * 2];
    iVar13 = (&DAT_0050b3b8)[param_1 * 0x19] + 0x28;
  }
  switch(iVar25) {
  case 1:
    iVar8 = DAT_0051c284 + 3;
    iVar25 = *(int *)(&DAT_0050afd0 + iVar16 * 4) + 1;
    (&DAT_0050b3f8)[param_1 * 0x19] = *(int *)(&DAT_0050afd0 + iVar16 * 4) + 10;
    *(int *)(&DAT_0050afd0 + iVar16 * 4) = iVar25;
    if (iVar25 == iVar8) {
      PlayManagedSoundById(DAT_0044ddd8,iVar7 + 0x253,0x5a,1);
      if (iVar16 == 0) {
        (&DAT_0050b3d4)[DAT_0050b39c * 0x19] = 0x32;
      }
      else if (iVar16 == 1) {
        (&DAT_0050b3d4)[DAT_0050b3a0 * 0x19] = 0x32;
      }
    }
    break;
  case 10:
  case 0xb:
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
    iVar7 = *(int *)(&DAT_00510600 + iVar16 * 8);
    iVar25 = *(int *)(&DAT_00510604 + iVar16 * 8);
    lVar22 = AngleBetweenIntegerPointsDegrees(iVar10,iVar13,iVar7 + -0x28,iVar25);
    iVar8 = ((int)lVar22 + 0x16) / 0x2d;
    (&DAT_0050b3b0)[param_1 * 0x19] = iVar8;
    if (7 < iVar8) {
      (&DAT_0050b3b0)[param_1 * 0x19] = iVar8 + -8;
    }
    fVar20 = (float10)(int)lVar22 * (float10)_DAT_0043b384;
    fVar21 = (float10)fsin(fVar20);
    (&DAT_0050b3c8)[param_1 * 0x19] =
         (float)(fVar21 * (float10)(float)(&DAT_0050b3f4)[param_1 * 0x19] +
                (float10)(float)(&DAT_0050b3c8)[param_1 * 0x19]);
    fVar20 = (float10)fcos(fVar20);
    (&DAT_0050b3cc)[param_1 * 0x19] =
         (float)((float10)(float)(&DAT_0050b3cc)[param_1 * 0x19] -
                fVar20 * (float10)(float)(&DAT_0050b3f4)[param_1 * 0x19]);
    lVar22 = __ftol();
    (&DAT_0050b3b4)[param_1 * 0x19] = (int)lVar22;
    lVar22 = __ftol();
    (&DAT_0050b3b8)[param_1 * 0x19] = (int)lVar22;
    (&DAT_0050b3f0)[param_1 * 0x19] = 1;
    if ((float)(&DAT_0050b3f4)[param_1 * 0x19] < _DAT_0043b438) {
      (&DAT_0050b3f4)[param_1 * 0x19] = (float)(&DAT_0050b3f4)[param_1 * 0x19] + _DAT_0043b434;
    }
    iVar8 = (&DAT_0050b3d4)[param_1 * 0x19];
    (&DAT_0050b3d4)[param_1 * 0x19] = iVar8 + -10;
    if (iVar8 + -10 < 1) {
      if (iVar16 == 2) {
        iVar8 = (&DAT_0050b3ac)[param_1 * 0x19];
        (&DAT_0050b3ac)[param_1 * 0x19] = iVar8 + 1;
        if (iVar8 + 1 < 9) {
          (&DAT_0050b3ac)[param_1 * 0x19] = 9;
        }
        if (0xb < (int)(&DAT_0050b3ac)[param_1 * 0x19]) {
          (&DAT_0050b3ac)[param_1 * 0x19] = 9;
        }
      }
      else {
        iVar8 = *(int *)(&DAT_00443afc + iVar16 * 4);
        (&DAT_0050b3d4)[param_1 * 0x19] = 100;
        iVar9 = (&DAT_0050b3ac)[param_1 * 0x19];
        (&DAT_0050b3ac)[param_1 * 0x19] = iVar9 + 1;
        if (iVar8 <= iVar9 + 1) {
          (&DAT_0050b3ac)[param_1 * 0x19] = *(undefined4 *)(&DAT_00443af0 + iVar16 * 4);
        }
      }
    }
    fVar20 = DistanceBetweenIntegerPoints(iVar10,iVar13,iVar7 + -0x28,iVar25);
    if (fVar20 < (float10)_DAT_0043b378) {
      (&DAT_0050b3f8)[param_1 * 0x19] = (&DAT_0050b3f8)[param_1 * 0x19] + 10;
    }
    break;
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
  case 0x19:
    iVar25 = iVar25 + (iVar7 + -5) * 5;
    iVar7 = (&DAT_0050b320)[iVar25 * 2];
    iVar25 = (&DAT_0050b324)[iVar25 * 2];
    lVar22 = AngleBetweenIntegerPointsDegrees(iVar10,iVar13,iVar7,iVar25);
    iVar8 = ((int)lVar22 + 0x16) / 0x2d;
    (&DAT_0050b3b0)[param_1 * 0x19] = iVar8;
    if (7 < iVar8) {
      (&DAT_0050b3b0)[param_1 * 0x19] = iVar8 + -8;
    }
    fVar20 = (float10)(int)lVar22 * (float10)_DAT_0043b384;
    fVar21 = (float10)fsin(fVar20);
    (&DAT_0050b3c8)[param_1 * 0x19] =
         (float)(fVar21 * (float10)(float)(&DAT_0050b3f4)[param_1 * 0x19] +
                (float10)(float)(&DAT_0050b3c8)[param_1 * 0x19]);
    fVar20 = (float10)fcos(fVar20);
    (&DAT_0050b3cc)[param_1 * 0x19] =
         (float)((float10)(float)(&DAT_0050b3cc)[param_1 * 0x19] -
                fVar20 * (float10)(float)(&DAT_0050b3f4)[param_1 * 0x19]);
    lVar22 = __ftol();
    (&DAT_0050b3b4)[param_1 * 0x19] = (int)lVar22;
    lVar22 = __ftol();
    (&DAT_0050b3b8)[param_1 * 0x19] = (int)lVar22;
    (&DAT_0050b3f0)[param_1 * 0x19] = 1;
    if ((float)(&DAT_0050b3f4)[param_1 * 0x19] < _DAT_0043b438) {
      (&DAT_0050b3f4)[param_1 * 0x19] = (float)(&DAT_0050b3f4)[param_1 * 0x19] + _DAT_0043b434;
    }
    iVar8 = (&DAT_0050b3d4)[param_1 * 0x19];
    (&DAT_0050b3d4)[param_1 * 0x19] = iVar8 + -5;
    if (iVar8 + -5 < 1) {
      (&DAT_0050b3d4)[param_1 * 0x19] = 100;
      if (iVar16 == 2) {
        iVar8 = (&DAT_0050b3ac)[param_1 * 0x19] + 1;
        (&DAT_0050b3ac)[param_1 * 0x19] = iVar8;
        if (iVar8 < 9) {
          (&DAT_0050b3ac)[param_1 * 0x19] = 9;
        }
        if (0xb < (int)(&DAT_0050b3ac)[param_1 * 0x19]) {
          (&DAT_0050b3ac)[param_1 * 0x19] = 9;
        }
      }
      else {
        iVar8 = *(int *)(&DAT_00443afc + iVar16 * 4);
        iVar9 = (&DAT_0050b3ac)[param_1 * 0x19] + 1;
        (&DAT_0050b3ac)[param_1 * 0x19] = iVar9;
        if (iVar8 <= iVar9) {
          (&DAT_0050b3ac)[param_1 * 0x19] = *(undefined4 *)(&DAT_00443af0 + iVar16 * 4);
        }
      }
    }
    fVar20 = DistanceBetweenIntegerPoints(iVar10,iVar13,iVar7,iVar25);
    if (fVar20 < (float10)_DAT_0043b378) {
      (&DAT_0050b3f8)[param_1 * 0x19] = 99;
      DAT_00510764 = DAT_00510764 + -1;
    }
    break;
  case 99:
    (&DAT_0050b3b0)[param_1 * 0x19] = 4;
    iVar25 = (&DAT_0050b3d4)[param_1 * 0x19];
    (&DAT_0050b3d4)[param_1 * 0x19] = iVar25 + -5;
    if (iVar25 + -5 < 1) {
      iVar25 = local_38[iVar16];
      (&DAT_0050b3d4)[param_1 * 0x19] = 0x96;
      iVar8 = (&DAT_0050b3ac)[param_1 * 0x19];
      (&DAT_0050b3ac)[param_1 * 0x19] = iVar8 + 1;
      if (iVar25 <= iVar8 + 1) {
        (&DAT_0050b3ac)[param_1 * 0x19] = local_38[iVar7 + 2];
      }
    }
  }
  if ((&DAT_0050b3f8)[param_1 * 0x19] != 0) {
    return 0;
  }
  if (0 < *(int *)(&DAT_0050b400 + iVar15)) {
    (&DAT_0050b3b4)[param_1 * 0x19] = (&DAT_0050b3bc)[param_1 * 0x19];
    (&DAT_0050b3b8)[param_1 * 0x19] = (&DAT_0050b3c0)[param_1 * 0x19];
    iVar17 = *(int *)(&DAT_0050b408 + iVar15);
    iVar7 = *(int *)(&DAT_0050b404 + iVar15);
    lVar22 = AngleBetweenIntegerPointsDegrees
                       ((int)((&DAT_0050b3dc)[param_1 * 0x19] - (&DAT_0050b3e4)[param_1 * 0x19]) / 2
                        + (&DAT_0050b3b4)[param_1 * 0x19],
                        ((&DAT_0050b3e8)[param_1 * 0x19] - (&DAT_0050b3e0)[param_1 * 0x19]) / 2 +
                        (&DAT_0050b3c0)[param_1 * 0x19],iVar7,iVar17);
    iVar12 = ((int)lVar22 + 0x16) / 0x2d;
    (&DAT_0050b3b0)[param_1 * 0x19] = iVar12;
    if (7 < iVar12) {
      (&DAT_0050b3b0)[param_1 * 0x19] = iVar12 + -8;
    }
    fVar20 = (float10)(int)lVar22 * (float10)_DAT_0043b384;
    fVar21 = (float10)fsin(fVar20);
    (&DAT_0050b3c8)[param_1 * 0x19] =
         (float)(fVar21 * (float10)_DAT_0043b430 + (float10)(float)(&DAT_0050b3c8)[param_1 * 0x19]);
    fVar20 = (float10)fcos(fVar20);
    (&DAT_0050b3cc)[param_1 * 0x19] =
         (float)((float10)(float)(&DAT_0050b3cc)[param_1 * 0x19] - fVar20 * (float10)_DAT_0043b430);
    lVar22 = __ftol();
    (&DAT_0050b3b4)[param_1 * 0x19] = (int)lVar22;
    lVar22 = __ftol();
    (&DAT_0050b3b8)[param_1 * 0x19] = (int)lVar22;
    (&DAT_0050b3f4)[param_1 * 0x19] = 0x40400000;
    fVar20 = DistanceBetweenIntegerPoints
                       ((&DAT_0050b3b4)[param_1 * 0x19],(&DAT_0050b3b8)[param_1 * 0x19],iVar7,iVar17
                       );
    if ((float10)_DAT_0043b42c <= fVar20) {
      return 0;
    }
    *(undefined4 *)(&DAT_0050b400 + iVar15) = 0;
    return 0;
  }
  if ((0 < (int)(&DAT_0050b3d8)[param_1 * 0x19]) || ((int)(&DAT_0050b3d8)[param_1 * 0x19] < 4)) {
    piVar5 = &DAT_0050af14;
    do {
      if (*piVar5 == param_1) goto LAB_0041721e;
      piVar5 = piVar5 + 1;
    } while ((int)piVar5 < 0x50af64);
    lVar22 = __ftol();
    lVar23 = __ftol();
    bVar4 = PointInPolygon(&DAT_005104e8,DAT_0050b398,(int)lVar22,(int)lVar23);
    iVar7 = CONCAT31(extraout_var,bVar4);
    while (iVar7 != 0) {
      (&DAT_0050b3b4)[param_1 * 0x19] = (&DAT_0050b3bc)[param_1 * 0x19];
      (&DAT_0050b3b8)[param_1 * 0x19] = (&DAT_0050b3c0)[param_1 * 0x19];
      lVar22 = AngleBetweenIntegerPointsDegrees
                         ((int)((&DAT_0050b3dc)[param_1 * 0x19] - (&DAT_0050b3e4)[param_1 * 0x19]) /
                          2 + (&DAT_0050b3b4)[param_1 * 0x19],
                          ((&DAT_0050b3e8)[param_1 * 0x19] - (&DAT_0050b3e0)[param_1 * 0x19]) / 2 +
                          (&DAT_0050b3c0)[param_1 * 0x19],0x1e6,0x28a);
      iVar7 = ((int)lVar22 + 0x16) / 0x2d;
      (&DAT_0050b3b0)[param_1 * 0x19] = iVar7;
      if (7 < iVar7) {
        (&DAT_0050b3b0)[param_1 * 0x19] = iVar7 + -8;
      }
      fVar20 = (float10)(int)lVar22 * (float10)_DAT_0043b384;
      fVar21 = (float10)fsin(fVar20);
      (&DAT_0050b3c8)[param_1 * 0x19] =
           (float)(fVar21 * (float10)_DAT_0043b430 + (float10)(float)(&DAT_0050b3c8)[param_1 * 0x19]
                  );
      fVar20 = (float10)fcos(fVar20);
      (&DAT_0050b3cc)[param_1 * 0x19] =
           (float)((float10)(float)(&DAT_0050b3cc)[param_1 * 0x19] - fVar20 * (float10)_DAT_0043b430
                  );
      lVar22 = __ftol();
      (&DAT_0050b3b4)[param_1 * 0x19] = (int)lVar22;
      lVar22 = __ftol();
      (&DAT_0050b3b8)[param_1 * 0x19] = (int)lVar22;
      (&DAT_0050b3f4)[param_1 * 0x19] = 0;
      lVar22 = __ftol();
      lVar23 = __ftol();
      bVar4 = PointInPolygon(&DAT_005104e8,DAT_0050b398,(int)lVar22,(int)lVar23);
      iVar7 = CONCAT31(extraout_var_00,bVar4);
    }
  }
LAB_0041721e:
  piVar5 = &DAT_0050af78;
  do {
    if (*piVar5 == param_1) {
      iVar17 = *(int *)(&DAT_0050b2f4 + (&DAT_0050b3d8)[param_1 * 0x19] * 8);
      iVar7 = *(int *)(&DAT_0050b2f8 + (&DAT_0050b3d8)[param_1 * 0x19] * 8);
      fVar20 = DistanceBetweenIntegerPoints(iVar10,iVar13,iVar17,iVar7);
      if ((float10)_DAT_0043b424 <= fVar20) {
        AngleBetweenIntegerPointsDegrees(iVar10,iVar13,iVar17,iVar7);
        lVar22 = __ftol();
        iVar17 = (int)lVar22;
        (&DAT_0050b3b0)[param_1 * 0x19] = iVar17;
        if (7 < iVar17) {
          (&DAT_0050b3b0)[param_1 * 0x19] = iVar17 + -8;
        }
        fVar20 = (float10)_DAT_0043b384;
        fVar21 = (float10)fsin(extraout_ST0 * fVar20);
        (&DAT_0050b3c8)[param_1 * 0x19] =
             (float)(fVar21 * (float10)(float)(&DAT_0050b3f4)[param_1 * 0x19] +
                    (float10)(float)(&DAT_0050b3c8)[param_1 * 0x19]);
        fVar20 = (float10)fcos(extraout_ST0 * fVar20);
        (&DAT_0050b3cc)[param_1 * 0x19] =
             (float)((float10)(float)(&DAT_0050b3cc)[param_1 * 0x19] -
                    fVar20 * (float10)(float)(&DAT_0050b3f4)[param_1 * 0x19]);
        lVar22 = __ftol();
        (&DAT_0050b3b4)[param_1 * 0x19] = (int)lVar22;
        lVar22 = __ftol();
        (&DAT_0050b3b8)[param_1 * 0x19] = (int)lVar22;
        (&DAT_0050b3f0)[param_1 * 0x19] = 1;
        if ((float)(&DAT_0050b3f4)[param_1 * 0x19] < _DAT_0043b438) {
          (&DAT_0050b3f4)[param_1 * 0x19] = (float)(&DAT_0050b3f4)[param_1 * 0x19] + _DAT_0043b434;
        }
        iVar17 = (&DAT_0050b3d4)[param_1 * 0x19];
        (&DAT_0050b3d4)[param_1 * 0x19] = iVar17 + -5;
        if (0 < iVar17 + -5) {
          return 0;
        }
        (&DAT_0050b3d4)[param_1 * 0x19] = 100;
        iVar17 = (&DAT_0050b3ac)[param_1 * 0x19];
        (&DAT_0050b3ac)[param_1 * 0x19] = iVar17 + 1;
        if (iVar17 + 1 < 3) {
          return 0;
        }
        (&DAT_0050b3ac)[param_1 * 0x19] = 0;
        return 0;
      }
      iVar17 = 0;
      piVar5 = &DAT_0050af78;
      goto LAB_004172ed;
    }
    piVar5 = piVar5 + 1;
  } while ((int)piVar5 < 0x50afc8);
  local_44 = &DAT_0050af14;
  do {
    if (*local_44 == param_1) {
      bVar19 = true;
      fVar20 = DistanceBetweenIntegerPoints
                         (iVar10,iVar13,
                          *(int *)(&DAT_0050b2f4 + (&DAT_0050b3d8)[param_1 * 0x19] * 8),
                          *(int *)(&DAT_0050b2f8 + (&DAT_0050b3d8)[param_1 * 0x19] * 8));
      if ((float10)_DAT_0043b428 <= fVar20) {
        iVar7 = 0;
        local_48 = 0;
        if ((&DAT_0050b3f8)[param_1 * 0x19] == 0) {
          local_64 = &DAT_00510778;
          local_3c = 2;
          piVar5 = (int *)&DAT_0050b300;
          piVar18 = (int *)&DAT_0050b2fc;
          iVar25 = iVar7;
          do {
            iVar7 = iVar25 + 1;
            piVar14 = piVar18 + 2;
            piVar11 = piVar5 + 2;
            piVar3 = local_64 + 1;
            if (iVar7 == (&DAT_0050b3d8)[param_1 * 0x19] + -1) {
              iVar7 = iVar25 + 2;
              piVar14 = piVar18 + 4;
              piVar11 = piVar5 + 4;
              piVar3 = local_64 + 2;
            }
            local_64 = piVar3;
            fVar20 = DistanceBetweenIntegerPoints(iVar10,iVar13,*piVar14,*piVar11);
            if (fVar20 < (float10)_DAT_0043b41c) {
              iVar25 = iVar7 + 0x3a9;
              if (iVar7 == 3) {
                iVar25 = 0x3a9;
              }
              if (*local_64 == 0) {
                PlayManagedSoundById(DAT_0044ddd8,iVar25,10,1);
              }
              local_48 = local_48 + 1;
              *local_64 = 1;
            }
            local_3c = local_3c + -1;
            piVar5 = piVar11;
            piVar18 = piVar14;
            iVar25 = iVar7;
          } while (local_3c != 0);
          if (local_48 != 0) goto LAB_00417517;
        }
        (&DAT_00510778)[iVar7] = 0;
      }
      else {
        iVar7 = 0;
        piVar5 = &DAT_0050af78;
        do {
          if (*piVar5 == -1) {
            DAT_0050b31c = DAT_0050b31c + -1;
            (&DAT_0050af78)[iVar7] = param_1;
            if (DAT_0050b31c != 0) {
              return 0;
            }
            DAT_00510718 = 0xffffffff;
            return 0;
          }
          piVar5 = piVar5 + 1;
          iVar7 = iVar7 + 1;
        } while ((int)piVar5 < 0x50afc8);
      }
    }
LAB_00417517:
    local_44 = local_44 + 1;
  } while ((int)local_44 < 0x50af64);
  if ((_DAT_0043b418 <= local_50) && (!bVar19)) goto LAB_00417c85;
  if ((DAT_00510718 == local_40) && (!bVar19)) {
    bVar19 = true;
    iVar7 = 0;
    piVar5 = &DAT_0050af14;
    do {
      if (*piVar5 == -1) {
        iVar25 = 1;
        uVar24 = 10;
        (&DAT_0050af14)[iVar7] = param_1;
        uVar6 = FUN_0042ffc4();
        uVar6 = uVar6 & 0x80000001;
        if ((int)uVar6 < 0) {
          uVar6 = (uVar6 - 1 | 0xfffffffe) + 1;
        }
        PlayManagedSoundById(DAT_0044ddd8,uVar6 + 0x249 + DAT_00510718 * 2,uVar24,iVar25);
        break;
      }
      piVar5 = piVar5 + 1;
      iVar7 = iVar7 + 1;
    } while ((int)piVar5 < 0x50af64);
  }
  bVar4 = false;
  if (bVar19) {
    fVar20 = DistanceBetweenIntegerPoints
                       (iVar10,iVar13,DAT_0050b3b4 + -0x28,
                        (DAT_0050b3e8 - DAT_0050b3e0) + -0x28 + DAT_0050b3b8);
    iVar17 = DAT_00510770;
    local_50 = (float)fVar20;
    local_3c = DAT_00510770 * 0x1e + 0xf;
    lVar22 = __ftol();
    iVar7 = (int)lVar22;
    if (iVar7 < 0) {
      iVar7 = -iVar7;
    }
    if (iVar7 < 3) {
      (&DAT_0050b3b0)[param_1 * 0x19] = DAT_0050b3b0;
      iVar17 = iVar17 + 1;
      bVar4 = true;
      DAT_00510770 = iVar17;
      if ((float)(&DAT_0050b3f4)[param_1 * 0x19] < (float)_DAT_0043b400) {
        (&DAT_0050b3f4)[param_1 * 0x19] = 0;
      }
      else {
        (&DAT_0050b3f4)[param_1 * 0x19] =
             (float)(&DAT_0050b3f4)[param_1 * 0x19] - (float)_DAT_0043b400;
      }
    }
    else {
      local_3c = (DAT_0050b3b0 * 5 + 0x14) * 9;
      if (0x167 < local_3c) {
        local_3c = local_3c + -0x168;
      }
      fsin((float10)local_3c * (float10)_DAT_0043b384);
      lVar22 = __ftol();
      fcos(extraout_ST0_00);
      lVar23 = __ftol();
      lVar22 = AngleBetweenIntegerPointsDegrees(iVar10,iVar13,(int)lVar22,(int)lVar23);
      local_3c = (int)lVar22;
      iVar7 = (local_3c + 0x16) / 0x2d;
      if (7 < iVar7) {
        iVar7 = iVar7 + -8;
      }
      iVar17 = DAT_00510770 + 1;
      DAT_00510770 = iVar17;
      if (((&DAT_0050b3b0)[param_1 * 0x19] != iVar7) &&
         ((float)_DAT_0043b410 < (float)(&DAT_0050b3f4)[param_1 * 0x19])) {
        (&DAT_0050b3f4)[param_1 * 0x19] =
             (float)(&DAT_0050b3f4)[param_1 * 0x19] * (float)_DAT_0043b408;
      }
      (&DAT_0050b3b0)[param_1 * 0x19] = iVar7;
    }
  }
  else {
    lVar22 = AngleBetweenIntegerPointsDegrees(iVar12,iVar17,iVar10,iVar13);
    iVar17 = DAT_00510770;
    local_3c = (int)lVar22;
    (&DAT_0050b3b0)[param_1 * 0x19] = (local_3c + 0x16) / 0x2d;
  }
  if (7 < (&DAT_0050b3b0)[param_1 * 0x19]) {
    (&DAT_0050b3b0)[param_1 * 0x19] = (&DAT_0050b3b0)[param_1 * 0x19] + -8;
  }
  fVar20 = (float10)fsin((float10)local_3c * (float10)_DAT_0043b384);
  fVar21 = (float10)fcos((float10)local_3c * (float10)_DAT_0043b384);
  if (bVar19) {
    if (!bVar4) {
      local_50 = local_50 - (float)(iVar17 * 0x32 + 0xf);
      if ((local_50 < _DAT_0043b2f0) || (local_50 <= _DAT_0043b3f8)) {
        if ((float)_DAT_0043b400 <= (float)(&DAT_0050b3f4)[param_1 * 0x19]) {
          fVar1 = (float)(&DAT_0050b3f4)[param_1 * 0x19] - (float)_DAT_0043b400;
          goto LAB_004178d7;
        }
        (&DAT_0050b3f4)[param_1 * 0x19] = 0;
      }
      else if (_DAT_0043b3a8 <= local_50) {
        if ((float)(&DAT_0050b3f4)[param_1 * 0x19] < _DAT_0043b3ec) {
          fVar1 = (float)(&DAT_0050b3f4)[param_1 * 0x19] + _DAT_0043b3e8;
          goto LAB_004178d7;
        }
      }
      else if ((float)_DAT_0043b3f0 <= (float)(&DAT_0050b3f4)[param_1 * 0x19]) {
        fVar1 = (float)(&DAT_0050b3f4)[param_1 * 0x19] - (float)_DAT_0043b370;
LAB_004178d7:
        (&DAT_0050b3f4)[param_1 * 0x19] = fVar1;
      }
      (&DAT_0050b3c8)[param_1 * 0x19] =
           (float)(&DAT_0050b3f4)[param_1 * 0x19] * (float)fVar20 +
           (float)(&DAT_0050b3c8)[param_1 * 0x19];
      (&DAT_0050b3cc)[param_1 * 0x19] =
           (float)(&DAT_0050b3cc)[param_1 * 0x19] -
           (float)(&DAT_0050b3f4)[param_1 * 0x19] * (float)fVar21;
      lVar22 = __ftol();
      (&DAT_0050b3b4)[param_1 * 0x19] = (int)lVar22;
      lVar22 = __ftol();
      (&DAT_0050b3b8)[param_1 * 0x19] = (int)lVar22;
      lVar22 = __ftol();
      (&DAT_0050b3d4)[param_1 * 0x19] = (int)lVar22;
      if ((float)(&DAT_0050b3f4)[param_1 * 0x19] == (float)_DAT_0043b2e8) {
        if ((&DAT_0050b3d8)[param_1 * 0x19] == 1) {
          (&DAT_0050b3ac)[param_1 * 0x19] = *(int *)(&DAT_00443af0 + iVar16 * 4) + -1;
        }
        else {
          (&DAT_0050b3ac)[param_1 * 0x19] = *(undefined4 *)(&DAT_00443af0 + iVar16 * 4);
        }
        (&DAT_0050b3d4)[param_1 * 0x19] = 0x65;
      }
    }
    iVar17 = DAT_00510760;
    (&DAT_0050b3f0)[param_1 * 0x19] = 1;
    local_3c = 1;
    if (1 < iVar17) {
      piVar5 = &DAT_0050b444;
      do {
        if (piVar5[-2] == 7) {
          local_c = (&DAT_0050b3b8)[param_1 * 0x19];
          local_4 = (local_c - (&DAT_0050b3e0)[param_1 * 0x19]) + (&DAT_0050b3e8)[param_1 * 0x19];
          local_10 = (&DAT_0050b3b4)[param_1 * 0x19];
          local_8 = (local_10 - (&DAT_0050b3dc)[param_1 * 0x19]) + (&DAT_0050b3e4)[param_1 * 0x19];
          local_1c = piVar5[-10];
          local_14 = (piVar5[2] - *piVar5) + local_1c;
          local_38[6] = piVar5[-0xb];
          local_18 = (piVar5[1] - piVar5[-1]) + local_38[6];
          iVar17 = AnyRectCornerInsideRect(&local_10,local_38 + 6);
          if (iVar17 != 0) {
            piVar18 = &DAT_0050af14;
            do {
              if (*piVar18 == param_1) {
                *(undefined4 *)(&DAT_0050b400 + iVar15) = 200;
                uVar6 = FUN_0042ffc4();
                *piVar18 = -1;
                *(int *)(&DAT_0050b404 + iVar15) = (int)uVar6 % 400 + 0x11e;
                *(undefined4 *)(&DAT_0050b408 + iVar15) = 0x28a;
                uVar6 = FUN_0042ffc4();
                uVar6 = uVar6 & 0x80000001;
                bVar19 = uVar6 == 0;
                if ((int)uVar6 < 0) {
                  bVar19 = (uVar6 - 1 | 0xfffffffe) == 0xffffffff;
                }
                if (bVar19) {
                  uVar6 = FUN_0042ffc4();
                  uVar6 = uVar6 & 0x80000001;
                  bVar19 = uVar6 == 0;
                  if ((int)uVar6 < 0) {
                    bVar19 = (uVar6 - 1 | 0xfffffffe) == 0xffffffff;
                  }
                  iVar17 = 1;
                  uVar24 = 0x5a;
                  if (bVar19) {
                    uVar6 = FUN_0042ffc4();
                    uVar6 = uVar6 & 0x80000001;
                    if ((int)uVar6 < 0) {
                      uVar6 = (uVar6 - 1 | 0xfffffffe) + 1;
                    }
                    iVar7 = uVar6 + 0x250;
                  }
                  else {
                    iVar7 = 0x264;
                  }
                }
                else {
                  iVar17 = 1;
                  uVar24 = 0x5a;
                  uVar6 = FUN_0042ffc4();
                  uVar6 = uVar6 & 0x80000001;
                  if ((int)uVar6 < 0) {
                    uVar6 = (uVar6 - 1 | 0xfffffffe) + 1;
                  }
                  iVar7 = uVar6 + 0x266;
                }
                PlayManagedSoundById(DAT_0044ddd8,iVar7,uVar24,iVar17);
              }
              piVar18 = piVar18 + 1;
            } while ((int)piVar18 < 0x50af64);
          }
        }
        local_3c = local_3c + 1;
        piVar5 = piVar5 + 0x19;
      } while (local_3c < DAT_00510760);
    }
  }
  else {
    bVar19 = true;
    uVar6 = FUN_0042ffc4();
    uVar6 = uVar6 & 0x80000007;
    bVar4 = uVar6 == 0;
    if ((int)uVar6 < 0) {
      bVar4 = (uVar6 - 1 | 0xfffffff8) == 0xffffffff;
    }
    if (bVar4) {
      iVar7 = 0;
      uVar24 = 10;
      uVar6 = FUN_0042ffc4();
      iVar17 = (int)uVar6 % 3 + 0x331 + iVar16 * 3;
LAB_00417ba5:
      PlayManagedSoundById(DAT_0044ddd8,iVar17,uVar24,iVar7);
    }
    else {
      iVar7 = 3;
      iVar17 = iVar16 * 3 + 0x331;
      do {
        bVar4 = IsSoundIdPlaying(DAT_0044ddd8,iVar17);
        if (CONCAT31(extraout_var_01,bVar4) != 0) {
          bVar19 = false;
        }
        iVar17 = iVar17 + 1;
        iVar7 = iVar7 + -1;
      } while (iVar7 != 0);
      if (bVar19) {
        iVar7 = 0;
        uVar24 = 10;
        uVar6 = FUN_0042ffc4();
        iVar17 = (int)uVar6 % 3 + 0x331 + iVar16 * 3;
        goto LAB_00417ba5;
      }
    }
    local_50 = local_50 * _DAT_0043b3e4;
    (&DAT_0050b3c8)[param_1 * 0x19] =
         (float)fVar20 * local_50 + (float)(&DAT_0050b3c8)[param_1 * 0x19];
    (&DAT_0050b3cc)[param_1 * 0x19] =
         (float)(&DAT_0050b3cc)[param_1 * 0x19] - (float)fVar21 * local_50;
    lVar22 = __ftol();
    (&DAT_0050b3b4)[param_1 * 0x19] = (int)lVar22;
    lVar22 = __ftol();
    (&DAT_0050b3b8)[param_1 * 0x19] = (int)lVar22;
    (&DAT_0050b3f0)[param_1 * 0x19] = 1;
    if ((float)(&DAT_0050b3f4)[param_1 * 0x19] <= _DAT_0043b3e0) {
      (&DAT_0050b3f4)[param_1 * 0x19] = 0x3dcccccd;
    }
    else {
      (&DAT_0050b3f4)[param_1 * 0x19] = (float)(&DAT_0050b3f4)[param_1 * 0x19] - _DAT_0043b3e0;
    }
    (&DAT_0050b3d4)[param_1 * 0x19] = (&DAT_0050b3d4)[param_1 * 0x19] + -0x14;
  }
  if ((int)(&DAT_0050b3d4)[param_1 * 0x19] < 1) {
    (&DAT_0050b3d4)[param_1 * 0x19] = 100;
    iVar17 = (&DAT_0050b3ac)[param_1 * 0x19];
    iVar7 = *(int *)(&DAT_00443afc + iVar16 * 4);
    (&DAT_0050b3ac)[param_1 * 0x19] = iVar17 + 1;
    if (iVar7 <= iVar17 + 1) {
      (&DAT_0050b3ac)[param_1 * 0x19] = *(undefined4 *)(&DAT_00443af0 + iVar16 * 4);
    }
  }
LAB_00417c85:
  bVar19 = true;
  piVar5 = &DAT_0050af14;
  do {
    if (*piVar5 == param_1) {
      bVar19 = false;
      break;
    }
    piVar5 = piVar5 + 1;
  } while ((int)piVar5 < 0x50af64);
  if ((&DAT_0050b3f0)[param_1 * 0x19] == 0) {
    if ((0 < (int)(&DAT_0050b3d8)[param_1 * 0x19]) || ((int)(&DAT_0050b3d8)[param_1 * 0x19] < 4)) {
      local_c = (&DAT_0050b3b8)[param_1 * 0x19];
      local_4 = (local_c - (&DAT_0050b3e0)[param_1 * 0x19]) + (&DAT_0050b3e8)[param_1 * 0x19];
      local_10 = (&DAT_0050b3b4)[param_1 * 0x19];
      local_8 = (local_10 - (&DAT_0050b3dc)[param_1 * 0x19]) + (&DAT_0050b3e4)[param_1 * 0x19];
      param_1 = param_1 + 1;
      if (param_1 < DAT_00510760) {
        piVar5 = &DAT_0050b3d8 + param_1 * 0x19;
        do {
          iVar17 = *piVar5;
          if ((iVar17 < 4) || (6 < iVar17)) {
            if ((0 < iVar17) && ((iVar17 < 4 && (bVar19)))) {
              local_1c = piVar5[-8];
              local_14 = (piVar5[4] - piVar5[2]) + local_1c;
              local_38[6] = piVar5[-9];
              local_18 = (local_38[6] - piVar5[1]) + piVar5[3];
              iVar10 = (int)((&DAT_0050b3dc)[iVar2 * 0x19] - (&DAT_0050b3e4)[iVar2 * 0x19]) / 2 +
                       (&DAT_0050b3b4)[iVar2 * 0x19];
              iVar12 = ((&DAT_0050b3e8)[iVar2 * 0x19] - (&DAT_0050b3e0)[iVar2 * 0x19]) / 2 +
                       (&DAT_0050b3b8)[iVar2 * 0x19];
              iVar17 = (piVar5[1] - piVar5[3]) / 2 + piVar5[-9];
              iVar7 = (piVar5[4] - piVar5[2]) / 2 + piVar5[-8];
              fVar20 = DistanceBetweenIntegerPoints(iVar10,iVar12,iVar17,iVar7);
              lVar22 = AngleBetweenIntegerPointsDegrees(iVar17,iVar7,iVar10,iVar12);
              if ((float)fVar20 < _DAT_0043b3dc) {
                fVar20 = (float10)(int)lVar22 * (float10)_DAT_0043b384;
                fVar21 = (float10)fsin(fVar20);
                (&DAT_0050b3b0)[iVar2 * 0x19] = ((int)lVar22 + 0x16) / 0x2d;
                fVar20 = (float10)fcos(fVar20);
                (&DAT_0050b3c8)[iVar2 * 0x19] =
                     (float)fVar21 * _DAT_0043b3d8 + (float)(&DAT_0050b3c8)[iVar2 * 0x19];
                (&DAT_0050b3cc)[iVar2 * 0x19] =
                     (float)((float10)(float)(&DAT_0050b3cc)[iVar2 * 0x19] -
                            fVar20 * (float10)_DAT_0043b3d8);
                lVar22 = __ftol();
                (&DAT_0050b3b4)[iVar2 * 0x19] = (int)lVar22;
                lVar22 = __ftol();
                fVar1 = -(float)fVar21 * _DAT_0043b3d8;
                (&DAT_0050b3b8)[iVar2 * 0x19] = (int)lVar22;
                (&DAT_0050b3f0)[iVar2 * 0x19] = 1;
                (&DAT_0050b3f4)[iVar2 * 0x19] = 0;
                piVar5[-4] = (int)(fVar1 + (float)piVar5[-4]);
                piVar5[-3] = (int)(float)((float10)(float)piVar5[-3] -
                                         -extraout_ST0_01 * (float10)_DAT_0043b3d8);
                lVar22 = __ftol();
                piVar5[-9] = (int)lVar22;
                lVar22 = __ftol();
                piVar5[-8] = (int)lVar22;
                iVar17 = (&DAT_0050b3b0)[iVar2 * 0x19];
                iVar7 = iVar17 + 4;
                piVar5[-10] = iVar7;
                if (7 < iVar7) {
                  piVar5[-10] = iVar17 + -4;
                }
                piVar5[6] = 1;
                piVar5[7] = 0;
              }
            }
          }
          else {
            local_1c = piVar5[-8];
            local_14 = (piVar5[4] - piVar5[2]) + local_1c;
            local_38[6] = piVar5[-9];
            local_18 = (piVar5[3] - piVar5[1]) + local_38[6];
            iVar17 = AnyRectCornerInsideRect(&local_10,local_38 + 6);
            if (iVar17 != 0) {
              lVar22 = AngleBetweenIntegerPointsDegrees
                                 ((int)((&DAT_0050b3dc)[param_1 * 0x19] -
                                       (&DAT_0050b3e4)[param_1 * 0x19]) / 2 +
                                  (&DAT_0050b3b4)[param_1 * 0x19],
                                  ((&DAT_0050b3e8)[param_1 * 0x19] - (&DAT_0050b3e0)[param_1 * 0x19]
                                  ) + (&DAT_0050b3b8)[param_1 * 0x19],
                                  (int)((&DAT_0050b3dc)[iVar2 * 0x19] -
                                       (&DAT_0050b3e4)[iVar2 * 0x19]) / 2 +
                                  (&DAT_0050b3b4)[iVar2 * 0x19],
                                  ((&DAT_0050b3e8)[iVar2 * 0x19] - (&DAT_0050b3e0)[iVar2 * 0x19]) +
                                  (&DAT_0050b3b8)[iVar2 * 0x19]);
              iVar17 = ((int)lVar22 + 0x16) / 0x2d;
              (&DAT_0050b3b0)[iVar2 * 0x19] = iVar17;
              if (7 < iVar17) {
                (&DAT_0050b3b0)[iVar2 * 0x19] = iVar17 + -8;
              }
              fVar20 = (float10)(int)lVar22 * (float10)_DAT_0043b384;
              fVar21 = (float10)fsin(fVar20);
              (&DAT_0050b3c8)[iVar2 * 0x19] =
                   (float)(fVar21 * (float10)_DAT_0043b3d8 +
                          (float10)(float)(&DAT_0050b3c8)[iVar2 * 0x19]);
              fVar20 = (float10)fcos(fVar20);
              (&DAT_0050b3cc)[iVar2 * 0x19] =
                   (float)((float10)(float)(&DAT_0050b3cc)[iVar2 * 0x19] -
                          fVar20 * (float10)_DAT_0043b3d8);
              lVar22 = __ftol();
              (&DAT_0050b3b4)[iVar2 * 0x19] = (int)lVar22;
              lVar22 = __ftol();
              (&DAT_0050b3b8)[iVar2 * 0x19] = (int)lVar22;
              (&DAT_0050b3f0)[iVar2 * 0x19] = 1;
              (&DAT_0050b3f4)[iVar2 * 0x19] = 0;
              return 0;
            }
          }
          param_1 = param_1 + 1;
          piVar5 = piVar5 + 0x19;
        } while (param_1 < DAT_00510760);
      }
    }
    if ((0 < (int)(&DAT_0050b3d8)[iVar2 * 0x19]) || ((int)(&DAT_0050b3d8)[iVar2 * 0x19] < 4)) {
      lVar22 = __ftol();
      lVar23 = __ftol();
      bVar19 = PointInPolygon(&DAT_005104e8,DAT_0050b398,(int)lVar22,(int)lVar23);
      iVar17 = CONCAT31(extraout_var_02,bVar19);
      while (iVar17 != 0) {
        (&DAT_0050b3b4)[iVar2 * 0x19] = (&DAT_0050b3bc)[iVar2 * 0x19];
        iVar17 = (&DAT_0050b3c0)[iVar2 * 0x19];
        (&DAT_0050b3b8)[iVar2 * 0x19] = iVar17;
        iVar7 = (&DAT_0050b3dc)[iVar2 * 0x19];
        iVar12 = (&DAT_0050b3e4)[iVar2 * 0x19];
        iVar10 = (&DAT_0050b3e0)[iVar2 * 0x19];
        iVar13 = (&DAT_0050b3b4)[iVar2 * 0x19];
        iVar15 = (&DAT_0050b3e8)[iVar2 * 0x19];
        uVar6 = FUN_0042ffc4();
        uVar6 = uVar6 & 0x80000003;
        if ((int)uVar6 < 0) {
          uVar6 = (uVar6 - 1 | 0xfffffffc) + 1;
        }
        iVar16 = *(int *)(&DAT_00443ac8 + uVar6 * 8);
        uVar6 = FUN_0042ffc4();
        uVar6 = uVar6 & 0x80000003;
        if ((int)uVar6 < 0) {
          uVar6 = (uVar6 - 1 | 0xfffffffc) + 1;
        }
        lVar22 = AngleBetweenIntegerPointsDegrees
                           ((iVar7 - iVar12) / 2 + iVar13,(iVar15 - iVar10) / 2 + iVar17,iVar16,
                            *(int *)(&DAT_00443acc + uVar6 * 8));
        iVar17 = ((int)lVar22 + 0x16) / 0x2d;
        (&DAT_0050b3b0)[iVar2 * 0x19] = iVar17;
        if (7 < iVar17) {
          (&DAT_0050b3b0)[iVar2 * 0x19] = iVar17 + -8;
        }
        fVar20 = (float10)(int)lVar22 * (float10)_DAT_0043b384;
        fVar21 = (float10)fsin(fVar20);
        (&DAT_0050b3c8)[iVar2 * 0x19] =
             (float)(fVar21 * (float10)_DAT_0043b430 + (float10)(float)(&DAT_0050b3c8)[iVar2 * 0x19]
                    );
        fVar20 = (float10)fcos(fVar20);
        (&DAT_0050b3cc)[iVar2 * 0x19] =
             (float)((float10)(float)(&DAT_0050b3cc)[iVar2 * 0x19] - fVar20 * (float10)_DAT_0043b430
                    );
        lVar22 = __ftol();
        (&DAT_0050b3b4)[iVar2 * 0x19] = (int)lVar22;
        lVar22 = __ftol();
        (&DAT_0050b3b8)[iVar2 * 0x19] = (int)lVar22;
        (&DAT_0050b3f4)[iVar2 * 0x19] = 0;
        lVar22 = __ftol();
        lVar23 = __ftol();
        bVar19 = PointInPolygon(&DAT_005104e8,DAT_0050b398,(int)lVar22,(int)lVar23);
        iVar17 = CONCAT31(extraout_var_03,bVar19);
      }
    }
  }
  return 0;
  while( true ) {
    piVar5 = piVar5 + 1;
    iVar17 = iVar17 + 1;
    if (0x50afc7 < (int)piVar5) break;
LAB_004172ed:
    if (*piVar5 == param_1) {
      (&DAT_0050af78)[iVar17] = 0xffffffff;
      break;
    }
  }
  FUN_0042ffc4();
  (&DAT_0050b3f8)[param_1 * 0x19] = 1;
  return 1;
}

