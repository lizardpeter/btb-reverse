/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00424f00; function: UnloadSquirrelActivityResources; body bytes: 601
 * callers: 1; callees: 3; success: True
 */


void UnloadSquirrelActivityResources(void)

{
  int *piVar1;
  
  UnregisterBitmapSurface(0x515068);
  if (DAT_00515068 != (int *)0x0) {
    (**(code **)(*DAT_00515068 + 8))(DAT_00515068);
    DAT_00515068 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51506c);
  if (DAT_0051506c != (int *)0x0) {
    (**(code **)(*DAT_0051506c + 8))(DAT_0051506c);
    DAT_0051506c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x515070);
  if (DAT_00515070 != (int *)0x0) {
    (**(code **)(*DAT_00515070 + 8))(DAT_00515070);
    DAT_00515070 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x515074);
  if (DAT_00515074 != (int *)0x0) {
    (**(code **)(*DAT_00515074 + 8))(DAT_00515074);
    DAT_00515074 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51509c);
  if (DAT_0051509c != (int *)0x0) {
    (**(code **)(*DAT_0051509c + 8))(DAT_0051509c);
    DAT_0051509c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150a0);
  if (DAT_005150a0 != (int *)0x0) {
    (**(code **)(*DAT_005150a0 + 8))(DAT_005150a0);
    DAT_005150a0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150a4);
  if (DAT_005150a4 != (int *)0x0) {
    (**(code **)(*DAT_005150a4 + 8))(DAT_005150a4);
    DAT_005150a4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150a8);
  if (DAT_005150a8 != (int *)0x0) {
    (**(code **)(*DAT_005150a8 + 8))(DAT_005150a8);
    DAT_005150a8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150ac);
  if (DAT_005150ac != (int *)0x0) {
    (**(code **)(*DAT_005150ac + 8))(DAT_005150ac);
    DAT_005150ac = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150b0);
  if (DAT_005150b0 != (int *)0x0) {
    (**(code **)(*DAT_005150b0 + 8))(DAT_005150b0);
    DAT_005150b0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150b4);
  if (DAT_005150b4 != (int *)0x0) {
    (**(code **)(*DAT_005150b4 + 8))(DAT_005150b4);
    DAT_005150b4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150b8);
  if (DAT_005150b8 != (int *)0x0) {
    (**(code **)(*DAT_005150b8 + 8))(DAT_005150b8);
    DAT_005150b8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150bc);
  if (DAT_005150bc != (int *)0x0) {
    (**(code **)(*DAT_005150bc + 8))(DAT_005150bc);
    DAT_005150bc = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150c0);
  if (DAT_005150c0 != (int *)0x0) {
    (**(code **)(*DAT_005150c0 + 8))(DAT_005150c0);
    DAT_005150c0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150c4);
  if (DAT_005150c4 != (int *)0x0) {
    (**(code **)(*DAT_005150c4 + 8))(DAT_005150c4);
    DAT_005150c4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5150c8);
  if (DAT_005150c8 != (int *)0x0) {
    (**(code **)(*DAT_005150c8 + 8))(DAT_005150c8);
    DAT_005150c8 = (int *)0x0;
  }
  piVar1 = &DAT_004fc084;
  do {
    if ((undefined4 *)*piVar1 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar1)(1);
      *piVar1 = 0;
    }
    piVar1 = piVar1 + 1;
  } while ((int)piVar1 < 0x4fc0d4);
  SetCursorSurface((int *)0x0);
  DAT_00519940 = 0;
  StopActivityMusic();
  return;
}

