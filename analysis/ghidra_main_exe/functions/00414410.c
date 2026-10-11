/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00414410; function: UnloadGolfActivityResources; body bytes: 472
 * callers: 1; callees: 3; success: True
 */


void UnloadGolfActivityResources(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  
  DAT_0043ed18 = 1;
  UnregisterBitmapSurface(0x50aedc);
  if (DAT_0050aedc != (int *)0x0) {
    (**(code **)(*DAT_0050aedc + 8))(DAT_0050aedc);
    DAT_0050aedc = (int *)0x0;
  }
  iVar3 = 0;
  do {
    piVar2 = (int *)((int)&DAT_0050ab88 + iVar3);
    UnregisterBitmapSurface((int)piVar2);
    piVar1 = (int *)*piVar2;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar2 = 0;
    }
    piVar2 = (int *)((int)&DAT_0050adfc + iVar3);
    UnregisterBitmapSurface((int)piVar2);
    piVar1 = (int *)*piVar2;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar2 = 0;
    }
    piVar2 = (int *)((int)&DAT_0050addc + iVar3);
    UnregisterBitmapSurface((int)piVar2);
    piVar1 = (int *)*piVar2;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar2 = 0;
    }
    iVar3 = iVar3 + 4;
  } while (iVar3 < 0xc);
  UnregisterBitmapSurface(0x50aee0);
  if (DAT_0050aee0 != (int *)0x0) {
    (**(code **)(*DAT_0050aee0 + 8))(DAT_0050aee0);
    DAT_0050aee0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50aee4);
  if (DAT_0050aee4 != (int *)0x0) {
    (**(code **)(*DAT_0050aee4 + 8))(DAT_0050aee4);
    DAT_0050aee4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50aee8);
  if (DAT_0050aee8 != (int *)0x0) {
    (**(code **)(*DAT_0050aee8 + 8))(DAT_0050aee8);
    DAT_0050aee8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50aef0);
  if (DAT_0050aef0 != (int *)0x0) {
    (**(code **)(*DAT_0050aef0 + 8))(DAT_0050aef0);
    DAT_0050aef0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50aef4);
  if (DAT_0050aef4 != (int *)0x0) {
    (**(code **)(*DAT_0050aef4 + 8))(DAT_0050aef4);
    DAT_0050aef4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50aef8);
  if (DAT_0050aef8 != (int *)0x0) {
    (**(code **)(*DAT_0050aef8 + 8))(DAT_0050aef8);
    DAT_0050aef8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50adec);
  if (DAT_0050adec != (int *)0x0) {
    (**(code **)(*DAT_0050adec + 8))(DAT_0050adec);
    DAT_0050adec = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50aeec);
  if (DAT_0050aeec != (int *)0x0) {
    (**(code **)(*DAT_0050aeec + 8))(DAT_0050aeec);
    DAT_0050aeec = (int *)0x0;
  }
  piVar2 = &DAT_004fc084;
  do {
    if ((undefined4 *)*piVar2 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar2)(1);
      *piVar2 = 0;
    }
    piVar2 = piVar2 + 1;
  } while ((int)piVar2 < 0x4fc0d4);
  SetCursorSurface((int *)0x0);
  DAT_00519940 = 0;
  StopActivityMusic();
  return;
}

