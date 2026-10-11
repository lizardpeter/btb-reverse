/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00424100; function: UnloadSpudSkateActivityResources; body bytes: 291
 * callers: 1; callees: 3; success: True
 */


void UnloadSpudSkateActivityResources(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  
  DAT_0043ed18 = 1;
  UnregisterBitmapSurface(0x514ea0);
  if (DAT_00514ea0 != (int *)0x0) {
    (**(code **)(*DAT_00514ea0 + 8))(DAT_00514ea0);
    DAT_00514ea0 = (int *)0x0;
  }
  piVar2 = &DAT_004fc084;
  do {
    if ((undefined4 *)*piVar2 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar2)(1);
      *piVar2 = 0;
    }
    piVar2 = piVar2 + 1;
  } while ((int)piVar2 < 0x4fc0d4);
  DAT_00519940 = 0;
  iVar3 = 0;
  do {
    CloseBinkMovie(*(undefined4 *)((int)&DAT_00514b2c + iVar3));
    piVar2 = (int *)((int)&DAT_00514e8c + iVar3);
    UnregisterBitmapSurface((int)piVar2);
    piVar1 = (int *)*piVar2;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar2 = 0;
    }
    FUN_00430d2a(*(undefined **)((int)&DAT_00514620 + iVar3));
    iVar3 = iVar3 + 4;
  } while (iVar3 < 0x14);
  UnregisterBitmapSurface(0x5148b4);
  if (DAT_005148b4 != (int *)0x0) {
    (**(code **)(*DAT_005148b4 + 8))(DAT_005148b4);
    DAT_005148b4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x514b60);
  if (DAT_00514b60 != (int *)0x0) {
    (**(code **)(*DAT_00514b60 + 8))(DAT_00514b60);
    DAT_00514b60 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x514b64);
  if (DAT_00514b64 != (int *)0x0) {
    (**(code **)(*DAT_00514b64 + 8))(DAT_00514b64);
    DAT_00514b64 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x514b68);
  if (DAT_00514b68 != (int *)0x0) {
    (**(code **)(*DAT_00514b68 + 8))(DAT_00514b68);
    DAT_00514b68 = (int *)0x0;
  }
  return;
}

