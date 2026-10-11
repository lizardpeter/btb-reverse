/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041fc60; function: FileTimeDeltaCentiseconds; body bytes: 34
 * callers: 2; callees: 0; success: True
 */


int __cdecl FileTimeDeltaCentiseconds(int *param_1,int *param_2)

{
  return (*param_2 - *param_1) / 100000;
}

