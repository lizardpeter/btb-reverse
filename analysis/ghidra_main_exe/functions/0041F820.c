/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041f820; function: UpdateGrandOpeningToolbar; body bytes: 862
 * callers: 1; callees: 10; success: True
 */


void UpdateGrandOpeningToolbar(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  int local_40 [16];
  
  piVar1 = *(int **)(DAT_0044de08 + 4);
  piVar2 = *(int **)(DAT_0044de08 + 0xc);
  iVar6 = 0;
  if (DAT_00512188 == 0) {
    if ((DAT_00513f18 != 0) && (iVar3 = AnyManagedSoundPlaying(DAT_0044ddd8), iVar3 == 0)) {
      DAT_00513f18 = 0;
      DAT_00512188 = 8;
      return;
    }
    local_40[3] = 0x1d8;
    local_40[5] = 0x1a0;
    local_40[7] = 0x1d8;
    local_40[9] = 0x1a0;
    local_40[0xb] = 0x1d8;
    local_40[0xd] = 0x1a0;
    local_40[0xf] = 0x1d8;
    local_40[2] = 0x17a;
    local_40[4] = 0x104;
    local_40[6] = 0x13a;
    local_40[8] = 0x67;
    local_40[10] = 0x9d;
    local_40[0xc] = 0x1e1;
    local_40[0xe] = 0x217;
    iVar3 = -1;
    piVar4 = local_40 + 2;
    do {
      if ((((piVar4[-2] < DAT_004fbd24) && (DAT_004fbd24 < *piVar4)) && (piVar4[-1] < DAT_004fbd30))
         && (DAT_004fbd30 < piVar4[1])) {
        if ((DAT_00513f24 == 0) &&
           ((**(code **)(*piVar2 + 0x1c))
                      (piVar2,*(undefined4 *)(&DAT_00444d5c + iVar6 * 8),
                       *(undefined4 *)(&DAT_00444d60 + iVar6 * 8),(&DAT_0051276c)[iVar6 * 2],0,0),
           DAT_00444d84 != iVar6)) {
          PlayManagedSoundById(DAT_0044ddd8,iVar6 + 6 + DAT_00512740,0x32,2);
          DAT_00444d84 = iVar6;
        }
        iVar3 = iVar6;
        if (iVar6 != -1) goto LAB_0041f980;
        break;
      }
      iVar6 = iVar6 + 1;
      piVar4 = piVar4 + 4;
    } while (iVar6 < 4);
    DAT_00444d84 = -1;
    iVar6 = iVar3;
LAB_0041f980:
    if ((DAT_004fbfb8 != 0) && (iVar6 != -1)) {
      (**(code **)(*piVar2 + 0x1c))
                (piVar2,*(undefined4 *)(&DAT_00444d5c + iVar6 * 8),
                 *(undefined4 *)(&DAT_00444d60 + iVar6 * 8),(&DAT_00512770)[iVar6 * 2],0,0);
    }
    if ((DAT_004fbfb4 != 0) && (iVar6 != -1)) {
      if (DAT_00512188 == 1) {
        if (iVar6 == 3) {
          DAT_00512188 = 0;
          SetCursorSurface((int *)0x0);
          return;
        }
      }
      else {
        switch(iVar6) {
        case 0:
          if ((DAT_00512188 == 0) && (DAT_00513f24 == 0)) {
            DAT_00513f18 = 1;
            StopAllManagedSounds(DAT_0044ddd8);
            PlayManagedSoundById(DAT_0044ddd8,DAT_00512740 + 1,0x32,1);
            return;
          }
          break;
        case 1:
          if ((DAT_00512188 == 9) && (DAT_00513f24 == 0)) {
            DAT_00512188 = 0;
            CSound_Stop(DAT_004fc14c);
            iVar6 = 1;
            uVar7 = 0x32;
            uVar5 = FUN_0042ffc4();
            uVar5 = uVar5 & 0x80000001;
            if ((int)uVar5 < 0) {
              uVar5 = (uVar5 - 1 | 0xfffffffe) + 1;
            }
            PlayManagedSoundById(DAT_0044ddd8,uVar5 + 3 + DAT_00512740,uVar7,iVar6);
            return;
          }
          break;
        case 2:
          if ((DAT_00512188 == 0) && (DAT_00513f24 == 0)) {
            DAT_0051c2d0 = 1;
            DAT_0051c310 = 2;
            DAT_0051c294 = LoadBitmapToDirectDrawSurface
                                     (piVar1,s_data_ui_Deletebricks_bmp_004459b0,0,0);
            RegisterBitmapSurface(&DAT_0051c294,s_data_ui_Deletebricks_bmp_004459b0);
            MarkRegisteredSurfaceColorKeyed(0x51c294);
            SetSurfaceTransparencyColorKey(DAT_0051c294,0xff00ff);
            return;
          }
          break;
        case 3:
          if (DAT_00512188 == 0) {
            if (DAT_00513f24 == 0) {
              iVar6 = 1;
              uVar7 = 0x32;
              uVar5 = FUN_0042ffc4();
              uVar5 = uVar5 & 0x80000001;
              if ((int)uVar5 < 0) {
                uVar5 = (uVar5 - 1 | 0xfffffffe) + 1;
              }
              PlayManagedSoundById(DAT_0044ddd8,uVar5 + 9 + DAT_00512740,uVar7,iVar6);
              SetCursorSurface(DAT_00508c0c);
              DAT_00513f24 = 1;
              return;
            }
            SetCursorSurface((int *)0x0);
            DAT_00513f24 = 0;
          }
        }
      }
    }
  }
  return;
}

