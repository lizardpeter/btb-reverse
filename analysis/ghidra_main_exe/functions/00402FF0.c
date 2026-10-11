/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402ff0; function: ReleaseDisplayResources; body bytes: 131
 * callers: 0; callees: 1; success: True
 */


undefined4 __fastcall ReleaseDisplayResources(int param_1)

{
  int *piVar1;
  int *piVar2;
  
  piVar1 = (int *)(param_1 + 0x10);
  UnregisterBitmapSurface((int)piVar1);
  piVar2 = (int *)*piVar1;
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 8))(piVar2);
    *piVar1 = 0;
  }
  piVar1 = (int *)(param_1 + 0xc);
  UnregisterBitmapSurface((int)piVar1);
  piVar2 = (int *)*piVar1;
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 8))(piVar2);
    *piVar1 = 0;
  }
  piVar1 = (int *)(param_1 + 8);
  UnregisterBitmapSurface((int)piVar1);
  piVar2 = (int *)*piVar1;
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 8))(piVar2);
    *piVar1 = 0;
  }
  piVar1 = *(int **)(param_1 + 4);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x50))(piVar1,*(undefined4 *)(param_1 + 0x14),8);
  }
  piVar1 = *(int **)(param_1 + 4);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return 0;
}

