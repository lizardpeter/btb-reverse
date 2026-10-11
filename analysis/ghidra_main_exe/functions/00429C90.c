/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00429c90; function: UpdateYesNoConfirmationOverlay; body bytes: 834
 * callers: 1; callees: 8; success: True
 */


void UpdateYesNoConfirmationOverlay(void)

{
  int *piVar1;
  int iVar2;
  undefined4 unaff_EBX;
  undefined4 unaff_EBP;
  int iStack_38;
  undefined4 uVar3;
  
  if (DAT_0051c2d0 < 2) {
    piVar1 = *(int **)(DAT_0044de08 + 0xc);
    if (DAT_0051c310 == 0) {
      iStack_38 = 0x429d3d;
      DrawEnterNamePopup();
    }
    else if (DAT_0051c310 == 1) {
      DrawParkDesignerActivity();
    }
    else if (DAT_0051c310 == 2) {
      DrawGrandOpeningCompositionAndMachines();
    }
    else if (DAT_0051c310 == 3) {
      DrawFireworksEditor();
    }
    uVar3 = 1;
    iStack_38 = 0;
    (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_0051c294);
    if (DAT_004fbfb8 == 1) {
      iVar2 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,&iStack_38);
      if ((iVar2 == 0) || (DAT_00446fec != 0)) {
        iVar2 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&stack0xffffffd8);
        if ((iVar2 != 0) && ((DAT_00446fec == 1 && (DAT_00446cec = 2, DAT_004fbfb8 != 0)))) {
          (**(code **)(*piVar1 + 0x1c))(piVar1,unaff_EBP,unaff_EBX,DAT_0051c2ac,0,1);
        }
      }
      else {
        DAT_00446cec = 1;
        if (DAT_004fbfb8 != 0) {
          (**(code **)(*piVar1 + 0x1c))(piVar1,iStack_38,uVar3,DAT_0051c2a8,0,1);
        }
      }
    }
    if (DAT_004fbfb4 != 0) {
      iVar2 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,&iStack_38);
      if (iVar2 == 0) {
        iVar2 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&stack0xffffffd8);
        if (((iVar2 != 0) && (DAT_00446cec == 2)) && (DAT_00446fec == 1)) {
          DAT_0051c2dc = 0;
          DAT_00446fec = -1;
          DAT_0051c2d0 = 0;
          ResetMouseBoundsToGameViewport();
          DAT_00446cec = -1;
        }
      }
      else if ((DAT_00446cec == 1) && (DAT_00446fec == 0)) {
        DAT_0051c2dc = 1;
        DAT_0051c2d0 = 0;
        ResetMouseBoundsToGameViewport();
        DAT_00446cec = -1;
        DAT_00446fec = -1;
      }
    }
    if (DAT_004fbfb8 == 0) {
      iVar2 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,&iStack_38);
      if (iVar2 != 0) {
        DAT_00446cec = 1;
        if (DAT_004fbfb8 != 0) {
          (**(code **)(*piVar1 + 0x1c))(piVar1,iStack_38,uVar3,DAT_0051c2a8,0,1);
          return;
        }
        DAT_00446fec = 0;
        (**(code **)(*piVar1 + 0x1c))(piVar1,iStack_38,uVar3,DAT_0051c2a0,0,1);
        return;
      }
      iVar2 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&stack0xffffffd8);
      if (iVar2 != 0) {
        DAT_00446cec = 2;
        if (DAT_004fbfb8 != 0) {
          (**(code **)(*piVar1 + 0x1c))(piVar1,unaff_EBP,unaff_EBX,DAT_0051c2ac,0,1);
          return;
        }
        (**(code **)(*piVar1 + 0x1c))(piVar1,unaff_EBP,unaff_EBX,DAT_0051c2a4,0,1);
        DAT_00446fec = 1;
        return;
      }
      DAT_00446fec = -1;
    }
  }
  else {
    DAT_0051c2d0 = 0;
    ResetMouseBoundsToGameViewport();
    ResumeGlobalBinkMovie();
    iStack_38 = 0x429cc1;
    UnregisterBitmapSurface(0x51c294);
    if (DAT_0051c294 != (int *)0x0) {
      iStack_38 = 0x429cd7;
      (**(code **)(*DAT_0051c294 + 8))();
      DAT_0051c294 = (int *)0x0;
      return;
    }
  }
  return;
}

