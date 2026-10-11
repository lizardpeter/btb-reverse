/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004031e0; function: InitializeWindowedDisplay; body bytes: 662
 * callers: 1; callees: 10; success: True
 */


undefined4 __fastcall InitializeWindowedDisplay(undefined4 *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  DWORD dwExStyle;
  HMENU pHVar4;
  DWORD dwStyle;
  int unaff_EBX;
  undefined *unaff_EBP;
  int unaff_ESI;
  int unaff_EDI;
  undefined4 *puVar5;
  int *piVar6;
  int *piVar7;
  undefined *puStack_b8;
  LONG LStack_b4;
  undefined4 auStack_98 [22];
  undefined4 uStack_40;
  undefined4 uStack_30;
  int iStack_14;
  int iStack_10;
  HWND pHStack_c;
  
  LStack_b4 = 0x4031f0;
  (**(code **)*param_1)();
  puVar1 = param_1 + 1;
  LStack_b4 = 0;
  puStack_b8 = &DAT_0043b568;
  iVar2 = DirectDrawCreateEx();
  if (iVar2 < 0) {
    return 0x80004005;
  }
  iVar2 = (**(code **)(*(int *)*puVar1 + 0x50))();
  if (iVar2 < 0) {
    return 0x80004005;
  }
  uVar3 = GetWindowLongA(pHStack_c,-0x10);
  SetWindowLongA(pHStack_c,-0x10,uVar3 & 0x7f39ffff | 0xc60000);
  SetRect((LPRECT)&puStack_b8,0,0,iStack_14,iStack_10);
  dwExStyle = GetWindowLongA(pHStack_c,-0x14);
  pHVar4 = GetMenu(pHStack_c);
  uVar3 = (uint)(pHVar4 != (HMENU)0x0);
  dwStyle = GetWindowLongA(pHStack_c,-0x10);
  AdjustWindowRectEx((LPRECT)&puStack_b8,dwStyle,uVar3,dwExStyle);
  SetWindowPos(pHStack_c,(HWND)0x0,0,0,unaff_EDI - (int)puStack_b8,unaff_ESI - LStack_b4,0x16);
  SetWindowPos(pHStack_c,(HWND)0xfffffffe,0,0,0,0,0x13);
  SystemParametersInfoA(0x30,0,&stack0xffffff58,0);
  GetWindowRect(pHStack_c,(LPRECT)&puStack_b8);
  if ((int)puStack_b8 < (int)unaff_EBP) {
    puStack_b8 = unaff_EBP;
  }
  if (LStack_b4 < unaff_EBX) {
    LStack_b4 = unaff_EBX;
  }
  SetWindowPos(pHStack_c,(HWND)0x0,(int)puStack_b8,LStack_b4,0,0,0x15);
  puVar5 = auStack_98;
  for (iVar2 = 0x1f; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  auStack_98[0] = 0x7c;
  auStack_98[1] = 1;
  uStack_30 = 0x200;
  iVar2 = (**(code **)(*(int *)*puVar1 + 0x18))();
  if (iVar2 < 0) {
    return 0x80004005;
  }
  piVar7 = (int *)*puVar1;
  uStack_40 = 0x2040;
  iVar2 = (**(code **)(*piVar7 + 0x18))(piVar7,&stack0xffffff58,param_1 + 3,0);
  if (iVar2 < 0) {
    return 0x80004005;
  }
  piVar6 = (int *)0x0;
  iVar2 = (**(code **)(*(int *)*puVar1 + 0x10))((int *)*puVar1,0,&stack0xffffff24,0);
  if (iVar2 < 0) {
    return 0x80004005;
  }
  iVar2 = (**(code **)(*piVar7 + 0x20))(piVar7,0);
  if (iVar2 < 0) {
    (**(code **)(*piVar6 + 8))(piVar6);
    return 0x80004005;
  }
  iVar2 = (**(code **)(*(int *)param_1[2] + 0x70))((int *)param_1[2],piVar6);
  if (iVar2 < 0) {
    (**(code **)(pHStack_c->unused + 8))(pHStack_c);
    return 0x80004005;
  }
  (**(code **)(pHStack_c->unused + 8))(pHStack_c);
  param_1[5] = pHStack_c;
  param_1[10] = 1;
  UpdateDisplayDestinationRect((int)param_1);
  return 0;
}

