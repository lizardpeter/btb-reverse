/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040d470; function: DrawParkDesignerToolbarAndHover; body bytes: 313
 * callers: 1; callees: 1; success: True
 */


void DrawParkDesignerToolbarAndHover(void)

{
  int *piVar1;
  bool bVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  bVar2 = false;
  iVar5 = 0;
  if (DAT_00441cdc != -1) {
    iVar3 = 0;
    do {
      if ((((*(int *)((int)&DAT_00441cd8 + iVar3) < DAT_004fbd24) &&
           (DAT_004fbd24 < *(int *)((int)&DAT_00441ce0 + iVar3))) &&
          (*(int *)((int)&DAT_00441cdc + iVar3) < DAT_004fbd30)) &&
         (DAT_004fbd30 < *(int *)((int)&DAT_00441ce4 + iVar3))) {
        bVar2 = true;
        if (((6 < iVar5) && (iVar5 < 0xc)) && ((iVar5 != 0xb || (DAT_00507b5c != 3)))) {
          if (DAT_004fbfb8 == 0) {
            uVar4 = *(undefined4 *)(iVar5 * 8 + 0x504158);
          }
          else {
            uVar4 = *(undefined4 *)(iVar5 * 8 + 0x50415c);
          }
          (**(code **)(*piVar1 + 0x1c))
                    (piVar1,*(int *)((int)&DAT_00441cd8 + iVar3),
                     *(int *)((int)&DAT_00441cdc + iVar3),uVar4,0,1);
        }
        if (DAT_00441f3c != iVar5) {
          DAT_00441f3c = iVar5;
          if (iVar5 == 7) {
            iVar3 = 0xec;
          }
          else if (iVar5 == 8) {
            iVar3 = 0xeb;
          }
          else if (iVar5 == 9) {
            iVar3 = 0xed;
          }
          else if (iVar5 == 10) {
            iVar3 = 0xee;
          }
          else {
            if ((iVar5 != 0xb) || (DAT_00507b5c == 3)) goto LAB_0040d586;
            iVar3 = 0xef;
          }
          PlayManagedSoundById(DAT_0044ddd8,iVar3,0x32,2);
        }
      }
LAB_0040d586:
      iVar5 = iVar5 + 1;
      iVar3 = iVar5 * 0x10;
    } while ((&DAT_00441cdc)[iVar5 * 4] != -1);
    if (bVar2) {
      return;
    }
  }
  DAT_00441f3c = 0xffffffff;
  return;
}

