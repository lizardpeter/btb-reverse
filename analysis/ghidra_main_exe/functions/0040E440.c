/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040e440; function: UpdateParkDesignerEditorInteraction; body bytes: 10433
 * callers: 1; callees: 25; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl UpdateParkDesignerEditorInteraction(HGLOBAL param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  undefined3 extraout_var;
  int *piVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  undefined4 *puVar15;
  undefined4 uVar16;
  bool bVar17;
  longlong lVar18;
  int local_118;
  int local_114;
  int local_110 [6];
  int *piStack_f8;
  int aiStack_f4 [7];
  undefined4 uStack_d8;
  int iStack_d4;
  int *local_d0;
  int iStack_cc;
  int aiStack_c8 [12];
  int *piStack_98;
  int iStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  local_110[5] = DAT_00507b14;
  local_110[0] = 0x36;
  local_110[1] = 0x36;
  local_110[2] = 0x36;
  piVar5 = *(int **)(DAT_0044de08 + 4);
  piVar2 = *(int **)(DAT_0044de08 + 0xc);
  bVar17 = true;
  local_110[3] = 0x24;
  iVar10 = DAT_004fbd30;
  local_d0 = piVar2;
  if (DAT_00507b5c == 3) {
    iVar10 = 0;
    puVar15 = &DAT_00508ad4;
    do {
      if (iVar10 == DAT_00509344) {
        (**(code **)(*piVar2 + 0x1c))
                  (piVar2,(&DAT_00441db8)[iVar10],(&DAT_00441da8)[iVar10],puVar15[2],0,1);
      }
      else {
        (**(code **)(*piVar2 + 0x1c))
                  (piVar2,(&DAT_00441db8)[iVar10],(&DAT_00441da8)[iVar10],*puVar15,0,1);
      }
      puVar15 = puVar15 + 3;
      iVar10 = iVar10 + 1;
    } while ((int)puVar15 < 0x508b04);
    iVar11 = 0;
    iVar10 = DAT_004fbd30;
    iVar12 = DAT_004fbd24;
    do {
      iVar3 = (&DAT_00441db8)[iVar11];
      if ((((iVar3 < iVar12) && (iVar12 < local_110[iVar11] + iVar3)) &&
          (iVar13 = (&DAT_00441da8)[iVar11], iVar13 < iVar10)) &&
         (iVar10 < local_110[iVar11] + iVar13)) {
        if (DAT_004fbfb8 == 0) {
          if (DAT_00509344 != iVar11) {
            iVar10 = *piVar2;
            uVar16 = *(undefined4 *)(&DAT_00508ad8 + iVar11 * 0xc);
            goto LAB_0040e56a;
          }
        }
        else {
          iVar10 = *piVar2;
          uVar16 = (&DAT_00508adc)[iVar11 * 3];
LAB_0040e56a:
          (**(code **)(iVar10 + 0x1c))(piVar2,iVar3,iVar13,uVar16,0,1);
          iVar10 = DAT_004fbd30;
          iVar12 = DAT_004fbd24;
        }
        if (DAT_004fbfb4 == 0) {
          if ((((int)(&DAT_00441db8)[iVar11] < iVar12) && (iVar12 < (&DAT_00441db8)[iVar11] + 0x36))
             && ((0x1a0 < iVar10 && ((iVar10 < 0x1d6 && (iVar11 == 0)))))) {
            if (DAT_0050935c == 0) {
              PlayManagedSoundById(DAT_0044ddd8,0x3e5,0x32,2);
              DAT_0050935c = 1;
              iVar10 = DAT_004fbd30;
              iVar12 = DAT_004fbd24;
            }
          }
          else {
            DAT_0050935c = 0;
          }
          if (((((int)(&DAT_00441db8)[iVar11] < iVar12) && (iVar12 < (&DAT_00441db8)[iVar11] + 0x36)
               ) && (0x1a0 < iVar10)) && ((iVar10 < 0x1d6 && (iVar11 == 1)))) {
            if (DAT_00509360 == 0) {
              PlayManagedSoundById(DAT_0044ddd8,0x3e6,0x32,2);
              DAT_00509360 = 1;
              iVar10 = DAT_004fbd30;
              iVar12 = DAT_004fbd24;
            }
          }
          else {
            DAT_00509360 = 0;
          }
          if ((((int)(&DAT_00441db8)[iVar11] < iVar12) && (iVar12 < (&DAT_00441db8)[iVar11] + 0x36))
             && ((0x1a0 < iVar10 && ((iVar10 < 0x1d6 && (iVar11 == 2)))))) {
            if (DAT_00509358 == 0) {
              PlayManagedSoundById(DAT_0044ddd8,0x125,0x32,2);
              DAT_00509358 = 1;
              iVar10 = DAT_004fbd30;
              iVar12 = DAT_004fbd24;
            }
          }
          else {
            DAT_00509358 = 0;
          }
          if (((((int)(&DAT_00441db8)[iVar11] < iVar12) && (iVar12 < (&DAT_00441db8)[iVar11] + 0x24)
               ) && ((int)(&DAT_00441da8)[iVar11] < iVar10)) &&
             ((iVar10 < (&DAT_00441da8)[iVar11] + 0x24 && (iVar11 == 3)))) {
            if (DAT_00509364 == 0) {
              PlayManagedSoundById(DAT_0044ddd8,299,0x32,2);
              DAT_00509364 = 1;
              iVar10 = DAT_004fbd30;
              iVar12 = DAT_004fbd24;
            }
          }
          else {
LAB_0040e8fe:
            DAT_00509364 = 0;
          }
        }
        else {
          if (iVar11 == 0) {
            iVar10 = 0;
            uVar4 = FUN_0042ffc4();
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            iVar12 = uVar4 + 0x126;
          }
          else {
            if (iVar11 != 1) {
              if (iVar11 == 2) {
                iVar10 = 0;
                piStack_f8 = &DAT_0051b404 + DAT_00519934;
                iStack_cc = *piStack_f8;
                if (0 < iStack_cc) {
                  piVar9 = &DAT_0051b41c + DAT_00519934 * 9;
                  iVar12 = iStack_cc;
                  do {
                    iVar11 = *piVar9;
                    piVar9 = piVar9 + 1;
                    iVar10 = iVar10 + (*(int *)(&DAT_00517958 + iVar11 * 0x10) -
                                      *(int *)(&DAT_00517950 + iVar11 * 0x10));
                    iVar12 = iVar12 + -1;
                  } while (iVar12 != 0);
                }
                iVar10 = 0x140 - iVar10 / 2;
                iVar11 = 0;
                if (0 < iStack_cc) {
                  piVar9 = &DAT_0051b41c + DAT_00519934 * 9;
                  do {
                    iVar12 = *piVar9 * 0x10;
                    aiStack_f4[1] = *(int *)(&DAT_00517954 + iVar12);
                    aiStack_f4[0] = *(int *)(&DAT_00517950 + iVar12);
                    aiStack_f4[2] = *(int *)(&DAT_00517958 + iVar12);
                    aiStack_f4[3] = *(int *)(&DAT_0051795c + iVar12) + 1;
                    (**(code **)(*piVar2 + 0x1c))(piVar2,iVar10,0x1a4,DAT_0051b3a4,aiStack_f4,1);
                    iVar10 = iVar10 + (aiStack_f4[2] - aiStack_f4[0]);
                    iVar11 = iVar11 + 1;
                    piVar9 = piVar9 + 1;
                  } while (iVar11 < *piStack_f8);
                }
                aiStack_f4[1] = 0;
                aiStack_f4[3] = 0x2d;
                aiStack_f4[0] = (&DAT_0051c24c)[DAT_00519934] * 0x32;
                aiStack_f4[2] = aiStack_f4[0] + 0x32;
                (**(code **)(*piVar2 + 0x1c))(piVar2,0xdc,0x1a4,DAT_00519958,aiStack_f4,1);
                (**(code **)(*piVar2 + 0x1c))(piVar2,0,0,DAT_00508ad0,0,1);
                DAT_004fbd24 = 0;
                ExportAndPrintGameImage(param_1);
                iVar10 = DAT_004fbd30;
                iVar12 = DAT_004fbd24;
              }
              goto LAB_0040e908;
            }
            iVar10 = 0;
            uVar4 = FUN_0042ffc4();
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            iVar12 = uVar4 + 0x129;
          }
          PlayInputInterruptibleParkDesignerVoice(iVar12,iVar10);
          DAT_00509344 = iVar11;
          ApplyParkDesignerSeason(iVar11);
          iVar10 = DAT_004fbd30;
          iVar12 = DAT_004fbd24;
        }
      }
      else if (iVar11 == 0) {
        DAT_0050935c = 0;
      }
      else {
        if (iVar11 != 1) {
          if (iVar11 == 2) {
            DAT_00509358 = 0;
            goto LAB_0040e908;
          }
          if (iVar11 != 3) goto LAB_0040e908;
          goto LAB_0040e8fe;
        }
        DAT_00509360 = 0;
      }
LAB_0040e908:
      iVar11 = iVar11 + 1;
    } while (iVar11 < 4);
  }
  if (((iVar10 < 0x192) && (iVar10 = FindTopmostParkDesignerObjectAtCursor(0), iVar10 != -1)) &&
     (DAT_00507b14 == 0)) {
    iVar10 = *(int *)((&DAT_00508c10)[iVar10] + 0x34);
    if (iVar10 < 100) {
      DAT_00507b5c = 0;
      DAT_00507ae4 = 2;
      (**(code **)(*DAT_00507a00 + 0x1c))
                (DAT_00507a00,0,0,
                 *(undefined4 *)(&DAT_0050819c + (&DAT_004fcacc)[iVar10 * 0x13] * 0x30),
                 &DAT_00508188 + (&DAT_004fcaec)[iVar10 * 0x13] * 0x30,0);
      SetCursorSurface(DAT_00507a00);
      DAT_00509338 = (int *)(&DAT_00507e40)[(&DAT_004fcacc)[iVar10 * 0x13]];
      DAT_00507b58 = iVar10;
      SetCursorSurface(DAT_00509338);
    }
    else {
      if (iVar10 < 300) {
        if ((199 < iVar10) && (DAT_00507b5c = 1, (&DAT_004fcae8)[iVar10 * 0x13] == 1)) {
          _DAT_00508bf8 = 1;
          ClearBandstandObjectsByCategorySelector(1);
          iVar12 = DAT_00507c54;
          if (DAT_00507c54 != -1) {
            DAT_00507c54 = -1;
            (&DAT_004fcacc)[iVar12 * 0x13] = 0xffffffff;
          }
          piVar2 = &DAT_00507a2c;
          do {
            if (*piVar2 == iVar10) {
              *piVar2 = -1;
              DAT_00507a40 = DAT_00507a40 + -1;
            }
            piVar2 = piVar2 + 1;
          } while ((int)piVar2 < 0x507a3c);
        }
      }
      else {
        DAT_00507b5c = 2;
        DAT_005079f8 = (&DAT_004fcae8)[iVar10 * 0x13];
        DAT_00507b10 = (&DAT_004fcaec)[iVar10 * 0x13];
        iVar12 = (DAT_005079f8 + DAT_00507b10 + DAT_005079f8 * 4) * 8;
        SetCursorHotspotOffset
                  (*(undefined4 *)(&DAT_00507ef0 + iVar12),*(undefined4 *)(&DAT_00507ef4 + iVar12));
      }
      iVar3 = DAT_00507b5c;
      iVar12 = (&DAT_004fcae8)[iVar10 * 0x13];
      iVar11 = (&DAT_004fcaec)[iVar10 * 0x13];
      DAT_00507b58 = iVar10;
      (&DAT_00507ae4)[DAT_00507b5c] = iVar12;
      iVar11 = (iVar12 + iVar3 * 4) * 5 + iVar11;
      (**(code **)(*DAT_00507a00 + 0x1c))
                (DAT_00507a00,0,0,(&DAT_00507fbc)[iVar11 * 0xc],&DAT_00507fa8 + iVar11 * 0x30,0);
      SetCursorSurface(DAT_00507a00);
      DAT_00507b14 = 1;
      iVar3 = ((&DAT_004fcae8)[iVar10 * 0x13] + DAT_00507b5c * 4) * 5 +
              (&DAT_004fcaec)[iVar10 * 0x13];
      iVar12 = (&DAT_00507f98)[iVar3 * 0xc];
      iVar11 = (&DAT_00507f90)[iVar3 * 0xc];
      piStack_f8 = (int *)((&DAT_00507f9c)[iVar3 * 0xc] - (&DAT_00507f94)[iVar3 * 0xc]);
      if (DAT_00507b5c == 0) {
        if (DAT_00507ae4 == 0) {
          DAT_00507b58 = iVar10;
          (**(code **)(*DAT_004fca90 + 0x1c))(DAT_004fca90,0,0,DAT_004fcaa8,0,1);
          (**(code **)(*DAT_004fca90 + 0x1c))
                    (DAT_004fca90,0,0,(&DAT_00504178)[iVar10],&stack0xfffffed8,1);
          SetCursorSurface(DAT_004fca90);
          DAT_00509338 = DAT_004fca90;
        }
        else if (DAT_00507ae4 == 1) {
          local_110[1] = 0;
          local_110[0] = iVar10 * 0x7b8;
          local_110[2] = local_110[0] + 0x98;
          local_110[3] = 0x9b;
          DAT_00507b58 = iVar10;
          (**(code **)(*DAT_00507b54 + 0x1c))(DAT_00507b54,0,0,DAT_005092ac,local_110,0);
          SetCursorSurface(DAT_00507b54);
          DAT_00509338 = DAT_00507b54;
        }
        else {
          DAT_00509338 = (int *)(&DAT_00507fa0)[(iVar10 + DAT_00507ae4 * 5) * 0xc];
          DAT_00507b58 = iVar10;
          SetCursorSurface(DAT_00509338);
        }
      }
      else {
        DAT_00507b58 = iVar10;
        if ((DAT_0050933c != (int *)0x0) &&
           (UnregisterBitmapSurface(0x50933c), DAT_0050933c != (int *)0x0)) {
          (**(code **)(*DAT_0050933c + 8))(DAT_0050933c);
          DAT_0050933c = (int *)0x0;
        }
        piVar2 = aiStack_c8 + 10;
        for (iVar3 = 0x1f; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = 0;
          piVar2 = piVar2 + 1;
        }
        piStack_98 = piStack_f8;
        aiStack_c8[10] = 0x7c;
        aiStack_c8[0xb] = 7;
        uStack_38 = 0x40;
        iStack_94 = iVar12 - iVar11;
        (**(code **)(*piVar5 + 0x18))(piVar5,aiStack_c8 + 10,&DAT_0050933c,0);
        iVar12 = ((&DAT_004fcae8)[iVar10 * 0x13] + DAT_00507b5c * 4) * 5 +
                 (&DAT_004fcaec)[iVar10 * 0x13];
        (**(code **)(*DAT_0050933c + 0x1c))
                  (DAT_0050933c,0,0,(&DAT_00507fa0)[iVar12 * 0xc],&DAT_00507f90 + iVar12 * 0xc,0);
        MarkRegisteredSurfaceColorKeyed(0x50933c);
        SetSurfaceTransparencyColorKey(DAT_0050933c,0xff00ff);
        DAT_00509338 = DAT_0050933c;
        SetCursorSurface(DAT_0050933c);
      }
    }
    DAT_00507b14 = 2;
    iVar12 = DAT_00507b58 * 0x13;
    DAT_00507b58 = iVar10;
    (&DAT_004fcad4)[iVar12] = 2;
    DAT_004fbd24 = (&DAT_004fcab0)[iVar10 * 0x13];
    DAT_004fbd30 = (&DAT_004fcab4)[iVar10 * 0x13];
    iVar12 = DAT_00507b5c;
  }
  else {
    DrawParkDesignerToolbarAndHover();
    iVar10 = HitTestParkDesignerToolbar();
    if ((DAT_004fbe54 != 0) || (iVar12 = DAT_00507b5c, DAT_004fbd50 == 1)) {
      if (400 < DAT_004fbd30) {
        ClearCursorHotspotAndClampNonnegative();
        FinishParkDesignerObjectDrag();
      }
      iVar12 = DAT_00507b5c;
      if (iVar10 != -1) {
        bVar17 = false;
        if ((iVar10 < 5) && (DAT_00507b5c != 3)) {
          iVar12 = (&DAT_00507ae4)[DAT_00507b5c] + DAT_00507b5c * 4;
          iVar12 = iVar12 + iVar10 + iVar12 * 4;
          (**(code **)(*DAT_00507a00 + 0x1c))
                    (DAT_00507a00,0,0,(&DAT_00507fbc)[iVar12 * 0xc],&DAT_00507fa8 + iVar12 * 0x30,0)
          ;
          SetCursorSurface(DAT_00507a00);
          DAT_004fbd30 = DAT_004fbd30 + -0x22;
          DAT_005079f8 = (&DAT_00507ae4)[DAT_00507b5c];
          DAT_004fbd24 = DAT_004fbd24 + -0x14;
          iVar12 = iVar10 + (DAT_005079f8 + DAT_00507b5c * 4) * 5;
          DAT_00507b14 = 1;
          iVar11 = (&DAT_00507f98)[iVar12 * 0xc];
          iVar3 = (&DAT_00507f90)[iVar12 * 0xc];
          iVar13 = (&DAT_00507f94)[iVar12 * 0xc];
          iVar12 = (&DAT_00507f9c)[iVar12 * 0xc];
          DAT_00507b10 = iVar10;
          DAT_00507b58 = iVar10;
          if (DAT_00507b5c == 0) {
            if (DAT_00507ae4 == 0) {
              (**(code **)(*DAT_004fca90 + 0x1c))(DAT_004fca90,0,0,DAT_004fcaa8,0,0);
              (**(code **)(*DAT_004fca90 + 0x1c))
                        (DAT_004fca90,0,0,(&DAT_00504178)[iVar10],&stack0xfffffed8,1);
              DAT_00509338 = DAT_004fca90;
              iVar12 = DAT_00507b5c;
              if (DAT_00507a28 != -1) {
                PlayInputInterruptibleParkDesignerVoice(0x121,99);
                iVar12 = DAT_00507b5c;
              }
            }
            else {
              if (DAT_00507ae4 != 1) {
                DAT_00509338 = (int *)(&DAT_00507fa0)[DAT_00507ae4 * 0x3c + iVar10 * 0xc];
                iVar12 = DAT_00507b5c;
                goto LAB_00410abc;
              }
              local_110[1] = 0;
              local_110[0] = iVar10 * 0x7b8;
              local_110[2] = local_110[0] + 0x98;
              local_110[3] = 0x9b;
              (**(code **)(*DAT_00507b54 + 0x1c))(DAT_00507b54,0,0,DAT_005092ac,local_110,0);
              DAT_00509338 = DAT_00507b54;
              iVar12 = DAT_00507b5c;
            }
          }
          else {
            if ((DAT_00507b5c == 1) && (DAT_00507ae8 == 0)) {
              PlayInputInterruptibleParkDesignerVoice(0xd7,99);
            }
            if ((DAT_0050933c != (int *)0x0) &&
               (UnregisterBitmapSurface(0x50933c), DAT_0050933c != (int *)0x0)) {
              (**(code **)(*DAT_0050933c + 8))(DAT_0050933c);
              DAT_0050933c = (int *)0x0;
            }
            piVar2 = aiStack_c8 + 10;
            for (iVar7 = 0x1f; iVar7 != 0; iVar7 = iVar7 + -1) {
              *piVar2 = 0;
              piVar2 = piVar2 + 1;
            }
            aiStack_c8[10] = 0x7c;
            aiStack_c8[0xb] = 7;
            uStack_38 = 0x40;
            piStack_98 = (int *)(iVar12 - iVar13);
            iStack_94 = iVar11 - iVar3;
            (**(code **)(*piVar5 + 0x18))(piVar5,aiStack_c8 + 10,&DAT_0050933c,0);
            iVar12 = (&DAT_00507ae4)[DAT_00507b5c] + DAT_00507b5c * 4;
            iVar12 = iVar12 + iVar10 + iVar12 * 4;
            (**(code **)(*DAT_0050933c + 0x1c))
                      (DAT_0050933c,0,0,(&DAT_00507fa0)[iVar12 * 0xc],&DAT_00507f90 + iVar12 * 0xc,0
                      );
            MarkRegisteredSurfaceColorKeyed(0x50933c);
            SetSurfaceTransparencyColorKey(DAT_0050933c,0xff00ff);
            DAT_00509338 = DAT_0050933c;
            iVar12 = DAT_00507b5c;
          }
        }
        else {
          if ((6 < iVar10) && (iVar10 < 0xb)) {
            if (iVar10 == 10) {
              DAT_00507b5c = 3;
              iVar12 = 3;
              goto LAB_0040f038;
            }
            DAT_00507b5c = iVar10 + -7;
            DAT_00441f18 = 1;
          }
          iVar12 = DAT_00507b5c;
          if (iVar10 == 0xb) {
            if (DAT_00507b5c == 3) {
              DAT_0051c310 = 1;
              DAT_0051c2d0 = 1;
              DAT_0051c294 = LoadBitmapToDirectDrawSurface
                                       (piVar5,s_data_ui_Deleteallobjects_bmp_00442618,0,0);
              RegisterBitmapSurface(&DAT_0051c294,s_data_ui_Deleteallobjects_bmp_00442618);
              MarkRegisteredSurfaceColorKeyed(0x51c294);
              SetSurfaceTransparencyColorKey(DAT_0051c294,0xff00ff);
              PlayInputInterruptibleParkDesignerVoice(300,1);
              DAT_0050418c = 0xffffffff;
              bVar17 = false;
              iVar12 = DAT_00507b5c;
            }
            else {
              if (local_110[5] == 5) {
                DAT_00507b14 = 0;
                piVar5 = (int *)0x0;
              }
              else {
                DAT_00507b14 = 5;
                piVar5 = DAT_00508c0c;
              }
              SetCursorSurface(piVar5);
              bVar17 = false;
              iVar12 = DAT_00507b5c;
            }
          }
          else if (iVar10 == 5) {
            if (DAT_00507a20 == 1) {
              DAT_00509348 = 0x3d;
              (&DAT_00507ae4)[DAT_00507b5c] = (&DAT_00507ae4)[DAT_00507b5c] + -1;
            }
          }
          else if ((iVar10 == 6) && (DAT_00507a24 == 1)) {
            DAT_00509348 = -0x3d;
            (&DAT_00507ae4)[DAT_00507b5c] = (&DAT_00507ae4)[DAT_00507b5c] + 1;
          }
        }
      }
    }
LAB_0040f038:
    iVar3 = DAT_00509340;
    iVar11 = DAT_004fbd30;
    iVar10 = DAT_004fbd24;
    uVar4 = 0;
    if (((DAT_00507b14 == 1) || (DAT_00507b14 == 2)) && (bVar17)) {
      if ((DAT_004fbe54 == 1) || (DAT_004fbd50 == 1)) {
        if (iVar12 == 0) {
          if (DAT_00507ae4 == 0) {
            aiStack_f4[0] = 0x6e;
            aiStack_f4[3] = 0x6e;
            local_110[0] = 0xc5;
            iVar10 = DAT_00509340 * 0x4c;
            local_110[1] = 0xa3;
            local_110[2] = 0xd6;
            (&DAT_004fcacc)[DAT_00509340 * 0x13] = 100;
            *(undefined4 *)(&DAT_004fcad0 + iVar10) = 0;
            (&DAT_004fcab4)[iVar3 * 0x13] = iVar11;
            local_110[3] = 0xfa;
            local_110[4] = 0xad;
            aiStack_f4[1] = 0x68;
            aiStack_f4[2] = 0x81;
            aiStack_f4[4] = 0x75;
            (&DAT_004fcab0)[iVar3 * 0x13] = DAT_004fbd24;
            RefreshParkDesignerObjectGeometry(iVar3);
            iVar11 = DAT_00509340;
            iVar12 = DAT_004fbd24;
            iVar10 = aiStack_f4[DAT_00507b58];
            (&DAT_004fcab8)[DAT_00509340 * 0x13] =
                 local_110[DAT_00507b58] + (&DAT_004fcab0)[DAT_00509340 * 0x13];
            (&DAT_004fcabc)[iVar11 * 0x13] = iVar10 + (&DAT_004fcab4)[iVar11 * 0x13];
            (&DAT_004fcae0)[iVar11 * 0x13] = 0xc6;
            (&DAT_004fcacc)[iVar11 * 0x13] = 0xffffffff;
            iVar10 = CanPlaceParkDesignerObjectWithoutOverlap
                               (iVar12,DAT_004fbd30,
                                (&DAT_004fcab8)[iVar11 * 0x13] - (&DAT_004fcab0)[iVar11 * 0x13],
                                (&DAT_004fcabc)[iVar11 * 0x13] - (&DAT_004fcab4)[iVar11 * 0x13],1,
                                iVar11);
            if (iVar10 == 0) {
              PlayInputInterruptibleParkDesignerVoice(0xf1,99);
              iVar12 = DAT_00507b5c;
            }
            else {
              iVar12 = DAT_00507b5c;
              if (DAT_004fbe68 == DAT_00509338) {
                ClearPondPrimaryObjectsBySelector(0);
                ClearPondPrimaryObjectsBySelector(1);
                ClearPondPrimaryObjectsBySelector(2);
                iVar12 = DAT_00509340;
                iVar10 = DAT_004fbd30;
                iVar11 = DAT_00509340 * 0x4c;
                (&DAT_004fcacc)[DAT_00509340 * 0x13] = 100;
                *(undefined4 *)(&DAT_004fcad0 + iVar11) = 0;
                (&DAT_004fcab4)[iVar12 * 0x13] = iVar10;
                (&DAT_004fcab0)[iVar12 * 0x13] = DAT_004fbd24;
                RefreshParkDesignerObjectGeometry(iVar12);
                iVar12 = DAT_00509340;
                iVar10 = DAT_00507b58;
                DAT_00507a28 = DAT_00509340;
                iVar3 = 1;
                iVar11 = DAT_00509340 * 0x4c;
                (&DAT_004fcae8)[DAT_00509340 * 0x13] = 0;
                (&DAT_004fcaec)[iVar12 * 0x13] = iVar10;
                *(undefined4 *)(&DAT_004fcaf0 + iVar11) = 0;
                *(undefined4 *)(&DAT_004fcaf4 + iVar11) = 0;
                *(undefined4 *)(&DAT_004fcaf8 + iVar11) = 9;
                uVar4 = FUN_0042ffc4();
                uVar4 = uVar4 & 0x80000001;
                if ((int)uVar4 < 0) {
                  uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
                }
                PlayInputInterruptibleParkDesignerVoice(uVar4 + 0x113,iVar3);
                iVar10 = DAT_004fbd30;
                iVar12 = DAT_00509340 + 1;
                DAT_00507e38 = DAT_00507b58;
                DAT_005079fc = -1;
                DAT_00509340 = iVar12;
                (&DAT_004fcacc)[iVar12 * 0x13] = 0x65;
                *(undefined4 *)(&DAT_004fcad0 + iVar12 * 0x4c) = 0;
                (&DAT_004fcab4)[iVar12 * 0x13] = iVar10;
                (&DAT_004fcab0)[iVar12 * 0x13] = DAT_004fbd24;
                RefreshParkDesignerObjectGeometry(iVar12);
                iVar11 = DAT_00507b5c;
                DAT_00507b14 = 0;
                iVar10 = DAT_00507b5c * 4;
                (&DAT_004fcae0)[DAT_00509340 * 0x13] = 200;
                iVar12 = DAT_00509340 * 0x13;
                DAT_00509340 = DAT_00509340 + 1;
                (&DAT_004fcac0)[iVar12] =
                     *(undefined4 *)
                      (&DAT_00507fb8 +
                      (DAT_00507b58 + ((&DAT_00507ae4)[iVar11] + iVar10) * 5) * 0x30);
                SetCursorSurface((int *)0x0);
                iVar12 = DAT_00507b5c;
                DAT_00441dd0 = DAT_004fbd24;
                iVar10 = (&DAT_00507ae4)[DAT_00507b5c];
                DAT_00509348 = -0x3d;
                (&DAT_00507ae4)[DAT_00507b5c] = iVar10 + 1;
                (&DAT_00508bf4)[iVar12] = iVar10 + 1;
                DAT_00441dd4 = DAT_004fbd30;
              }
            }
          }
          else if (((DAT_00507ae4 == 1) || (DAT_00507ae4 == 2)) && (DAT_004fbe68 == DAT_00509338)) {
            aiStack_f4[0] = 200;
            aiStack_f4[1] = 0xa0;
            aiStack_f4[2] = 0xd4;
            aiStack_f4[3] = 0xe6;
            aiStack_f4[4] = 0xa6;
            aiStack_c8[0] = 0x6e;
            aiStack_c8[1] = 0x68;
            aiStack_c8[2] = 0x80;
            aiStack_c8[3] = 0x82;
            aiStack_c8[4] = 0x73;
            if (DAT_00507ae4 == 1) {
              local_110[0] = DAT_00441dd0 + -0x40;
              local_110[1] = DAT_00441dd4 + -0x80;
              local_110[2] = aiStack_f4[DAT_00507e38] + -0x40 + DAT_00441dd0;
              local_110[3] = aiStack_c8[DAT_00507e38] + -0x80 + DAT_00441dd4;
              uVar4 = IsDinoCursorInsideRect(local_110);
              if ((uVar4 & 0xff) == 0) {
LAB_0040fc95:
                iVar12 = DAT_00507b5c;
                if (DAT_00507b5c == 0) {
                  if (DAT_00507ae4 == 1) {
                    PlayInputInterruptibleParkDesignerVoice(0x118,99);
                    iVar12 = DAT_00507b5c;
                  }
                  else if (DAT_00507ae4 == 2) {
                    PlayInputInterruptibleParkDesignerVoice(0x11f,99);
                    iVar12 = DAT_00507b5c;
                  }
                }
                goto LAB_00410abc;
              }
            }
            else {
              iVar10 = *(int *)(&DAT_004418a8 + DAT_00507e38 * 4);
              iVar12 = 0;
              local_110[5] = iVar10;
              if (0 < iVar10) {
                piVar5 = (int *)(&DAT_004418c4 + DAT_00507e38 * 0x78);
                piVar2 = aiStack_c8 + 0xb;
                iVar11 = iVar10;
                do {
                  piVar2[-1] = piVar5[-1] + DAT_00441dd0;
                  iVar11 = iVar11 + -1;
                  *piVar2 = DAT_00441dd4 + *piVar5;
                  piVar5 = piVar5 + 2;
                  piVar2 = piVar2 + 2;
                } while (iVar11 != 0);
              }
              do {
                if (DAT_00507b14 == 2) {
                  iVar12 = (&DAT_004fcacc)[DAT_00507b58 * 0x13];
                }
                else {
                  iVar12 = (&DAT_00507fa4)
                           [(DAT_00507b58 + ((&DAT_00507ae4)[iVar12] + iVar12 * 4) * 5) * 0xc];
                }
                if (99 < iVar12) {
                  iVar12 = iVar12 + -0x5c;
                }
                if ((int)uVar4 < 2) {
                  iVar11 = *(int *)(&DAT_00441c58 + iVar12 * 0x10);
                }
                else {
                  iVar11 = *(int *)(&DAT_00441c60 + iVar12 * 0x10);
                }
                uVar8 = uVar4 & 0x80000001;
                bVar17 = uVar8 == 0;
                if ((int)uVar8 < 0) {
                  bVar17 = (uVar8 - 1 | 0xfffffffe) == 0xffffffff;
                }
                if (bVar17) {
                  iVar12 = *(int *)(&DAT_00441c5c + iVar12 * 0x10);
                }
                else {
                  iVar12 = *(int *)(&DAT_00441c64 + iVar12 * 0x10);
                }
                bVar17 = PointInPolygon(aiStack_c8 + 10,iVar10,DAT_004fbd24 + iVar11,
                                        DAT_004fbd30 + iVar12);
                if (CONCAT31(extraout_var,bVar17) == 1) goto LAB_0040fc95;
                uVar4 = uVar4 + 1;
                iVar12 = DAT_00507b5c;
              } while ((int)uVar4 < 4);
            }
            iVar12 = DAT_00507b5c;
            iVar10 = DAT_00507b58;
            iVar11 = DAT_00507b58;
            if (DAT_00507b14 != 2) {
              if (0x57 < DAT_00509340) {
                PlayManagedSoundById(DAT_0044ddd8,0xf1,0x32,1);
                return;
              }
              iVar11 = DAT_00509340;
              DAT_00509340 = DAT_00509340 + 1;
            }
            if ((DAT_00507b5c == 0) && (DAT_00507ae4 == 1)) {
              ClearPondPrimaryObjectsBySelector(1);
              ClearPondPrimaryObjectsBySelector(2);
              iVar10 = DAT_00507a28;
              iVar13 = iVar11 * 0x4c;
              (&DAT_004fcab0)[iVar11 * 0x13] =
                   *(int *)(&DAT_0044186c + (&DAT_004fcaec)[DAT_00507a28 * 0x13] * 8) + -0x3e +
                   (&DAT_004fcab0)[DAT_00507a28 * 0x13];
              iVar12 = DAT_00507b5c;
              (&DAT_004fcab4)[iVar11 * 0x13] =
                   *(int *)(&DAT_00441870 + (&DAT_004fcaec)[iVar10 * 0x13] * 8) + -0x6a +
                   (&DAT_004fcab4)[iVar10 * 0x13];
              (&DAT_004fcacc)[iVar11 * 0x13] =
                   (&DAT_00507fa4)
                   [(DAT_00507b58 + ((&DAT_00507ae4)[iVar12] + iVar12 * 4) * 5) * 0xc];
              *(undefined4 *)(&DAT_004fcad0 + iVar13) = 0;
              RefreshParkDesignerObjectGeometry(iVar11);
              iVar3 = DAT_00507b5c;
              iVar12 = DAT_00507b58;
              (&DAT_004fcae8)[iVar11 * 0x13] = 1;
              (&DAT_004fcaec)[iVar11 * 0x13] = iVar12;
              *(undefined4 *)(&DAT_004fcaf0 + iVar13) = 0;
              *(undefined4 *)(&DAT_004fcaf4 + iVar13) = 0;
              iVar10 = (&DAT_00507ae4)[iVar3];
              *(undefined4 *)(&DAT_004fcaf8 + iVar13) = 9;
              iVar13 = 1;
              DAT_005079fc = iVar11;
              (&DAT_004fcac0)[iVar11 * 0x13] =
                   *(undefined4 *)(&DAT_00507fb8 + (iVar12 + (iVar10 + iVar3 * 4) * 5) * 0x30);
              uVar4 = FUN_0042ffc4();
              uVar4 = uVar4 & 0x80000001;
              if ((int)uVar4 < 0) {
                uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
              }
              PlayInputInterruptibleParkDesignerVoice(uVar4 + 0x116,iVar13);
              DAT_00507b14 = 0;
              SetCursorSurface((int *)0x0);
              iVar12 = DAT_00507b5c;
              if ((&DAT_00507ae4)[DAT_00507b5c] < 2) {
                iVar10 = (&DAT_00507ae4)[DAT_00507b5c] + 1;
                DAT_00509348 = -0x3d;
                (&DAT_00507ae4)[DAT_00507b5c] = iVar10;
                (&DAT_00508bf4)[iVar12] = iVar10;
              }
              *(undefined4 *)(&DAT_004fcaf0 + DAT_00507a28 * 0x4c) = 0;
            }
            else {
              bVar17 = DAT_00507b14 != 2;
              (&DAT_004fcab0)[iVar11 * 0x13] = DAT_004fbd24;
              (&DAT_004fcab4)[iVar11 * 0x13] = DAT_004fbd30;
              if (bVar17) {
                (&DAT_004fcacc)[iVar11 * 0x13] =
                     (&DAT_00507fa4)[(iVar10 + ((&DAT_00507ae4)[iVar12] + iVar12 * 4) * 5) * 0xc];
                (&DAT_004fcae8)[iVar11 * 0x13] = 2;
                (&DAT_004fcaec)[iVar11 * 0x13] = iVar10;
              }
              *(undefined4 *)(&DAT_004fcad0 + iVar11 * 0x4c) = 0;
              (&DAT_004fcac0)[iVar11 * 0x13] =
                   *(undefined4 *)
                    (&DAT_00507fb8 + (iVar10 + ((&DAT_00507ae4)[iVar12] + iVar12 * 4) * 5) * 0x30);
              RefreshParkDesignerObjectGeometry(iVar11);
              DAT_00507b14 = 0;
              SetCursorSurface((int *)0x0);
              iVar10 = 1;
              uVar4 = FUN_0042ffc4();
              PlayInputInterruptibleParkDesignerVoice((int)uVar4 % 3 + 0x119,iVar10);
              iVar12 = DAT_00507b5c;
              if ((&DAT_00507ae4)[DAT_00507b5c] < 2) {
                iVar10 = (&DAT_00507ae4)[DAT_00507b5c] + 1;
                (&DAT_00507ae4)[DAT_00507b5c] = iVar10;
                (&DAT_00508bf4)[iVar12] = iVar10;
              }
            }
          }
        }
        else if (iVar12 == 1) {
          if (DAT_00507b14 == 2) {
            iVar3 = (&DAT_004fcaec)[DAT_00507b58 * 0x13];
            iVar13 = (&DAT_004fcae8)[DAT_00507b58 * 0x13];
            iVar7 = DAT_00507b58;
          }
          else {
            iVar3 = DAT_00507b58;
            iVar13 = DAT_00507ae8;
            iVar7 = DAT_00441dcc;
            DAT_00441dcc = DAT_00441dcc + 1;
          }
          if (DAT_004fbe68 == DAT_00509338) {
            if (iVar13 == 0) {
              iVar12 = iVar3 * 3 + 0x3c;
              iVar13 = (&DAT_00507f98)[iVar12 * 4] - (&DAT_00507f90)[iVar12 * 4];
              local_110[5] = (&DAT_00507f9c)[iVar12 * 4] - (&DAT_00507f94)[iVar12 * 4];
              iVar14 = iVar7 * 0x4c;
              (&DAT_004fcae8)[iVar7 * 0x13] = 0;
              (&DAT_004fcaec)[iVar7 * 0x13] = iVar3;
              iVar12 = CanPlaceParkDesignerObjectWithoutOverlap
                                 (iVar10,iVar11,iVar13,local_110[5],1,-1);
              iVar10 = DAT_004fbd30;
              if ((iVar12 == 0) || (DAT_00507e3c != -1)) {
                iVar10 = 0xf1;
LAB_0041048a:
                PlayInputInterruptibleParkDesignerVoice(iVar10,99);
                iVar12 = DAT_00507b5c;
              }
              else {
                (&DAT_004fcab0)[iVar7 * 0x13] = DAT_004fbd24;
                (&DAT_004fcab4)[iVar7 * 0x13] = iVar10;
                (&DAT_004fcacc)[iVar7 * 0x13] = 300;
                *(undefined4 *)(&DAT_004fcad0 + iVar14) = 0;
                iVar12 = 1;
                DAT_00507e3c = iVar7;
                (&DAT_004fcabc)[iVar7 * 0x13] = local_110[5] + (&DAT_004fcab4)[iVar7 * 0x13];
                (&DAT_004fcab8)[iVar7 * 0x13] = (&DAT_004fcab0)[iVar7 * 0x13] + iVar13;
                (&DAT_004fcad4)[iVar7 * 0x13] = 1;
                (&DAT_004fcad8)[iVar7 * 0x13] = iVar13;
                (&DAT_004fcadc)[iVar7 * 0x13] = local_110[5];
                iVar10 = DAT_00507b5c;
                (&DAT_004fcae0)[iVar7 * 0x13] = 0xc9;
                (&DAT_004fcae4)[iVar7 * 0x13] = iVar7;
                (&DAT_004fcae8)[iVar7 * 0x13] = 0;
                (&DAT_004fcaec)[iVar7 * 0x13] = iVar3;
                *(undefined4 *)(&DAT_004fcaf0 + iVar14) = 0;
                *(undefined4 *)(&DAT_004fcaf4 + iVar14) = 0;
                *(undefined4 *)(&DAT_004fcaf8 + iVar14) = 9;
                (&DAT_004fcac0)[iVar7 * 0x13] =
                     *(undefined4 *)
                      (&DAT_00507fb8 +
                      (DAT_00507b58 + ((&DAT_00507ae4)[iVar10] + iVar10 * 4) * 5) * 0x30);
                uVar4 = FUN_0042ffc4();
                uVar4 = uVar4 & 0x80000001;
                if ((int)uVar4 < 0) {
                  uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
                }
                PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xca,iVar12);
                piVar5 = &DAT_00507a2c;
                do {
                  iVar10 = *piVar5;
                  if (iVar10 != -1) {
                    *piVar5 = -1;
                    (&DAT_004fcacc)[iVar10 * 0x13] = 0xffffffff;
                  }
                  iVar10 = DAT_00507c54;
                  piVar5 = piVar5 + 1;
                } while ((int)piVar5 < 0x507a3c);
                DAT_00507a40 = 0;
                if (DAT_00507c54 != -1) {
                  DAT_00507c54 = -1;
                  (&DAT_004fcacc)[iVar10 * 0x13] = 0xffffffff;
                }
                DAT_00507b14 = 0;
                SetCursorSurface((int *)0x0);
                iVar12 = DAT_00507b5c;
                if ((&DAT_00507ae4)[DAT_00507b5c] == 0) {
                  DAT_00509348 = -0x3d;
                  (&DAT_00507ae4)[DAT_00507b5c] = 1;
                  (&DAT_00508bf4)[iVar12] = 1;
                }
              }
            }
            else if (iVar13 == 1) {
              aiStack_c8[10] = 0xf;
              aiStack_c8[0xb] = 0x20;
              piStack_98 = (int *)0x31;
              iStack_94 = 10;
              uStack_90 = 0x5e;
              uStack_8c = 0x1a;
              uStack_88 = 0x3d;
              uStack_84 = 0x32;
              uStack_80 = 0xf;
              uStack_7c = 0x20;
              uStack_78 = 0x31;
              uStack_74 = 10;
              uStack_70 = 0x5e;
              uStack_6c = 0x1a;
              uStack_68 = 0x3d;
              uStack_64 = 0x32;
              uStack_60 = 0xf;
              uStack_5c = 0x20;
              uStack_58 = 0x31;
              uStack_54 = 10;
              uStack_50 = 0x5e;
              uStack_4c = 0x1a;
              uStack_48 = 0x3d;
              uStack_44 = 0x32;
              uStack_40 = 0xf;
              uStack_3c = 0x20;
              uStack_38 = 0x31;
              uStack_34 = 10;
              uStack_30 = 0x5e;
              uStack_2c = 0x1a;
              uStack_28 = 0x3d;
              uStack_24 = 0x32;
              uStack_20 = 0xf;
              uStack_1c = 0x20;
              uStack_18 = 0x31;
              uStack_14 = 10;
              uStack_10 = 0x5e;
              uStack_c = 0x1a;
              uStack_8 = 0x3d;
              uStack_4 = 0x32;
              local_118 = 0;
              do {
                iVar10 = local_118 + (&DAT_004fcaec)[DAT_00507e3c * 0x13] * 4;
                iVar12 = (&DAT_004fcab0)[DAT_00507e3c * 0x13];
                iVar11 = aiStack_c8[iVar10 * 2 + 10];
                iVar10 = aiStack_c8[iVar10 * 2 + 0xb] + -0x40 + (&DAT_004fcab4)[DAT_00507e3c * 0x13]
                ;
                DistanceBetweenIntegerPoints(DAT_004fbd24,DAT_004fbd30,iVar11 + iVar12,iVar10);
                lVar18 = __ftol();
                if ((int)lVar18 < 0x1e) {
                  iVar13 = (&DAT_00507a2c)[local_118];
                  if (iVar13 == -1) {
                    DAT_00507a40 = DAT_00507a40 + 1;
                  }
                  else {
                    (&DAT_00507a2c)[local_118] = iVar7;
                    (&DAT_004fcacc)[iVar13 * 0x13] = 0xffffffff;
                  }
                  iVar1 = DAT_00507b5c;
                  iVar6 = iVar7 * 0x4c;
                  (&DAT_004fcab0)[iVar7 * 0x13] = iVar11 + iVar12;
                  (&DAT_004fcab4)[iVar7 * 0x13] = iVar10;
                  iVar10 = iVar1 * 4;
                  (&DAT_004fcacc)[iVar7 * 0x13] = 300;
                  *(undefined4 *)(&DAT_004fcad0 + iVar6) = 0;
                  iVar12 = iVar3 + (iVar10 + 1) * 5;
                  iVar11 = (&DAT_00507f98)[iVar12 * 0xc];
                  iVar13 = (&DAT_00507f90)[iVar12 * 0xc];
                  iVar14 = (&DAT_00507f9c)[iVar12 * 0xc];
                  iVar12 = (&DAT_00507f94)[iVar12 * 0xc];
                  (&DAT_004fcabc)[iVar7 * 0x13] = (iVar14 - iVar12) + (&DAT_004fcab4)[iVar7 * 0x13];
                  (&DAT_004fcab8)[iVar7 * 0x13] = (iVar11 - iVar13) + (&DAT_004fcab0)[iVar7 * 0x13];
                  (&DAT_004fcad4)[iVar7 * 0x13] = 1;
                  (&DAT_004fcad8)[iVar7 * 0x13] = iVar11 - iVar13;
                  (&DAT_004fcadc)[iVar7 * 0x13] = iVar14 - iVar12;
                  iVar11 = DAT_00507b58;
                  (&DAT_004fcae0)[iVar7 * 0x13] = 0xc9;
                  (&DAT_004fcae4)[iVar7 * 0x13] = iVar7;
                  (&DAT_004fcae8)[iVar7 * 0x13] = 1;
                  iVar12 = (&DAT_00507ae4)[iVar1];
                  (&DAT_004fcaec)[iVar7 * 0x13] = iVar3;
                  *(undefined4 *)(&DAT_004fcaf0 + iVar6) = 0;
                  *(undefined4 *)(&DAT_004fcaf4 + iVar6) = 0;
                  DAT_00507b14 = 0;
                  (&DAT_004fcac0)[iVar7 * 0x13] =
                       *(undefined4 *)(&DAT_00507fb8 + (iVar11 + (iVar12 + iVar10) * 5) * 0x30);
                  *(undefined4 *)(&DAT_004fcaf8 + iVar6) = 9;
                  SetCursorSurface((int *)0x0);
                  (&DAT_00507a2c)[local_118] = iVar7;
                  break;
                }
                local_118 = local_118 + 1;
              } while (local_118 < 4);
              iVar12 = DAT_00507b5c;
              if (local_118 == 4) {
                iVar10 = 0xcf;
                goto LAB_0041048a;
              }
            }
            else if (iVar13 == 2) {
              aiStack_c8[1] = 0xffffffa9;
              aiStack_c8[3] = 0xffffffa9;
              aiStack_c8[5] = 0xffffffa9;
              aiStack_c8[7] = 0xffffffa9;
              aiStack_c8[9] = 0xffffffa9;
              aiStack_c8[0] = -2;
              aiStack_c8[2] = 0xfffffffe;
              aiStack_c8[4] = 0xfffffffe;
              aiStack_c8[6] = 0xfffffffe;
              aiStack_c8[8] = 0xfffffffe;
              if (DAT_00507b14 == 2) {
                local_114 = (&DAT_004fcaec)[DAT_00507b58 * 0x13];
                iVar10 = (&DAT_004fcae8)[DAT_00507b58 * 0x13];
                iVar12 = DAT_00507b58;
              }
              else {
                local_114 = DAT_00507b58;
                iVar10 = DAT_00507ae8;
                iVar12 = DAT_00441dcc;
                DAT_00441dcc = DAT_00441dcc + 1;
              }
              iVar11 = (&DAT_004fcab0)[DAT_00507e3c * 0x13];
              iVar3 = aiStack_c8[(&DAT_004fcaec)[DAT_00507e3c * 0x13] * 2];
              iVar13 = aiStack_c8[(&DAT_004fcaec)[DAT_00507e3c * 0x13] * 2 + 1];
              iVar7 = (&DAT_004fcab4)[DAT_00507e3c * 0x13];
              DistanceBetweenIntegerPoints(DAT_004fbd24,DAT_004fbd30,iVar3 + iVar11,iVar13 + iVar7);
              lVar18 = __ftol();
              if (0x31 < (int)lVar18) {
                iVar10 = 0xd2;
                goto LAB_0041048a;
              }
              if (DAT_00507c54 != -1) {
                (&DAT_004fcacc)[DAT_00507c54 * 0x13] = 0xffffffff;
              }
              iVar14 = 1;
              DAT_00507c54 = iVar12;
              uVar4 = FUN_0042ffc4();
              uVar4 = uVar4 & 0x80000001;
              if ((int)uVar4 < 0) {
                uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
              }
              PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xd0,iVar14);
              iVar1 = DAT_00507b5c;
              iVar6 = iVar12 * 0x4c;
              (&DAT_004fcab0)[iVar12 * 0x13] = iVar3 + iVar11;
              (&DAT_004fcab4)[iVar12 * 0x13] = iVar13 + iVar7;
              iVar3 = iVar1 * 4;
              (&DAT_004fcacc)[iVar12 * 0x13] = 300;
              *(undefined4 *)(&DAT_004fcad0 + iVar6) = 0;
              iVar11 = local_114 + (iVar3 + iVar10) * 5;
              iVar13 = (&DAT_00507f98)[iVar11 * 0xc];
              iVar7 = (&DAT_00507f90)[iVar11 * 0xc];
              iVar14 = (&DAT_00507f9c)[iVar11 * 0xc];
              iVar11 = (&DAT_00507f94)[iVar11 * 0xc];
              (&DAT_004fcabc)[iVar12 * 0x13] = (iVar14 - iVar11) + (&DAT_004fcab4)[iVar12 * 0x13];
              (&DAT_004fcab8)[iVar12 * 0x13] = (iVar13 - iVar7) + (&DAT_004fcab0)[iVar12 * 0x13];
              iVar1 = (&DAT_00507ae4)[iVar1];
              (&DAT_004fcad4)[iVar12 * 0x13] = 1;
              (&DAT_004fcad8)[iVar12 * 0x13] = iVar13 - iVar7;
              (&DAT_004fcadc)[iVar12 * 0x13] = iVar14 - iVar11;
              (&DAT_004fcae0)[iVar12 * 0x13] = 0xc9;
              (&DAT_004fcae4)[iVar12 * 0x13] = iVar12;
              iVar11 = DAT_00507b58;
              (&DAT_004fcae8)[iVar12 * 0x13] = iVar10;
              (&DAT_004fcaec)[iVar12 * 0x13] = local_114;
              *(undefined4 *)(&DAT_004fcaf0 + iVar6) = 0;
              *(undefined4 *)(&DAT_004fcaf4 + iVar6) = 0;
              *(undefined4 *)(&DAT_004fcaf8 + iVar6) = 9;
              DAT_00507b14 = 0;
              (&DAT_004fcac0)[iVar12 * 0x13] =
                   *(undefined4 *)(&DAT_00507fb8 + (iVar11 + (iVar1 + iVar3) * 5) * 0x30);
              SetCursorSurface((int *)0x0);
              iVar12 = DAT_00507b5c;
            }
            if (((&DAT_00507ae4)[iVar12] == 1) && (3 < DAT_00507a40)) {
              DAT_00509348 = -0x3d;
              (&DAT_00507ae4)[iVar12] = 2;
              (&DAT_00508bf4)[iVar12] = 2;
              if (DAT_00507c54 == -1) {
                PlayInputInterruptibleParkDesignerVoice(0xcd,1);
                iVar12 = DAT_00507b5c;
              }
            }
          }
        }
        else if ((iVar12 == 2) && (DAT_004fbe68 == DAT_00509338)) {
          iVar10 = DAT_00507aec;
          iVar11 = DAT_00507b58;
          if (DAT_00507b14 == 2) {
            iVar10 = (&DAT_004fcae8)[DAT_00507b58 * 0x13];
            iVar11 = (&DAT_004fcaec)[DAT_00507b58 * 0x13];
          }
          iVar12 = (iVar11 + iVar10 * 5) * 0x10;
          piStack_f8 = (int *)(DAT_004fbd24 + *(int *)(&DAT_00441dd8 + iVar12));
          local_110[5] = DAT_004fbd30 + *(int *)(&DAT_00441ddc + iVar12);
          iVar13 = CanPlaceParkDesignerObjectWithoutOverlap
                             ((int)piStack_f8,local_110[5],
                              *(int *)(&DAT_00441de0 + iVar12) - *(int *)(&DAT_00441dd8 + iVar12),
                              *(int *)(&DAT_00441de4 + iVar12) - *(int *)(&DAT_00441ddc + iVar12),0,
                              -1);
          iVar3 = DAT_004fbd30;
          iVar12 = DAT_00507b5c;
          if (iVar13 != 0) {
            iVar12 = DAT_00507b58;
            if (DAT_00507b14 != 2) {
              if (0x18d < DAT_00441dc8) {
                PlayManagedSoundById(DAT_0044ddd8,0xf1,0x32,1);
                return;
              }
              iVar12 = DAT_00441dc8;
              DAT_00441dc8 = DAT_00441dc8 + 1;
            }
            iVar6 = iVar12 * 0x4c;
            (&DAT_004fcab0)[iVar12 * 0x13] = DAT_004fbd24;
            iVar13 = DAT_00507b5c;
            (&DAT_004fcab4)[iVar12 * 0x13] = iVar3;
            (&DAT_004fcacc)[iVar12 * 0x13] = 400;
            *(undefined4 *)(&DAT_004fcad0 + iVar6) = 0;
            iVar3 = iVar11 + (iVar10 + iVar13 * 4) * 5;
            iVar13 = (&DAT_00507f98)[iVar3 * 0xc];
            iVar7 = (&DAT_00507f90)[iVar3 * 0xc];
            iVar14 = (&DAT_00507f94)[iVar3 * 0xc];
            iVar1 = (&DAT_00507f9c)[iVar3 * 0xc];
            uVar16 = *(undefined4 *)(&DAT_00507fb8 + iVar3 * 0x30);
            (&DAT_004fcabc)[iVar12 * 0x13] = (iVar1 - iVar14) + (&DAT_004fcab4)[iVar12 * 0x13];
            (&DAT_004fcab8)[iVar12 * 0x13] = (iVar13 - iVar7) + (&DAT_004fcab0)[iVar12 * 0x13];
            (&DAT_004fcad4)[iVar12 * 0x13] = 1;
            (&DAT_004fcad8)[iVar12 * 0x13] = iVar13 - iVar7;
            (&DAT_004fcadc)[iVar12 * 0x13] = iVar1 - iVar14;
            (&DAT_004fcae0)[iVar12 * 0x13] = 0xc9;
            (&DAT_004fcae4)[iVar12 * 0x13] = iVar12;
            (&DAT_004fcae8)[iVar12 * 0x13] = iVar10;
            (&DAT_004fcaec)[iVar12 * 0x13] = iVar11;
            *(undefined4 *)(&DAT_004fcaf0 + iVar6) = 0;
            *(undefined4 *)(&DAT_004fcaf4 + iVar6) = 0;
            (&DAT_004fcac0)[iVar12 * 0x13] = uVar16;
            *(undefined4 *)(&DAT_004fcaf8 + iVar6) = 9;
            if (DAT_00507b14 == 2) {
              DAT_004fbd24 = DAT_004fbd24 +
                             (int)((&DAT_004fcab8)[DAT_00507b58 * 0x13] -
                                  (&DAT_004fcab0)[DAT_00507b58 * 0x13]) / 2;
              DAT_004fbd30 = DAT_004fbd30 +
                             (int)((&DAT_004fcabc)[DAT_00507b58 * 0x13] -
                                  (&DAT_004fcab4)[DAT_00507b58 * 0x13]) / 2;
            }
            else {
              DAT_004fbd24 = (int)((&DAT_004fcab8)[iVar12 * 0x13] - (&DAT_004fcab0)[iVar12 * 0x13])
                             / 2 + (&DAT_004fcab0)[iVar12 * 0x13];
              DAT_004fbd30 = (int)((&DAT_004fcabc)[iVar12 * 0x13] - (&DAT_004fcab4)[iVar12 * 0x13])
                             / 2 + (&DAT_004fcab4)[iVar12 * 0x13];
            }
            DAT_00507b14 = 0;
            SetCursorSurface((int *)0x0);
            ClearCursorHotspotAndClampNonnegative();
            uVar4 = FUN_0042ffc4();
            iVar12 = DAT_00507b5c;
            if ((int)uVar4 % 3 == 0) {
              if (DAT_00507aec == 0) {
                iVar10 = 99;
                uVar4 = FUN_0042ffc4();
                uVar4 = uVar4 & 0x80000001;
                if ((int)uVar4 < 0) {
                  uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
                }
                PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xdb,iVar10);
              }
              if (DAT_00507aec == 1) {
                iVar10 = 99;
                uVar4 = FUN_0042ffc4();
                uVar4 = uVar4 & 0x80000001;
                if ((int)uVar4 < 0) {
                  uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
                }
                PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xd8,iVar10);
              }
              if (DAT_00507aec == 2) {
                iVar10 = 99;
                if (iVar11 < 3) {
                  uVar4 = FUN_0042ffc4();
                  uVar4 = uVar4 & 0x80000001;
                  if ((int)uVar4 < 0) {
                    uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
                  }
                  PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xe1,iVar10);
                  iVar12 = DAT_00507b5c;
                  if (DAT_00507aec == 2) goto LAB_00410abc;
                }
                else {
                  uVar4 = FUN_0042ffc4();
                  uVar4 = uVar4 & 0x80000001;
                  if ((int)uVar4 < 0) {
                    uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
                  }
                  PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xe5,iVar10);
                }
              }
              iVar12 = DAT_00507b5c;
              if (DAT_00507aec == 3) {
                iVar10 = 99;
                uVar4 = FUN_0042ffc4();
                uVar4 = uVar4 & 0x80000001;
                if ((int)uVar4 < 0) {
                  uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
                }
                PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xde,iVar10);
                iVar12 = DAT_00507b5c;
              }
            }
          }
        }
      }
      else {
        if (DAT_004fbe68 == DAT_00509338) {
          if (DAT_00507b14 == 2) {
            if (DAT_00507b58 < 200) {
              iVar11 = (&DAT_00507ae4)[iVar12];
              iVar10 = (&DAT_004fcaec)[DAT_00507b58 * 0x13];
            }
            else {
              iVar11 = (&DAT_004fcae8)[DAT_00507b58 * 0x13];
              iVar10 = (&DAT_004fcaec)[DAT_00507b58 * 0x13];
            }
          }
          else {
            iVar11 = (&DAT_00507ae4)[iVar12];
            iVar10 = DAT_00507b58;
          }
          iVar10 = iVar10 + (iVar11 + iVar12 * 4) * 5;
          iVar11 = (&DAT_00507f98)[iVar10 * 0xc] - (&DAT_00507f90)[iVar10 * 0xc];
          iVar10 = (&DAT_00507f9c)[iVar10 * 0xc] - (&DAT_00507f94)[iVar10 * 0xc];
        }
        else {
          iVar11 = 0x2a;
          iVar10 = 0x22;
        }
        if (((DAT_004fbd24 < -300) || (DAT_004fbd30 < -300)) ||
           ((0x348 < iVar11 + DAT_004fbd24 || (400 < (iVar10 - DAT_004fbd4c / 2) + DAT_004fbd30))))
        {
          if (DAT_004fbe68 == DAT_00509338) {
            if (0x348 < iVar11 + DAT_004fbd24) {
              DAT_004fbd24 = iVar11 + DAT_004fbd24 + -0x2a;
            }
            DAT_004fbd30 = 0x17c;
          }
          SetCursorSurface(DAT_00507a00);
          ClearCursorHotspotAndClampNonnegative();
          iVar12 = DAT_00507b5c;
        }
        else {
          if (DAT_004fbe68 == DAT_00507a00) {
            if (DAT_00507b14 == 2) {
              if (DAT_00507b58 < 200) {
                iVar11 = (&DAT_00507ae4)[iVar12];
                iVar10 = (&DAT_004fcaec)[DAT_00507b58 * 0x13];
              }
              else {
                iVar11 = (&DAT_004fcae8)[DAT_00507b58 * 0x13];
                iVar10 = (&DAT_004fcaec)[DAT_00507b58 * 0x13];
              }
            }
            else {
              iVar11 = (&DAT_00507ae4)[iVar12];
              iVar10 = DAT_00507b58;
            }
            iVar10 = iVar10 + (iVar11 + iVar12 * 4) * 5;
            iVar11 = (&DAT_00507f9c)[iVar10 * 0xc] - (&DAT_00507f94)[iVar10 * 0xc];
            if ((DAT_004fbd24 + 0x2a < 0x348) && (0x348 - (DAT_004fbd24 + 0x2a) < 10)) {
              DAT_004fbd24 = DAT_004fbd24 + 0x2a +
                             ((&DAT_00507f90)[iVar10 * 0xc] - (&DAT_00507f98)[iVar10 * 0xc]);
            }
            if (DAT_004fbd30 + -0x22 < -300) {
              DAT_004fbd30 = DAT_004fbd30 + 5;
            }
            else if (DAT_004fbd30 + 0x22 < 400) {
              DAT_004fbd30 = DAT_004fbd30 + (0x22 - iVar11);
            }
            if (iVar12 == 2) {
              iVar10 = (DAT_00507b10 + DAT_005079f8 * 5) * 8;
              SetCursorHotspotOffset
                        (*(undefined4 *)(&DAT_00507ef0 + iVar10),
                         *(undefined4 *)(&DAT_00507ef4 + iVar10));
            }
            if (DAT_004fbe68 != DAT_00509338) {
              DAT_004fbd30 = (iVar11 / 3 - iVar11) + 0x168;
            }
          }
          SetCursorSurface(DAT_00509338);
          iVar12 = DAT_00507b5c;
          if ((DAT_00507b5c == 0) && (DAT_00507ae4 == 0)) {
            local_110[0] = 0xc5;
            local_110[1] = 0xa3;
            local_110[2] = 0xd6;
            local_110[3] = 0xfa;
            local_110[4] = 0xad;
            DAT_004fbd18 = local_110[DAT_00507b58];
          }
        }
      }
    }
    else if (((DAT_00507b14 == 5) && (bVar17)) && ((DAT_004fbe54 != 0 || (DAT_004fbd50 == 1)))) {
      iVar10 = FindTopmostParkDesignerObjectAtCursor(1);
      if (iVar10 == -1) {
        iVar10 = HitTestParkDesignerToolbar();
        iVar12 = DAT_00507b5c;
        if (iVar10 == 0xb) {
          DAT_00507b14 = 0;
          SetCursorSurface((int *)0x0);
          iVar12 = DAT_00507b5c;
        }
      }
      else {
        iVar12 = *(int *)((&DAT_00508c10)[iVar10] + 0x34);
        if (iVar12 < 100) {
          if ((&DAT_004fcacc)[iVar12 * 0x13] == 100) {
            ClearPondPrimaryObjectsBySelector(0);
            ClearPondPrimaryObjectsBySelector(1);
            ClearPondPrimaryObjectsBySelector(2);
            DAT_00508bf4 = 0;
            PlayInputInterruptibleParkDesignerVoice(0xf5,1);
            *(undefined4 *)((&DAT_00508c10)[iVar10] + 0x1c) = 0xffffffff;
            iVar12 = DAT_00507b5c;
          }
          else {
            iVar11 = 1;
            if ((&DAT_004fcacc)[iVar12 * 0x13] != 7) {
LAB_00410a62:
              iVar12 = 1;
              uVar4 = FUN_0042ffc4();
              goto joined_r0x00410a7c;
            }
            uVar4 = FUN_0042ffc4();
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xf2,iVar11);
            ClearPondPrimaryObjectsBySelector(1);
            ClearPondPrimaryObjectsBySelector(2);
            DAT_00508bf4 = 1;
            *(undefined4 *)((&DAT_00508c10)[iVar10] + 0x1c) = 0xffffffff;
            iVar12 = DAT_00507b5c;
          }
        }
        else {
          if ((iVar12 < 200) || (299 < iVar12)) {
            iVar12 = 1;
            uVar4 = FUN_0042ffc4();
joined_r0x00410a7c:
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            iVar11 = uVar4 + 0xf2;
          }
          else {
            iVar11 = (&DAT_004fcae8)[iVar12 * 0x13];
            if (iVar11 != 0) {
              if (iVar11 != 1) {
                DAT_00507c54 = -1;
                goto LAB_00410a62;
              }
              uVar4 = FUN_0042ffc4();
              uVar4 = uVar4 & 0x80000001;
              if ((int)uVar4 < 0) {
                uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
              }
              PlayInputInterruptibleParkDesignerVoice(uVar4 + 0xf2,iVar11);
              _DAT_00508bf8 = 1;
              ClearBandstandObjectsByCategorySelector(1);
              iVar11 = DAT_00507c54;
              if (DAT_00507c54 != -1) {
                DAT_00507c54 = -1;
                (&DAT_004fcacc)[iVar11 * 0x13] = 0xffffffff;
              }
              piVar5 = &DAT_00507a2c;
              do {
                if (*piVar5 == iVar12) {
                  *piVar5 = -1;
                  DAT_00507a40 = DAT_00507a40 + -1;
                }
                piVar5 = piVar5 + 1;
              } while ((int)piVar5 < 0x507a3c);
              *(undefined4 *)((&DAT_00508c10)[iVar10] + 0x1c) = 0xffffffff;
              iVar12 = DAT_00507b5c;
              goto LAB_00410abc;
            }
            _DAT_00508bf8 = 0;
            ClearBandstandObjectsByCategorySelector(0);
            DAT_00507e3c = -1;
            DAT_00507a40 = 0;
            piVar5 = &DAT_00507a2c;
            do {
              iVar12 = *piVar5;
              if (iVar12 != -1) {
                *piVar5 = -1;
                (&DAT_004fcacc)[iVar12 * 0x13] = 0xffffffff;
              }
              if (DAT_00507c54 != -1) {
                (&DAT_004fcacc)[DAT_00507c54 * 0x13] = 0xffffffff;
                DAT_00507c54 = -1;
              }
              piVar5 = piVar5 + 1;
            } while ((int)piVar5 < 0x507a3c);
            iVar12 = 1;
            iVar11 = 0xf4;
          }
          PlayInputInterruptibleParkDesignerVoice(iVar11,iVar12);
          *(undefined4 *)((&DAT_00508c10)[iVar10] + 0x1c) = 0xffffffff;
          iVar12 = DAT_00507b5c;
        }
      }
    }
  }
