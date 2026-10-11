/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041aeb0; function: DebounceMazeDirectionalInput; body bytes: 176
 * callers: 1; callees: 0; success: True
 */


void __cdecl DebounceMazeDirectionalInput(undefined4 *param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int local_10 [4];
  
  iVar3 = 0;
  do {
    iVar4 = 2;
    puVar1 = &DAT_00512130 + iVar3 * 3;
    do {
      *puVar1 = puVar1[1];
      puVar1 = puVar1 + 1;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    if (iVar3 == 0) {
      DAT_00512138 = *param_1;
    }
    else {
      (&DAT_00512138)[iVar3 * 3] = *param_2;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 2);
  local_10[2] = DAT_00512130;
  local_10[0] = 1;
  local_10[1] = 1;
  local_10[3] = DAT_0051213c;
  piVar2 = &DAT_00512130;
  iVar3 = 0;
  do {
    iVar4 = 3;
    do {
      if (*(int *)((int)local_10 + iVar3 + 8) != *piVar2) {
        *(undefined4 *)((int)local_10 + iVar3) = 0;
      }
      piVar2 = piVar2 + 1;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    iVar3 = iVar3 + 4;
  } while ((int)piVar2 < 0x512148);
  if ((local_10[0] == 0) || (local_10[1] == 0)) {
    *param_1 = 0;
    *param_2 = 0;
  }
  return;
}

