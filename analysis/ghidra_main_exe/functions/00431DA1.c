/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00431da1; function: FUN_00431da1; body bytes: 36
 * callers: 1; callees: 2; success: True
 */


uint __cdecl FUN_00431da1(int *param_1,undefined4 *param_2)

{
  uint uVar1;
  uint uVar2;
  void *this;
  
  do {
    *param_1 = *param_1 + 1;
    uVar1 = FUN_00431d70(param_2);
    uVar2 = FUN_00436598(this,uVar1);
  } while (uVar2 != 0);
  return uVar1;
}

