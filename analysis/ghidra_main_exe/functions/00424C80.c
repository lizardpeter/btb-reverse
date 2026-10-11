/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00424c80; function: SeekAndDecodeBinkToExactFrame; body bytes: 133
 * callers: 1; callees: 6; success: True
 */


void __cdecl SeekAndDecodeBinkToExactFrame(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int unaff_EBP;
  int *unaff_ESI;
  
  piVar2 = (int *)_BinkGetKeyFrame_12(param_1,param_2,0);
  _BinkGoto_12(param_1,piVar2,0);
  (**(code **)(*unaff_ESI + 100))(unaff_ESI,0,unaff_EBP,1,0);
  uVar3 = _BinkDDSurfaceType_4(unaff_ESI);
  iVar1 = *(int *)(param_1 + 0xc);
  while (iVar1 != param_2) {
    _BinkDoFrame_4(param_1);
    _BinkCopyToBuffer_28
              (param_1,*(undefined4 *)(unaff_EBP + 0x24),*(undefined4 *)(unaff_EBP + 0x10),
               *(undefined4 *)(param_1 + 4),0,0,uVar3);
    _BinkNextFrame_4(param_1);
    iVar1 = *(int *)(param_1 + 0xc);
  }
  (**(code **)(*piVar2 + 0x80))(piVar2,0);
  return;
}

