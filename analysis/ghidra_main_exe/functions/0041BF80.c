/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041bf80; function: DrawMazeTimer; body bytes: 399
 * callers: 2; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int DrawMazeTimer(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  longlong lVar6;
  int iVar7;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  int iStack_14;
  _SYSTEMTIME local_10;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  GetSystemTime(&local_10);
  SystemTimeToFileTime(&local_10,(LPFILETIME)&DAT_005120f0);
  uVar5 = Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
  DAT_005109c8 = (DAT_00510950 + DAT_00510d08) - (int)uVar5;
  if (1 < DAT_005120fc - DAT_005109c8) {
    DAT_00510d08 = DAT_00510d08 + (DAT_005120fc - DAT_005109c8);
    uVar5 = Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
    DAT_005109c8 = (DAT_00510d08 + DAT_00510950) - (int)uVar5;
  }
  iVar4 = 0;
  uStack_1c = 0;
  iStack_14 = 0x19;
  uStack_20 = 0;
  _DAT_00510d1c = DAT_005109c8;
  DAT_005120fc = DAT_005109c8;
  lVar6 = __ftol();
  uStack_18 = (undefined4)lVar6;
  (**(code **)(*piVar1 + 0x1c))(piVar1,0x1a6,0x1ae,DAT_005144d0,&uStack_20,1);
  iVar3 = 0;
  iVar7 = 0;
  iVar2 = ((int)(&DAT_00444224)[DAT_0051c284] / 2) * -0x14 + 0xf4;
  if (0 < (int)(&DAT_00444224)[DAT_0051c284]) {
    do {
      if (0xfa < iVar4) {
        iVar4 = 0;
        iVar3 = 0x14;
      }
      if (iVar7 < iStack_14) {
        (**(code **)(*piVar1 + 0x1c))(piVar1,iVar2 + iVar4,iVar3 + 0x1ae,DAT_0051211c,0,1);
      }
      else {
        (**(code **)(*piVar1 + 0x1c))(piVar1,iVar2 + iVar4,iVar3 + 0x1ae,DAT_00512118,0,1);
      }
      iVar4 = iVar4 + 0x14;
      iVar7 = iVar7 + 1;
    } while (iVar7 < (int)(&DAT_00444224)[DAT_0051c284]);
  }
  return iStack_14;
}

