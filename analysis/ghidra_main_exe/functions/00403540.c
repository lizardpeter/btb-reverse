/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403540; function: ClearBackBuffer; body bytes: 80
 * callers: 2; callees: 0; success: True
 */


undefined4 __thiscall ClearBackBuffer(void *this,undefined4 param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 local_64 [20];
  undefined4 local_14;
  
  piVar1 = *(int **)((int)this + 0xc);
  if (piVar1 == (int *)0x0) {
    return 0x80004003;
  }
  puVar2 = local_64;
  for (iVar4 = 0x19; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  local_64[0] = 100;
  local_14 = param_1;
  uVar3 = (**(code **)(*piVar1 + 0x14))(piVar1,0,0,0,0x400,local_64);
  return uVar3;
}

