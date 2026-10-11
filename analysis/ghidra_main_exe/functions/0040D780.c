/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d780; function: CompareParkDesignerObjectsBySortLayer; body bytes: 78
 * callers: 0; callees: 0; success: True
 */


int CompareParkDesignerObjectsBySortLayer(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = -1;
  iVar1 = *(int *)(*param_1 + 0x1c);
  if ((iVar1 == -1) && (*(int *)(*param_2 + 0x1c) == -1)) {
    return 0;
  }
  if (iVar1 != -1) {
    if (*(int *)(*param_2 + 0x1c) == -1) {
      return 1;
    }
    iVar2 = ((*(int *)(*param_1 + 0x30) <= *(int *)(*param_2 + 0x30)) - 1 & 2) - 1;
  }
  return iVar2;
}

