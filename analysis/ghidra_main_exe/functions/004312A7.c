/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004312a7; function: flsall; body bytes: 109
 * callers: 1; callees: 1; success: True
 */


/* Library Function - Single Match
    _flsall
   
   Library: Visual Studio 2003 Release */

int __cdecl flsall(int param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = 0;
  iVar3 = 0;
  iVar5 = 0;
  if (0 < DAT_0051d900) {
    do {
      piVar1 = *(int **)(DAT_0051c8e8 + iVar4 * 4);
      if ((piVar1 != (int *)0x0) && ((piVar1[3] & 0x83U) != 0)) {
        if (param_1 == 1) {
          iVar2 = FUN_00431207(piVar1);
          if (iVar2 != -1) {
            iVar3 = iVar3 + 1;
          }
        }
        else if ((param_1 == 0) && ((piVar1[3] & 2U) != 0)) {
          iVar2 = FUN_00431207(piVar1);
          if (iVar2 == -1) {
            iVar5 = -1;
          }
        }
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < DAT_0051d900);
  }
  if (param_1 != 1) {
    iVar3 = iVar5;
  }
  return iVar3;
}

