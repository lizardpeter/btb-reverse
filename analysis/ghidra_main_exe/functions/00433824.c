/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00433824; function: FUN_00433824; body bytes: 70
 * callers: 0; callees: 2; success: True
 */


int FUN_00433824(int *param_1)

{
  int *piVar1;
  bool bVar2;
  int iVar3;
  undefined3 extraout_var;
  
  piVar1 = (int *)*param_1;
  if (((*piVar1 == -0x1f928c9d) && (piVar1[4] == 3)) && (piVar1[5] == 0x19930520)) {
    iVar3 = FUN_0043260c();
    return iVar3;
  }
  if ((DAT_0051c440 != (FARPROC)0x0) &&
     (bVar2 = FUN_00436745(DAT_0051c440), CONCAT31(extraout_var,bVar2) != 0)) {
    iVar3 = (*DAT_0051c440)(param_1);
    return iVar3;
  }
  return 0;
}

