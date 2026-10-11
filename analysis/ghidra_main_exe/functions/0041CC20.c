/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041cc20; function: MoveMazeActorTowardNode; body bytes: 143
 * callers: 1; callees: 3; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl MoveMazeActorTowardNode(int param_1,float *param_2,int param_3)

{
  int iVar1;
  int iVar2;
  float10 fVar3;
  float10 fVar4;
  longlong lVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar6 = param_1 + DAT_00510d18 * 0x1e;
  iVar7 = (&DAT_00510e1c)[iVar6 * 8];
  iVar6 = (&DAT_00510e18)[iVar6 * 8];
  iVar2 = iVar6;
  iVar8 = iVar7;
  lVar5 = __ftol();
  iVar1 = (int)lVar5;
  lVar5 = __ftol();
  DistanceBetweenIntegerPoints((int)lVar5,iVar1,iVar2,iVar8);
  lVar5 = __ftol();
  iVar2 = (int)lVar5;
  lVar5 = __ftol();
  lVar5 = AngleBetweenIntegerPointsDegrees((int)lVar5,iVar2,iVar6,iVar7);
  fVar3 = (float10)(int)lVar5 * (float10)_DAT_0043b384;
  fVar4 = (float10)fcos(fVar3);
  param_2[1] = (float)((float10)param_2[1] - fVar4 * (float10)param_3);
  fVar3 = (float10)fsin(fVar3);
  *param_2 = (float)(fVar3 * (float10)param_3 + (float10)*param_2);
  return;
}

