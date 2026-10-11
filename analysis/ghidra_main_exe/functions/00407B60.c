/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407b60; function: ReleaseInputAndCursorResources; body bytes: 683
 * callers: 2; callees: 2; success: True
 */


void ReleaseInputAndCursorResources(void)

{
  UnregisterBitmapSurface(0x4fbf88);
  if (DAT_004fbf88 != (int *)0x0) {
    (**(code **)(*DAT_004fbf88 + 8))(DAT_004fbf88);
    DAT_004fbf88 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x4fbd38);
  if (DAT_004fbd38 != (int *)0x0) {
    (**(code **)(*DAT_004fbd38 + 8))(DAT_004fbd38);
    DAT_004fbd38 = (int *)0x0;
  }
  if (DAT_004fbf90 != (int *)0x0) {
    (**(code **)(*DAT_004fbf90 + 8))(DAT_004fbf90);
  }
  DAT_004fbf90 = (int *)0x0;
  if (DAT_004fbf94 != (int *)0x0) {
    (**(code **)(*DAT_004fbf94 + 0x20))(DAT_004fbf94);
    (**(code **)(*DAT_004fbf94 + 8))(DAT_004fbf94);
    DAT_004fbf94 = (int *)0x0;
  }
  if (DAT_004fbf98 != (int *)0x0) {
    (**(code **)(*DAT_004fbf98 + 0x20))(DAT_004fbf98);
    (**(code **)(*DAT_004fbf98 + 8))(DAT_004fbf98);
    DAT_004fbf98 = (int *)0x0;
  }
  if (DAT_004fbf8c != (undefined4 *)0x0) {
    (**(code **)*DAT_004fbf8c)(1);
    DAT_004fbf8c = (undefined4 *)0x0;
  }
  if (DAT_0051c2b8 != (undefined4 *)0x0) {
    (**(code **)*DAT_0051c2b8)(1);
    DAT_0051c2b8 = (undefined4 *)0x0;
  }
  UnregisterBitmapSurface(0x51c288);
  if (DAT_0051c288 != (int *)0x0) {
    (**(code **)(*DAT_0051c288 + 8))(DAT_0051c288);
    DAT_0051c288 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51c28c);
  if (DAT_0051c28c != (int *)0x0) {
    (**(code **)(*DAT_0051c28c + 8))(DAT_0051c28c);
    DAT_0051c28c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51c290);
  if (DAT_0051c290 != (int *)0x0) {
    (**(code **)(*DAT_0051c290 + 8))(DAT_0051c290);
    DAT_0051c290 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51c298);
  if (DAT_0051c298 != (int *)0x0) {
    (**(code **)(*DAT_0051c298 + 8))(DAT_0051c298);
    DAT_0051c298 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x519954);
  if (DAT_00519954 != (int *)0x0) {
    (**(code **)(*DAT_00519954 + 8))(DAT_00519954);
    DAT_00519954 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51be40);
  if (DAT_0051be40 != (int *)0x0) {
    (**(code **)(*DAT_0051be40 + 8))(DAT_0051be40);
    DAT_0051be40 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51be44);
  if (DAT_0051be44 != (int *)0x0) {
    (**(code **)(*DAT_0051be44 + 8))(DAT_0051be44);
    DAT_0051be44 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51be34);
  if (DAT_0051be34 != (int *)0x0) {
    (**(code **)(*DAT_0051be34 + 8))(DAT_0051be34);
    DAT_0051be34 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51be38);
  if (DAT_0051be38 != (int *)0x0) {
    (**(code **)(*DAT_0051be38 + 8))(DAT_0051be38);
    DAT_0051be38 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x519944);
  if (DAT_00519944 != (int *)0x0) {
    (**(code **)(*DAT_00519944 + 8))(DAT_00519944);
    DAT_00519944 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51c2a0);
  if (DAT_0051c2a0 != (int *)0x0) {
    (**(code **)(*DAT_0051c2a0 + 8))(DAT_0051c2a0);
    DAT_0051c2a0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51c2a4);
  if (DAT_0051c2a4 != (int *)0x0) {
    (**(code **)(*DAT_0051c2a4 + 8))(DAT_0051c2a4);
    DAT_0051c2a4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51c2a8);
  if (DAT_0051c2a8 != (int *)0x0) {
    (**(code **)(*DAT_0051c2a8 + 8))(DAT_0051c2a8);
    DAT_0051c2a8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51c2ac);
  if (DAT_0051c2ac != (int *)0x0) {
    (**(code **)(*DAT_0051c2ac + 8))(DAT_0051c2ac);
    DAT_0051c2ac = (int *)0x0;
  }
  SaveAndUnloadPlayerProfiles();
  return;
}

