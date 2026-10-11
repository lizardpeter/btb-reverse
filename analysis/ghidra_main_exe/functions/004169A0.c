/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004169a0; function: AnyRectCornerInsideRect; body bytes: 142
 * callers: 2; callees: 0; success: True
 */


undefined4 __cdecl AnyRectCornerInsideRect(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = *param_1;
  iVar2 = *param_2;
  iVar3 = param_1[1];
  if ((((iVar2 <= iVar1) && (iVar1 <= param_2[2])) && (param_2[1] <= iVar3)) &&
     (iVar3 <= param_2[3])) {
    return 1;
  }
  iVar4 = param_1[2];
  if (((iVar2 <= iVar4) && (iVar4 <= param_2[2])) &&
     ((param_2[1] <= iVar3 && (iVar3 <= param_2[3])))) {
    return 1;
  }
  iVar3 = param_1[3];
  if (((iVar2 <= iVar1) && (iVar1 <= param_2[2])) &&
     ((param_2[1] <= iVar3 && (iVar3 <= param_2[3])))) {
    return 1;
  }
  if ((((iVar2 <= iVar4) && (iVar4 <= param_2[2])) && (param_2[1] <= iVar3)) &&
     (iVar3 <= param_2[3])) {
    return 1;
  }
  return 0;
}

