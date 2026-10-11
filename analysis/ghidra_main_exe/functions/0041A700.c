/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041a700; function: Divide64BitDeltaByTenMillion; body bytes: 39
 * callers: 7; callees: 1; success: True
 */


undefined8 __cdecl Divide64BitDeltaByTenMillion(uint *param_1,uint *param_2)

{
  undefined8 uVar1;
  
  uVar1 = __aulldiv(*param_2 - *param_1,(param_2[1] - param_1[1]) - (uint)(*param_2 < *param_1),
                    10000000,0);
  return uVar1;
}

