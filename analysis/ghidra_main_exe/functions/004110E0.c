/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004110e0; function: UnloadFireworksEditorResources; body bytes: 210
 * callers: 2; callees: 1; success: True
 */


void UnloadFireworksEditorResources(void)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  
  UnregisterBitmapSurface(0x50ab18);
  if (DAT_0050ab18 != (int *)0x0) {
    (**(code **)(*DAT_0050ab18 + 8))(DAT_0050ab18);
    DAT_0050ab18 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50ab1c);
  if (DAT_0050ab1c != (int *)0x0) {
    (**(code **)(*DAT_0050ab1c + 8))(DAT_0050ab1c);
    DAT_0050ab1c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50aaa8);
  if (DAT_0050aaa8 != (int *)0x0) {
    (**(code **)(*DAT_0050aaa8 + 8))(DAT_0050aaa8);
    DAT_0050aaa8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50ab08);
  if (DAT_0050ab08 != (int *)0x0) {
    (**(code **)(*DAT_0050ab08 + 8))(DAT_0050ab08);
    DAT_0050ab08 = (int *)0x0;
  }
  piVar3 = &DAT_0050a4a0;
  do {
    piVar2 = piVar3 + -1;
    UnregisterBitmapSurface((int)piVar2);
    piVar1 = (int *)*piVar2;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar2 = 0;
    }
    UnregisterBitmapSurface((int)piVar3);
    piVar2 = (int *)*piVar3;
    if (piVar2 != (int *)0x0) {
      (**(code **)(*piVar2 + 8))(piVar2);
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 2;
  } while ((int)piVar3 < 0x50a4b8);
  return;
}

