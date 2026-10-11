/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00413b60; function: UpdateFireworksEditorControls; body bytes: 942
 * callers: 1; callees: 6; success: True
 */


void UpdateFireworksEditorControls(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int local_4;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  if (0 < DAT_0050ab84) {
    DAT_0050ab84 = DAT_0050ab84 + -1;
    return;
  }
  local_4 = 0;
  if (0 < DAT_00509378) {
    piVar6 = &DAT_0050a6c4;
    iVar7 = DAT_00442a30;
    do {
      iVar2 = piVar6[3];
      if (iVar2 != -1) {
        if ((0 < DAT_0050ab20) || (iVar4 = DAT_004fbd24, iVar5 = DAT_004fbd30, 0 < DAT_0050ab24)) {
          iVar4 = DAT_004fbd24 + 0x28;
          iVar5 = DAT_004fbd30 + 0x14;
        }
        iVar3 = piVar6[-1];
        if ((((iVar3 < iVar4) && (iVar4 < piVar6[1])) && (iVar4 = *piVar6, iVar4 < iVar5)) &&
           (iVar5 < piVar6[2])) {
          if (iVar2 == 0x1b) {
            if ((DAT_0050ab20 == 0) && (DAT_0050ab24 == 0)) {
              if ((DAT_004fbfb4 == 0) || (DAT_0050a5bc != 0)) {
                if (DAT_004fbfb8 == 0) {
                  if (iVar7 != 0x1b) {
                    PlayManagedSoundById(DAT_0044ddd8,0x371,0x32,2);
                  }
                  (**(code **)(*piVar1 + 0x1c))(piVar1,piVar6[-1],*piVar6,DAT_0050a4ac,0,1);
                  iVar7 = 0x1b;
                  DAT_00442a30 = iVar7;
                }
                else {
                  (**(code **)(*piVar1 + 0x1c))(piVar1,iVar3,iVar4,DAT_0050a4b0,0,1);
                  iVar7 = 0x1b;
                  DAT_00442a30 = iVar7;
                }
              }
              else {
                DAT_0050ab74 = 0xffffffff;
                PlayManagedSoundById(DAT_0044ddd8,0x370,0x32,2);
                DAT_0050a5bc = 0xe;
                iVar7 = 0x1b;
                DAT_00442a30 = iVar7;
              }
            }
          }
          else if (iVar2 == 0x1a) {
            if ((DAT_004fbe54 != 0) && ((DAT_0050ab20 == 1 || (DAT_0050ab24 == 1)))) {
              DAT_0050ab20 = 0;
              DAT_0050ab24 = 0;
              DAT_0050a5bc = 0;
              SetCursorSurface((int *)0x0);
              DAT_0050ab84 = 10;
              return;
            }
            if ((DAT_004fbfb4 == 0) || (DAT_0050ab84 != 0)) {
              if (DAT_004fbfb8 == 0) {
                if (iVar7 != 0x1a) {
                  PlayManagedSoundById(DAT_0044ddd8,0x372,0x32,2);
                }
                (**(code **)(*piVar1 + 0x1c))(piVar1,piVar6[-1],*piVar6,DAT_0050a4a4,0,1);
                iVar7 = 0x1a;
                DAT_00442a30 = iVar7;
              }
              else {
                (**(code **)(*piVar1 + 0x1c))(piVar1,iVar3,iVar4,DAT_0050a4a8,0,1);
                iVar7 = 0x1a;
                DAT_00442a30 = iVar7;
              }
            }
            else {
              DAT_0050ab74 = 0xffffffff;
              if (DAT_0050a5bc == 0xd) {
                DAT_0050a5bc = 0;
                SetCursorSurface((int *)0x0);
                iVar7 = 0x1a;
                DAT_00442a30 = iVar7;
              }
              else {
                PlayManagedSoundById(DAT_0044ddd8,0x37a,0x32,2);
                DAT_0050a5bc = 0xd;
                SetCursorSurface(DAT_00508c0c);
                iVar7 = 0x1a;
                DAT_00442a30 = iVar7;
              }
            }
          }
          else if (iVar2 == 0x19) {
            if ((DAT_004fbfb4 == 0) || (DAT_0050a5bc != 0)) {
              if (DAT_004fbfb8 == 0) {
                if (iVar7 != 0x19) {
                  PlayManagedSoundById(DAT_0044ddd8,0x373,0x32,2);
                }
                (**(code **)(*piVar1 + 0x1c))(piVar1,piVar6[-1],*piVar6,DAT_0050a49c,0,1);
              }
              else {
                (**(code **)(*piVar1 + 0x1c))(piVar1,iVar3,iVar4,DAT_0050a4a0,0,1);
              }
            }
            else {
              DAT_0050ab74 = 0xffffffff;
              PlayManagedSoundById(DAT_0044ddd8,0x375,0x32,2);
              DAT_0051c2d0 = 1;
              DAT_0051c310 = 3;
              DAT_0051c294 = LoadBitmapToDirectDrawSurface
                                       (*(int **)(DAT_0044de08 + 4),
                                        s_data_ui_DeleteFireworks_bmp_004435d8,0,0);
              RegisterBitmapSurface(&DAT_0051c294,s_data_ui_DeleteFireworks_bmp_004435d8);
              MarkRegisteredSurfaceColorKeyed(0x51c294);
              SetSurfaceTransparencyColorKey(DAT_0051c294,0xff00ff);
            }
            iVar7 = 0x19;
            DAT_00442a30 = iVar7;
          }
        }
      }
      local_4 = local_4 + 1;
      piVar6 = piVar6 + 5;
      if (DAT_00509378 <= local_4) {
        return;
      }
    } while( true );
  }
  return;
}

