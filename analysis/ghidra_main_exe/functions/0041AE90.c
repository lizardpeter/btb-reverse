/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041ae90; function: FloatAbs; body bytes: 29
 * callers: 2; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float10 __cdecl FloatAbs(float param_1)

{
  if (param_1 < _DAT_0043b2f0) {
    return -(float10)param_1;
  }
  return (float10)param_1;
}

