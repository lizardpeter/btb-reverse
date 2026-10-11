/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040dff0; function: CanPlaceParkDesignerObjectWithoutOverlap; body bytes: 700
 * callers: 1; callees: 1; success: True
 */


int __cdecl
CanPlaceParkDesignerObjectWithoutOverlap
          (int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  bool bVar1;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined3 extraout_var_01;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28 [10];
  
  local_60 = 1;
  local_5c = 0;
  piVar3 = &DAT_004fcac0;
  do {
    if ((local_5c != param_6) && (iVar2 = piVar3[3], iVar2 != -1)) {
      if (((param_5 == 1) &&
          ((((iVar2 != 300 || (piVar3[10] != 0)) || ((int)piVar3 < 0x500620)) ||
           (0x5023cf < (int)piVar3)))) && ((iVar2 < 100 || (199 < iVar2)))) {
        if (*piVar3 == -1) {
          piVar4 = piVar3 + -4;
        }
        else {
          iVar2 = (*piVar3 % 5 + (*piVar3 / 5) * 5) * 0x10;
          local_58 = *(int *)(&DAT_00441b18 + iVar2) + piVar3[-4];
          local_50 = *(int *)(&DAT_00441b20 + iVar2) + piVar3[-4];
          local_54 = *(int *)(&DAT_00441b1c + iVar2) + piVar3[-3];
          piVar4 = &local_58;
          local_4c = *(int *)(&DAT_00441b24 + iVar2) + piVar3[-3];
        }
        bVar1 = ParkDesignerRectanglesDoNotOverlapBySamplePoints
                          (param_1,param_2,param_3,param_4,piVar4);
        if ((CONCAT31(extraout_var,bVar1) == 0) && (399 < piVar3[3])) {
          piVar3[3] = -1;
        }
      }
      if ((((piVar3[3] == 300) && (piVar3[10] == 0)) && (0x50061f < (int)piVar3)) &&
         ((int)piVar3 < 0x5023d0)) {
        if (*piVar3 == -1) {
          piVar4 = piVar3 + -4;
        }
        else {
          iVar2 = (*piVar3 % 5 + (*piVar3 / 5) * 5) * 0x10;
          local_48 = *(int *)(&DAT_00441b18 + iVar2) + piVar3[1];
          local_40 = *(int *)(&DAT_00441b20 + iVar2) + piVar3[1];
          local_44 = *(int *)(&DAT_00441b1c + iVar2) + piVar3[2];
          piVar4 = &local_48;
          local_3c = *(int *)(&DAT_00441b24 + iVar2) + piVar3[2];
        }
        bVar1 = ParkDesignerRectanglesDoNotOverlapBySamplePoints
                          (param_1,param_2,param_3,param_4,piVar4);
        local_60 = CONCAT31(extraout_var_00,bVar1);
      }
      else {
        if (piVar3[3] != 100) goto LAB_0040e27e;
        local_38 = piVar3[-4];
        local_28[0] = 199;
        local_28[1] = 0x6d;
        local_28[2] = 0x9c;
        local_28[3] = 0x68;
        local_28[4] = 0xe0;
        local_28[5] = 0x8b;
        local_28[6] = 0xfb;
        local_28[7] = 0x8a;
        local_28[8] = 0xad;
        local_28[9] = 0x75;
        local_30 = local_28[piVar3[0xb] * 2] + local_38;
        local_34 = piVar3[-3];
        local_2c = local_28[piVar3[0xb] * 2 + 1] + local_34;
        bVar1 = ParkDesignerRectanglesDoNotOverlapBySamplePoints
                          (param_1,param_2,param_3,param_4,&local_38);
        local_60 = CONCAT31(extraout_var_01,bVar1);
      }
      if (local_60 == 0) {
        return 0;
      }
    }
LAB_0040e27e:
    piVar3 = piVar3 + 0x13;
    local_5c = local_5c + 1;
    if (0x50417f < (int)piVar3) {
      return local_60;
    }
  } while( true );
}

