/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d2e0; function: FindTopmostParkDesignerObjectAtCursor; body bytes: 307
 * callers: 1; callees: 1; success: True
 */


int __cdecl FindTopmostParkDesignerObjectAtCursor(int param_1)

{
  int *piVar1;
  bool bVar2;
  undefined3 extraout_var;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int local_4;
  
  if ((DAT_004fbe54 == 1) || (DAT_004fbd50 == 1)) {
    local_4 = 399;
    piVar6 = &DAT_0050924c;
    iVar3 = DAT_004fbd24;
    iVar5 = DAT_004fbd30;
    do {
      piVar1 = (int *)*piVar6;
      iVar4 = piVar1[7];
      if (iVar4 != -1) {
        if (param_1 == 0) {
          if (((iVar4 != 7) && ((iVar4 < 100 || (199 < iVar4)))) && (piVar1[0xd] != DAT_00507e3c)) {
LAB_0040d370:
            if (((*piVar1 < iVar3) && (iVar3 < piVar1[2])) &&
               ((piVar1[1] < iVar5 && (iVar5 < piVar1[3])))) {
              if (piVar1[0xd] < 100) {
                iVar4 = 0;
              }
              else {
                iVar4 = (299 < piVar1[0xd]) + 1;
              }
              iVar4 = piVar1[0xf] + (piVar1[0xe] + iVar4 * 4) * 5;
              bVar2 = PointInPolygon(&DAT_005041b8 + iVar4 * 0x3c,(&DAT_00508b04)[iVar4] + -1,
                                     iVar3 - *piVar1,iVar5 - piVar1[1]);
              iVar3 = DAT_004fbd24;
              iVar5 = DAT_004fbd30;
              if (CONCAT31(extraout_var,bVar2) == 0) {
                return local_4;
              }
            }
          }
        }
        else if (((param_1 != 1) || (iVar4 < 0x65)) || (199 < iVar4)) goto LAB_0040d370;
      }
      piVar6 = piVar6 + -1;
      local_4 = local_4 + -1;
    } while (0x508c0f < (int)piVar6);
  }
  return -1;
}

