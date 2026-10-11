/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040b190; function: UnloadParkDesignerActivityResources; body bytes: 1607
 * callers: 1; callees: 3; success: True
 */


void UnloadParkDesignerActivityResources(void)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  undefined4 *puVar4;
  
  SaveParkDesignerData();
  piVar3 = &DAT_004fc084;
  DAT_0050936c = 0;
  do {
    if ((undefined4 *)*piVar3 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar3)(1);
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 1;
  } while ((int)piVar3 < 0x4fc0d4);
  puVar4 = &DAT_00440294;
  for (iVar2 = 300; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  DAT_00519940 = 0;
  UnregisterBitmapSurface(0x509250);
  if (DAT_00509250 != (int *)0x0) {
    (**(code **)(*DAT_00509250 + 8))(DAT_00509250);
    DAT_00509250 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509254);
  if (DAT_00509254 != (int *)0x0) {
    (**(code **)(*DAT_00509254 + 8))(DAT_00509254);
    DAT_00509254 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509258);
  if (DAT_00509258 != (int *)0x0) {
    (**(code **)(*DAT_00509258 + 8))(DAT_00509258);
    DAT_00509258 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509264);
  if (DAT_00509264 != (int *)0x0) {
    (**(code **)(*DAT_00509264 + 8))(DAT_00509264);
    DAT_00509264 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509268);
  if (DAT_00509268 != (int *)0x0) {
    (**(code **)(*DAT_00509268 + 8))(DAT_00509268);
    DAT_00509268 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50925c);
  if (DAT_0050925c != (int *)0x0) {
    (**(code **)(*DAT_0050925c + 8))(DAT_0050925c);
    DAT_0050925c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509260);
  if (DAT_00509260 != (int *)0x0) {
    (**(code **)(*DAT_00509260 + 8))(DAT_00509260);
    DAT_00509260 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50926c);
  if (DAT_0050926c != (int *)0x0) {
    (**(code **)(*DAT_0050926c + 8))(DAT_0050926c);
    DAT_0050926c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509270);
  if (DAT_00509270 != (int *)0x0) {
    (**(code **)(*DAT_00509270 + 8))(DAT_00509270);
    DAT_00509270 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509274);
  if (DAT_00509274 != (int *)0x0) {
    (**(code **)(*DAT_00509274 + 8))(DAT_00509274);
    DAT_00509274 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092b4);
  if (DAT_005092b4 != (int *)0x0) {
    (**(code **)(*DAT_005092b4 + 8))(DAT_005092b4);
    DAT_005092b4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092b0);
  if (DAT_005092b0 != (int *)0x0) {
    (**(code **)(*DAT_005092b0 + 8))(DAT_005092b0);
    DAT_005092b0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x508ad0);
  if (DAT_00508ad0 != (int *)0x0) {
    (**(code **)(*DAT_00508ad0 + 8))(DAT_00508ad0);
    DAT_00508ad0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092b8);
  if (DAT_005092b8 != (int *)0x0) {
    (**(code **)(*DAT_005092b8 + 8))(DAT_005092b8);
    DAT_005092b8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092bc);
  if (DAT_005092bc != (int *)0x0) {
    (**(code **)(*DAT_005092bc + 8))(DAT_005092bc);
    DAT_005092bc = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092c0);
  if (DAT_005092c0 != (int *)0x0) {
    (**(code **)(*DAT_005092c0 + 8))(DAT_005092c0);
    DAT_005092c0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092c4);
  if (DAT_005092c4 != (int *)0x0) {
    (**(code **)(*DAT_005092c4 + 8))(DAT_005092c4);
    DAT_005092c4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092c8);
  if (DAT_005092c8 != (int *)0x0) {
    (**(code **)(*DAT_005092c8 + 8))(DAT_005092c8);
    DAT_005092c8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092cc);
  if (DAT_005092cc != (int *)0x0) {
    (**(code **)(*DAT_005092cc + 8))(DAT_005092cc);
    DAT_005092cc = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092d0);
  if (DAT_005092d0 != (int *)0x0) {
    (**(code **)(*DAT_005092d0 + 8))(DAT_005092d0);
    DAT_005092d0 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092d4);
  if (DAT_005092d4 != (int *)0x0) {
    (**(code **)(*DAT_005092d4 + 8))(DAT_005092d4);
    DAT_005092d4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092d8);
  if (DAT_005092d8 != (int *)0x0) {
    (**(code **)(*DAT_005092d8 + 8))(DAT_005092d8);
    DAT_005092d8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x5092dc);
  if (DAT_005092dc != (int *)0x0) {
    (**(code **)(*DAT_005092dc + 8))(DAT_005092dc);
    DAT_005092dc = (int *)0x0;
  }
  piVar3 = &DAT_00507a14;
  do {
    UnregisterBitmapSurface((int)piVar3);
    piVar1 = (int *)*piVar3;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 1;
  } while ((int)piVar3 < 0x507a20);
  piVar3 = &DAT_00504190;
  do {
    iVar2 = 2;
    do {
      UnregisterBitmapSurface((int)piVar3);
      piVar1 = (int *)*piVar3;
      if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 8))(piVar1);
        *piVar3 = 0;
      }
      piVar3 = piVar3 + 1;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  } while ((int)piVar3 < 0x5041b8);
  piVar3 = &DAT_00507b18;
  do {
    UnregisterBitmapSurface((int)piVar3);
    piVar1 = (int *)*piVar3;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 1;
  } while ((int)piVar3 < 0x507b3c);
  UnregisterBitmapSurface(0x507a00);
  if (DAT_00507a00 != (int *)0x0) {
    (**(code **)(*DAT_00507a00 + 8))(DAT_00507a00);
    DAT_00507a00 = (int *)0x0;
  }
  iVar2 = 0;
  do {
    piVar3 = (int *)((int)&DAT_00507b3c + iVar2);
    UnregisterBitmapSurface((int)piVar3);
    piVar1 = (int *)*piVar3;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar3 = 0;
    }
    piVar3 = (int *)((int)&DAT_004fca94 + iVar2);
    UnregisterBitmapSurface((int)piVar3);
    piVar1 = (int *)*piVar3;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar3 = 0;
    }
    iVar2 = iVar2 + 4;
  } while (iVar2 < 0x14);
  UnregisterBitmapSurface(0x4fca90);
  if (DAT_004fca90 != (int *)0x0) {
    (**(code **)(*DAT_004fca90 + 8))(DAT_004fca90);
    DAT_004fca90 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x4fcaa8);
  if (DAT_004fcaa8 != (int *)0x0) {
    (**(code **)(*DAT_004fcaa8 + 8))(DAT_004fcaa8);
    DAT_004fcaa8 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x507b54);
  if (DAT_00507b54 != (int *)0x0) {
    (**(code **)(*DAT_00507b54 + 8))(DAT_00507b54);
    DAT_00507b54 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x508c0c);
  if (DAT_00508c0c != (int *)0x0) {
    (**(code **)(*DAT_00508c0c + 8))(DAT_00508c0c);
    DAT_00508c0c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509278);
  if (DAT_00509278 != (int *)0x0) {
    (**(code **)(*DAT_00509278 + 8))(DAT_00509278);
    DAT_00509278 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50927c);
  if (DAT_0050927c != (int *)0x0) {
    (**(code **)(*DAT_0050927c + 8))(DAT_0050927c);
    DAT_0050927c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509280);
  if (DAT_00509280 != (int *)0x0) {
    (**(code **)(*DAT_00509280 + 8))(DAT_00509280);
    DAT_00509280 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509284);
  if (DAT_00509284 != (int *)0x0) {
    (**(code **)(*DAT_00509284 + 8))(DAT_00509284);
    DAT_00509284 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509288);
  if (DAT_00509288 != (int *)0x0) {
    (**(code **)(*DAT_00509288 + 8))(DAT_00509288);
    DAT_00509288 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50928c);
  if (DAT_0050928c != (int *)0x0) {
    (**(code **)(*DAT_0050928c + 8))(DAT_0050928c);
    DAT_0050928c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509290);
  if (DAT_00509290 != (int *)0x0) {
    (**(code **)(*DAT_00509290 + 8))(DAT_00509290);
    DAT_00509290 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509294);
  if (DAT_00509294 != (int *)0x0) {
    (**(code **)(*DAT_00509294 + 8))(DAT_00509294);
    DAT_00509294 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x509298);
  if (DAT_00509298 != (int *)0x0) {
    (**(code **)(*DAT_00509298 + 8))(DAT_00509298);
    DAT_00509298 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50929c);
  if (DAT_0050929c != (int *)0x0) {
    (**(code **)(*DAT_0050929c + 8))(DAT_0050929c);
    DAT_0050929c = (int *)0x0;
  }
  piVar3 = &DAT_00508ad4;
  do {
    iVar2 = 3;
    do {
      UnregisterBitmapSurface((int)piVar3);
      piVar1 = (int *)*piVar3;
      if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 8))(piVar1);
        *piVar3 = 0;
      }
      piVar3 = piVar3 + 1;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  } while ((int)piVar3 < 0x508b04);
  StopActivityMusic();
  return;
}

