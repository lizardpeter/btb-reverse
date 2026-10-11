/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00431207; function: FUN_00431207; body bytes: 59
 * callers: 1; callees: 3; success: True
 */


int __cdecl FUN_00431207(int *param_1)

{
  int iVar1;
  
  if (param_1 == (int *)0x0) {
    iVar1 = flsall(0);
    return iVar1;
  }
  iVar1 = FUN_00431242(param_1);
  if (iVar1 != 0) {
    return -1;
  }
  if ((*(byte *)((int)param_1 + 0xd) & 0x40) != 0) {
    iVar1 = FUN_0043635f(param_1[4]);
    return -(uint)(iVar1 != 0);
  }
  return 0;
}

