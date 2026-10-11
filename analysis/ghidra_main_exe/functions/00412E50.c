/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00412e50; function: DrawFireworksEditor; body bytes: 1482
 * callers: 2; callees: 6; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DrawFireworksEditor(void)

{
  int iVar1;
  bool bVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int *piVar7;
  int *unaff_EDI;
  int iVar8;
  int *piVar9;
  float10 extraout_ST0;
  unkbyte10 extraout_ST0_00;
  float10 fVar10;
  longlong lVar11;
  int iVar12;
  int iVar13;
  int iStack_58;
  int *local_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  
  local_54 = *(int **)(DAT_0044de08 + 0xc);
  iVar8 = 0;
  (**(code **)(*local_54 + 0x1c))(local_54,0,0,DAT_0050ab0c,0,0);
  iVar13 = 0;
  iVar12 = 0;
  do {
    if (iVar8 == 2) {
      iVar13 = 4;
      iVar12 = 7;
    }
    iVar5 = 6;
    piVar7 = &DAT_0050a678 + iVar8 * 6;
    piVar9 = &DAT_0050a6c0 + iVar8 * 0x1e;
    do {
      iVar1 = *piVar7;
      if ((-1 < iVar1) && (iVar1 < 0xc)) {
        BlitColorKeyedSurfaceClipped
                  ((int *)(&DAT_0050a4b8)[iVar1],*piVar9 + iVar13,piVar9[1] + -0x1e + iVar12,
                   (int *)0x0);
      }
      piVar9 = piVar9 + 5;
      piVar7 = piVar7 + 1;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    iVar8 = iVar8 + 2;
  } while (iVar8 < 3);
  uStack_4c = 0x128;
  uStack_44 = 0x128;
  local_54 = (int *)0xb1;
  uStack_3c = 0xb1;
  uStack_40 = 0x280;
  uStack_20 = 0x280;
  uStack_34 = 0x80;
  uStack_2c = 0x80;
  uStack_24 = 0x80;
  uStack_1c = 0x80;
  iStack_58 = 0;
  uStack_50 = 0xf0;
  uStack_48 = 400;
  uStack_38 = 0;
  uStack_30 = 300;
  uStack_28 = 500;
  iVar8 = 0xa0;
  if ((DAT_0050ab20 == 1) || (DAT_0050ab24 == 1)) {
    iVar8 = 0x23;
  }
  uVar6 = (uint)(DAT_004fbd30 <= iVar8);
  bVar2 = iVar8 < DAT_004fbd30;
  iVar8 = (&DAT_0050ab30)[bVar2];
  (&DAT_00442a24)[bVar2] = 4;
  (&DAT_0050ab30)[bVar2] = iVar8 + 1;
  if (5 < iVar8 + 1) {
    iVar8 = (&DAT_0050ab28)[bVar2];
    (&DAT_0050ab30)[bVar2] = 0;
    (&DAT_0050ab28)[bVar2] = iVar8 + 1;
    if (8 < iVar8 + 1) {
      (&DAT_0050ab28)[bVar2] = 0;
    }
  }
  iVar8 = 0;
  if (DAT_004fbd24 < 0x3d) {
    iVar12 = 0x3d;
  }
  else {
    iVar12 = DAT_004fbd24;
    if (0x23f < DAT_004fbd24) {
      iVar12 = 0x23f;
    }
  }
  piVar7 = &iStack_58 + uVar6 * 8;
  do {
    if ((&DAT_00442a04)[uVar6 * 2] + 0x40 < *piVar7) break;
    iVar8 = iVar8 + 1;
    piVar7 = piVar7 + 2;
  } while (iVar8 < 5);
  iVar8 = 0;
  lVar11 = __ftol();
  fVar10 = (float10)_DAT_0043b2f0;
  (&DAT_00442a08)[uVar6 * 2] = (int)lVar11 + -0x80;
  if (fVar10 <= extraout_ST0) {
    if ((float10)_DAT_0043b2f0 < extraout_ST0) {
      iVar8 = -1;
    }
  }
  else {
    iVar8 = 1;
  }
  iVar13 = (&DAT_00442a04)[uVar6 * 2];
  uVar3 = (iVar13 - iVar12) + 0x40;
  uVar4 = (int)uVar3 >> 0x1f;
  if ((int)((uVar3 ^ uVar4) - uVar4) < 0x1e - DAT_0050ab7c) {
    iVar8 = (&DAT_0050ab30)[uVar6];
    DAT_0050ab7c = 0;
    (&DAT_0050ab30)[uVar6] = iVar8 + 1;
    (&DAT_00442a24)[uVar6] = 4;
    if (5 < iVar8 + 1) {
      iVar8 = (&DAT_0050ab28)[uVar6];
      (&DAT_0050ab30)[uVar6] = 0;
      (&DAT_0050ab28)[uVar6] = iVar8 + 1;
      if (8 < iVar8 + 1) {
        (&DAT_0050ab28)[uVar6] = 0;
      }
    }
    goto LAB_0041317e;
  }
  DAT_0050ab7c = 0x19;
  if (iVar13 < iVar12 + -0x40) {
    iVar8 = iVar8 + 2;
    (&DAT_00442a04)[uVar6 * 2] = iVar13 + 1;
LAB_0041312b:
    (&DAT_00442a24)[uVar6] = iVar8;
  }
  else if (iVar12 + -0x40 < iVar13) {
    iVar8 = iVar8 + 6;
    (&DAT_00442a04)[uVar6 * 2] = iVar13 + -1;
    goto LAB_0041312b;
  }
  iVar8 = (&DAT_0050ab30)[uVar6];
  (&DAT_0050ab30)[uVar6] = iVar8 + 1;
  if (5 < iVar8 + 1) {
    iVar8 = (&DAT_0050ab28)[uVar6];
    piVar7 = &DAT_0050ab28 + uVar6;
    (&DAT_0050ab30)[uVar6] = 0;
    *piVar7 = iVar8 + 1;
    if (0x18 < iVar8 + 1) {
      *piVar7 = 0xd;
    }
    if (*piVar7 < 0xe) {
      *piVar7 = 0xd;
    }
  }
LAB_0041317e:
  iVar8 = 1;
  do {
    if (iVar8 == 0) {
      (**(code **)(*unaff_EDI + 0x1c))(unaff_EDI,0xcd,0x68,DAT_0050aaa8,0,1);
    }
    BlitColorKeyedSurfaceClipped
              ((int *)(&DAT_0050a494)[iVar8],(&DAT_00442a04)[iVar8 * 2],(&DAT_00442a08)[iVar8 * 2],
               (int *)&stack0xffffff98);
    iVar8 = iVar8 + -1;
  } while (-1 < iVar8);
  (**(code **)(*unaff_EDI + 0x1c))(unaff_EDI,0x25,0x11c,DAT_0050ab08,0,1);
  iVar8 = 0;
  do {
    iVar12 = (&DAT_0050ab20)[iVar8];
    if (iVar12 == 10) {
      (&DAT_0050ab20)[iVar8] = 0xb;
    }
    else if (iVar12 == 0xb) {
      PlaceFireworkGridItem
                (*(int *)(&DAT_005093dc + iVar8 * 4),*(int *)(&DAT_005093e4 + iVar8 * 4),
                 DAT_005093d8,1);
      (&DAT_0050ab20)[iVar8] = 0;
      iVar12 = 0;
      piVar7 = &DAT_0050a678;
      do {
        iVar13 = 6;
        do {
          if (*piVar7 != -1) {
            iVar12 = iVar12 + 1;
          }
          piVar7 = piVar7 + 1;
          iVar13 = iVar13 + -1;
        } while (iVar13 != 0);
      } while ((int)piVar7 < 0x50a6c0);
      if (0x11 < iVar12) {
        if (iVar8 == 0) {
          PlayManagedSoundById(DAT_0044ddd8,0x35c,0x32,2);
        }
        else {
          PlayManagedSoundById(DAT_0044ddd8,0x381,0x32,2);
        }
      }
    }
    else if ((iVar12 == 0xc) &&
            (iVar12 = (&DAT_0050ab30)[iVar8], (&DAT_0050ab30)[iVar8] = iVar12 + 1, 5 < iVar12 + 1))
    {
      iVar12 = (&DAT_0050ab28)[iVar8];
      (&DAT_0050ab30)[iVar8] = 0;
      (&DAT_0050ab28)[iVar8] = iVar12 + 1;
      if (0x18 < iVar12 + 1) {
        (&DAT_0050ab28)[iVar8] = 0xd;
      }
      iVar12 = *(int *)(iVar8 * 8 + 0x442a18);
      iVar13 = *(int *)(iVar8 * 8 + 0x442a14);
      lVar11 = AngleBetweenIntegerPointsDegrees
                         ((&DAT_00442a04)[iVar8 * 2],(&DAT_00442a08)[iVar8 * 2],iVar13,iVar12);
      iVar5 = ((int)lVar11 + 0x16) / 0x2d;
      (&DAT_00442a24)[iVar8] = iVar5;
      if (7 < iVar5) {
        (&DAT_00442a24)[iVar8] = iVar5 + -8;
      }
      fsin((float10)(int)lVar11 * (float10)_DAT_0043b384);
      lVar11 = __ftol();
      fcos(extraout_ST0_00);
      (&DAT_00442a04)[iVar8 * 2] = (&DAT_00442a04)[iVar8 * 2] + (int)lVar11;
      lVar11 = __ftol();
      iVar5 = (&DAT_00442a08)[iVar8 * 2] + (int)lVar11;
      (&DAT_00442a08)[iVar8 * 2] = iVar5;
      fVar10 = DistanceBetweenIntegerPoints((&DAT_00442a04)[iVar8 * 2],iVar5,iVar13,iVar12);
      if (fVar10 < (float10)_DAT_0043b378) {
        (&DAT_0050ab20)[iVar8] = 0;
        (&DAT_00442a24)[iVar8] = 4;
      }
    }
    iVar8 = iVar8 + 1;
  } while (iVar8 < 2);
  piVar9 = &DAT_0050a690;
  piVar7 = &DAT_0050a738;
  do {
    iVar8 = *piVar9;
    if ((-1 < iVar8) && (iVar8 < 0xc)) {
      BlitColorKeyedSurfaceClipped
                ((int *)(&DAT_0050a4b8)[iVar8],*piVar7,piVar7[1] + -0x1e,(int *)0x0);
    }
    piVar7 = piVar7 + 5;
    piVar9 = piVar9 + 1;
  } while ((int)piVar7 < 0x50a7b0);
  return;
}

