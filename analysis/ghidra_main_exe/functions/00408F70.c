/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00408f70; function: DecodeAndBlitBinkFrame; body bytes: 251
 * callers: 3; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl DecodeAndBlitBinkFrame(undefined4 param_1,int *param_2,int param_3)

{
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  int *piVar4;
  
  _BinkDoFrame_4(param_2);
  piVar4 = param_2;
  iVar1 = (**(code **)(*param_2 + 100))(param_2,0,param_3,1,0);
  if (iVar1 != 0) {
    return 1;
  }
  piVar3 = param_2;
  uVar2 = _BinkDDSurfaceType_4(param_2);
  _BinkCopyToBuffer_28
            (param_2,*(undefined4 *)(param_3 + 0x24),*(undefined4 *)(param_3 + 0x10),param_2[1],0,0,
             uVar2);
  (**(code **)(*param_2 + 0x80))(param_2,0);
  if (((piVar4 != (int *)0x0) ||
      ((((param_2[3] != param_2[2] && (DAT_004fbe54 == 0)) && (DAT_004fbd50 == 0)) &&
       (_DAT_004fbe5c == 0)))) && ((param_2[3] != param_2[2] || (piVar4 != (int *)0x2)))) {
    if ((DAT_00442a2c == 1) || (piVar4 == (int *)0x1)) {
      _BinkNextFrame_4(param_2);
    }
    if (piVar4 != (int *)0x2) {
      iVar1 = _BinkWait_4(param_2);
      while (iVar1 != 0) {
        iVar1 = _BinkWait_4(param_2);
      }
      DAT_00442a2c = 1;
    }
    (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))
              (*(int **)(DAT_0044de08 + 0xc),uVar2,piVar3,param_2,0,0);
    return 0;
  }
  return 1;
}

