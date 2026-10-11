/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041c710; function: DrawMazeScreenTransition; body bytes: 724
 * callers: 1; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DrawMazeScreenTransition(void)

{
  int *piVar1;
  int iVar2;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  local_10 = 0;
  local_8 = 0x280;
  local_c = 0;
  local_4 = 0x1e0;
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,(&DAT_00512100)[DAT_00510cf0],&local_10,0);
  DrawMazeTimer();
  if (DAT_00512128 == 1) {
    DAT_00512128 = 2;
    if ((DAT_00510cf0 == 0) || ((DAT_00510cf0 == 1 && (DAT_005120f8 == 2)))) {
      DAT_005107d8 = 0x15;
      DAT_00512150 = 1;
      _DAT_005107dc = 0x14;
      _DAT_005107e4 = 400;
      DAT_005107e0 = 0x26c;
      _DAT_005120dc = 0x14;
      _DAT_005120e4 = 400;
      _DAT_005120d8 = 0x14;
      DAT_005120e0 = 0x15;
LAB_0041c850:
      DAT_005107d8 = DAT_005107d8 + DAT_00444234 / 5;
      DAT_005120e0 = DAT_005120e0 + DAT_00444234 / 5;
      iVar2 = 0x26c - DAT_005120e0;
      DAT_005144dc = 0x14 - DAT_005107d8;
      if (0x26b < DAT_005120e0) {
        _DAT_0051214c = 0;
        DAT_00512150 = 0;
        DAT_00512128 = 0;
      }
      DAT_005144d8 = iVar2;
      (**(code **)(*piVar1 + 0x1c))(piVar1,0x14,0x14,(&DAT_00512100)[DAT_00510cf0],&DAT_005107d8,0);
      (**(code **)(*piVar1 + 0x1c))
                (piVar1,(DAT_005107e0 - DAT_005107d8) + 0x14,0x14,(&DAT_00512100)[DAT_005120f8],
                 &DAT_005120d8,0);
      goto LAB_0041c9b0;
    }
    _DAT_005107dc = 0x14;
    _DAT_005107e4 = 400;
    DAT_005107d8 = 0x26b;
    DAT_005107e0 = 0x26c;
    _DAT_005120dc = 0x14;
    _DAT_005120e4 = 400;
    _DAT_005120d8 = 0x14;
    DAT_005120e0 = 0x26c;
    DAT_00512150 = -1;
    DAT_00444234 = 10;
  }
  else if (0 < DAT_00512150) goto LAB_0041c850;
  DAT_005107d8 = DAT_005107d8 - DAT_00444234 / 5;
  DAT_005120e0 = DAT_005120e0 - DAT_00444234 / 5;
  DAT_005144d8 = 0x14 - DAT_005107d8;
  DAT_005144dc = DAT_005107e0 - DAT_005107d8;
  iVar2 = DAT_005120e0 + -0x14;
  if (DAT_005120e0 < 0x15) {
    _DAT_0051214c = 0;
    DAT_00512150 = 0;
    DAT_00512128 = 0;
  }
  (**(code **)(*piVar1 + 0x1c))(piVar1,0x14,0x14,(&DAT_00512100)[DAT_005120f8],&DAT_005107d8,0);
  (**(code **)(*piVar1 + 0x1c))
            (piVar1,(DAT_005107e0 - DAT_005107d8) + 0x14,0x14,(&DAT_00512100)[DAT_00510cf0],
             &DAT_005120d8,0);
LAB_0041c9b0:
  if (DAT_00444234 < 200) {
    DAT_00444234 = DAT_00444234 + 1;
  }
  if (iVar2 < DAT_00444234) {
    DAT_00444234 = iVar2;
  }
  if (DAT_00444234 < 10) {
    DAT_00444234 = 10;
  }
  return;
}

