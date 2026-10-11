/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403500; function: BltFastToBackBuffer; body bytes: 49
 * callers: 1; callees: 0; success: True
 */


undefined4 __thiscall
BltFastToBackBuffer(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3,
                   undefined4 param_4,undefined4 param_5)

{
  int *piVar1;
  undefined4 uVar2;
  
  piVar1 = *(int **)((int)this + 0xc);
  if (piVar1 == (int *)0x0) {
    return 0x80004003;
  }
  uVar2 = (**(code **)(*piVar1 + 0x1c))(piVar1,param_1,param_2,param_3,param_4,param_5);
  return uVar2;
}

