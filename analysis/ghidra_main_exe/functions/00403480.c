/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403480; function: PresentDisplay; body bytes: 128
 * callers: 1; callees: 0; success: True
 */


int __fastcall PresentDisplay(int param_1)

{
  int *piVar1;
  int iVar2;
  
  if ((*(int *)(param_1 + 8) == 0) && (*(int *)(param_1 + 0xc) == 0)) {
    return -0x7fffbffd;
  }
  while( true ) {
    piVar1 = *(int **)(param_1 + 8);
    if (*(int *)(param_1 + 0x28) == 0) {
      iVar2 = (**(code **)(*piVar1 + 0x2c))(piVar1,0,0);
    }
    else {
      iVar2 = (**(code **)(*piVar1 + 0x14))
                        (piVar1,param_1 + 0x18,*(undefined4 *)(param_1 + 0xc),0,0x1000000);
    }
    if (iVar2 == -0x7789fe3e) break;
    if (iVar2 != -0x7789fde4) {
      return iVar2;
    }
  }
  (**(code **)(**(int **)(param_1 + 8) + 0x6c))(*(int **)(param_1 + 8));
  (**(code **)(**(int **)(param_1 + 0xc) + 0x6c))(*(int **)(param_1 + 0xc));
  (**(code **)(**(int **)(param_1 + 4) + 100))(*(int **)(param_1 + 4));
  DAT_0051c320 = 1;
  return -0x7789fe3e;
}