LAB_00410abc:
  piVar5 = local_d0;
  aiStack_f4[6] = iVar12 * 0x2a;
  iStack_d4 = aiStack_f4[6] + 0x2a;
  aiStack_f4[5] = 0x66;
  uStack_d8 = 0x88;
  (**(code **)(*local_d0 + 0x1c))
            (local_d0,(&DAT_00441f1c)[iVar12 * 2],(&DAT_00441f20)[iVar12 * 2],
             (&DAT_00504194)[iVar12 * 2],0,1);
  if (2 < DAT_00507b5c) goto LAB_00410c71;
  if (DAT_00509348 != 0) {
    if (DAT_00509348 < 0) {
      DAT_00509348 = DAT_00509348 + 5;
      if (0 < DAT_00509348) {
LAB_00410b3a:
        DAT_00509348 = 0;
      }
    }
    else {
      DAT_00509348 = DAT_00509348 + -5;
      if (DAT_00509348 < 0) goto LAB_00410b3a;
    }
  }
  aiStack_f4[0] = (&DAT_00507ae4)[DAT_00507b5c] * 0x3d + DAT_00509348;
  aiStack_f4[2] = ((&DAT_00507ae4)[DAT_00507b5c] + 1) * 0x3d + DAT_00509348;
  piStack_f8 = (int *)0x0;
  aiStack_f4[1] = 0xaa;
  (**(code **)(*piVar5 + 0x1c))(piVar5,0xeb,400,(&DAT_00507a14)[DAT_00507b5c],0,1);
  (**(code **)(*piVar5 + 0x1c))(piVar5,0xf5,0x191,(&DAT_00507b30)[DAT_00507b5c],local_110,1);
  if (((&DAT_00507ae4)[DAT_00507b5c] < 1) ||
     (((DAT_00507b5c == 0 || (DAT_00507b5c == 1)) && ((&DAT_00507ae4)[DAT_00507b5c] == 1)))) {
    DAT_00507a20 = 0;
  }
  else {
    aiStack_f4[0] = 0;
    aiStack_f4[2] = 0x19;
    piStack_f8 = (int *)0x0;
    aiStack_f4[1] = 0x30;
    (**(code **)(*piVar5 + 0x1c))(piVar5,0x1a9,0x192,DAT_00507b18,&piStack_f8,1);
    DAT_00507a20 = 1;
  }
  if ((&DAT_00507ae4)[DAT_00507b5c] < (int)(&DAT_00508bf4)[DAT_00507b5c]) {
    aiStack_f4[0] = 0x19;
    aiStack_f4[2] = 0x32;
    piStack_f8 = (int *)0x0;
    aiStack_f4[1] = 0x30;
    (**(code **)(*piVar5 + 0x1c))(piVar5,0x1a9,0x1ac,DAT_00507b18,&piStack_f8,1);
    DAT_00507a24 = 1;
  }
  else {
    DAT_00507a24 = 0;
  }
LAB_00410c71:
  iVar10 = DAT_00507b5c;
  if ((int)(&DAT_00508bf4)[DAT_00507b5c] < (&DAT_00507ae4)[DAT_00507b5c]) {
    (&DAT_00507ae4)[DAT_00507b5c] = (&DAT_00508bf4)[DAT_00507b5c];
  }
  if (DAT_00441f18 != 1) {
    return;
  }
  DAT_00441f18 = 0;
  if (iVar10 == 0) {
    if (DAT_00507ae4 != 0) {
      DAT_00441f18 = 0;
      return;
    }
    if (DAT_00507a28 != -1) {
      DAT_00441f18 = 0;
      return;
    }
    PlayInputInterruptibleParkDesignerVoice(0x112,0);
    iVar10 = DAT_00507b5c;
  }
  if (((iVar10 == 1) && (DAT_00507ae8 == 0)) && (DAT_00507e3c == -1)) {
    PlayInputInterruptibleParkDesignerVoice(0xc9,0);
    return;
  }
  DAT_00441f18 = 0;
  return;
}

