/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00419390; function: UnloadHerdingActivityResources; body bytes: 609
 * callers: 1; callees: 2; success: True
 */


void UnloadHerdingActivityResources(void)

{
  int *piVar1;
  int *piVar2;
  
  UnregisterBitmapSurface(0x510724);
  if (DAT_00510724 != (int *)0x0) {
    (**(code **)(*DAT_00510724 + 8))(DAT_00510724);
    DAT_00510724 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510728);
  if (DAT_00510728 != (int *)0x0) {
    (**(code **)(*DAT_00510728 + 8))(DAT_00510728);
    DAT_00510728 = (int *)0x0;
  }
  piVar2 = &DAT_004fc084;
  do {
    if ((undefined4 *)*piVar2 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar2)(1);
      *piVar2 = 0;
    }
    piVar2 = piVar2 + 1;
  } while ((int)piVar2 < 0x4fc0d4);
  UnregisterBitmapSurface(0x51072c);
  if (DAT_0051072c != (int *)0x0) {
    (**(code **)(*DAT_0051072c + 8))(DAT_0051072c);
    DAT_0051072c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510730);
  if (DAT_00510730 != (int *)0x0) {
    (**(code **)(*DAT_00510730 + 8))(DAT_00510730);
    DAT_00510730 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510734);
  if (DAT_00510734 != (int *)0x0) {
    (**(code **)(*DAT_00510734 + 8))(DAT_00510734);
    DAT_00510734 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51073c);
  if (DAT_0051073c != (int *)0x0) {
    (**(code **)(*DAT_0051073c + 8))(DAT_0051073c);
    DAT_0051073c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510740);
  if (DAT_00510740 != (int *)0x0) {
    (**(code **)(*DAT_00510740 + 8))(DAT_00510740);
    DAT_00510740 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510744);
  if (DAT_00510744 != (int *)0x0) {
    (**(code **)(*DAT_00510744 + 8))(DAT_00510744);
    DAT_00510744 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510738);
  if (DAT_00510738 != (int *)0x0) {
    (**(code **)(*DAT_00510738 + 8))(DAT_00510738);
    DAT_00510738 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510748);
  if (DAT_00510748 != (int *)0x0) {
    (**(code **)(*DAT_00510748 + 8))(DAT_00510748);
    DAT_00510748 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51074c);
  if (DAT_0051074c != (int *)0x0) {
    (**(code **)(*DAT_0051074c + 8))(DAT_0051074c);
    DAT_0051074c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510750);
  if (DAT_00510750 != (int *)0x0) {
    (**(code **)(*DAT_00510750 + 8))(DAT_00510750);
    DAT_00510750 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510754);
  if (DAT_00510754 != (int *)0x0) {
    (**(code **)(*DAT_00510754 + 8))(DAT_00510754);
    DAT_00510754 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x510758);
  if (DAT_00510758 != (int *)0x0) {
    (**(code **)(*DAT_00510758 + 8))(DAT_00510758);
    DAT_00510758 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51075c);
  if (DAT_0051075c != (int *)0x0) {
    (**(code **)(*DAT_0051075c + 8))(DAT_0051075c);
    DAT_0051075c = (int *)0x0;
  }
  piVar2 = &DAT_005105a0;
  do {
    UnregisterBitmapSurface((int)piVar2);
    piVar1 = (int *)*piVar2;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar2 = 0;
    }
    piVar2 = piVar2 + 1;
  } while ((int)piVar2 < 0x5105ac);
  DAT_0051071c = 0;
  DAT_00510720 = 0;
  DAT_00519940 = 0;
  StopActivityMusic();
  return;
}

