/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004369e7; function: FUN_004369e7; body bytes: 428
 * callers: 1; callees: 1; success: True
 */


bool __cdecl FUN_004369e7(int *param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  
  if (DAT_00449d34 != 0) {
    uVar5 = param_1[5];
    if ((uVar5 != DAT_00449dc8) || (uVar5 != DAT_00449dd8)) {
      if (DAT_0051c570 == 0) {
        FUN_00436b93(1,1,uVar5,4,1,0,0,2,0,0,0);
        FUN_00436b93(0,1,param_1[5],10,5,0,0,2,0,0,0);
      }
      else {
        if (DAT_0051c610 != 0) {
          uVar6 = (uint)DAT_0051c616;
          uVar3 = 0;
          uVar4 = 0;
        }
        else {
          uVar3 = (uint)DAT_0051c614;
          uVar6 = 0;
          uVar4 = (uint)DAT_0051c616;
        }
        FUN_00436b93(1,(uint)(DAT_0051c610 == 0),uVar5,(uint)DAT_0051c612,uVar4,uVar3,uVar6,
                     (uint)DAT_0051c618,(uint)DAT_0051c61a,(uint)DAT_0051c61c,(uint)DAT_0051c61e);
        if (DAT_0051c5bc != 0) {
          uVar6 = (uint)DAT_0051c5c2;
          uVar3 = 0;
          uVar4 = 0;
          uVar5 = param_1[5];
        }
        else {
          uVar3 = (uint)DAT_0051c5c0;
          uVar6 = 0;
          uVar4 = (uint)DAT_0051c5c2;
          uVar5 = param_1[5];
        }
        FUN_00436b93(0,(uint)(DAT_0051c5bc == 0),uVar5,(uint)DAT_0051c5be,uVar4,uVar3,uVar6,
                     (uint)DAT_0051c5c4,(uint)DAT_0051c5c6,(uint)DAT_0051c5c8,(uint)DAT_0051c5ca);
      }
    }
    iVar1 = param_1[7];
    if (DAT_00449dcc < DAT_00449ddc) {
      if ((DAT_00449dcc <= iVar1) && (iVar1 <= DAT_00449ddc)) {
        if ((DAT_00449dcc < iVar1) && (iVar1 < DAT_00449ddc)) {
          return true;
        }
LAB_00436b5f:
        iVar2 = ((param_1[2] * 0x3c + param_1[1]) * 0x3c + *param_1) * 1000;
        if (iVar1 == DAT_00449dcc) {
          return DAT_00449dd0 <= iVar2;
        }
        return iVar2 < DAT_00449de0;
      }
    }
    else {
      if (iVar1 < DAT_00449ddc) {
        return true;
      }
      if (DAT_00449dcc < iVar1) {
        return true;
      }
      if ((iVar1 <= DAT_00449ddc) || (DAT_00449dcc <= iVar1)) goto LAB_00436b5f;
    }
  }
  return false;
}

