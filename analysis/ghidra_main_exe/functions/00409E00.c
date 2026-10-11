/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409e00; function: UpdateDinoActivity; body bytes: 1590
 * callers: 1; callees: 16; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 UpdateDinoActivity(void)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  char *pcVar8;
  CHAR *pCVar9;
  char *pcVar10;
  undefined4 *puVar11;
  int *piVar12;
  int local_31c;
  int local_318;
  int local_314;
  int local_310;
  CHAR local_30c [25];
  undefined4 local_2f3;
  char local_208 [25];
  undefined4 local_1ef;
  undefined4 local_104 [6];
  undefined4 local_ea [58];
  
  EnableInputProcessing();
  iVar7 = 0;
  piVar12 = *(int **)(DAT_0044de08 + 4);
  if ((DAT_0044ddb0 != 0) || (DAT_0051c2e8 != 0)) {
    UnloadDinoActivityResources();
    if (DAT_0051c2e8 != 0) {
      DAT_0051c2e8 = 0;
      DAT_0044de14 = 0x10;
      if (DAT_00446f38 != -1) {
        DAT_0044de14 = 0x40;
      }
    }
    return 1;
  }
  if (DAT_004fbe68 != DAT_004fbf88) {
    if (0x244 < DAT_004fbd24) {
      DAT_004fbd24 = 0x244;
    }
    if (400 < DAT_004fbd30) {
      DAT_004fbd30 = 400;
    }
  }
  if (DAT_0043ee8c <= DAT_004fc440) {
    iVar3 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if ((iVar3 == 0) || (DAT_004fca84 == 1)) {
      if (DAT_004fca84 == 10) {
        PreparePlayAgainTransition();
        DAT_00446ce8 = 0x10;
        DAT_0044de14 = 0x3c;
        DAT_0051c2fc = 1;
        DAT_0051b418 = 0x14;
        UnloadDinoActivityResources();
        iVar7 = DAT_004fc438 / 3 + DAT_00519934 * 100;
        if (*(int *)(&DAT_0051b5b0 + iVar7 * 4) == 0) {
          *(undefined4 *)(&DAT_0051b5b0 + iVar7 * 4) = 1;
        }
        return 1;
      }
      if (DAT_004fca84 == 0) {
        iVar7 = DAT_004fc438 / 3;
        StopAllManagedSounds(DAT_0044ddd8);
        PlayManagedSoundById(DAT_0044ddd8,DAT_004fc43c + 0xb9,0x32,1);
        iVar7 = iVar7 + DAT_00519934 * 100;
        if (*(int *)(&DAT_0051b5b0 + iVar7 * 4) == 0) {
          *(undefined4 *)(&DAT_0051b5b0 + iVar7 * 4) = 1;
        }
        pcVar8 = s_data_subgamedino_vel_bmp_0043f838;
        pCVar9 = local_30c;
        for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
          *(undefined4 *)pCVar9 = *(undefined4 *)pcVar8;
          pcVar8 = pcVar8 + 4;
          pCVar9 = pCVar9 + 4;
        }
        *pCVar9 = *pcVar8;
        puVar11 = &local_2f3;
        for (iVar7 = 0x3a; iVar7 != 0; iVar7 = iVar7 + -1) {
          *puVar11 = 0;
          puVar11 = puVar11 + 1;
        }
        *(undefined2 *)puVar11 = 0;
        *(undefined1 *)((int)puVar11 + 2) = 0;
        pcVar8 = s_data_subgamedino_tri_bmp_0043f81c;
        pcVar10 = local_208;
        for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
          *(undefined4 *)pcVar10 = *(undefined4 *)pcVar8;
          pcVar8 = pcVar8 + 4;
          pcVar10 = pcVar10 + 4;
        }
        *pcVar10 = *pcVar8;
        puVar11 = &local_1ef;
        for (iVar7 = 0x3a; iVar7 != 0; iVar7 = iVar7 + -1) {
          *puVar11 = 0;
          puVar11 = puVar11 + 1;
        }
        *(undefined2 *)puVar11 = 0;
        *(undefined1 *)((int)puVar11 + 2) = 0;
        pcVar8 = s_data_subgamedino_trex_bmp_0043f800;
        puVar11 = local_104;
        for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
          *puVar11 = *(undefined4 *)pcVar8;
          pcVar8 = pcVar8 + 4;
          puVar11 = puVar11 + 1;
        }
        *(undefined2 *)puVar11 = *(undefined2 *)pcVar8;
        puVar11 = local_ea;
        for (iVar7 = 0x3a; iVar7 != 0; iVar7 = iVar7 + -1) {
          *puVar11 = 0;
          puVar11 = puVar11 + 1;
        }
        *(undefined2 *)puVar11 = 0;
        DAT_00446f38 = 1;
        DAT_004fc2a8 = LoadBitmapToDirectDrawSurface(piVar12,local_30c + DAT_004fc43c * 0x104,0,0);
        RegisterBitmapSurface(&DAT_004fc2a8,local_30c + DAT_004fc43c * 0x104);
        DAT_004fca84 = DAT_004fca84 + 1;
      }
      UpdateDinoPrintButton();
      return 1;
    }
    DrawDinoActivity();
  }
  iVar3 = DAT_0043ee90;
  iVar2 = _DAT_004fc3f4;
  if (DAT_004fc3f0 == 0) {
    if ((DAT_004fbe54 != 0) && (iVar7 = 0, 0 < DAT_0043ee8c)) {
      piVar12 = &DAT_004fc454;
      do {
        if (piVar12[1] == 0) {
          local_318 = *piVar12;
          local_31c = piVar12[-1];
          local_310 = local_318 + piVar12[5];
          local_314 = piVar12[4] + local_31c;
          uVar5 = IsDinoCursorInsideRect(&local_31c);
          if ((char)uVar5 != '\0') {
            DAT_004fc3f0 = 1;
            SetCursorSurface((int *)(&DAT_004fc474)[iVar7 * 0xc]);
            DAT_004fbd24 = (&DAT_004fc450)[iVar7 * 0xc];
            DAT_004fbd30 = (&DAT_004fc454)[iVar7 * 0xc];
            (&DAT_004fc458)[iVar7 * 0xc] = 1;
            DAT_0043ee90 = iVar7;
            (&DAT_004fc46c)[iVar7 * 0xc] = 0;
            uVar4 = FUN_0042ffc4();
            uVar4 = uVar4 & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            uVar6 = FUN_0042ffc4();
            uVar6 = uVar6 & 0x80000001;
            if ((int)uVar6 < 0) {
              uVar6 = (uVar6 - 1 | 0xfffffffe) + 1;
            }
            (&DAT_0043f7f8)[uVar6] = uVar4 + 1;
            iVar2 = _DAT_004fc3f4;
            break;
          }
        }
        iVar7 = iVar7 + 1;
        piVar12 = piVar12 + 0xc;
        iVar2 = _DAT_004fc3f4;
      } while (iVar7 < DAT_0043ee8c);
    }
  }
  else if (DAT_004fc3f0 == 1) {
    if (DAT_004fbe54 != 0) {
      if (0 < DAT_0043ee8c) {
        piVar12 = &DAT_004fc44c;
        do {
          if ((((&DAT_004fc458)[iVar3 * 0xc] != 4) &&
              (uVar4 = IsDinoCursorNearPoint(piVar12[-1],*piVar12,DAT_004fc3f8),
              iVar3 = DAT_0043ee90, (char)uVar4 != '\0')) && (iVar7 == DAT_0043ee90)) {
            DAT_004fc3f0 = 2;
            DAT_004fc418 = 1;
            SetCursorSurface((int *)0x0);
            iVar2 = iVar7;
            if (iVar7 == DAT_0043ee90) {
              uVar4 = FUN_0042ffc4();
              iVar7 = DAT_0043ee90;
              DAT_004fca64 = (int)uVar4 % 6;
              (&DAT_0043f7fc)[-(DAT_004fca64 / 3)] = 5;
              pvVar1 = DAT_0044ddd8;
              (&DAT_004fc458)[iVar7 * 0xc] = 2;
              StopAllManagedSounds(pvVar1);
              PlayManagedSoundById(DAT_0044ddd8,DAT_004fca64 + 0xa4,0x32,1);
            }
            goto LAB_0040a360;
          }
          iVar7 = iVar7 + 1;
          piVar12 = piVar12 + 0xc;
        } while (iVar7 < DAT_0043ee8c);
      }
      (&DAT_004fc458)[iVar3 * 0xc] = 3;
      uVar4 = FUN_0042ffc4();
      DAT_004fca68 = (int)uVar4 % 6;
      PlayManagedSoundById(DAT_0044ddd8,DAT_004fca68 + 0xaa,0x32,1);
      iVar7 = 1 - DAT_004fca68 / 3;
      iVar2 = _DAT_004fc3f4;
      if ((&DAT_004fca7c)[iVar7] == 0) {
        (&DAT_0043f7f8)[iVar7] = 5;
        uVar4 = FUN_0042ffc4();
        uVar4 = uVar4 & 0x80000001;
        if ((int)uVar4 < 0) {
          uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
        }
        (&DAT_0043f7f8)[iVar7] = uVar4 + 3;
        iVar2 = _DAT_004fc3f4;
      }
    }
  }
  else if (DAT_004fc3f0 == 2) {
    if ((&DAT_004fc458)[DAT_0043ee90 * 0xc] == 2) {
      (&DAT_004fc458)[DAT_0043ee90 * 0xc] = 4;
      (&DAT_004fc46c)[iVar3 * 0xc] = 1;
      DAT_004fc418 = DAT_004fc418 + 2;
      DAT_004fc440 = DAT_004fc440 + 1;
      FUN_0042ffc4();
    }
    else {
      (&DAT_004fc458)[DAT_0043ee90 * 0xc] = 0;
      (&DAT_004fc46c)[DAT_0043ee90 * 0xc] = 1;
    }
    DAT_004fc418 = 0;
    DAT_0043ee90 = -1;
    DAT_004fc3f0 = 0;
    _DAT_004fc41c = 0;
    DAT_004fc424 = 4;
    iVar2 = _DAT_004fc3f4;
  }
LAB_0040a360:
  _DAT_004fc3f4 = iVar2;
  DrawDinoActivity();
  if (DAT_0051c30c == 1) {
    CSound_Stop(DAT_0051c2b8);
    PlayActivityMusicByIndex(1);
    DAT_0051c30c = 0;
    PlayManagedSoundById(DAT_0044ddd8,DAT_004fc438 / 3 + 0xbc,0x5a,1);
    *(undefined4 *)
     ((int)DAT_0044ddd8 + *(char *)(DAT_004fc438 / 3 + 0x33c + (int)DAT_0044ddd8) * 4 + 0xd98) = 1;
  }
  return 1;
}

