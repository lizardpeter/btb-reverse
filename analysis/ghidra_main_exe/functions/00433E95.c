/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00433e95; function: FUN_00433e95; body bytes: 368
 * callers: 1; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __cdecl FUN_00433e95(LPCSTR param_1,char *param_2,uint param_3,undefined4 *param_4)

{
  char cVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  undefined *puVar5;
  uint uVar6;
  uint uVar7;
  
  bVar4 = false;
  bVar3 = false;
  cVar1 = *param_2;
  if (cVar1 == 'a') {
    uVar6 = 0x109;
  }
  else {
    if (cVar1 == 'r') {
      uVar6 = 0;
      uVar7 = DAT_0051c668 | 1;
      goto LAB_00433ed6;
    }
    if (cVar1 != 'w') {
      return (undefined4 *)0x0;
    }
    uVar6 = 0x301;
  }
  uVar7 = DAT_0051c668 | 2;
LAB_00433ed6:
  bVar2 = true;
LAB_00433ed9:
  cVar1 = param_2[1];
  param_2 = param_2 + 1;
  if ((cVar1 == '\0') || (!bVar2)) {
    puVar5 = FUN_00437bfa(param_1,uVar6,param_3,0x1a4);
    if ((int)puVar5 < 0) {
      return (undefined4 *)0x0;
    }
    _DAT_0051c568 = _DAT_0051c568 + 1;
    param_4[3] = uVar7;
    param_4[1] = 0;
    *param_4 = 0;
    param_4[2] = 0;
    param_4[7] = 0;
    param_4[4] = puVar5;
    return param_4;
  }
  if (cVar1 < 'U') {
    if (cVar1 == 'T') {
      if ((uVar6 & 0x1000) == 0) {
        uVar6 = uVar6 | 0x1000;
        goto LAB_00433ed9;
      }
    }
    else if (cVar1 == '+') {
      if ((uVar6 & 2) == 0) {
        uVar6 = uVar6 & 0xfffffffe | 2;
        uVar7 = uVar7 & 0xfffffffc | 0x80;
        goto LAB_00433ed9;
      }
    }
    else if (cVar1 == 'D') {
      if ((uVar6 & 0x40) == 0) {
        uVar6 = uVar6 | 0x40;
        goto LAB_00433ed9;
      }
    }
    else if (cVar1 == 'R') {
      if (!bVar3) {
        bVar3 = true;
        uVar6 = uVar6 | 0x10;
        goto LAB_00433ed9;
      }
    }
    else if ((cVar1 == 'S') && (!bVar3)) {
      bVar3 = true;
      uVar6 = uVar6 | 0x20;
      goto LAB_00433ed9;
    }
  }
  else {
    if (cVar1 == 'b') {
      if ((uVar6 & 0xc000) != 0) goto LAB_00433fb9;
      uVar6 = uVar6 | 0x8000;
      goto LAB_00433ed9;
    }
    if (cVar1 == 'c') {
      if (!bVar4) {
        bVar4 = true;
        uVar7 = uVar7 | 0x4000;
        goto LAB_00433ed9;
      }
    }
    else {
      if (cVar1 != 'n') {
        if ((cVar1 != 't') || ((uVar6 & 0xc000) != 0)) goto LAB_00433fb9;
        uVar6 = uVar6 | 0x4000;
        goto LAB_00433ed9;
      }
      if (!bVar4) {
        bVar4 = true;
        uVar7 = uVar7 & 0xffffbfff;
        goto LAB_00433ed9;
      }
    }
  }
LAB_00433fb9:
  bVar2 = false;
  goto LAB_00433ed9;
}

