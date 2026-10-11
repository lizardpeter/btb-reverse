/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040dd20; function: PointInsideRectExclusive; body bytes: 42
 * callers: 1; callees: 0; success: True
 */


undefined4 __cdecl PointInsideRectExclusive(int param_1,int param_2,int *param_3)

{
  if ((((*param_3 < param_1) && (param_1 < param_3[2])) && (param_3[1] < param_2)) &&
     (param_2 < param_3[3])) {
    return 1;
  }
  return 0;
}

