/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040da60; function: DrawSortedParkDesignerObjects; body bytes: 231
 * callers: 1; callees: 4; success: True
 */


void DrawSortedParkDesignerObjects(void)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  
  iVar5 = 0;
  piVar4 = &DAT_00508c10;
  do {
    piVar1 = (int *)*piVar4;
    iVar2 = piVar1[7];
    if ((iVar2 != -1) && (piVar1[9] == 1)) {
      if ((iVar2 < 100) || (199 < iVar2)) {
        if (iVar2 == 7) {
          UpdateAndDrawParkDesignerFountain();
        }
        else {
          if (piVar1[0xd] < 300) {
            if (piVar1[0xd] < 200) {
              iVar7 = piVar1[1];
              iVar6 = *piVar1;
              piVar3 = (int *)(&DAT_00507e40)[iVar2];
              piVar8 = (int *)0x0;
            }
            else {
              iVar2 = piVar1[0xe] * 5 + piVar1[0xf];
              piVar8 = (int *)(&DAT_00508350 + iVar2 * 0x30);
              piVar3 = (int *)(&DAT_00508360)[iVar2 * 0xc];
              iVar7 = piVar1[1];
              iVar6 = *piVar1;
            }
          }
          else {
            iVar2 = (piVar1[0xe] * 5 + piVar1[0xf]) * 0x30;
            piVar8 = (int *)(&DAT_00508710 + iVar2);
            piVar3 = *(int **)(&DAT_00508720 + iVar2);
            iVar7 = piVar1[1];
            iVar6 = *piVar1;
          }
          BlitColorKeyedSurfaceClipped(piVar3,iVar6,iVar7,piVar8);
        }
      }
      else if (iVar2 == 100) {
        DrawParkDesignerPondSurfaceSegment(iVar5);
      }
      else {
        DrawParkDesignerSegmentedPondSurface(iVar5);
      }
    }
    piVar4 = piVar4 + 1;
    iVar5 = iVar5 + 1;
  } while ((int)piVar4 < 0x509250);
  return;
}

