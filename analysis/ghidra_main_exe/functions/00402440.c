/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402440; function: RunActiveGameFrame; body bytes: 443
 * callers: 1; callees: 16; success: True
 */


uint __cdecl RunActiveGameFrame(HWND param_1)

{
  byte bVar1;
  UINT UVar2;
  byte *pbVar3;
  int iVar4;
  DWORD DVar5;
  uint uVar6;
  byte *pbVar7;
  bool bVar8;
  DWORD DStack_74;
  DWORD DStack_70;
  char local_6c;
  undefined3 uStack_6b;
  undefined1 local_68;
  byte abStack_64 [100];
  
  DAT_0044de1c = DAT_0044de1c + 1;
  if (100 < DAT_0044de1c) {
    DAT_0044de1c = 0;
    local_68 = 0;
    _local_6c = CONCAT31((int3)((uint)DAT_0043e0a4 >> 8),(char)DAT_0043e874 + 'a');
    UVar2 = GetDriveTypeA(&local_6c);
    if (UVar2 == 5) {
      GetVolumeInformationA
                (&local_6c,(LPSTR)abStack_64,100,(LPDWORD)0x0,&DStack_70,&DStack_74,(LPSTR)0x0,100);
      pbVar7 = abStack_64;
      pbVar3 = &DAT_0043e09c;
      do {
        bVar1 = *pbVar3;
        bVar8 = bVar1 < *pbVar7;
        if (bVar1 != *pbVar7) {
LAB_004024d8:
          iVar4 = (1 - (uint)bVar8) - (uint)(bVar8 != 0);
          goto LAB_004024dd;
        }
        if (bVar1 == 0) break;
        bVar1 = pbVar3[1];
        bVar8 = bVar1 < pbVar7[1];
        if (bVar1 != pbVar7[1]) goto LAB_004024d8;
        pbVar3 = pbVar3 + 2;
        pbVar7 = pbVar7 + 2;
      } while (bVar1 != 0);
      iVar4 = 0;
LAB_004024dd:
      if (iVar4 == 0) goto LAB_004024e6;
    }
    AbortForRemovedRetailCD();
  }
LAB_004024e6:
  DVar5 = timeGetTime();
  if (DVar5 != DAT_0044ddd4) {
    if (DAT_0051c320 != 0) {
      ReloadRegisteredBitmapSurfaces();
      DAT_0051c320 = 0;
      if (DAT_0044dda0 != 0) {
        DAT_0044dda0 = 0;
        DAT_004fbe54 = 0;
        StopAllManagedSounds(DAT_0044ddd8);
        SetCursorSurface((int *)0x0);
        DAT_0051c31c = 0x32;
      }
    }
    DAT_0044ddd4 = DVar5;
    ReapFinishedSounds(DAT_0044ddd8);
    DAT_0051c334 = 0;
    if (DAT_0044dda0 == 0) {
      RunMainGameFlow(param_1);
    }
    else {
      UpdateContextualHelpMode();
    }
    if (1 < DAT_0044a2a0) {
      EnterContextualHelpMode();
    }
    PollDirectInputAndUpdateState();
    uVar6 = (**(code **)(**(int **)(DAT_0044de08 + 4) + 0x68))(*(int **)(DAT_0044de08 + 4));
    if ((int)uVar6 < 0) {
      if ((uVar6 != 0x887600e1) && (uVar6 != 0x88760245)) {
        if (uVar6 != 0x8876024b) {
          return uVar6;
        }
        uVar6 = CreateOrResetDisplayManager(param_1,DAT_0044de0c);
        return uVar6;
      }
      Sleep(10);
      return 0;
    }
    uVar6 = PresentGameFrame();
    if ((int)uVar6 < 0) {
      if (uVar6 != 0x887601c2) {
        return uVar6;
      }
      RestoreAllDirectDrawSurfaces();
    }
  }
  return 0;
}

