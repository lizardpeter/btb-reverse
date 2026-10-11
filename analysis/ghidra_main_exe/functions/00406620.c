/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406620; function: FUN_00406620; body bytes: 349
 * callers: 0; callees: 7; success: True
 */


undefined4 * __thiscall FUN_00406620(undefined4 *param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  uint uVar6;
  uint uVar7;
  undefined4 *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0043a518;
  local_c = (undefined4 *)ExceptionList;
  ExceptionList = &local_c;
  FUN_004303f7(param_1,param_2);
  puVar5 = param_1 + 3;
  local_4 = 0;
  *(undefined1 *)puVar5 = *(undefined1 *)(param_2 + 0xc);
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  uVar7 = DAT_0043b334;
  uVar2 = *(uint *)(param_2 + 0x14);
  uVar6 = uVar2;
  if (DAT_0043b334 < uVar2) {
    uVar6 = DAT_0043b334;
  }
  if (puVar5 == (undefined4 *)(param_2 + 0xc)) {
    if (uVar6 != 0) {
      FUN_0043a0f5();
    }
    MsvcStringDetachSharedBuffer(puVar5);
    uVar2 = param_1[5] - uVar6;
    if (uVar2 < uVar7) {
      uVar7 = uVar2;
    }
    if (uVar7 != 0) {
      FUN_00430500((undefined4 *)(uVar6 + param_1[4]),
                   (undefined4 *)((int)(uVar6 + param_1[4]) + uVar7),uVar2 - uVar7);
      iVar1 = param_1[5];
      uVar3 = MsvcStringGrow(puVar5,iVar1 - uVar7,'\0');
      if ((char)uVar3 != '\0') {
        MsvcStringSetEnd(puVar5,iVar1 - uVar7);
      }
    }
    MsvcStringDetachSharedBuffer(puVar5);
  }
  else {
    if ((uVar6 != 0) && (uVar6 == uVar2)) {
      puVar4 = *(undefined4 **)(param_2 + 0x10);
      if (puVar4 == (undefined4 *)0x0) {
        puVar4 = &DAT_0043b358;
      }
      if (*(byte *)((int)puVar4 + -1) < 0xfe) {
        MsvcStringTidy(puVar5,'\x01');
        puVar5 = *(undefined4 **)(param_2 + 0x10);
        if (puVar5 == (undefined4 *)0x0) {
          puVar5 = &DAT_0043b358;
        }
        param_1[4] = puVar5;
        param_1[5] = *(undefined4 *)(param_2 + 0x14);
        param_1[6] = *(undefined4 *)(param_2 + 0x18);
        *(char *)((int)puVar5 + -1) = *(char *)((int)puVar5 + -1) + '\x01';
        goto LAB_004066d2;
      }
    }
    uVar3 = MsvcStringGrow(puVar5,uVar6,'\x01');
    if ((char)uVar3 != '\0') {
      puVar5 = *(undefined4 **)(param_2 + 0x10);
      if (puVar5 == (undefined4 *)0x0) {
        puVar5 = &DAT_0043b358;
      }
      puVar4 = (undefined4 *)param_1[4];
      for (uVar2 = uVar6 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
        *puVar4 = *puVar5;
        puVar5 = puVar5 + 1;
        puVar4 = puVar4 + 1;
      }
      for (uVar2 = uVar6 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        *(undefined1 *)puVar4 = *(undefined1 *)puVar5;
        puVar5 = (undefined4 *)((int)puVar5 + 1);
        puVar4 = (undefined4 *)((int)puVar4 + 1);
      }
      param_1[5] = uVar6;
      *(undefined1 *)(param_1[4] + uVar6) = 0;
      *param_1 = &PTR_FUN_0043b328;
      ExceptionList = local_c;
      return local_c;
    }
  }
LAB_004066d2:
  *param_1 = &PTR_FUN_0043b328;
  ExceptionList = local_c;
  return local_c;
}

