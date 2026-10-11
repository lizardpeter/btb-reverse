/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041a390; function: UnloadMazeActivityResources; body bytes: 331
 * callers: 2; callees: 2; success: True
 */


void UnloadMazeActivityResources(void)

{
  int *piVar1;
  int *piVar2;
  
  piVar2 = &DAT_00512100;
  do {
    UnregisterBitmapSurface((int)piVar2);
    piVar1 = (int *)*piVar2;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar2 = 0;
    }
    piVar2 = piVar2 + 1;
  } while ((int)piVar2 < 0x51210c);
  UnregisterBitmapSurface(0x51210c);
  if (DAT_0051210c != (int *)0x0) {
    (**(code **)(*DAT_0051210c + 8))(DAT_0051210c);
    DAT_0051210c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x512110);
  if (DAT_00512110 != (int *)0x0) {
    (**(code **)(*DAT_00512110 + 8))(DAT_00512110);
    DAT_00512110 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x512120);
  if (DAT_00512120 != (int *)0x0) {
    (**(code **)(*DAT_00512120 + 8))(DAT_00512120);
    DAT_00512120 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x512114);
  if (DAT_00512114 != (int *)0x0) {
    (**(code **)(*DAT_00512114 + 8))(DAT_00512114);
    DAT_00512114 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x512118);
  if (DAT_00512118 != (int *)0x0) {
    (**(code **)(*DAT_00512118 + 8))(DAT_00512118);
    DAT_00512118 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51211c);
  if (DAT_0051211c != (int *)0x0) {
    (**(code **)(*DAT_0051211c + 8))(DAT_0051211c);
    DAT_0051211c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5144d0);
  if (DAT_005144d0 != (int *)0x0) {
    (**(code **)(*DAT_005144d0 + 8))(DAT_005144d0);
    DAT_005144d0 = (int *)0x0;
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
  StopActivityMusic();
  DAT_0051c334 = 0;
  return;
}

