/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403080; function: InitializeFullscreenDisplay; body bytes: 347
 * callers: 1; callees: 2; success: True
 */


undefined4 __fastcall InitializeFullscreenDisplay(undefined4 *param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  undefined **ppuVar4;
  int *piStack_d0;
  undefined4 uStack_cc;
  undefined4 uStack_c8;
  undefined4 uStack_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  int *piStack_b8;
  undefined *apuStack_b0 [4];
  undefined4 uStack_a0;
  undefined4 uStack_c;
  
  uStack_a0 = 0x403090;
  (**(code **)*param_1)();
  puVar1 = param_1 + 1;
  uStack_a0 = 0;
  apuStack_b0[3] = &DAT_0043b568;
  apuStack_b0[1] = (undefined *)0x0;
  apuStack_b0[0] = (undefined *)0x4030a2;
  apuStack_b0[2] = (undefined *)puVar1;
  iVar3 = DirectDrawCreateEx();
  if (iVar3 < 0) {
    return 0x80004005;
  }
  piStack_b8 = (int *)*puVar1;
  apuStack_b0[0] = (undefined *)0x11;
  uStack_bc = 0x4030ca;
  iVar3 = (**(code **)(*piStack_b8 + 0x50))();
  if (iVar3 < 0) {
    return 0x80004005;
  }
  piStack_d0 = (int *)*puVar1;
  uStack_bc = 0;
  uStack_c0 = 0;
  iVar3 = (**(code **)(*piStack_d0 + 0x54))();
  if (iVar3 < 0) {
    return 0x80004005;
  }
  piVar2 = (int *)*puVar1;
  ppuVar4 = apuStack_b0;
  for (iVar3 = 0x1f; iVar3 != 0; iVar3 = iVar3 + -1) {
    *ppuVar4 = (undefined *)0x0;
    ppuVar4 = ppuVar4 + 1;
  }
  apuStack_b0[0] = (undefined *)0x7c;
  apuStack_b0[1] = (undefined *)0x21;
  iVar3 = (**(code **)(*piVar2 + 0x18))(piVar2,apuStack_b0,param_1 + 2,0);
  if (iVar3 < 0) {
    return 0x80004005;
  }
  piVar2 = (int *)param_1[2];
  uStack_cc = 0;
  uStack_c8 = 0;
  piStack_d0 = (int *)0x4;
  uStack_c4 = 0;
  iVar3 = (**(code **)(*piVar2 + 0x30))(piVar2,&piStack_d0,param_1 + 3);
  if (iVar3 < 0) {
    return 0x80004005;
  }
  piVar2 = (int *)param_1[3];
  (**(code **)(*piVar2 + 4))(piVar2);
  param_1[5] = uStack_c;
  param_1[10] = 0;
  UpdateDisplayDestinationRect((int)param_1);
  return 0;
}

