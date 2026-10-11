/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402fe0; function: DisplayManagerDestructor; body bytes: 11
 * callers: 5; callees: 0; success: True
 */


undefined4 __fastcall DisplayManagerDestructor(undefined4 *param_1)

{
  int *piVar1;
  int *piVar2;
  
  *param_1 = &DAT_0043b2e0;
  piVar2 = param_1 + 4;
  UnregisterBitmapSurface((int)piVar2);
  piVar1 = (int *)*piVar2;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *piVar2 = 0;
  }
  piVar2 = param_1 + 3;
  UnregisterBitmapSurface((int)piVar2);
  piVar1 = (int *)*piVar2;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *piVar2 = 0;
  }
  piVar2 = param_1 + 2;
  UnregisterBitmapSurface((int)piVar2);
  piVar1 = (int *)*piVar2;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *piVar2 = 0;
  }
  piVar2 = (int *)param_1[1];
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 0x50))(piVar2,param_1[5],8);
  }
  piVar2 = (int *)param_1[1];
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 8))(piVar2);
  }
  return 0;
}

