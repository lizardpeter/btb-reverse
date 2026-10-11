/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403be0; function: CSoundManager_Initialize; body bytes: 85
 * callers: 1; callees: 2; success: True
 */


int __fastcall CSoundManager_Initialize(int *param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 unaff_ESI;
  undefined4 uVar3;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *param_1 = 0;
  }
  uVar3 = 0;
  iVar2 = Ordinal_11(0,param_1,0);
  if (-1 < iVar2) {
    iVar2 = (**(code **)(*(int *)*param_1 + 0x18))((int *)*param_1,uVar3,unaff_ESI);
    if (-1 < iVar2) {
      CSoundManager_SetPrimaryBufferFormat(param_1);
      iVar2 = 0;
    }
  }
  return iVar2;
}

