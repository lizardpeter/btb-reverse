/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00430cb0; function: FUN_00430cb0; body bytes: 78
 * callers: 1; callees: 1; success: True
 */


void __cdecl FUN_00430cb0(undefined1 *param_1,undefined1 *param_2,int param_3,undefined *param_4)

{
  int iVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  if (param_1 < param_2) {
    puVar2 = param_1;
    puVar3 = param_1 + param_3;
    do {
      for (; puVar3 <= param_2; puVar3 = puVar3 + param_3) {
        iVar1 = (*(code *)param_4)(puVar3,puVar2);
        if (0 < iVar1) {
          puVar2 = puVar3;
        }
      }
      FUN_00430cfe(puVar2,param_2,param_3);
      param_2 = param_2 + -param_3;
      puVar2 = param_1;
      puVar3 = param_1 + param_3;
    } while (param_1 < param_2);
  }
  return;
}

