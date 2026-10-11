/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004259d0; function: SelectSquirrelPlacementMotionProfile; body bytes: 1379
 * callers: 1; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void SelectSquirrelPlacementMotionProfile(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int local_24c [147];
  
  _DAT_00514f5c = 0;
  _DAT_00514edc = 0;
  local_24c[0] = 0;
  local_24c[1] = 0x10;
  local_24c[2] = 0x21;
  local_24c[3] = 0;
  local_24c[4] = 0;
  local_24c[5] = 0;
  local_24c[6] = 0xc;
  local_24c[7] = 0xc;
  local_24c[8] = 9;
  local_24c[9] = 0;
  local_24c[10] = 0;
  local_24c[0xb] = 0xc;
  local_24c[0xc] = 3;
  local_24c[0xd] = 3;
  local_24c[0xe] = 3;
  local_24c[0xf] = 5;
  local_24c[0x10] = 0xc;
  local_24c[0x11] = 7;
  local_24c[0x12] = 5;
  local_24c[0x13] = 0;
  local_24c[0x14] = 0;
  local_24c[0x15] = 0xc;
  local_24c[0x16] = 0;
  local_24c[0x17] = 0xc;
  local_24c[0x18] = 0;
  local_24c[0x19] = 0;
  local_24c[0x1a] = 0;
  local_24c[0x1b] = 0xc;
  local_24c[0x1c] = 5;
  local_24c[0x1d] = 5;
  local_24c[0x1e] = 5;
  local_24c[0x1f] = 5;
  local_24c[0x20] = 5;
  local_24c[0x21] = 5;
  local_24c[0x22] = 0xc;
  local_24c[0x23] = 0xc;
  local_24c[0x24] = 0;
  local_24c[0x25] = 0;
  local_24c[0x26] = 0;
  local_24c[0x27] = 0;
  local_24c[0x28] = 0;
  local_24c[0x29] = 0;
  local_24c[0x2a] = 0;
  local_24c[0x2b] = 0xc;
  local_24c[0x2c] = 9;
  local_24c[0x2d] = 3;
  local_24c[0x2e] = 3;
  local_24c[0x2f] = 0xc;
  local_24c[0x30] = 3;
  local_24c[0x31] = 3;
  local_24c[0x32] = 3;
  local_24c[0x33] = 0;
  local_24c[0x34] = 0xc;
  local_24c[0x35] = 7;
  local_24c[0x36] = 0;
  local_24c[0x37] = 0;
  local_24c[0x38] = 0;
  local_24c[0x39] = 0xc;
  local_24c[0x3a] = 0;
  local_24c[0x3b] = 0xc;
  local_24c[0x3c] = 3;
  local_24c[0x3d] = 3;
  local_24c[0x3e] = 3;
  local_24c[0x3f] = 0xc;
  local_24c[0x40] = 5;
  local_24c[0x41] = 5;
  local_24c[0x42] = 5;
  local_24c[0x43] = 0;
  local_24c[0x44] = 0;
  local_24c[0x46] = 0xc;
  local_24c[0x47] = 0xc;
  local_24c[0x45] = 0;
  local_24c[0x48] = 0;
  local_24c[0x49] = 0;
  local_24c[0x4a] = 0;
  local_24c[0x4b] = 1;
  local_24c[0x4c] = 1;
  local_24c[0x4d] = 1;
  local_24c[0x4e] = 0xd;
  local_24c[0x4f] = 0xd;
  local_24c[0x50] = 8;
  local_24c[0x51] = 1;
  local_24c[0x52] = 1;
  local_24c[0x53] = 0xd;
  local_24c[0x54] = 2;
  local_24c[0x55] = 2;
  local_24c[0x56] = 2;
  local_24c[0x57] = 4;
  local_24c[0x58] = 0xd;
  local_24c[0x59] = 6;
  local_24c[0x5a] = 4;
  local_24c[0x5b] = 1;
  local_24c[0x5c] = 1;
  local_24c[0x5d] = 0xd;
  local_24c[0x5e] = 1;
  local_24c[0x5f] = 0xd;
  local_24c[0x60] = 1;
  local_24c[0x61] = 1;
  local_24c[0x62] = 1;
  local_24c[99] = 0xd;
  local_24c[100] = 4;
  local_24c[0x65] = 4;
  local_24c[0x66] = 4;
  local_24c[0x67] = 4;
  local_24c[0x68] = 4;
  local_24c[0x69] = 4;
  local_24c[0x6a] = 0xd;
  local_24c[0x6b] = 0xd;
  local_24c[0x6c] = 1;
  local_24c[0x6d] = 1;
  local_24c[0x6e] = 1;
  local_24c[0x6f] = 1;
  local_24c[0x70] = 1;
  local_24c[0x71] = 1;
  local_24c[0x72] = 0xd;
  local_24c[0x73] = 0xd;
  local_24c[0x74] = 8;
  local_24c[0x75] = 2;
  local_24c[0x76] = 2;
  local_24c[0x77] = 0xd;
  local_24c[0x78] = 2;
  local_24c[0x79] = 2;
  local_24c[0x7a] = 2;
  local_24c[0x7b] = 1;
  local_24c[0x7c] = 0xd;
  local_24c[0x7d] = 6;
  local_24c[0x7e] = 1;
  local_24c[0x7f] = 1;
  local_24c[0x80] = 1;
  local_24c[0x81] = 0xd;
  local_24c[0x82] = 1;
  local_24c[0x83] = 0xd;
  local_24c[0x84] = 2;
  local_24c[0x85] = 2;
  local_24c[0x86] = 2;
  local_24c[0x87] = 0xd;
  local_24c[0x88] = 4;
  local_24c[0x89] = 4;
  local_24c[0x8a] = 4;
  local_24c[0x8b] = 1;
  local_24c[0x8c] = 1;
  local_24c[0x8d] = 1;
  local_24c[0x8e] = 0xd;
  local_24c[0x8f] = 0xd;
  local_24c[0x90] = 1;
  local_24c[0x91] = 1;
  local_24c[0x92] = 1;
  iVar1 = DAT_00514fb4;
  if (0 < DAT_00514fb4) {
    iVar1 = DAT_00514fb4 + -1;
  }
  iVar2 = iVar1;
  if ((1 < iVar1) && (iVar2 = iVar1 + -1, 1 < iVar2)) {
    iVar2 = iVar1 + -2;
  }
  if (2 < iVar2) {
    iVar2 = iVar2 + -1;
  }
  iVar2 = (&DAT_00514fbc)[iVar2 + DAT_005150d0 * 4];
  iVar1 = iVar2 / 9 + (iVar2 % 9) * 4;
  if (DAT_00514fb4 == 0) {
    iVar2 = local_24c[(&DAT_00514f0c)[DAT_005150d0 * 4]];
    iVar4 = local_24c[(&DAT_00514f0c)[DAT_005150d0 * 4]];
  }
  else if (DAT_00514fb4 == 2) {
    iVar2 = local_24c[(&DAT_00514f10)[DAT_005150d0 * 4]];
    iVar4 = (int)(&DAT_00514fbc)[DAT_005150d0 * 4] / 9 +
            ((int)(&DAT_00514fbc)[DAT_005150d0 * 4] % 9) * 4;
  }
  else if (DAT_00514fb4 == 1) {
    iVar2 = iVar1;
    iVar4 = local_24c[(&DAT_00514f0c)[DAT_005150d0 * 4]];
  }
  else {
    if (DAT_00514fb4 != 3) {
      if (DAT_00514fb4 == 4) {
        iVar2 = local_24c[*(int *)(&DAT_00514f14 + DAT_005150d0 * 0x10)];
        iVar4 = iVar1;
        goto LAB_00425ee3;
      }
      if (DAT_00514fb4 != 5) {
        iVar4 = iVar2;
        if (DAT_00514fb4 == 6) {
          iVar2 = local_24c[(&DAT_00514f18)[DAT_005150d0 * 4]];
          iVar4 = iVar1;
        }
        goto LAB_00425ee3;
      }
    }
    iVar2 = iVar1;
    iVar4 = local_24c[*(int *)(&DAT_00514f14 + DAT_005150d0 * 0x10)];
  }
LAB_00425ee3:
  DAT_00514ee0 = local_24c[iVar4 + 0x27];
  DAT_00514ee4 = local_24c[iVar2 + 0x4b];
  DAT_00514ee8 = local_24c[iVar2 + 3];
  DAT_00514eec = local_24c[iVar2 + 0x6f];
  piVar3 = &DAT_00514ee0;
  do {
    if (*piVar3 < 0) {
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 1;
  } while ((int)piVar3 < 0x514ef0);
  return;
}

