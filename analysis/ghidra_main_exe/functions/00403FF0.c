/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403ff0; function: CSound_Constructor; body bytes: 123
 * callers: 1; callees: 2; success: True
 */


undefined4 * __thiscall
CSound_Constructor(void *this,int param_1,undefined4 param_2,uint param_3,undefined4 param_4)

{
  int *piVar1;
  void *pvVar2;
  uint uVar3;
  
  *(undefined ***)this = &PTR_CSound_ScalarDeletingDestructor_0043b2e4;
  pvVar2 = operator_new(param_3 * 4);
  *(void **)((int)this + 4) = pvVar2;
  uVar3 = 0;
  if (param_3 != 0) {
    do {
      *(undefined4 *)(*(int *)((int)this + 4) + uVar3 * 4) = *(undefined4 *)(param_1 + uVar3 * 4);
      uVar3 = uVar3 + 1;
    } while (uVar3 < param_3);
  }
  *(undefined4 *)((int)this + 8) = param_2;
  *(uint *)((int)this + 0x10) = param_3;
  *(undefined4 *)((int)this + 0xc) = param_4;
  CSound_FillBufferWithSound(this,(int *)**(undefined4 **)((int)this + 4));
  uVar3 = 0;
  if (param_3 != 0) {
    do {
      piVar1 = *(int **)(*(int *)((int)this + 4) + uVar3 * 4);
      (**(code **)(*piVar1 + 0x34))(piVar1,0);
      uVar3 = uVar3 + 1;
    } while (uVar3 < param_3);
  }
  return (undefined4 *)this;
}

