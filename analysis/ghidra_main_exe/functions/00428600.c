/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00428600; function: UpdateGenericUIScreenInteraction; body bytes: 2083
 * callers: 1; callees: 11; success: True
 */


undefined4 UpdateGenericUIScreenInteraction(void)

{
  int *piVar1;
  bool bVar2;
  int iVar3;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  
  EnableInputProcessing();
  piVar1 = *(int **)((int)DAT_0044de08 + 0xc);
  DAT_0051c328 = DAT_0051c328 + -1;
  if (DAT_0051c328 < 0) {
    DAT_0051c328 = 5;
  }
  if (DAT_0051c378 != 0) {
    iVar3 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if (iVar3 == 0) {
      DAT_0051c378 = 0;
      if (*(int *)(&DAT_00494710 + (DAT_0051c27c * 0x20 + DAT_0051b400) * 0x40c) == -99) {
        DAT_0051c2bc = 1;
        (**(code **)(*DAT_0051c298 + 0x1c))
                  (DAT_0051c298,0,0,*(undefined4 *)((int)DAT_0044de08 + 0xc),0,0);
        return 1;
      }
      NoOpLegacyHook();
      if (DAT_0051c304 != 0) {
        if (*(int *)(&DAT_00494710 + (DAT_0051c27c * 0x20 + DAT_0051b400) * 0x40c) != 0x22) {
          DAT_0044de14 = *(int *)(&DAT_00494710 + (DAT_0051c27c * 0x20 + DAT_0051b400) * 0x40c);
          DAT_0051c304 = 0;
          return 1;
        }
        StopAllManagedSounds(DAT_0044ddd8);
        piVar1 = *(int **)((int)DAT_0044de08 + 4);
        DAT_0051bca0 = LoadBitmapToDirectDrawSurface
                                 (piVar1,s_data_ui_Progress_screen_bmp_00447028,0,0);
        RegisterBitmapSurface(&DAT_0051bca0,s_data_ui_Progress_screen_bmp_00447028);
        DAT_0051a664 = LoadBitmapToDirectDrawSurface(piVar1,s_data_ui_star_bmp_00447014,0,0);
        RegisterBitmapSurface(&DAT_0051a664,s_data_ui_star_bmp_00447014);
        MarkRegisteredSurfaceColorKeyed(0x51a664);
        SetSurfaceTransparencyColorKey(DAT_0051a664,0xff00ff);
        DAT_0051c324 = 1;
        DAT_0051c330 = 1;
        return 1;
      }
      DAT_0044de14 = *(int *)(&DAT_00494710 + (DAT_0051c27c * 0x20 + DAT_0051b400) * 0x40c);
    }
    return 1;
  }
  iVar8 = 0;
  iVar3 = DAT_0051c27c;
  if (0 < (int)(&DAT_0048fb40)[DAT_0051c27c]) {
    do {
      if (DAT_0051c2f8 != 0) {
        iVar4 = iVar3 * 0x20 + iVar8;
        iVar5 = iVar4 * 0x40c;
        iVar7 = *(int *)(&DAT_00494710 + iVar5);
        if (((iVar7 < -1) && (-5 < iVar7)) && (-2 - iVar7 == DAT_0051c284)) {
          (**(code **)(*piVar1 + 0x1c))
                    (piVar1,*(undefined4 *)(&DAT_00494570 + iVar5),
                     *(undefined4 *)(&DAT_00494574 + iVar5),
                     *(undefined4 *)
                      (&DAT_00494838 + (*(int *)(&DAT_00494798 + iVar5) + iVar4 * 0x103) * 4),0,0);
          iVar3 = DAT_0051c27c;
        }
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 < (int)(&DAT_0048fb40)[iVar3]);
  }
  iVar4 = DAT_004fbd30;
  iVar8 = DAT_004fbd24;
  if (DAT_0051c280 == 0) {
    iVar7 = 0;
    if (0 < (int)(&DAT_0048fb40)[iVar3]) {
      do {
        iVar3 = iVar3 * 0x20 + iVar7;
        iVar5 = 0;
        if ((&DAT_004839c8)[iVar3 * 0x1a] != -1) {
          piVar6 = &DAT_004839c8 + iVar3 * 0x1a;
          do {
            piVar6 = piVar6 + 2;
            iVar5 = iVar5 + 1;
          } while (*piVar6 != -1);
        }
        bVar2 = PointInPolygon(&DAT_004839c8 + iVar3 * 0x1a,iVar5,iVar8,iVar4);
        iVar3 = DAT_0051c27c;
        if (CONCAT31(extraout_var,bVar2) == 0) {
          if (DAT_004fbfb8 == 0) {
LAB_004288fc:
            DAT_00446cec = iVar7;
          }
          else {
            if (DAT_00446cec != iVar7) goto LAB_004288e4;
            if (DAT_004fbfb8 == 0) goto LAB_004288fc;
          }
          DAT_0051c280 = iVar7 + 1;
          iVar4 = DAT_0051c27c * 0x20 + iVar7;
          iVar8 = iVar4 * 0x40c;
          bVar2 = DAT_0051c27c == 1;
          *(undefined4 *)(&DAT_00494824 + iVar8) = 8;
          *(undefined4 *)(&DAT_00494798 + iVar8) = 0;
          if (((bVar2) && (iVar7 == 2)) && (DAT_0051c304 == 1)) {
            PlayManagedSoundById(DAT_0044ddd8,0x41,0x32,2);
            iVar3 = DAT_0051c27c;
          }
          else if (0 < *(int *)(&DAT_00494830 + iVar8)) {
            iVar3 = *(int *)(&DAT_0049482c + iVar8);
            *(int *)(&DAT_0049482c + iVar8) = iVar3 + 1;
            if (*(int *)(&DAT_00494830 + iVar8) <= iVar3 + 1) {
              *(undefined4 *)(&DAT_0049482c + iVar8) = 0;
            }
            PlayManagedSoundById
                      (DAT_0044ddd8,
                       *(int *)(&DAT_00494600 +
                               (iVar4 * 0x103 + *(int *)(&DAT_0049482c + iVar8)) * 4),0x32,2);
            iVar3 = DAT_0051c27c;
          }
          break;
        }
LAB_004288e4:
        iVar7 = iVar7 + 1;
      } while (iVar7 < (int)(&DAT_0048fb40)[DAT_0051c27c]);
    }
  }
  else {
    iVar8 = DAT_0051c280 + -1;
    iVar3 = iVar3 * 0x20 + iVar8;
    iVar4 = 0;
    if ((&DAT_004839c8)[iVar3 * 0x1a] != -1) {
      piVar6 = &DAT_004839c8 + iVar3 * 0x1a;
      do {
        piVar6 = piVar6 + 2;
        iVar4 = iVar4 + 1;
      } while (*piVar6 != -1);
    }
    bVar2 = PointInPolygon(&DAT_004839c8 + iVar3 * 0x1a,iVar4,DAT_004fbd24,DAT_004fbd30);
    iVar3 = DAT_0051c27c;
    if (CONCAT31(extraout_var_00,bVar2) == 0) {
      if ((DAT_004fbfb8 == 0) || (DAT_00446cec == iVar8)) {
        iVar5 = DAT_0051c27c * 0x20;
        iVar7 = (iVar5 + iVar8) * 0x40c;
        iVar4 = *(int *)(&DAT_00494824 + iVar7);
        *(int *)(&DAT_00494824 + iVar7) = iVar4 + -1;
        if (iVar4 + -1 < 1) {
          *(undefined4 *)(&DAT_00494824 + iVar7) = 8;
          iVar4 = *(int *)(&DAT_00494798 + iVar7);
          *(int *)(&DAT_00494798 + iVar7) = iVar4 + 1;
          if (*(int *)(&DAT_00494714 + iVar7) <= iVar4 + 1) {
            *(undefined4 *)(&DAT_00494798 + iVar7) = 0;
          }
        }
        if (0 < *(int *)(&DAT_00494714 + iVar7)) {
          iVar5 = iVar5 + DAT_0051c280;
          if (*(int *)(&DAT_0049442c + (*(int *)(&DAT_00494798 + iVar7) + iVar5 * 0x103) * 4) != 0)
          {
            iVar3 = iVar5 * 0x40c;
            (**(code **)(*piVar1 + 0x1c))
                      (piVar1,*(undefined4 *)(&DAT_00494164 + iVar3),
                       *(undefined4 *)(&DAT_00494168 + iVar3),
                       *(int *)(&DAT_0049442c +
                               (*(int *)(&DAT_00494798 + iVar7) + iVar5 * 0x103) * 4),0,0);
            iVar3 = DAT_0051c27c;
          }
        }
        piVar6 = *(int **)(&DAT_00494428 + (iVar3 * 0x20 + DAT_0051c280) * 0x40c);
        if (piVar6 != (int *)0x0) {
          uStack_7c = 0x7c;
          uStack_78 = 6;
          (**(code **)(*piVar6 + 0x58))(piVar6,&uStack_7c);
          iVar3 = (DAT_0051c27c * 0x20 + DAT_0051c280) * 0x40c;
          (**(code **)(*piVar1 + 0x1c))
                    (piVar1,*(undefined4 *)(&DAT_0049416c + iVar3),
                     *(undefined4 *)(&DAT_00494170 + iVar3),*(undefined4 *)(&DAT_00494428 + iVar3),0
                     ,1);
          iVar3 = DAT_0051c27c;
        }
      }
    }
    else {
      DAT_0051c280 = 0;
    }
    if (DAT_004fbe54 != 0) {
      iVar4 = (iVar3 * 0x20 + DAT_0051c280) * 0x40c;
      if (*(int *)(&DAT_0049456c + iVar4) != 0) {
        BltFastToBackBuffer(DAT_0044de08,*(undefined4 *)(&DAT_00494164 + iVar4),
                            *(undefined4 *)(&DAT_00494168 + iVar4),*(int *)(&DAT_0049456c + iVar4),0
                            ,0);
        iVar3 = DAT_0051c27c;
      }
      DAT_00446cec = DAT_0051c280 + -1;
    }
    if ((DAT_004fbfb8 != 0) && (DAT_00446cec == DAT_0051c280 + -1)) {
      iVar4 = (iVar3 * 0x20 + DAT_0051c280) * 0x40c;
      if (*(int *)(&DAT_0049456c + iVar4) != 0) {
        BltFastToBackBuffer(DAT_0044de08,*(undefined4 *)(&DAT_00494164 + iVar4),
                            *(undefined4 *)(&DAT_00494168 + iVar4),*(int *)(&DAT_0049456c + iVar4),0
                            ,0);
        iVar3 = DAT_0051c27c;
      }
    }
    if (((DAT_004fbfb4 != 0) && (DAT_00446cec == iVar8)) && (iVar8 < (int)(&DAT_0048fb40)[iVar3])) {
      DAT_00446cec = -1;
      DAT_0051c378 = 1;
      DAT_0051b400 = iVar8;
      if (*(int *)(&DAT_0049460c + (iVar3 * 0x20 + iVar8) * 0x40c) != -1) {
        StopAllManagedSounds(DAT_0044ddd8);
        PlayManagedSoundById
                  (DAT_0044ddd8,*(int *)(&DAT_0049460c + (DAT_0051c27c * 0x20 + iVar8) * 0x40c),0x32
                   ,2);
        iVar3 = DAT_0051c27c;
        if ((DAT_0051c304 != 0) &&
           (*(int *)(&DAT_00494710 + (DAT_0051c27c * 0x20 + iVar8) * 0x40c) == 0x22)) {
          StopAllManagedSounds(DAT_0044ddd8);
          iVar3 = DAT_0051c27c;
        }
      }
    }
  }
  iVar8 = 0;
  if (0 < (int)(&DAT_0048fb40)[iVar3]) {
    do {
      if (iVar8 != DAT_0051c280 + -1) {
        if (((DAT_0051c328 == 5) &&
            (iVar4 = (iVar3 * 0x20 + iVar8) * 0x40c, 0 < *(int *)(&DAT_004947a0 + iVar4))) &&
           (iVar7 = *(int *)(&DAT_0049479c + iVar4), *(int *)(&DAT_0049479c + iVar4) = iVar7 + 1,
           *(int *)(&DAT_004947a0 + iVar4) <= iVar7 + 1)) {
          *(undefined4 *)(&DAT_0049479c + iVar4) = 0;
        }
        iVar4 = iVar3 * 0x20 + iVar8;
        iVar7 = iVar4 * 0x40c;
        if (0 < *(int *)(&DAT_004947a0 + iVar7)) {
          (**(code **)(*piVar1 + 0x1c))
                    (piVar1,*(undefined4 *)(&DAT_00494570 + iVar7),
                     *(undefined4 *)(&DAT_00494574 + iVar7),
                     *(undefined4 *)
                      (&DAT_004948d8 + (*(int *)(&DAT_0049479c + iVar7) + iVar4 * 0x103) * 4),0,0);
          iVar3 = DAT_0051c27c;
        }
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 < (int)(&DAT_0048fb40)[iVar3]);
  }
  iVar3 = 0;
  if (DAT_0044de14 == 5) {
    iVar8 = 0x32;
    do {
      iVar4 = (&DAT_0051b4d0)[iVar8 + DAT_00519934 * 100];
      if ((0 < iVar4) && (0 < iVar4)) {
        iVar3 = iVar3 + iVar4;
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 < 0x41);
  }
  if (0xc < iVar3) {
    DAT_0051c308 = 1;
  }
  DAT_0051c304 = (uint)(0xc >= iVar3);
  return 1;
}

