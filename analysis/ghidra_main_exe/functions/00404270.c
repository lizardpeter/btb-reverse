/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404270; function: CSound_RestoreBuffer; body bytes: 118
 * callers: 2; callees: 1; success: True
 */


int CSound_RestoreBuffer(int *param_1,undefined4 *param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  uint unaff_EBX;
  
  puVar2 = param_2;
  piVar1 = param_1;
  if (param_1 == (int *)0x0) {
    return -0x7ffbfe10;
  }
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = 0;
  }
  iVar3 = (**(code **)(*param_1 + 0x24))(param_1,&param_1);
  if (-1 < iVar3) {
    if ((unaff_EBX & 2) != 0) {
      do {
        iVar3 = (**(code **)(*piVar1 + 0x50))(piVar1);
        if (iVar3 == -0x7787ff6a) {
          Sleep(10);
        }
        iVar3 = (**(code **)(*piVar1 + 0x50))(piVar1);
      } while (iVar3 != 0);
      if (puVar2 != (undefined4 *)0x0) {
        *puVar2 = 1;
      }
      return 0;
    }
    iVar3 = 1;
  }
  return iVar3;
}

