/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409070; function: DecodeAndBlitBinkFrameNoAdvance; body bytes: 96
 * callers: 1; callees: 3; success: True
 */


undefined4 __cdecl DecodeAndBlitBinkFrameNoAdvance(undefined4 param_1,int *param_2,int param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  _BinkDoFrame_4(param_2);
  iVar1 = (**(code **)(*param_2 + 100))(param_2,0,param_3,1,0);
  if (iVar1 != 0) {
    return 1;
  }
  uVar2 = _BinkDDSurfaceType_4(param_2);
  _BinkCopyToBuffer_28
            (param_2,*(undefined4 *)(param_3 + 0x24),*(undefined4 *)(param_3 + 0x10),param_2[1],0,0,
             uVar2);
  (**(code **)(*param_2 + 0x80))(param_2,0);
  return 0;
}

