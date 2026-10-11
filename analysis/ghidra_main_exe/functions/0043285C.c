/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043285c; function: FUN_0043285c; body bytes: 277
 * callers: 3; callees: 4; success: True
 */


uint __cdecl FUN_0043285c(uint param_1,int *param_2)

{
  uint uVar1;
  undefined *puVar2;
  char *pcVar3;
  int *piVar4;
  byte bVar5;
  undefined3 extraout_var;
  undefined *puVar6;
  int *piVar7;
  
  piVar4 = param_2;
  uVar1 = param_2[3];
  puVar2 = (undefined *)param_2[4];
  if (((uVar1 & 0x82) == 0) || ((uVar1 & 0x40) != 0)) {
LAB_00432965:
    param_2[3] = uVar1 | 0x20;
  }
  else {
    if ((uVar1 & 1) != 0) {
      param_2[1] = 0;
      if ((uVar1 & 0x10) == 0) goto LAB_00432965;
      *param_2 = param_2[2];
      param_2[3] = uVar1 & 0xfffffffe;
    }
    uVar1 = param_2[3];
    param_2[1] = 0;
    param_2 = (int *)0x0;
    piVar4[3] = uVar1 & 0xffffffef | 2;
    if (((uVar1 & 0x10c) == 0) &&
       (((piVar4 != (int *)&DAT_004498b0 && (piVar4 != (int *)&DAT_004498d0)) ||
        (bVar5 = FUN_004373b5((uint)puVar2), CONCAT31(extraout_var,bVar5) == 0)))) {
      FUN_00437371(piVar4);
    }
    if ((*(ushort *)(piVar4 + 3) & 0x108) == 0) {
      piVar7 = (int *)0x1;
      param_2 = (int *)FUN_0043407d(puVar2,(char *)&param_1,1);
    }
    else {
      pcVar3 = (char *)piVar4[2];
      piVar7 = (int *)(*piVar4 - (int)pcVar3);
      *piVar4 = (int)(pcVar3 + 1);
      piVar4[1] = piVar4[6] + -1;
      if ((int)piVar7 < 1) {
        if (puVar2 == (undefined *)0xffffffff) {
          puVar6 = &DAT_004497f8;
        }
        else {
          puVar6 = (undefined *)((&DAT_0051d920)[(int)puVar2 >> 5] + ((uint)puVar2 & 0x1f) * 8);
        }
        if ((puVar6[4] & 0x20) != 0) {
          FUN_004372d7((uint)puVar2,0,2);
        }
      }
      else {
        param_2 = (int *)FUN_0043407d(puVar2,pcVar3,(uint)piVar7);
      }
      *(undefined1 *)piVar4[2] = (undefined1)param_1;
    }
    if (param_2 == piVar7) {
      return param_1 & 0xff;
    }
    piVar4[3] = piVar4[3] | 0x20;
  }
  return 0xffffffff;
}

