/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00415d30; function: DistanceBetweenIntegerPoints; body bytes: 63
 * callers: 16; callees: 0; success: True
 */


float10 __cdecl DistanceBetweenIntegerPoints(int param_1,int param_2,int param_3,int param_4)

{
  uint uVar1;
  float10 fVar2;
  float10 fVar3;
  
  uVar1 = param_1 - param_3 >> 0x1f;
  fVar2 = (float10)(int)((param_1 - param_3 ^ uVar1) - uVar1);
  uVar1 = param_2 - param_4 >> 0x1f;
  fVar3 = (float10)(int)((param_2 - param_4 ^ uVar1) - uVar1);
  return SQRT(fVar2 * fVar2 + fVar3 * fVar3);
}

