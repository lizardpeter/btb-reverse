/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00425f40; function: UpdateAndDrawSquirrelRunAssembly; body bytes: 2156
 * callers: 1; callees: 3; success: True
 */


void UpdateAndDrawSquirrelRunAssembly(void)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  bool bVar6;
  int local_180 [4];
  int *local_170;
  int local_16c [91];
  
  local_170 = *(int **)(DAT_0044de08 + 0xc);
  local_16c[0x17] = 6;
  local_16c[0x1b] = 6;
  local_16c[0x13] = 0;
  local_16c[0x14] = 0;
  local_16c[0x15] = 0;
  local_16c[0x16] = 1;
  local_16c[0x18] = 2;
  local_16c[0x19] = 0;
  local_16c[0x1a] = 0;
  local_16c[0x1c] = 3;
  local_16c[0x1d] = 3;
  local_16c[0x1e] = 3;
  local_16c[0x1f] = 5;
  local_16c[0x20] = 7;
  local_16c[0x21] = 4;
  local_16c[0x22] = 5;
  local_16c[0x23] = 0;
  local_16c[0x24] = 0;
  local_16c[0x25] = 1;
  local_16c[0x26] = 0;
  local_16c[0x27] = 6;
  local_16c[0x28] = 0;
  local_16c[0x29] = 0;
  local_16c[0x2a] = 0;
  local_16c[0x2b] = 7;
  local_16c[0x2c] = 5;
  local_16c[0x2d] = 5;
  local_16c[0x2e] = 5;
  local_16c[0x2f] = 5;
  local_16c[0x30] = 5;
  local_16c[0x31] = 5;
  local_16c[0x32] = 7;
  local_16c[0x33] = 1;
  local_16c[0x34] = 0;
  local_16c[0x35] = 0;
  local_16c[0x36] = 0;
  local_16c[0x37] = 0;
  local_16c[0x38] = 0;
  local_16c[0x39] = 0;
  local_16c[0x3a] = 1;
  local_16c[0x3b] = 1;
  local_16c[0x3c] = 2;
  local_16c[0x3d] = 3;
  local_16c[0x3e] = 3;
  local_16c[0x3f] = 6;
  local_16c[0x40] = 3;
  local_16c[0x41] = 3;
  local_16c[0x42] = 3;
  local_16c[0x43] = 0;
  local_16c[0x44] = 1;
  local_16c[0x45] = 4;
  local_16c[0x46] = 0;
  local_16c[0x47] = 0;
  local_16c[0x48] = 0;
  local_16c[0x49] = 1;
  local_16c[0x4a] = 0;
  local_16c[0x4b] = 1;
  local_16c[0x4c] = 3;
  local_16c[0x4d] = 3;
  local_16c[0x4e] = 3;
  local_16c[0x4f] = 7;
  local_16c[0x50] = 5;
  local_16c[0x51] = 5;
  local_16c[0x52] = 5;
  local_16c[0x53] = 0;
  local_16c[0x54] = 0;
  local_16c[0x55] = 0;
  local_16c[0x56] = 1;
  local_16c[0x57] = 1;
  local_16c[0x58] = 0;
  local_16c[0x59] = 0;
  local_16c[0x5a] = 0;
  local_16c[0] = 0;
  local_16c[1] = 0;
  local_16c[2] = 3;
  local_16c[3] = 3;
  local_16c[4] = 5;
  local_16c[5] = 5;
  local_16c[6] = 4;
  local_16c[7] = 4;
  local_16c[8] = 2;
  local_16c[9] = 2;
  local_16c[10] = 0;
  local_16c[0xb] = 0;
  local_16c[0xc] = 1;
  local_16c[0xd] = 1;
  local_16c[0xe] = 1;
  local_16c[0xf] = 1;
  local_16c[0x10] = 1;
  local_16c[0x11] = 1;
  local_16c[0x12] = 1;
  if (DAT_00514fb8 == 0) {
    if (DAT_00515004 < DAT_00514fb4) {
      DAT_00514fa0 = DAT_00514fa0 + -1;
      if (DAT_00514fa0 < 1) {
        DAT_00514fa0 = DAT_00446778;
        DAT_00514fa8 = DAT_00514fa8 + 1;
        DAT_00514f9c = 0;
        if (local_16c[DAT_00514eec + 0x37] == 1) {
          if (DAT_00514fa8 < DAT_00446870) {
            DAT_00514fa8 = DAT_00446870;
          }
          if (DAT_00446874 < DAT_00514fa8) {
            DAT_00514fa8 = DAT_00446870;
          }
        }
        else {
          if (DAT_00514fa8 < DAT_00446838) {
            DAT_00514fa8 = DAT_00446838;
          }
          if (DAT_00446844 < DAT_00514fa8) {
            uVar2 = FUN_0042ffc4();
            uVar2 = uVar2 & 0x80000003;
            bVar6 = uVar2 == 0;
            if ((int)uVar2 < 0) {
              bVar6 = (uVar2 - 1 | 0xfffffffc) == 0xffffffff;
            }
            if (bVar6) {
              DAT_00514fa8 = DAT_00446838;
            }
            else {
              DAT_00514fa8 = DAT_00446840;
            }
          }
        }
      }
      iVar3 = 0;
      local_180[0] = (DAT_00514fa8 + DAT_00514f9c) * DAT_0044677c;
      local_180[2] = local_180[0] + DAT_0044677c;
      local_180[3] = DAT_00446780;
      local_180[1] = 0;
      if (0 < DAT_005150d8) {
        iVar3 = (int)((ulonglong)((longlong)DAT_005150f0 * -0x66666667) >> 0x20);
        iVar3 = (iVar3 >> 1) - (iVar3 >> 0x1f);
      }
      if ((3 < DAT_00514fb8) && (DAT_00514fb8 < 6)) {
        if (local_16c[DAT_00514ee4 + 0x37] == 1) {
          return;
        }
        if (local_16c[DAT_00514ee8 + 0x37] == 1) {
          return;
        }
      }
      iVar4 = *local_170;
      goto LAB_00426785;
    }
    SelectSquirrelPlacementMotionProfile();
    DAT_00514fb8 = 1;
    DAT_00514fa8 = 0;
    DAT_00514f9c = 0;
    DAT_00514fa4 = 0;
    DAT_00515114 = DAT_00514f54;
    DAT_00514fa0 = DAT_00446778;
LAB_0042636e:
    iVar3 = DAT_00514f9c;
    iVar4 = DAT_00446778;
    piVar1 = local_16c + DAT_00514ee0;
    iVar5 = local_16c[DAT_00514ee0] * 7 + DAT_00514f9c;
    if (0 < *(int *)(&DAT_00446878 + iVar5 * 4)) {
      *(int *)(&DAT_00446878 + iVar5 * 4) = *(int *)(&DAT_00446878 + iVar5 * 4);
    }
    DAT_00514f94 = DAT_00514f94 + (&DAT_00446958)[iVar3];
    if (local_16c[DAT_00514ee4] == 7) {
      local_16c[DAT_00514ee4] = 7;
    }
    DAT_00514f98 = DAT_00514f98 + *(int *)(&DAT_00446878 + (*piVar1 * 7 + iVar3) * 4);
LAB_004263f5:
    DAT_00514fa0 = DAT_00514fa0 + -1;
    iVar5 = DAT_00514f9c;
    if ((DAT_00514fa0 < 1) &&
       (DAT_00514f9c = iVar3 + 1, iVar3 = DAT_00514f9c, iVar5 = DAT_00514f9c, DAT_00514fa0 = iVar4,
       2 < DAT_00514f9c)) {
      DAT_00514fb8 = 2;
LAB_00426424:
      NoOpLegacyHook();
      DAT_00514f9c = 0;
      DAT_00514fa8 = 3;
      DAT_00514fa0 = DAT_00446778;
LAB_00426461:
      DAT_00514f94 = DAT_00514f94 + (&DAT_00446964)[DAT_00514f9c];
      DAT_00514f98 = DAT_00514f98 +
                     *(int *)(&DAT_00446884 + (local_16c[DAT_00514ee4] * 7 + DAT_00514f9c) * 4);
      if (DAT_00515110 != 0) {
        local_16c[DAT_00514ee4 + 0x37] = 1;
      }
LAB_004264c4:
      DAT_00514fa0 = DAT_00514fa0 + -1;
      DAT_00514fb8 = 3;
      DAT_00514fa4 = 1;
      iVar3 = DAT_00514f9c;
      iVar5 = DAT_00514f9c;
      if ((DAT_00514fa0 < 1) &&
         (iVar3 = DAT_00514f9c + 1, iVar5 = iVar3, DAT_00514fa0 = DAT_00446778, 3 < iVar3)) {
        DAT_00514fb8 = 4;
        DAT_00514f9c = 0;
        DAT_00514fa4 = 2;
        DAT_00514fa8 = 0;
LAB_0042652d:
        DAT_00514f94 = DAT_00514f94 + (&DAT_00446958)[DAT_00514f9c];
        DAT_00514f98 = DAT_00514f98 +
                       *(int *)(&DAT_00446878 + (local_16c[DAT_00514ee8] * 7 + DAT_00514f9c) * 4);
LAB_00426571:
        DAT_00514fa0 = DAT_00514fa0 + -1;
        iVar3 = DAT_00514f9c;
        iVar5 = DAT_00514f9c;
        if ((DAT_00514fa0 < 1) &&
           (iVar3 = DAT_00514f9c + 1, iVar5 = iVar3, DAT_00514fa0 = DAT_00446778, 2 < iVar3)) {
LAB_004265a0:
          DAT_00514fb8 = 6;
          DAT_00514f9c = 0;
          DAT_00514fa8 = 3;
          DAT_00514fa0 = DAT_00446778;
          if (DAT_00515110 != 0) {
            local_180[0] = 0xed;
            local_180[1] = 0xcd;
            local_180[2] = 0xad;
            DAT_00514f98 = local_180[DAT_00515114];
          }
LAB_0042660f:
          DAT_00514f94 = DAT_00514f94 + (&DAT_00446964)[DAT_00514f9c];
          DAT_00514f98 = DAT_00514f98 +
                         *(int *)(&DAT_00446884 + (local_16c[DAT_00514eec] * 7 + DAT_00514f9c) * 4);
          goto LAB_0042664e;
        }
      }
    }
  }
  else {
    iVar3 = DAT_00514f9c;
    if (DAT_00514fb8 == 1) {
      iVar4 = DAT_00446778;
      if (DAT_00514fa0 == DAT_00446778) goto LAB_0042636e;
      goto LAB_004263f5;
    }
    if (DAT_00514fb8 == 2) goto LAB_00426424;
    if (DAT_00514fb8 == 3) {
      if (DAT_00514fa0 == DAT_00446778) goto LAB_00426461;
      goto LAB_004264c4;
    }
    if (DAT_00514fb8 == 4) {
      if (DAT_00514fa0 == DAT_00446778) goto LAB_0042652d;
      goto LAB_00426571;
    }
    if (DAT_00514fb8 == 5) goto LAB_004265a0;
    iVar5 = DAT_00514f9c;
    if (DAT_00514fb8 != 6) goto LAB_004266a8;
    if (DAT_00514fa0 == DAT_00446778) goto LAB_0042660f;
LAB_0042664e:
    DAT_00514fa0 = DAT_00514fa0 + -1;
    DAT_00514fa4 = 3;
    iVar3 = DAT_00514f9c;
    iVar5 = DAT_00514f9c;
    if ((DAT_00514fa0 < 1) &&
       (iVar3 = DAT_00514f9c + 1, iVar5 = iVar3, DAT_00514fa0 = DAT_00446778, 3 < iVar3)) {
      DAT_00514fb4 = DAT_00514fb4 + 1;
      DAT_00514fb8 = 0;
      iVar3 = DAT_00514f9c;
      iVar5 = DAT_00514f9c;
      DAT_00514fa0 = DAT_00446778;
    }
  }
