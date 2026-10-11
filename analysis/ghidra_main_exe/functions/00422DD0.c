/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00422dd0; function: DrawSpudMazeScreenTransition; body bytes: 596
 * callers: 1; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void DrawSpudMazeScreenTransition(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  if (DAT_00512128 == 1) {
    DAT_00512128 = 2;
    if (DAT_005144b4 != 2) {
      _DAT_00514104 = 0x14;
      _DAT_0051410c = 400;
      DAT_00514100 = 0x26a;
      DAT_00514108 = 0x26b;
      _DAT_005142cc = 0x14;
      _DAT_005142d4 = 400;
      _DAT_005142c8 = 0x14;
      DAT_005142d0 = 0x26c;
      DAT_00514534 = -1;
      DAT_00445f18 = 10;
LAB_00422f51:
      DAT_00514100 = DAT_00514100 - DAT_00445f18 / 5;
      DAT_005142d0 = DAT_005142d0 - DAT_00445f18 / 5;
      DAT_005144dc = DAT_00514108 - DAT_00514100;
      DAT_005144d8 = 0x14 - DAT_00514100;
      iVar4 = DAT_005142d0 + -0x14;
      if (DAT_005142d0 < 0x15) {
        _DAT_00514544 = 0;
        DAT_00514534 = 0;
        DAT_00512128 = 0;
      }
      (**(code **)(*piVar1 + 0x1c))(piVar1,0x14,0x14,(&DAT_005144e4)[DAT_005120f8],&DAT_00514100,0);
      iVar2 = *piVar1;
      iVar3 = DAT_00510cf0;
      goto LAB_00422fd4;
    }
    DAT_00514534 = 1;
    _DAT_00514104 = 0x14;
    _DAT_0051410c = 400;
    DAT_00514100 = 0x14;
    DAT_00514108 = 0x26b;
    _DAT_005142cc = 0x14;
    _DAT_005142d4 = 400;
    _DAT_005142c8 = 0x14;
    DAT_005142d0 = 0x16;
  }
  else if (DAT_00514534 < 1) goto LAB_00422f51;
  DAT_00514100 = DAT_00514100 + DAT_00445f18 / 5;
  DAT_005142d0 = DAT_005142d0 + DAT_00445f18 / 5;
  iVar4 = 0x26b - DAT_005142d0;
  DAT_005144dc = 0x14 - DAT_00514100;
  if (0x26a < DAT_005142d0) {
    _DAT_00514544 = 0;
    DAT_00514534 = 0;
    DAT_00512128 = 0;
  }
  DAT_005144d8 = iVar4;
  (**(code **)(*piVar1 + 0x1c))(piVar1,0x14,0x14,(&DAT_005144e4)[DAT_00510cf0],&DAT_00514100,0);
  iVar2 = *piVar1;
  iVar3 = DAT_005120f8;
LAB_00422fd4:
  (**(code **)(iVar2 + 0x1c))
            (piVar1,(DAT_00514108 - DAT_00514100) + 0x14,0x14,(&DAT_005144e4)[iVar3],&DAT_005142c8,0
            );
  if (DAT_00445f18 < 200) {
    DAT_00445f18 = DAT_00445f18 + 1;
  }
  if (iVar4 < DAT_00445f18) {
    DAT_00445f18 = iVar4;
  }
  if (DAT_00445f18 < 10) {
    DAT_00445f18 = 10;
  }
  return;
}

