/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004225e0; function: UpdatePilchardAnimation; body bytes: 369
 * callers: 1; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdatePilchardAnimation(void)

{
  int iVar1;
  bool bVar2;
  
  DAT_00445ef8 = -1;
  DAT_00445efc = -1;
  DAT_005144d4 = 0;
  if ((DAT_005144c8 < 1) || (DAT_005109c8 < 0)) {
    DAT_005107a4 = 1;
    DAT_005107b0 = DAT_005107b0 + -1;
    if (DAT_005107b0 < 1) {
      DAT_005107b0 = 10;
      DAT_005107ac = DAT_005107ac + 1;
      if (8 < DAT_005107ac) {
        DAT_005107ac = 0;
      }
    }
    return;
  }
  if (DAT_005107a4 == 4) {
    iVar1 = 3;
  }
  else {
    iVar1 = DAT_005107a4;
    if (DAT_005107a4 == 0) {
      DAT_00445efc = 0;
    }
    else if (DAT_005107a4 == 2) {
      DAT_00445ef8 = 0;
    }
  }
  if (0 < DAT_005107b0 + -1) {
    DAT_005107b0 = DAT_005107b0 + -1;
    DAT_005144d4 = 0;
    return;
  }
  DAT_005107ac = DAT_005107ac + 1;
  if (iVar1 == 0) {
    if (DAT_005107ac < 0x10) {
      DAT_005107ac = 0x10;
    }
    DAT_00445efc = DAT_005107ac + -0x10;
    bVar2 = SBORROW4(DAT_005107ac,0x18);
    iVar1 = DAT_005107ac + -0x18;
  }
  else {
    if (iVar1 != 2) {
      if ((iVar1 == 1) || (iVar1 == 3)) {
        DAT_005144d4 = 1;
        if (0xf < DAT_005107ac) {
          _DAT_005144cc = 0;
          DAT_005107ac = 9;
          DAT_005107b0 = DAT_005107ec / 2;
          return;
        }
        if (DAT_005107ac < 9) {
          DAT_005107ac = 9;
        }
      }
      goto LAB_00422707;
    }
    if (DAT_005107ac < 0x1b) {
      DAT_005107ac = 0x1b;
    }
    DAT_00445ef8 = DAT_005107ac + -0x1b;
    bVar2 = SBORROW4(DAT_005107ac,0x24);
    iVar1 = DAT_005107ac + -0x24;
  }
  if (bVar2 == iVar1 < 0) {
    DAT_005107ac = 0;
    _DAT_005144cc = 1;
    DAT_005107b0 = DAT_005107ec / 2;
    return;
  }
LAB_00422707:
  DAT_005107b0 = DAT_005107ec / 2;
  return;
}

