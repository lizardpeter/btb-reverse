/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00423030; function: SearchShortestSpudMazeGraphPath; body bytes: 490
 * callers: 2; callees: 3; success: True
 */


ulonglong __fastcall
SearchShortestSpudMazeGraphPath
          (undefined4 param_1,int *param_2,int *param_3,int param_4,int param_5,int param_6)

{
  bool bVar1;
  bool bVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  int *piVar8;
  int *piVar9;
  int *piVar10;
  int *piVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  longlong lVar14;
  ulonglong uVar15;
  int *local_14;
  
  uVar15 = ZEXT48(param_2) << 0x20;
  DAT_005107c8 = DAT_005107c8 + 1;
  if (param_6 < 4) {
    do {
      piVar11 = &DAT_00510954 + (int)param_3;
      local_14 = &DAT_00510958 + (int)param_3;
      bVar2 = false;
      piVar8 = param_3;
      piVar9 = param_3;
      param_3 = &DAT_00510950 + (int)param_3;
      do {
        if (3 < param_6) goto LAB_0042320f;
        piVar6 = (int *)(DAT_00510d18 * 0x1e);
        param_2 = piVar6;
        if ((&DAT_00510e28)[param_6 + (*piVar11 + (int)piVar6) * 8] != -1) {
          bVar1 = false;
          if (0x13 < (int)piVar8) goto LAB_0042320f;
          piVar7 = (int *)((int)piVar8 + 1);
          *local_14 = (&DAT_00510e28)[param_6 + (*piVar11 + (int)piVar6) * 8];
          piVar10 = piVar11 + 1;
          if ((int)DAT_00510a10 < (int)piVar7) {
            DAT_00510a10 = piVar7;
          }
          if (0 < (int)piVar7) {
            piVar3 = param_3 + 1;
            piVar4 = (int *)((int)piVar9 + 1);
            do {
              if (*piVar10 == *piVar3) {
                bVar1 = true;
              }
              piVar3 = piVar3 + -1;
              piVar4 = (int *)((int)piVar4 + -1);
            } while (piVar4 != (int *)0x0);
            param_2 = param_3;
            if (bVar1) goto LAB_0042318f;
          }
          bVar2 = true;
          DistanceBetweenIntegerPoints
                    ((&DAT_00510e18)[(*piVar10 + (int)piVar6) * 8],
                     (&DAT_00510e1c)[(*piVar10 + (int)piVar6) * 8],
                     (&DAT_00510e18)[(*piVar11 + (int)piVar6) * 8],
                     (&DAT_00510e1c)[(*piVar11 + (int)piVar6) * 8]);
          lVar14 = __ftol();
          param_2 = (int *)((ulonglong)lVar14 >> 0x20);
          param_5 = param_5 + (int)lVar14;
          piVar8 = piVar7;
          piVar9 = (int *)((int)piVar9 + 1);
          piVar11 = piVar10;
          param_3 = param_3 + 1;
          local_14 = local_14 + 1;
          if (DAT_005107cc < param_5) goto LAB_0042320f;
        }
LAB_0042318f:
        param_6 = param_6 + 1;
      } while (!bVar2);
      if ((&DAT_00510954)[(int)piVar8] == param_4) {
        if ((param_5 < DAT_005107cc) &&
           (DAT_005107cc = param_5, DAT_00510d0c = piVar8, -1 < (int)piVar8)) {
          puVar12 = &DAT_00510954;
          puVar13 = &DAT_00510988;
          for (iVar5 = (int)piVar8 + 1; iVar5 != 0; iVar5 = iVar5 + -1) {
            *puVar13 = *puVar12;
            puVar12 = puVar12 + 1;
            puVar13 = puVar13 + 1;
          }
        }
LAB_0042320f:
        return CONCAT44(param_2,0xffffffff);
      }
      SearchShortestSpudMazeGraphPath((&DAT_00510954)[(int)piVar8],param_2,piVar8,param_4,param_5,0)
      ;
      param_3 = (int *)((int)piVar8 + -1);
      uVar15 = __ftol();
      param_2 = (int *)(uVar15 >> 0x20);
      param_5 = param_5 - (int)uVar15;
    } while (param_6 < 4);
  }
  return uVar15 & 0xffffffff00000000;
}

