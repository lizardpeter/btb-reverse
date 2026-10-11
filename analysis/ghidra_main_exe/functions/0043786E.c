/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043786e; function: FUN_0043786e; body bytes: 364
 * callers: 2; callees: 5; success: True
 */


undefined4 __cdecl FUN_0043786e(ushort *param_1,uint *param_2,int *param_3)

{
  ushort uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 local_1c [3];
  uint local_10;
  uint local_c;
  int local_8;
  
  uVar1 = param_1[5];
  local_10 = *(uint *)(param_1 + 3);
  local_c = *(uint *)(param_1 + 1);
  uVar3 = uVar1 & 0x7fff;
  iVar4 = uVar3 - 0x3fff;
  local_8 = (uint)*param_1 << 0x10;
  if (iVar4 == -0x3fff) {
    iVar4 = 0;
    iVar2 = FUN_004377c6((int *)&local_10);
    if (iVar2 != 0) {
LAB_0043799a:
      uVar5 = 0;
      goto LAB_0043799c;
    }
    FUN_004377ba(&local_10);
  }
  else {
    FUN_0043779f((int)local_1c,&local_10);
    iVar2 = FUN_00437713((int)&local_10,param_3[2]);
    if (iVar2 != 0) {
      iVar4 = uVar3 - 0x3ffe;
    }
    iVar2 = param_3[1];
    if (iVar4 < iVar2 - param_3[2]) {
      FUN_004377ba(&local_10);
    }
    else {
      if (iVar2 < iVar4) {
        if (*param_3 <= iVar4) {
          FUN_004377ba(&local_10);
          local_10 = local_10 | 0x80000000;
          FUN_004377e1(&local_10,param_3[3]);
          iVar4 = param_3[5] + *param_3;
          uVar5 = 1;
          goto LAB_0043799c;
        }
        local_10 = local_10 & 0x7fffffff;
        iVar4 = param_3[5] + iVar4;
        FUN_004377e1(&local_10,param_3[3]);
        goto LAB_0043799a;
      }
      FUN_0043779f((int)&local_10,local_1c);
      FUN_004377e1(&local_10,iVar2 - iVar4);
      FUN_00437713((int)&local_10,param_3[2]);
      FUN_004377e1(&local_10,param_3[3] + 1);
    }
  }
  iVar4 = 0;
  uVar5 = 2;
LAB_0043799c:
  local_10 = iVar4 << (0x1fU - (char)param_3[3] & 0x1f) |
             -(uint)((uVar1 & 0x8000) != 0) & 0x80000000 | local_10;
  if (param_3[4] == 0x40) {
    param_2[1] = local_10;
    *param_2 = local_c;
  }
  else if (param_3[4] == 0x20) {
    *param_2 = local_10;
  }
  return uVar5;
}

