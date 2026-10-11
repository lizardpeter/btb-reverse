/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040dd50; function: ParkDesignerRectanglesDoNotOverlapBySamplePoints; body bytes: 665
 * callers: 1; callees: 1; success: True
 */


bool __cdecl
ParkDesignerRectanglesDoNotOverlapBySamplePoints
          (int param_1,int param_2,int param_3,int param_4,int *param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  int iVar20;
  int iVar21;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar2 = PointInsideRectExclusive(param_1,param_2,param_5);
  iVar18 = param_1 + param_3;
  iVar3 = PointInsideRectExclusive(iVar18,param_2,param_5);
  iVar16 = param_2 + param_4;
  iVar4 = PointInsideRectExclusive(iVar18,iVar16,param_5);
  iVar5 = PointInsideRectExclusive(param_1,iVar16,param_5);
  iVar6 = PointInsideRectExclusive(param_3 / 2 + param_1,param_2 + param_4 / 2,param_5);
  iVar17 = param_3 / 2 + param_1;
  iVar7 = PointInsideRectExclusive(iVar17,param_2,param_5);
  iVar19 = param_4 / 2 + param_2;
  iVar8 = PointInsideRectExclusive(iVar18,iVar19,param_5);
  iVar9 = PointInsideRectExclusive(iVar17,iVar16,param_5);
  iVar10 = PointInsideRectExclusive(param_1,iVar19,param_5);
  local_c = param_2;
  iVar19 = param_5[1];
  local_10 = param_1;
  iVar14 = param_5[2];
  iVar1 = *param_5;
  iVar15 = param_5[3];
  local_8 = iVar18;
  local_4 = iVar16;
  iVar18 = PointInsideRectExclusive(iVar1,iVar19,&local_10);
  iVar16 = iVar1 + (iVar14 - iVar1);
  iVar11 = PointInsideRectExclusive(iVar16,iVar19,&local_10);
  iVar17 = iVar19 + (iVar15 - iVar19);
  iVar12 = PointInsideRectExclusive(iVar16,iVar17,&local_10);
  iVar13 = PointInsideRectExclusive(iVar1,iVar17,&local_10);
  iVar21 = (iVar15 - iVar19) / 2;
  iVar20 = (iVar14 - iVar1) / 2;
  iVar14 = PointInsideRectExclusive(iVar1 + iVar20,iVar21 + iVar19,&local_10);
  iVar20 = iVar20 + iVar1;
  iVar15 = PointInsideRectExclusive(iVar20,iVar19,&local_10);
  iVar19 = iVar19 + iVar21;
  iVar16 = PointInsideRectExclusive(iVar16,iVar19,&local_10);
  iVar17 = PointInsideRectExclusive(iVar20,iVar17,&local_10);
  iVar19 = PointInsideRectExclusive(iVar1,iVar19,&local_10);
  if (iVar19 != 0) {
    return false;
  }
  return iVar17 == 0 &&
         (iVar16 == 0 &&
         (iVar15 == 0 &&
         (iVar14 == 0 &&
         (iVar13 == 0 &&
         (iVar12 == 0 &&
         (iVar11 == 0 &&
         (iVar18 == 0 &&
         (iVar10 == 0 &&
         (iVar9 == 0 &&
         (iVar8 == 0 &&
         (iVar7 == 0 && (iVar6 == 0 && (iVar5 == 0 && (iVar4 == 0 && (iVar3 == 0 && iVar2 == 0))))))
         )))))))));
}

