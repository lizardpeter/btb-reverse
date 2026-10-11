/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042e110; function: UpdatePlayerProfileScreen; body bytes: 2495
 * callers: 1; callees: 16; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdatePlayerProfileScreen(void)

{
  undefined1 uVar1;
  FILE *pFVar2;
  int iVar3;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  uint uVar4;
  undefined3 extraout_var_01;
  undefined3 extraout_var_02;
  undefined3 extraout_var_03;
  int iVar5;
  int iVar6;
  undefined1 *puVar7;
  int *unaff_EBX;
  int iVar8;
  int **ppiVar9;
  undefined4 *puVar10;
  int iVar11;
  int *piVar12;
  bool bVar13;
  undefined4 uVar14;
  int *apiStack_fc [4];
  int *piStack_ec;
  int iStack_e8;
  int *apiStack_d4 [10];
  int aiStack_ac [43];
  
  piVar12 = *(int **)(DAT_0044de08 + 0xc);
  iVar8 = 0;
  iStack_e8 = 0;
  piStack_ec = (int *)0x0;
  apiStack_fc[3] = (int *)DAT_0051c268;
  apiStack_fc[2] = (int *)0x0;
  apiStack_fc[1] = (int *)0x0;
  apiStack_fc[0] = piVar12;
  apiStack_d4[5] = piVar12;
  (**(code **)(*piVar12 + 0x1c))();
  if (0 < DAT_0051c31c) {
    DAT_0051c31c = DAT_0051c31c + -1;
  }
  UpdateHelpButtonController();
  DAT_0051c370 = 1;
  UpdateGenericHelpHoverVoice();
  DAT_0051c370 = 0;
  EnableInputProcessing();
  puVar10 = &DAT_0051c340;
  for (iVar5 = 10; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar10 = 0;
    puVar10 = puVar10 + 1;
  }
  if ((DAT_0051c338 == 1) && (iVar5 = AnyManagedSoundPlaying(DAT_0044ddd8), iVar5 == 0)) {
    DAT_0051c338 = 0;
    (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,piVar12,0,0);
    DAT_0051c2c0 = 1;
    DAT_0051c3a0 = 0;
    return;
  }
  iVar5 = DAT_00447004;
  if (DAT_0051c2dc == 1) {
    DAT_0051c2dc = 0;
    (&DAT_0051c24c)[DAT_00447004] = 0xffffffff;
    (&DAT_0051b404)[iVar5] = 0;
    puVar10 = (undefined4 *)(&DAT_0051b598 + iVar5 * 400);
    for (iVar6 = 0xf; iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar10 = 0;
      puVar10 = puVar10 + 1;
    }
    pFVar2 = (FILE *)crt_fopen(s_dypdata1_txt_0043f9c8 + iVar5 * 0x32,&DAT_00447544);
    if (pFVar2 != (FILE *)0x0) {
      crt_fclose(pFVar2);
    }
    pFVar2 = (FILE *)crt_fopen(s_firedata1_txt_00442638 + DAT_00447004 * 0x32,&DAT_00447544);
    if (pFVar2 != (FILE *)0x0) {
      crt_fclose(pFVar2);
    }
    pFVar2 = (FILE *)crt_fopen(s_musicbob1_txt_00444384 + DAT_00447004 * 0x80,&DAT_00447544);
    if (pFVar2 != (FILE *)0x0) {
      crt_fclose(pFVar2);
    }
    pFVar2 = (FILE *)crt_fopen(s_musicwendy1_txt_00444604 + DAT_00447004 * 0x80,&DAT_00447544);
    if (pFVar2 != (FILE *)0x0) {
      crt_fclose(pFVar2);
    }
    pFVar2 = (FILE *)crt_fopen(s_musicfarmer1_txt_00444884 + DAT_00447004 * 0x80,&DAT_00447544);
    if (pFVar2 != (FILE *)0x0) {
      crt_fclose(pFVar2);
    }
  }
  aiStack_ac[0] = 0x5c;
  aiStack_ac[1] = 0xa7;
  aiStack_ac[2] = 0x122;
  aiStack_ac[3] = 0x184;
  aiStack_ac[4] = 0x219;
  apiStack_d4[0] = (int *)0xc7;
  apiStack_d4[1] = (int *)0x150;
  apiStack_d4[2] = (int *)0xe0;
  apiStack_d4[3] = (int *)0x148;
  apiStack_d4[4] = (int *)0x107;
  piStack_ec = &DAT_0051b41c;
  do {
    iVar5 = *(int *)((int)&DAT_0051b404 + iVar8);
    iVar3 = 0;
    piVar12 = piStack_ec;
    iVar6 = iVar5;
    if (0 < iVar5) {
      do {
        iVar3 = iVar3 + (*(int *)(&DAT_00517958 + *piVar12 * 0x10) -
                        *(int *)(&DAT_00517950 + *piVar12 * 0x10));
        iVar6 = iVar6 + -1;
        piVar12 = piVar12 + 1;
      } while (iVar6 != 0);
    }
    iVar11 = 0;
    iVar6 = *(int *)((int)aiStack_ac + iVar8) - iVar3 / 2;
    piVar12 = piStack_ec;
    if (0 < iVar5) {
      do {
        iStack_e8 = *(int *)(&DAT_00517950 + *piVar12 * 0x10);
        iVar5 = *(int *)(&DAT_00517958 + *piVar12 * 0x10);
        (**(code **)(*unaff_EBX + 0x1c))
                  (unaff_EBX,iVar6,*(undefined4 *)((int)apiStack_d4 + iVar8),DAT_0051b3a4,&iStack_e8
                   ,1);
        iVar6 = iVar6 + (iVar5 - iStack_e8);
        iVar11 = iVar11 + 1;
        piVar12 = piVar12 + 1;
      } while (iVar11 < *(int *)((int)&DAT_0051b404 + iVar8));
    }
    iVar8 = iVar8 + 4;
    piStack_ec = piStack_ec + 9;
  } while ((int)piStack_ec < 0x51b4d0);
  apiStack_d4[0] = (int *)0x42;
  apiStack_d4[1] = (int *)0x8c;
  apiStack_d4[2] = (int *)0x8a;
  apiStack_d4[3] = (int *)0x10d;
  apiStack_d4[4] = (int *)0x10b;
  apiStack_d4[5] = (int *)0xa4;
  apiStack_d4[6] = (int *)0x16b;
  apiStack_d4[7] = (int *)0x109;
  apiStack_d4[8] = (int *)0x205;
  apiStack_d4[9] = (int *)0xc8;
  ppiVar9 = apiStack_d4;
  piVar12 = &DAT_0051c24c;
  do {
    if (*piVar12 != -1) {
      iStack_e8 = *piVar12 * 0x32;
      (**(code **)(*unaff_EBX + 0x1c))(unaff_EBX,*ppiVar9,ppiVar9[1],DAT_00519958,&iStack_e8,1);
    }
    piVar12 = piVar12 + 1;
    ppiVar9 = ppiVar9 + 2;
  } while ((int)piVar12 < 0x51c260);
  if (-1 < DAT_0044700c) {
    iVar5 = AnyManagedSoundPlaying(DAT_0044ddd8);
    iVar8 = DAT_0051b400;
    if (iVar5 == 0) {
      iVar5 = 0;
      DAT_0044700c = 0xffffffff;
      DAT_0051be48 = DAT_0051b400;
      if (0 < DAT_0051b400) {
        puVar7 = &DAT_0051b3f8;
        do {
          uVar1 = *puVar7;
          puVar7 = puVar7 + 4;
          (&DAT_00519948)[iVar5] = uVar1;
          iVar5 = iVar5 + 1;
        } while (iVar5 < iVar8);
      }
      puVar10 = DAT_0051c2b4;
      DAT_0051b3a0 = 0xffffffff;
      DAT_0051c2f4 = 1;
      DAT_0044de14 = 4;
      bVar13 = DAT_0051c2b4 == (undefined4 *)0x0;
      (&DAT_0051b4d0)[DAT_00519934 * 100] = 1;
      if (bVar13) {
        return;
      }
      CSound_Stop((int)puVar10);
      if (DAT_0051c2b4 == (undefined4 *)0x0) {
        return;
      }
      (**(code **)*DAT_0051c2b4)(1);
      DAT_0051c2b4 = (undefined4 *)0x0;
      return;
    }
    if (-1 < DAT_0044700c) {
      return;
    }
  }
  iVar8 = _DAT_00447010;
  if (DAT_0051c338 != 1) {
    aiStack_ac[10] = 0x105;
    aiStack_ac[0x12] = 0x105;
    aiStack_ac[0x1c] = 0x1d3;
    aiStack_ac[0x24] = 0x1d3;
    apiStack_d4[1] = (int *)0x1a2;
    apiStack_d4[5] = (int *)0x1a2;
    aiStack_ac[5] = 0x2a;
    aiStack_ac[6] = 0x8c;
    aiStack_ac[7] = 0x8e;
    aiStack_ac[8] = 0xdf;
    aiStack_ac[9] = 0x5a;
    aiStack_ac[0xb] = 0xe6;
    aiStack_ac[0xc] = 0x182;
    aiStack_ac[0xd] = 0xe8;
    aiStack_ac[0xe] = 0x9c;
    aiStack_ac[0xf] = 0x160;
    aiStack_ac[0x10] = 0xf7;
    aiStack_ac[0x11] = 0x141;
    aiStack_ac[0x13] = 0x1c6;
    aiStack_ac[0x14] = 0x174;
    aiStack_ac[0x15] = 0x1d7;
    aiStack_ac[0x16] = 0xc3;
    aiStack_ac[0x17] = 0x255;
    aiStack_ac[0x18] = 0x123;
    aiStack_ac[0x19] = 0x11;
    aiStack_ac[0x1a] = 0x1a9;
    aiStack_ac[0x1b] = 0x3d;
    aiStack_ac[0x1d] = 0x125;
    aiStack_ac[0x1e] = 0x1a5;
    aiStack_ac[0x1f] = 0x159;
    aiStack_ac[0x20] = 0x1d8;
    aiStack_ac[0x21] = 0x240;
    aiStack_ac[0x22] = 0x1a8;
    aiStack_ac[0x23] = 0x26a;
    iVar8 = -1;
    apiStack_d4[0] = (int *)0xe;
    apiStack_d4[2] = (int *)0x124;
    apiStack_d4[3] = (int *)0x1a0;
    apiStack_d4[4] = (int *)0x23e;
    iVar5 = 0;
    piVar12 = aiStack_ac + 7;
    do {
      if ((((piVar12[-2] < DAT_004fbd24) && (DAT_004fbd24 < *piVar12)) &&
          (piVar12[-1] < DAT_004fbd30)) && (DAT_004fbd30 < piVar12[1])) {
        if (iVar5 < 5) {
          iVar8 = DAT_00447008;
          if (((&DAT_0051b404)[iVar5] == 0) && ((&DAT_0051c24c)[iVar5] == -1)) {
            if ((DAT_00447008 != iVar5) && (iVar8 = iVar5, DAT_0051c3a0 == 0)) {
              PlayManagedSoundById(DAT_0044ddd8,0x135,0x32,2);
            }
          }
          else if (((DAT_00447008 != iVar5) &&
                   ((DAT_00447008 = iVar5, bVar13 = IsSoundIdPlaying(DAT_0044ddd8,0x136),
                    iVar8 = DAT_00447008, CONCAT31(extraout_var,bVar13) == 0 &&
                    (bVar13 = IsSoundIdPlaying(DAT_0044ddd8,0x137), iVar8 = DAT_00447008,
                    CONCAT31(extraout_var_00,bVar13) == 0)))) && (DAT_0051c3a0 == 0)) {
            iVar8 = 2;
            uVar14 = 0x32;
            uVar4 = FUN_0042ffc4();
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            PlayManagedSoundById(DAT_0044ddd8,uVar4 + 0x136,uVar14,iVar8);
            iVar8 = DAT_00447008;
          }
        }
        else {
          if ((iVar5 == 6) && (DAT_00447008 != 6)) {
            PlayManagedSoundById(DAT_0044ddd8,0x1b2,0x32,2);
          }
          DAT_00447008 = iVar5;
          (**(code **)(*unaff_EBX + 0x1c))
                    (unaff_EBX,apiStack_fc[iVar5 * 2],apiStack_fc[iVar5 * 2 + 1],
                     (&DAT_0051b3a0)[iVar5 * 2],0,0);
          iVar8 = DAT_00447008;
        }
        DAT_00447008 = iVar8;
        iVar8 = iVar5;
        if (iVar5 != -1) goto LAB_0042e832;
        break;
      }
      iVar5 = iVar5 + 1;
      piVar12 = piVar12 + 4;
    } while (iVar5 < 8);
    DAT_00447008 = -1;
    _DAT_00447010 = -1;
    iVar5 = iVar8;
LAB_0042e832:
    if ((DAT_004fbfb8 != 0) && (4 < iVar5)) {
      (**(code **)(*unaff_EBX + 0x1c))
                (unaff_EBX,apiStack_fc[iVar5 * 2],apiStack_fc[iVar5 * 2 + 1],
                 (&DAT_0051b3a4)[iVar5 * 2],0,0);
    }
    iVar8 = _DAT_00447010;
    if ((((DAT_004fbfb4 != 0) && (*(char *)((int)DAT_0044ddd8 + 0x7fb) == '\0')) &&
        (bVar13 = IsSoundIdPlaying(DAT_0044ddd8,0x12f), iVar8 = _DAT_00447010,
        CONCAT31(extraout_var_01,bVar13) == 0)) &&
       ((bVar13 = IsSoundIdPlaying(DAT_0044ddd8,0x130), iVar8 = _DAT_00447010,
        CONCAT31(extraout_var_02,bVar13) == 0 &&
        (bVar13 = IsSoundIdPlaying(DAT_0044ddd8,0x131), iVar8 = _DAT_00447010,
        CONCAT31(extraout_var_03,bVar13) == 0)))) {
      if ((-1 < iVar5) && (iVar5 < 5)) {
        if (DAT_0051c3a0 == 0) {
          if (((int)(&DAT_0051b404)[iVar5] < 1) && ((int)(&DAT_0051c24c)[iVar5] < 0)) {
            StopAllManagedSounds(DAT_0044ddd8);
            PlayManagedSoundById(DAT_0044ddd8,0x13f,0x32,1);
            *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x3bf) * 4 + 0xd98) =
                 1;
            iVar8 = (&DAT_0051b404)[iVar5];
            DAT_0051c248 = (&DAT_0051c24c)[iVar5];
            iVar6 = 0;
            DAT_0051be48 = iVar8;
            if (0 < iVar8) {
              puVar10 = &DAT_0051b41c + iVar5 * 9;
              do {
                uVar1 = *(undefined1 *)puVar10;
                puVar10 = puVar10 + 1;
                (&DAT_00519948)[iVar6] = uVar1;
                iVar6 = iVar6 + 1;
              } while (iVar6 < iVar8);
            }
            DAT_0051c2f4 = 1;
            DAT_0051b3a0 = iVar5;
          }
          else {
            PlayManagedSoundById(DAT_0044ddd8,0x131,0x32,2);
            DAT_0044700c = 1;
            DAT_00519934 = iVar5;
          }
        }
        else if ((0 < (int)(&DAT_0051b404)[iVar5]) || (-1 < (int)(&DAT_0051c24c)[iVar5])) {
          DAT_0051c310 = 0;
          DAT_0051c2d0 = 1;
          DAT_0051c294 = LoadBitmapToDirectDrawSurface
                                   (*(int **)(DAT_0044de08 + 4),s_data_ui_deletegame_bmp_0044752c,0,
                                    0);
          RegisterBitmapSurface(&DAT_0051c294,s_data_ui_deletegame_bmp_0044752c);
          MarkRegisteredSurfaceColorKeyed(0x51c294);
          SetSurfaceTransparencyColorKey(DAT_0051c294,0xff00ff);
          StopAllManagedSounds(DAT_0044ddd8);
          PlayManagedSoundById(DAT_0044ddd8,0x330,0x32,2);
          _DAT_0051c2d4 = 1;
          DAT_0051c3a0 = 0;
          DAT_00447004 = iVar5;
          SetCursorSurface((int *)0x0);
          return;
        }
      }
      if (iVar5 == 7) {
        DAT_0051c338 = 1;
        PlayManagedSoundById(DAT_0044ddd8,0x133,0x32,2);
        _DAT_00447010 = iVar5;
        return;
      }
      iVar8 = iVar5;
      if (iVar5 == 6) {
        bVar13 = DAT_0051c3a0 != 0;
        if (bVar13) {
          piVar12 = (int *)0x0;
        }
        else {
          PlayManagedSoundById(DAT_0044ddd8,0x132,0x32,2);
          piVar12 = DAT_00519938;
        }
        DAT_0051c3a0 = (uint)!bVar13;
        SetCursorSurface(piVar12);
      }
    }
  }
  _DAT_00447010 = iVar8;
  return;
}

