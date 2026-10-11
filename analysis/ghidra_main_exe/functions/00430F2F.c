/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00430f2f; function: FUN_00430f2f; body bytes: 45
 * callers: 1; callees: 1; success: True
 */


char * __cdecl FUN_00430f2f(uint param_1,char *param_2,uint param_3)

{
  int iVar1;
  
  if ((param_3 == 10) && ((int)param_1 < 0)) {
    iVar1 = 1;
    param_3 = 10;
  }
  else {
    iVar1 = 0;
  }
  FUN_00430f5c(param_1,param_2,param_3,iVar1);
  return param_2;
}

