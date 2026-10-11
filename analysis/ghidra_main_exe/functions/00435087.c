/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00435087; function: FUN_00435087; body bytes: 86
 * callers: 1; callees: 2; success: True
 */


void __cdecl FUN_00435087(undefined **param_1)

{
  VirtualFree(param_1[4],0,0x8000);
  if ((undefined **)PTR_PTR_LOOP_00449768 == param_1) {
    PTR_PTR_LOOP_00449768 = param_1[1];
  }
  if (param_1 != &PTR_LOOP_00447748) {
    *(undefined **)param_1[1] = *param_1;
    *(undefined **)(*param_1 + 4) = param_1[1];
    HeapFree(DAT_0051da40,0,param_1);
    return;
  }
  DAT_00447758 = 0xffffffff;
  return;
}

