/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040a530; function: UnloadDinoActivityResources; body bytes: 315
 * callers: 2; callees: 3; success: True
 */


void UnloadDinoActivityResources(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  
  UnregisterBitmapSurface(0x4fc9e8);
  if (DAT_004fc9e8 != (int *)0x0) {
    (**(code **)(*DAT_004fc9e8 + 8))(DAT_004fc9e8);
    DAT_004fc9e8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50a5c4);
  if (DAT_0050a5c4 != (int *)0x0) {
    (**(code **)(*DAT_0050a5c4 + 8))(DAT_0050a5c4);
    DAT_0050a5c4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50a5c8);
  if (DAT_0050a5c8 != (int *)0x0) {
    (**(code **)(*DAT_0050a5c8 + 8))(DAT_0050a5c8);
    DAT_0050a5c8 = (int *)0x0;
  }
  iVar3 = 0;
  if (0 < DAT_0043ee8c) {
    piVar2 = &DAT_004fc9ec;
    do {
      UnregisterBitmapSurface((int)piVar2);
      piVar1 = (int *)*piVar2;
      if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 8))(piVar1);
        *piVar2 = 0;
      }
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar3 < DAT_0043ee8c);
  }
  piVar2 = &DAT_004fc084;
  do {
    if ((undefined4 *)*piVar2 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar2)(1);
      *piVar2 = 0;
    }
    piVar2 = piVar2 + 1;
  } while ((int)piVar2 < 0x4fc0d4);
  UnregisterBitmapSurface(0x4fc2ac);
  if (DAT_004fc2ac != (int *)0x0) {
    (**(code **)(*DAT_004fc2ac + 8))(DAT_004fc2ac);
    DAT_004fc2ac = (int *)0x0;
  }
  UnregisterBitmapSurface(0x4fc434);
  if (DAT_004fc434 != (int *)0x0) {
    (**(code **)(*DAT_004fc434 + 8))(DAT_004fc434);
    DAT_004fc434 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x508ad0);
  if (DAT_00508ad0 != (int *)0x0) {
    (**(code **)(*DAT_00508ad0 + 8))(DAT_00508ad0);
    DAT_00508ad0 = (int *)0x0;
  }
  SetCursorSurface((int *)0x0);
  DAT_00519940 = 0;
  StopActivityMusic();
  return;
}

