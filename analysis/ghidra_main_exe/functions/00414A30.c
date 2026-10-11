/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00414a30; function: VectorFromDegreesAndMagnitude; body bytes: 43
 * callers: 2; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl VectorFromDegreesAndMagnitude(float *param_1,float *param_2,int param_3,int param_4)

{
  float10 fVar1;
  float10 fVar2;
  
  fVar2 = (float10)_DAT_0043b388;
  fVar1 = (float10)fcos((float10)param_3 * fVar2);
  *param_1 = (float)(fVar1 * (float10)param_4);
  fVar2 = (float10)fsin((float10)param_3 * fVar2);
  *param_2 = (float)(fVar2 * (float10)param_4);
  return;
}

