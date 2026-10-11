/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d900; function: UpdateAndDrawParkDesignerFountain; body bytes: 338
 * callers: 1; callees: 1; success: True
 */


void UpdateAndDrawParkDesignerFountain(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int local_10 [3];
  undefined4 local_4;
  
  iVar2 = DAT_005079fc;
  if (DAT_005079fc != -1) {
    iVar4 = DAT_005079fc * 0x4c;
    if (*(int *)(&DAT_004fcaf4 + iVar4) == 0) {
      iVar1 = *(int *)(&DAT_004fcaf8 + iVar4);
      *(int *)(&DAT_004fcaf8 + iVar4) = iVar1 + -1;
      if (iVar1 + -1 < 1) {
        *(undefined4 *)(&DAT_004fcaf8 + iVar4) = 9;
        iVar1 = *(int *)(&DAT_004fcaf0 + iVar4);
        *(int *)(&DAT_004fcaf0 + iVar4) = iVar1 + 1;
        iVar3 = DAT_00507a28;
        if (4 < iVar1 + 1) {
          *(int *)(&DAT_004fcaf0 + DAT_00507a28 * 0x4c) =
               *(int *)(&DAT_004fcaf0 + DAT_00507a28 * 0x4c) + 1;
        }
        iVar1 = *(int *)(&DAT_004fcaf0 + iVar4);
        if (7 < iVar1) {
          *(undefined4 *)(&DAT_004fcaf4 + iVar4) = 1;
          iVar1 = *(int *)(&DAT_004fcaf0 + iVar4);
        }
        if ((7 < iVar1) && (*(int *)(&DAT_004fcaf0 + iVar3 * 0x4c) < 4)) {
          *(int *)(&DAT_004fcaf0 + iVar3 * 0x4c) = *(int *)(&DAT_004fcaf0 + iVar3 * 0x4c) + 1;
        }
      }
    }
    else if ((*(int *)(&DAT_004fcaf4 + iVar4) == 1) &&
            (iVar1 = *(int *)(&DAT_004fcaf8 + iVar4), *(int *)(&DAT_004fcaf8 + iVar4) = iVar1 + -1,
            iVar1 + -1 < 1)) {
      *(undefined4 *)(&DAT_004fcaf8 + iVar4) = 9;
      iVar1 = *(int *)(&DAT_004fcaf0 + iVar4);
      *(int *)(&DAT_004fcaf0 + iVar4) = iVar1 + 1;
      if (0xc < iVar1 + 1) {
        *(undefined4 *)(&DAT_004fcaf0 + iVar4) = 8;
      }
    }
    local_10[1] = 0;
    local_4 = 0x98;
    local_10[0] = ((&DAT_004fcaec)[iVar2 * 0x13] * 0xd + *(int *)(&DAT_004fcaf0 + iVar4)) * 0x98;
    local_10[2] = local_10[0] + 0x98;
    BlitColorKeyedSurfaceClipped
              (DAT_005092ac,(&DAT_004fcab0)[iVar2 * 0x13],(&DAT_004fcab4)[iVar2 * 0x13],local_10);
  }
  return;
}

