/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00422400; function: UpdateSpudMazeBobAnimation; body bytes: 472
 * callers: 1; callees: 0; success: True
 */


void UpdateSpudMazeBobAnimation(void)

{
  int iVar1;
  
  if (DAT_00514528 != 0) {
    if ((0 < DAT_005109f0 + -1) && (DAT_0044de0c == 0)) {
      DAT_005109f0 = DAT_005109f0 + -1;
      return;
    }
    DAT_005109f0 = 8;
    DAT_005109ec = DAT_005109ec + 1;
    DAT_0051451c = 1;
    if (9 < DAT_005109ec) {
      DAT_005109ec = 0;
      return;
    }
    if (DAT_005109ec < 0) {
      DAT_005109ec = 0;
      return;
    }
    DAT_005109f0 = 8;
    DAT_0051451c = 1;
    return;
  }
  if ((0 < DAT_005109f0 + -1) && (DAT_0044de0c == 0)) {
    DAT_005109f0 = DAT_005109f0 + -1;
    return;
  }
  iVar1 = DAT_005109ec + 1;
  DAT_0051451c = 1;
  if (DAT_00514510 == 0) {
    if (DAT_00514518 == 0) {
      if (DAT_00510a00 == 0) {
        DAT_005109f0 = 4;
        DAT_0051451c = 1;
        return;
      }
      if (DAT_00510a00 == 2) {
        DAT_005109f0 = 4;
        DAT_0051451c = 1;
        return;
      }
      if (9 < iVar1) {
        DAT_005109ec = 0;
        DAT_005109f0 = 4;
        return;
      }
    }
    else {
      if (DAT_00510a00 != 0) {
        if (DAT_00510a00 == 2) {
          DAT_005109ec = DAT_005109ec + -1;
          if (DAT_005109ec < 1) {
            DAT_005109ec = 6;
            DAT_005109f0 = 4;
            return;
          }
          if (DAT_005109ec < 7) {
            DAT_005109f0 = 4;
            DAT_0051451c = 1;
            return;
          }
          DAT_005109ec = 6;
          DAT_005109f0 = 4;
          return;
        }
        if (0x18 < iVar1) {
          DAT_005109ec = 0xd;
          DAT_005109f0 = 4;
          return;
        }
        if (0xc < iVar1) {
          DAT_005109ec = iVar1;
          DAT_005109f0 = 4;
          DAT_0051451c = 1;
          return;
        }
        DAT_005109ec = 0xd;
        DAT_005109f0 = 4;
        return;
      }
      if (6 < iVar1) {
        DAT_005109ec = 0;
        DAT_005109f0 = 4;
        return;
      }
    }
    if (iVar1 < 0) {
      DAT_005109ec = 0;
      DAT_005109f0 = 4;
      return;
    }
  }
  else if (0xe < iVar1) {
    DAT_00514514 = DAT_00514514 + -1;
    if (0 < DAT_00514514) {
      DAT_005109ec = 9;
      DAT_005109f0 = 4;
      return;
    }
    DAT_005109ec = 0;
    DAT_00514510 = 0;
    DAT_005144c8 = DAT_005144c8 + -1;
    DAT_005109f0 = 4;
    return;
  }
  DAT_0051451c = 1;
  DAT_005109f0 = 4;
  DAT_005109ec = iVar1;
  return;
}

