/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00415d70; function: AngleBetweenIntegerPointsDegrees; body bytes: 212
 * callers: 9; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong __cdecl AngleBetweenIntegerPointsDegrees(int param_1,int param_2,int param_3,int param_4)

{
  bool bVar1;
  uint uVar2;
  float10 fVar3;
  longlong lVar4;
  
  uVar2 = param_1 - param_3 >> 0x1f;
  fVar3 = (float10)(int)((param_1 - param_3 ^ uVar2) - uVar2);
  if (fVar3 == (float10)_DAT_0043b2f0) {
    fVar3 = (float10)_DAT_0043b3c8;
  }
  else {
    uVar2 = param_2 - param_4 >> 0x1f;
    fVar3 = (float10)(int)((param_2 - param_4 ^ uVar2) - uVar2) / fVar3;
  }
  fpatan(fVar3,(float10)1);
  bVar1 = _DAT_0043b2f0 <= (float)param_2 - (float)param_4;
  if (_DAT_0043b2f0 <= (float)param_1 - (float)param_3) {
    if (bVar1) {
      lVar4 = __ftol();
      return lVar4;
    }
    if (_DAT_0043b2f0 <= (float)param_1 - (float)param_3) {
      lVar4 = __ftol();
      return lVar4;
    }
  }
  else if (bVar1) {
    lVar4 = __ftol();
    return lVar4;
  }
  lVar4 = __ftol();
  return lVar4;
}