LAB_004266a8:
  DAT_00514f9c = iVar5;
  local_180[0] = (*(int *)(&DAT_004467e8 + (&DAT_00514ee0)[DAT_00514fa4] * 8) + iVar3) *
                 DAT_0044677c;
  local_180[2] = local_180[0] + DAT_0044677c;
  bVar6 = false;
  if ((((DAT_00514fb4 == 0) || (DAT_00514fb4 == 2)) || (DAT_00514fb4 == 4)) || (DAT_00514fb4 == 6))
  {
    bVar6 = true;
  }
  iVar4 = DAT_00514fb4;
  if (0 < DAT_00514fb4) {
    iVar4 = DAT_00514fb4 + -1;
  }
  if (2 < iVar4) {
    iVar4 = iVar4 + -1;
  }
  DAT_00515110 = (uint)(0x3e < *(int *)(&DAT_004467e8 + (&DAT_00514ee0)[DAT_00514fa4] * 8) + iVar3);
  if (((&DAT_00514fbc)[iVar4 + DAT_005150d0 * 4] == 1) && (!bVar6)) {
    return;
  }
  if (DAT_00446a4c != DAT_00514fa8 + iVar3) {
    DAT_00446a4c = DAT_00514fa8 + iVar3;
  }
  iVar3 = 0;
  if (0 < DAT_005150d8) {
    iVar3 = (int)((ulonglong)((longlong)DAT_005150f0 * -0x66666667) >> 0x20);
    iVar3 = (iVar3 >> 1) - (iVar3 >> 0x1f);
  }
  if (((3 < DAT_00514fb8) && (DAT_00514fb8 < 8)) && (DAT_00515110 != 0)) {
    return;
  }
  iVar4 = *local_170;
LAB_00426785:
  local_180[1] = 0;
  local_180[3] = DAT_00446780;
  (**(code **)(iVar4 + 0x1c))(local_170,iVar3 + DAT_00514f94,DAT_00514f98,DAT_005150bc,local_180,1);
  return;
}

