/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403c40; function: CSoundManager_SetPrimaryBufferFormat; body bytes: 214
 * callers: 1; callees: 0; success: True
 */


/* WARNING: Restarted to delay deadcode elimination for space: stack */

int __fastcall CSoundManager_SetPrimaryBufferFormat(int *param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 local_3c;
  undefined2 uStack_38;
  int local_24 [4];
  undefined4 local_14;
  short sStack_c;
  undefined4 uStack_8;
  ushort uStack_4;
  
  uStack_48 = &local_3c;
  piVar1 = (int *)*param_1;
  local_3c = 0;
  if (piVar1 == (int *)0x0) {
    return -0x7ffbfe10;
  }
  piVar3 = local_24;
  for (iVar2 = 9; iVar2 != 0; iVar2 = iVar2 + -1) {
    *piVar3 = 0;
    piVar3 = piVar3 + 1;
  }
  uStack_44 = 0;
  piVar3 = local_24;
  local_24[0] = 0x24;
  local_24[1] = 0x81;
  local_24[2] = 0;
  local_14 = 0;
  iVar2 = (**(code **)(*piVar1 + 0xc))(piVar1);
  if (-1 < iVar2) {
    uStack_48 = (undefined4 *)CONCAT22(sStack_c,1);
    local_3c = CONCAT22(uStack_4,(uStack_4 >> 3) * sStack_c);
    uStack_38 = 0;
    uStack_44 = uStack_8;
    iVar2 = (**(code **)(local_24[0] + 0x38))(piVar3,&uStack_48);
    if (-1 < iVar2) {
      if (piVar3 != (int *)0x0) {
        (**(code **)(*piVar3 + 8))(piVar3);
      }
      iVar2 = 0;
    }
  }
  return iVar2;
}

