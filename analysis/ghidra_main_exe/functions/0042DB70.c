/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042db70; function: DrawEnterNamePopup; body bytes: 324
 * callers: 2; callees: 0; success: True
 */


void DrawEnterNamePopup(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piStack_40;
  undefined4 uStack_3c;
  int *piStack_38;
  int iStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  int aiStack_28 [2];
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  iVar4 = 0;
  aiStack_28[1] = 0;
  aiStack_28[0] = 0;
  uStack_2c = DAT_0051c268;
  uStack_30 = 0;
  iStack_34 = 0;
  uStack_3c = 0x42db92;
  piStack_38 = piVar1;
  (**(code **)(*piVar1 + 0x1c))();
  piStack_40 = aiStack_28;
  uStack_3c = 1;
  aiStack_28[1] = 0;
  aiStack_28[0] = DAT_0051c248 * 0x32;
  (**(code **)(*piVar1 + 0x1c))(piVar1,0x128,0x67,DAT_0051b398);
  iVar2 = 0;
  iVar3 = 0;
  if (0 < DAT_0051be48) {
    do {
      iVar2 = iVar2 + (*(int *)(&DAT_00516968 + (uint)(byte)(&DAT_00519948)[iVar3] * 0x10) -
                      *(int *)(&DAT_00516960 + (uint)(byte)(&DAT_00519948)[iVar3] * 0x10));
      iVar3 = iVar3 + 1;
    } while (iVar3 < DAT_0051be48);
  }
  iVar2 = 0x142 - iVar2 / 2;
  if (0 < DAT_0051be48) {
    do {
      iVar3 = (uint)(byte)(&DAT_00519948)[iVar4] * 0x10;
      uStack_3c = *(undefined4 *)(&DAT_00516964 + iVar3);
      piStack_40 = *(int **)(&DAT_00516960 + iVar3);
      piStack_38 = *(int **)(&DAT_00516968 + iVar3);
      iStack_34 = *(int *)(&DAT_0051696c + iVar3) + 1;
      (**(code **)(*piVar1 + 0x1c))(piVar1,iVar2,200,DAT_0051b39c,&piStack_40,1);
      iVar2 = (int)piStack_38 + (iVar2 - (int)piStack_40);
      iVar4 = iVar4 + 1;
    } while (iVar4 < DAT_0051be48);
  }
  DAT_0051c398 = DAT_0051c398 + 1;
  if (DAT_0051c398 < 0x21) {
    if (0x10 < DAT_0051c398) {
      return;
    }
  }
  else {
    DAT_0051c398 = 0;
  }
  (**(code **)(*piVar1 + 0x1c))(piVar1,iVar2 + -3,0xc5,DAT_0051be44,0,1);
  return;
}

