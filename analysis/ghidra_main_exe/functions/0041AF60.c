/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041af60; function: ComputeMazePlayerInputDirection; body bytes: 471
 * callers: 1; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl ComputeMazePlayerInputDirection(undefined4 *param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  float10 fVar3;
  float10 fVar4;
  float10 fVar5;
  longlong lVar6;
  longlong lVar7;
  
  iVar2 = DAT_004fbd30;
  iVar1 = DAT_004fbd24;
  if (DAT_00512124 == 1) {
    if (_DAT_004fbe5c != 0) {
      DAT_00512124 = 0;
    }
    lVar6 = __ftol();
    lVar7 = __ftol();
    fVar3 = DistanceBetweenIntegerPoints(iVar1,iVar2,(int)lVar6,(int)lVar7);
    if (fVar3 <= (float10)_DAT_0043b458) {
      return;
    }
    lVar6 = AngleBetweenIntegerPointsDegrees((int)lVar6,(int)lVar7,DAT_004fbd24,DAT_004fbd30);
    fVar3 = (float10)(int)lVar6 * (float10)_DAT_0043b384;
    fVar4 = (float10)fsin(fVar3);
    fVar3 = (float10)fcos(fVar3);
    fVar5 = FloatAbs((float)fVar4);
    if ((float10)_DAT_0043b450 < fVar5) {
      if ((float)fVar4 <= _DAT_0043b2f0) {
        *param_1 = 0xffffffff;
      }
      else {
        *param_1 = 1;
      }
    }
    fVar4 = FloatAbs((float)fVar3);
    if ((float10)_DAT_0043b450 < fVar4) {
      if (_DAT_0043b2f0 < (float)fVar3) {
        *param_2 = 1;
        DebounceMazeDirectionalInput(param_1,param_2);
        _DAT_00443d64 = 4;
        return;
      }
      *param_2 = 0xffffffff;
    }
    DebounceMazeDirectionalInput(param_1,param_2);
    _DAT_00443d64 = 4;
    return;
  }
  if ((DAT_004fbd24 != DAT_0050afc8) || (DAT_004fbd30 != DAT_0050afcc)) {
    DAT_00512124 = 1;
  }
  DAT_0050afc8 = DAT_004fbd24;
  DAT_0050afcc = DAT_004fbd30;
  if ((_DAT_004fbe5c & 2) == 0) {
    if ((_DAT_004fbe5c & 1) == 0) goto LAB_0041b0ff;
    *param_1 = 0xffffffff;
  }
  else {
    *param_1 = 1;
  }
LAB_0041b0ff:
  if ((DAT_004fbe5c & 8) == 0) {
    if ((DAT_004fbe5c & 4) != 0) {
      *param_2 = 1;
    }
    _DAT_00443d64 = 4;
    return;
  }
  *param_2 = 0xffffffff;
  _DAT_00443d64 = 4;
  return;
}

