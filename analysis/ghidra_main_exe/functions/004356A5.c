/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004356a5; function: FUN_004356a5; body bytes: 141
 * callers: 1; callees: 2; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_004356a5(undefined4 *param_1)

{
  undefined4 uVar1;
  byte bVar2;
  undefined3 extraout_var;
  int iVar3;
  void *pvVar4;
  
  bVar2 = FUN_004373b5(param_1[4]);
  if (CONCAT31(extraout_var,bVar2) == 0) {
    return 0;
  }
  if (param_1 == (undefined4 *)&DAT_004498b0) {
    iVar3 = 0;
  }
  else {
    if (param_1 != (undefined4 *)&DAT_004498d0) {
      return 0;
    }
    iVar3 = 1;
  }
  _DAT_0051c568 = _DAT_0051c568 + 1;
  if ((*(ushort *)(param_1 + 3) & 0x10c) != 0) {
    return 0;
  }
  if ((&DAT_0051c450)[iVar3] == 0) {
    pvVar4 = _malloc(0x1000);
    (&DAT_0051c450)[iVar3] = pvVar4;
    if (pvVar4 == (void *)0x0) {
      param_1[2] = param_1 + 5;
      *param_1 = param_1 + 5;
      param_1[6] = 2;
      param_1[1] = 2;
      goto LAB_00435721;
    }
  }
  uVar1 = (&DAT_0051c450)[iVar3];
  param_1[6] = 0x1000;
  param_1[2] = uVar1;
  *param_1 = uVar1;
  param_1[1] = 0x1000;
LAB_00435721:
  *(ushort *)(param_1 + 3) = *(ushort *)(param_1 + 3) | 0x1102;
  return 1;
}

