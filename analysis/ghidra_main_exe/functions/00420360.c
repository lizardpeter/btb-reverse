/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00420360; function: UnloadSpudMazeActivityResources; body bytes: 507
 * callers: 1; callees: 2; success: True
 */


void UnloadSpudMazeActivityResources(void)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  
  piVar3 = &DAT_005144e4;
  do {
    UnregisterBitmapSurface((int)piVar3);
    piVar5 = (int *)*piVar3;
    if (piVar5 != (int *)0x0) {
      (**(code **)(*piVar5 + 8))(piVar5);
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 1;
  } while ((int)piVar3 < 0x5144f8);
  UnregisterBitmapSurface(0x5144f8);
  if (DAT_005144f8 != (int *)0x0) {
    (**(code **)(*DAT_005144f8 + 8))(DAT_005144f8);
    DAT_005144f8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5144fc);
  if (DAT_005144fc != (int *)0x0) {
    (**(code **)(*DAT_005144fc + 8))(DAT_005144fc);
    DAT_005144fc = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51450c);
  if (DAT_0051450c != (int *)0x0) {
    (**(code **)(*DAT_0051450c + 8))(DAT_0051450c);
    DAT_0051450c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x514500);
  if (DAT_00514500 != (int *)0x0) {
    (**(code **)(*DAT_00514500 + 8))(DAT_00514500);
    DAT_00514500 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x514504);
  if (DAT_00514504 != (int *)0x0) {
    (**(code **)(*DAT_00514504 + 8))(DAT_00514504);
    DAT_00514504 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x514508);
  if (DAT_00514508 != (int *)0x0) {
    (**(code **)(*DAT_00514508 + 8))(DAT_00514508);
    DAT_00514508 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x514494);
  if (DAT_00514494 != (int *)0x0) {
    (**(code **)(*DAT_00514494 + 8))(DAT_00514494);
    DAT_00514494 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5144d0);
  if (DAT_005144d0 != (int *)0x0) {
    (**(code **)(*DAT_005144d0 + 8))(DAT_005144d0);
    DAT_005144d0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5144b8);
  if (DAT_005144b8 != (int *)0x0) {
    (**(code **)(*DAT_005144b8 + 8))(DAT_005144b8);
    DAT_005144b8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5144c4);
  if (DAT_005144c4 != (int *)0x0) {
    (**(code **)(*DAT_005144c4 + 8))(DAT_005144c4);
    DAT_005144c4 = (int *)0x0;
  }
  piVar3 = &DAT_004fc084;
  do {
    if ((undefined4 *)*piVar3 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar3)(1);
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 1;
  } while ((int)piVar3 < 0x4fc0d4);
  piVar3 = &DAT_005141ec;
  piVar5 = &DAT_004459cc;
  do {
    iVar2 = 0;
    piVar4 = piVar3;
    if (0 < *piVar5) {
      do {
        UnregisterBitmapSurface((int)piVar4);
        piVar1 = (int *)*piVar4;
        if (piVar1 != (int *)0x0) {
          (**(code **)(*piVar1 + 8))(piVar1);
          *piVar4 = 0;
        }
        iVar2 = iVar2 + 1;
        piVar4 = piVar4 + 1;
      } while (iVar2 < *piVar5);
    }
    piVar5 = piVar5 + 1;
    piVar3 = piVar3 + 0xb;
  } while ((int)piVar5 < 0x4459e0);
  DAT_00519940 = 0;
  StopActivityMusic();
  return;
}

