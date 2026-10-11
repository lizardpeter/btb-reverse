/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004232b0; function: MovePilchardTowardGraphNode; body bytes: 442
 * callers: 1; callees: 3; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl MovePilchardTowardGraphNode(int param_1,float *param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  float10 fVar4;
  float10 fVar5;
  longlong lVar6;
  int iVar7;
  int iVar8;
  
  if (DAT_00445ef8 < 0) {
    if (DAT_00445efc < 0) {
      if (((DAT_005144d4 != 0) && (10 < (int)param_2[7])) && ((int)param_2[7] < 0xe)) {
        iVar1 = param_1 + DAT_00510d18 * 0x1e;
        iVar8 = (&DAT_00510e1c)[iVar1 * 8];
        iVar1 = (&DAT_00510e18)[iVar1 * 8];
        iVar7 = iVar1;
        iVar3 = iVar8;
        lVar6 = __ftol();
        iVar2 = (int)lVar6;
        lVar6 = __ftol();
        DistanceBetweenIntegerPoints((int)lVar6,iVar2,iVar7,iVar3);
        iVar7 = iVar1;
        lVar6 = __ftol();
        iVar3 = (int)lVar6;
        lVar6 = __ftol();
        lVar6 = AngleBetweenIntegerPointsDegrees((int)lVar6,iVar3,iVar7,iVar8);
        fVar4 = (float10)(int)lVar6 * (float10)_DAT_0043b384;
        fVar5 = (float10)fcos(fVar4);
        param_2[1] = (float)((float10)param_2[1] - fVar5 * (float10)param_3 * (float10)_DAT_0043b378
                            );
        fVar4 = (float10)fsin(fVar4);
        fVar4 = fVar4 * (float10)param_3 * (float10)_DAT_0043b378 + (float10)*param_2;
        *param_2 = (float)fVar4;
        if ((param_2[5] == 1.4013e-45) && ((float)iVar1 < (float)fVar4)) {
          *param_2 = (float)iVar1;
        }
        if ((param_2[5] == 4.2039e-45) && (*param_2 < (float)iVar1)) {
          *param_2 = (float)iVar1;
          return;
        }
      }
    }
    else if ((2 < DAT_00445efc) && (param_2[5] == 0.0)) {
      if ((DAT_00445efc == 3) || (DAT_00445efc == 4)) {
        param_2[1] = param_2[1] - _DAT_0043b4e0;
        return;
      }
      if (DAT_00445efc == 5) {
        param_2[1] = (float)(int)(&DAT_00510e1c)[(param_1 + DAT_00510d18 * 0x1e) * 8];
        return;
      }
    }
  }
  else if (2 < DAT_00445ef8) {
    if ((DAT_00445ef8 == 3) || (DAT_00445efc == 4)) {
      param_2[1] = param_2[1] + _DAT_0043b4e0;
      return;
    }
    if (DAT_00445ef8 == 5) {
      param_2[1] = (float)(int)(&DAT_00510e1c)[(param_1 + DAT_00510d18 * 0x1e) * 8];
      return;
    }
  }
  return;
}

