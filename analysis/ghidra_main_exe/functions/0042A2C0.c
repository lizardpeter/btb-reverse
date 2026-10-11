/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042a2c0; function: RunMainGameFlow; body bytes: 10923
 * callers: 2; callees: 65; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl RunMainGameFlow(HGLOBAL param_1)

{
  char cVar1;
  undefined4 uVar2;
  bool bVar3;
  int iVar4;
  undefined3 extraout_var;
  int *piVar5;
  char *pcVar6;
  
  piVar5 = *(int **)(DAT_0044de08 + 4);
  uVar2 = *(undefined4 *)(DAT_0044de08 + 0xc);
  DAT_0051bca8 = 0;
  if (DAT_0044ddb0 != 0) {
    UnloadGenericUIScreenResources();
    if ((DAT_0051c268 != (int *)0x0) &&
       (UnregisterBitmapSurface(0x51c268), DAT_0051c268 != (int *)0x0)) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    if (DAT_00519940 != (code *)0x0) {
      (*DAT_00519940)();
      return;
    }
    return;
  }
  if (DAT_0051c2bc != 0) {
    PauseGlobalBinkMovie();
    UpdateOptionsOverlay();
    return;
  }
  if (DAT_0051c324 != 0) {
    UpdateProgressScreen();
    return;
  }
  if (DAT_0051c2d0 != 0) {
    PauseGlobalBinkMovie();
    UpdateYesNoConfirmationOverlay();
    return;
  }
  if (DAT_0051c2c0 != 0) {
    PauseGlobalBinkMovie();
    UpdateQuitConfirmationOverlay();
    return;
  }
  if (DAT_0051c2c8 != 0) {
    PauseGlobalBinkMovie();
    UpdateLeaveActivityConfirmation();
    return;
  }
  if (DAT_0051c2cc != 0) {
    PauseGlobalBinkMovie();
    UpdatePlayAgainPrompt();
    return;
  }
  if (0 < DAT_0051c2d8) {
    if (DAT_0051c2d8 < 10) {
      NoOpLegacyHook();
      return;
    }
LAB_0042a406:
    NoOpLegacyHook();
    return;
  }
  if (9 < DAT_0051c2d8) goto LAB_0042a406;
  if (DAT_0051c27c == 0xd) {
    iVar4 = UpdateGlobalBinkMovie();
    if (iVar4 == 0) {
      return;
    }
    DAT_0044de14 = DAT_0051b418;
    DAT_0051c27c = 2;
    FUN_00408430();
  }
  if (DAT_0051c27c == 0xe) {
    iVar4 = UpdateGlobalBinkMovie();
    if (iVar4 == 0) {
      return;
    }
    DAT_0051c27c = (-(uint)(DAT_0044de14 != 0x22) & 0xfffffffb) + 7;
    DAT_0044de14 = DAT_0044de14 + 1;
    FUN_00408430();
  }
  switch(DAT_0044de14) {
  case 0:
    iVar4 = UpdateStartupVideoSequence();
    if (iVar4 != 0) {
      DAT_0044de14 = DAT_0044de14 + 1;
      DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,s_Data_ui_name_signs_bmp_00447138,0,0);
      RegisterBitmapSurface(&DAT_0051c268,s_Data_ui_name_signs_bmp_00447138);
      pcVar6 = s_Data_music_entername_wav_0044711c;
      goto LAB_0042a51a;
    }
    break;
  case 1:
    if (2 < DAT_004fc074) {
      DAT_00446fdc = 2;
    }
    if (DAT_0051c2f4 != 0) {
      if (DAT_0051c2f4 == 1) {
        DAT_0051c310 = 1;
        UnregisterBitmapSurface(0x51c268);
        if (DAT_0051c268 != (int *)0x0) {
          (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
          DAT_0051c268 = (int *)0x0;
        }
        DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,s_Data_ui_enter_name_bmp_00447104,0,0);
        RegisterBitmapSurface(&DAT_0051c268,s_Data_ui_enter_name_bmp_00447104);
        DAT_0051c2f4 = 2;
        return;
      }
      if (DAT_0051c2f4 == 2) {
        DrawEnterNamePopup();
        UpdateEnterNamePopup();
        UpdateHelpButtonController();
        return;
      }
      return;
    }
    UpdatePlayerProfileScreen();
    DAT_0051c310 = 0;
    PlayManagedSoundById(DAT_0044ddd8,0x12e,0x5a,1);
    *(undefined1 *)((int)DAT_0044ddd8 + 0x7fa) = 0;
    *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x3ae) * 4 + 0xd98) = 1;
    if (*(char *)((int)DAT_0044ddd8 + 0x7fb) == '\0') {
      return;
    }
    bVar3 = IsSoundIdPlaying(DAT_0044ddd8,0x12e);
    if (CONCAT31(extraout_var,bVar3) != 0) {
      return;
    }
    iVar4 = 0;
    piVar5 = &DAT_0051b404;
    do {
      if (0 < *piVar5) {
        iVar4 = iVar4 + 1;
      }
      piVar5 = piVar5 + 1;
    } while ((int)piVar5 < 0x51b418);
    if (iVar4 == 0) {
      PlayManagedSoundById(DAT_0044ddd8,0x12f,0x5a,1);
      cVar1 = *(char *)((int)DAT_0044ddd8 + 0x3af);
    }
    else {
      if (iVar4 < 5) {
        PlayManagedSoundById(DAT_0044ddd8,0x130,0x5a,1);
        *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x3b0) * 4 + 0xd98) = 1;
        goto LAB_0042a654;
      }
      if (iVar4 != 5) goto LAB_0042a654;
      PlayManagedSoundById(DAT_0044ddd8,0x134,0x5a,1);
      cVar1 = *(char *)((int)DAT_0044ddd8 + 0x3b4);
    }
    *(undefined4 *)((int)DAT_0044ddd8 + cVar1 * 4 + 0xd98) = 1;
LAB_0042a654:
    *(undefined1 *)((int)DAT_0044ddd8 + 0x7fb) = 0;
    *(undefined1 *)((int)DAT_0044ddd8 + 0x7fc) = 0;
    *(undefined1 *)((int)DAT_0044ddd8 + 0x800) = 0;
    return;
  case 2:
    DAT_0051c310 = 0;
    DAT_0044de14 = 4;
    return;
  case 3:
    NoOpLegacyHook();
    DrawGenericUIScreen();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 == -6) {
      DAT_0044de14 = 3;
      NoOpLegacyHook();
      return;
    }
    if (DAT_0044de14 == -1) {
      UnloadGenericUIScreenResources();
      UnregisterBitmapSurface(0x51c268);
      if (DAT_0051c268 != (int *)0x0) {
        (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
        DAT_0051c268 = (int *)0x0;
      }
      DAT_0044de14 = 1;
      DAT_0051c2f4 = 0;
      DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,s_Data_ui_name_signs_bmp_00447138,0,0);
      RegisterBitmapSurface(&DAT_0051c268,s_Data_ui_name_signs_bmp_00447138);
      return;
    }
    break;
  case 4:
    if (DAT_00446f38 != -1) {
      DAT_0044de14 = 0x40;
      return;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 1;
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00490970,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00490970);
    LoadGenericUIScreenResources();
    _DAT_0051c264 = 1;
    if (DAT_0051c308 == 0) {
      DAT_0051c304 = 1;
    }
    pcVar6 = s_Data_music_actselectintro_wav_004470e4;
LAB_0042a51a:
    CSoundManager_Create
              (DAT_004fc174,&DAT_0051c2b4,pcVar6,0,DAT_0043b5a8,DAT_0043b5ac,DAT_0043b5b0,
               DAT_0043b5b4,1);
    CSound_Play(DAT_0051c2b4,0,0);
    return;
  case 5:
    NoOpLegacyHook();
    DrawGenericUIScreen();
    UpdateGenericUIScreenInteraction();
    if (((DAT_0044de14 != 5) && (DAT_0051c2b4 != (undefined4 *)0x0)) &&
       (CSound_Stop((int)DAT_0051c2b4), DAT_0051c2b4 != (undefined4 *)0x0)) {
      (**(code **)*DAT_0051c2b4)(1);
      DAT_0051c2b4 = (undefined4 *)0x0;
    }
    UpdateHelpButtonController();
    if (DAT_0044de14 < 0) {
      if (DAT_0044de14 == -6) {
        DAT_0044de14 = 5;
        NoOpLegacyHook();
        return;
      }
      if (DAT_0044de14 == -1) {
        UnloadGenericUIScreenResources();
        UnregisterBitmapSurface(0x51c268);
        if (DAT_0051c268 != (int *)0x0) {
          (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
          DAT_0051c268 = (int *)0x0;
        }
        DAT_0044de14 = 1;
        DAT_0051c2f4 = 0;
        DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,s_Data_ui_name_signs_bmp_00447138,0,0);
        RegisterBitmapSurface(&DAT_0051c268,s_Data_ui_name_signs_bmp_00447138);
        DAT_0051c304 = 0;
        return;
      }
    }
    break;
  case 6:
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    return;
  case 8:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,uVar2,0,0);
    return;
  case 9:
    NoOpLegacyHook();
    DAT_0051c2c0 = 1;
    DAT_0044de14 = 4;
    return;
  case 0xc:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 2;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00490b70,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00490b70);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + -1;
    DAT_0051c27c = 0xe;
    OpenGlobalBinkMovie(&DAT_00493970);
    iVar4 = 0;
    DAT_0051c284 = DAT_0051c340;
    goto LAB_0042aab0;
  case 0xd:
    NoOpLegacyHook();
    if (DAT_0051c318 == 1) {
      DAT_00446fd8 = 0x1f5;
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1f5,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x475) * 4 + 0xd98) = 1;
    }
    UpdateHelpButtonController();
    DrawGenericUIScreen();
    UpdateWalkthroughMovie(param_1);
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0xd;
        return;
      case -5:
        DAT_0044de14 = 0xe;
        return;
      case -4:
        DAT_0044de14 = 0xd;
        DAT_0051c284 = 2;
        DAT_0051c340 = 2;
        return;
      case -3:
        DAT_0051c284 = 1;
        DAT_0051c340 = 1;
        DAT_0044de14 = 0xd;
        return;
      case -2:
        DAT_0044de14 = 0xd;
        DAT_0051c284 = 0;
        DAT_0051c340 = 0;
        return;
      case -1:
        goto switchD_0042c8c6_caseD_ffffffff;
      default:
        return;
      }
    }
    return;
  case 0xe:
    DAT_0051c2f8 = 0;
    DAT_0051c300 = 0;
    CloseWalkthroughMovie();
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c27c = 0xc;
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeHerdingActivity();
    EndLoadingCursorAnimation();
    return;
  case 0xf:
    DAT_0051c334 = 1;
    DAT_0051b418 = DAT_0044de14;
    iVar4 = UpdateHerdingActivity();
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
      DAT_0051c334 = 0;
    }
    UpdateGenericHelpHoverVoice();
    UpdateHelpButtonController();
    if ((DAT_004fbfbc & 1) != 0) {
      StopAllManagedSounds(DAT_0044ddd8);
LAB_0042ac86:
      DAT_0051c2c8 = 1;
      (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,uVar2,0,0);
    }
    goto LAB_0042aca0;
  case 0x10:
    if ((DAT_0051c2b4 != (undefined4 *)0x0) &&
       (CSound_Stop((int)DAT_0051c2b4), DAT_0051c2b4 != (undefined4 *)0x0)) {
      (**(code **)*DAT_0051c2b4)(1);
      DAT_0051c2b4 = (undefined4 *)0x0;
    }
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 3;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00490f70,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00490f70);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    if (DAT_0051c32c == 0) {
      DAT_0044de14 = DAT_0044de14 + -1;
      DAT_0051c27c = 0xe;
      OpenGlobalBinkMovie(&DAT_00493a70);
      DAT_0051c314 = 1;
      DAT_0051c32c = 0;
      return;
    }
    DAT_0051c32c = 0;
    return;
  case 0x11:
    UpdateHelpButtonController();
    if (DAT_0051c314 == 1) {
      DAT_0051c314 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x12,0x32,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x292) * 4 + 0xd98) = 1;
    }
    DAT_0051c27c = 3;
    DrawGenericUIScreen();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x11;
        return;
      case -4:
        DAT_0051c2e4 = 2;
        DAT_0051c284 = DAT_0051c34c;
        DAT_0044de14 = 0x12;
        return;
      case -3:
        DAT_0051c2e4 = 1;
        DAT_0051c284 = DAT_0051c348;
        DAT_0044de14 = 0x12;
        return;
      case -2:
        DAT_0051c2e4 = 0;
        DAT_0051c284 = DAT_0051c344;
        DAT_0044de14 = 0x12;
        return;
      case -1:
switchD_0042c8c6_caseD_ffffffff:
        DAT_0044de14 = 4;
        DAT_0051c2f8 = 0;
        return;
      }
    }
    break;
  case 0x12:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 2;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00490c70 + DAT_0051c2e4 * 0x100,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00490c70 + DAT_0051c2e4 * 0x100);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    OpenWalkthroughMovie(1);
    DAT_00446f38 = 0xffffffff;
    DAT_0051c2f8 = 1;
    DAT_0051c318 = 1;
    return;
  case 0x13:
    if (DAT_0051c318 == 1) {
      DAT_00446fd8 = 0x1f0;
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1f0,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x470) * 4 + 0xd98) = 1;
    }
    DrawGenericUIScreen();
    UpdateHelpButtonController();
    UpdateWalkthroughMovie(param_1);
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x13;
        return;
      case -5:
        DAT_0044de14 = 0x14;
        return;
      case -4:
        DAT_0044de14 = 0x13;
        DAT_0051c284 = 2;
        (&DAT_0051c344)[DAT_0051c2e4] = 2;
        return;
      case -3:
        DAT_0051c284 = 1;
        DAT_0044de14 = 0x13;
        (&DAT_0051c344)[DAT_0051c2e4] = 1;
        return;
      case -2:
        DAT_0051c284 = 0;
        (&DAT_0051c344)[DAT_0051c2e4] = 0;
        DAT_0044de14 = 0x13;
        return;
      case -1:
        DAT_0044de14 = 0x10;
        DAT_0051c32c = 1;
        DAT_0051c2f8 = 0;
        return;
      }
    }
    break;
  case 0x14:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    CloseWalkthroughMovie();
    DAT_0051b418 = DAT_0044de14;
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c27c = 0xc;
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeDinoActivity(param_1,DAT_0051c2e4);
    goto LAB_0042b0a7;
  case 0x15:
    iVar4 = UpdateDinoActivity();
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    UpdateGenericHelpHoverVoice();
    UpdateHelpButtonController();
    if ((DAT_004fbfbc & 1) != 0) {
      if (DAT_004fca84 != 0) {
        DAT_00446ce8 = 0x10;
        PreparePlayAgainTransition();
        DAT_0044de14 = 0x3c;
        DAT_0051b418 = 0x14;
        UnloadDinoActivityResources();
        DAT_0051c2fc = 1;
        DAT_0051c32c = 1;
        return;
      }
      StopAllManagedSounds(DAT_0044ddd8);
      DAT_0051c32c = 1;
      DAT_0051c2c8 = 1;
      (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,uVar2,0,0);
      return;
    }
    break;
  case 0x16:
    if ((DAT_0051c2b4 != (undefined4 *)0x0) &&
       (CSound_Stop((int)DAT_0051c2b4), DAT_0051c2b4 != (undefined4 *)0x0)) {
      (**(code **)*DAT_0051c2b4)(1);
      DAT_0051c2b4 = (undefined4 *)0x0;
    }
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 5;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491070,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491070);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    if (DAT_0051c32c == 0) {
      DAT_0044de14 = DAT_0044de14 + -1;
      DAT_0051c27c = 0xe;
      OpenGlobalBinkMovie(&DAT_00493b70);
      DAT_0051c314 = 1;
      DAT_0051c32c = 0;
      return;
    }
    DAT_0051c32c = 0;
    return;
  case 0x17:
    if (DAT_0051c314 == 1) {
      DAT_0051c314 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1c,0x32,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x29c) * 4 + 0xd98) = 1;
    }
    DAT_0051c27c = 5;
    DrawGenericUIScreen();
    UpdateHelpButtonController();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x17;
        return;
      case -3:
        DAT_0051c2e4 = 1;
        DAT_0044de14 = 0x18;
        return;
      case -2:
        DAT_0051c2e4 = 0;
        DAT_0044de14 = 0x38;
        return;
      case -1:
        goto switchD_0042c8c6_caseD_ffffffff;
      }
    }
    break;
  case 0x18:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 8;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491170 + DAT_0051c2e4 * 0x100,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491170 + DAT_0051c2e4 * 0x100);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    OpenWalkthroughMovie(DAT_0051c2e4 + 2);
    DAT_00446f38 = 0xffffffff;
    DAT_0051c2f8 = 0;
    DAT_0051c318 = 1;
    return;
  case 0x19:
    if (DAT_0051c318 == 1) {
      DAT_0051c318 = 0;
      DAT_00446fd8 = 0x1fa;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1fa,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x47a) * 4 + 0xd98) = 1;
    }
    UpdateHelpButtonController();
    DrawGenericUIScreen();
    UpdateWalkthroughMovie(param_1);
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x19;
        return;
      case -5:
        DAT_0044de14 = 0x1a;
        return;
      case -4:
        DAT_0044de14 = 0x19;
        DAT_0051c284 = 2;
        return;
      case -3:
        DAT_0044de14 = 0x19;
        DAT_0051c284 = 1;
        return;
      case -2:
        DAT_0044de14 = 0x1a;
        DAT_0051c32c = 1;
        DAT_0051c2f8 = 0;
        DAT_0051c284 = 0;
        return;
      case -1:
switchD_0042b435_caseD_ffffffff:
        DAT_0044de14 = 0x16;
        DAT_0051c32c = 1;
        DAT_0051c2f8 = 0;
        return;
      }
    }
    break;
  case 0x1a:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    CloseWalkthroughMovie();
    DAT_0051b418 = DAT_0044de14;
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c27c = 0xc;
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeSpudSkateActivity();
    goto LAB_0042b0a7;
  case 0x1b:
    iVar4 = UpdateSpudSkateActivity();
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    UpdateGenericHelpHoverVoice();
    UpdateHelpButtonController();
    goto LAB_0042b557;
  case 0x1c:
    if ((DAT_0051c2b4 != (undefined4 *)0x0) &&
       (CSound_Stop((int)DAT_0051c2b4), DAT_0051c2b4 != (undefined4 *)0x0)) {
      (**(code **)*DAT_0051c2b4)(1);
      DAT_0051c2b4 = (undefined4 *)0x0;
    }
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 6;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491370,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491370);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    if (DAT_0051c32c == 0) {
      DAT_0044de14 = DAT_0044de14 + -1;
      DAT_0051c27c = 0xe;
      OpenGlobalBinkMovie(&DAT_00493c70);
      DAT_0051c314 = 1;
      DAT_0051c32c = 0;
      return;
    }
    DAT_0051c32c = 0;
    return;
  case 0x1d:
    UpdateHelpButtonController();
    if (DAT_0051c314 == 1) {
      DAT_0051c314 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0xc,0x32,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x28c) * 4 + 0xd98) = 1;
    }
    DAT_0051c27c = 6;
    DrawGenericUIScreen();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x1d;
        return;
      case -3:
        DAT_0051c2e4 = 1;
        DAT_0044de14 = 0x34;
        return;
      case -2:
        DAT_0051c2e4 = 0;
        DAT_0044de14 = 0x1e;
        return;
      case -1:
        goto switchD_0042c8c6_caseD_ffffffff;
      }
    }
    break;
  case 0x1e:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 2;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491470 + DAT_0051c2e4 * 0x100,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491470 + DAT_0051c2e4 * 0x100);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    OpenWalkthroughMovie(DAT_0051c2e4 + 4);
    DAT_0051c2f8 = 1;
    DAT_0051c318 = 1;
    DAT_0051c284 = DAT_0051c358;
    DAT_00446f38 = 0xffffffff;
    return;
  case 0x1f:
    if (DAT_0051c318 == 1) {
      DAT_00446fd8 = 0x1e8;
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1e8,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x468) * 4 + 0xd98) = 1;
    }
    UpdateHelpButtonController();
    DrawGenericUIScreen();
    UpdateWalkthroughMovie(param_1);
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x1f;
        return;
      case -5:
        DAT_0044de14 = 0x20;
        return;
      case -4:
        DAT_0044de14 = 0x1f;
        DAT_0051c284 = 2;
        DAT_0051c358 = 2;
        return;
      case -3:
        DAT_0051c284 = 1;
        DAT_0051c358 = 1;
        DAT_0044de14 = 0x1f;
        return;
      case -2:
        DAT_0044de14 = 0x1f;
        DAT_0051c284 = 0;
        DAT_0051c358 = 0;
        return;
      case -1:
switchD_0042ba7c_caseD_ffffffff:
        DAT_0044de14 = 0x1c;
        DAT_0051c32c = 1;
        DAT_0051c2f8 = 0;
        return;
      }
    }
    break;
  case 0x20:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    CloseWalkthroughMovie();
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeMazeActivity();
    goto LAB_0042bb32;
  case 0x21:
    DAT_0051c334 = 1;
    DAT_0051b418 = DAT_0044de14;
    iVar4 = UpdateMazeActivity();
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    UpdateGenericHelpHoverVoice();
    UpdateHelpButtonController();
    if ((DAT_004fbfbc & 1) != 0) {
      DAT_0051c2c8 = 1;
      DAT_0051c32c = 1;
      (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,uVar2,0,0);
    }
    if ((DAT_004fbfbc & 2) != 0) {
      NoOpLegacyHook();
      return;
    }
    break;
  case 0x22:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 7;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491670,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491670);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + -1;
    DAT_0051c27c = 0xe;
    OpenGlobalBinkMovie(&DAT_00493d70);
    OpenWalkthroughMovie(6);
    DAT_0051c2f8 = 0;
    DAT_0051c318 = 2;
    return;
  case 0x23:
    DAT_00446fd8 = 499;
    if (DAT_0051c318 == 1) {
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,499,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x473) * 4 + 0xd98) = 1;
    }
    else if (DAT_0051c318 == 2) {
      DAT_0051c318 = 3;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1f2,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x472) * 4 + 0xd98) = 1;
    }
    else if ((DAT_0051c318 == 3) && (iVar4 = AnyManagedSoundPlaying(DAT_0044ddd8), iVar4 == 0)) {
      DAT_0051c318 = 1;
    }
    UpdateHelpButtonController();
    DrawGenericUIScreen();
    UpdateWalkthroughMovie(param_1);
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x23;
        return;
      case -5:
        DAT_0044de14 = 0x24;
        return;
      case -4:
        DAT_0051c284 = 2;
        DAT_0044de14 = 0x23;
        return;
      case -3:
        DAT_0044de14 = 0x23;
        DAT_0051c284 = 1;
        return;
      case -2:
        DAT_0044de14 = 0x24;
        DAT_0051c284 = 0;
        return;
      case -1:
        goto switchD_0042c8c6_caseD_ffffffff;
      default:
        return;
      }
    }
    return;
  case 0x24:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    CloseWalkthroughMovie();
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeFireworksActivity();
    goto LAB_0042bb32;
  case 0x25:
    DAT_0051b418 = DAT_0044de14;
    iVar4 = UpdateFireworksActivity(param_1);
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    UpdateGenericHelpHoverVoice();
    if ((DAT_0050a5bc != 9) && (DAT_0050a5bc != 0xf)) {
      UpdateHelpButtonController();
    }
    if ((DAT_004fbfbc & 1) != 0) {
      StopActivityMusic();
      if (DAT_0050a5bc < 9) goto LAB_0042ac86;
      PreparePlayAgainTransition();
      DAT_0051c2fc = 2;
      piVar5 = &DAT_004fc084;
      do {
        if (*piVar5 != 0) {
          CSound_Stop(*piVar5);
        }
        piVar5 = piVar5 + 1;
      } while ((int)piVar5 < 0x4fc0d4);
      DAT_0044de14 = 0x3c;
      DAT_0051b418 = 0x25;
    }
LAB_0042aca0:
    if ((DAT_004fbfbc & 2) != 0) {
      NoOpLegacyHook();
      return;
    }
    break;
  case 0x26:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 2;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491770,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491770);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + -1;
    DAT_0051c27c = 0xe;
    OpenGlobalBinkMovie(&DAT_00493e70);
    iVar4 = 7;
LAB_0042aab0:
    OpenWalkthroughMovie(iVar4);
    DAT_0051c2f8 = 1;
    DAT_0051c318 = 1;
    DAT_00446f38 = 0xffffffff;
    return;
  case 0x27:
    if (DAT_0051c318 == 1) {
      DAT_00446fd8 = 0x1f7;
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1f7,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x477) * 4 + 0xd98) = 1;
      DAT_0051c284 = DAT_0051c35c;
    }
    DrawGenericUIScreen();
    UpdateHelpButtonController();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        break;
      case -5:
        DAT_0044de14 = 0x28;
        UpdateWalkthroughMovie(param_1);
        return;
      case -4:
        DAT_0051c284 = 2;
        DAT_0051c35c = 2;
        break;
      case -3:
        DAT_0051c284 = 1;
        DAT_0051c35c = 1;
        break;
      case -2:
        DAT_0051c284 = 0;
        DAT_0051c35c = 0;
        break;
      case -1:
        DAT_0044de14 = 4;
        DAT_0051c2f8 = 0;
        UpdateWalkthroughMovie(param_1);
        return;
      default:
        goto switchD_0042c23e_default;
      }
      DAT_0044de14 = 0x27;
    }
switchD_0042c23e_default:
    UpdateWalkthroughMovie(param_1);
    return;
  case 0x28:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    CloseWalkthroughMovie();
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeSquirrelActivity();
    goto LAB_0042bb32;
  case 0x29:
    DAT_0051b418 = DAT_0044de14;
    iVar4 = UpdateSquirrelActivity();
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    UpdateGenericHelpHoverVoice();
    UpdateHelpButtonController();
    if ((DAT_004fbfbc & 1) != 0) {
      DAT_0051c2c8 = 1;
      (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,uVar2,0,0);
    }
    if ((DAT_004fbfbc & 2) != 0) {
      NoOpLegacyHook();
      return;
    }
    break;
  case 0x2a:
    if ((DAT_0051c2b4 != (undefined4 *)0x0) &&
       (CSound_Stop((int)DAT_0051c2b4), DAT_0051c2b4 != (undefined4 *)0x0)) {
      (**(code **)*DAT_0051c2b4)(1);
      DAT_0051c2b4 = (undefined4 *)0x0;
    }
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 4;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491d70,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491d70);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    if (DAT_0051c32c == 0) {
      DAT_0044de14 = DAT_0044de14 + -1;
      DAT_0051c27c = 0xe;
      OpenGlobalBinkMovie(&DAT_00493f70);
      DAT_0051c314 = 1;
      DAT_0051c32c = 0;
      return;
    }
    DAT_0051c32c = 0;
    return;
  case 0x2b:
    UpdateHelpButtonController();
    if (DAT_0051c314 == 1) {
      DAT_0051c314 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x17,0x32,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x297) * 4 + 0xd98) = 1;
    }
    DAT_0051c27c = 4;
    DrawGenericUIScreen();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x2b;
        return;
      case -4:
        DAT_0051c2e4 = 2;
        DAT_0044de14 = 0x2c;
        return;
      case -3:
        DAT_0051c2e4 = 1;
        DAT_0044de14 = 0x2c;
        return;
      case -2:
        DAT_0051c2e4 = 0;
        DAT_0044de14 = 0x2c;
        return;
      case -1:
        goto switchD_0042c8c6_caseD_ffffffff;
      }
    }
    break;
  case 0x2c:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 8;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491870,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491870);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    OpenWalkthroughMovie(8);
    DAT_00446f38 = 0xffffffff;
    DAT_0051c2f8 = 0;
    DAT_0051c318 = 1;
    return;
  case 0x2d:
    if (DAT_0051c318 == 1) {
      DAT_00446fd8 = 500;
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,500,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x474) * 4 + 0xd98) = 1;
    }
    UpdateHelpButtonController();
    DAT_0051c284 = DAT_0051c2e4;
    DrawGenericUIScreen();
    UpdateWalkthroughMovie(param_1);
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x2d;
        return;
      case -5:
        DAT_0044de14 = 0x2e;
        return;
      case -4:
        DAT_0051c284 = 2;
        DAT_0044de14 = 0x2d;
        return;
      case -3:
        DAT_0044de14 = 0x2d;
        DAT_0051c284 = 1;
        return;
      case -2:
        DAT_0044de14 = 0x2e;
        DAT_0051c284 = 0;
        return;
      case -1:
        DAT_0044de14 = 0x2a;
        DAT_0051c32c = 1;
        DAT_0051c2f8 = 0;
        return;
      }
    }
    break;
  case 0x2e:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    CloseWalkthroughMovie();
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c33c = 1;
    DAT_0051c284 = DAT_0051c2e4;
    BeginLoadingCursorAnimation();
    InitializeBobsBandActivity();
    goto LAB_0042bb32;
  case 0x2f:
    DAT_0051b418 = DAT_0044de14;
    iVar4 = UpdateBobsBandActivity();
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    if ((DAT_004fbfbc & 1) != 0) {
      StopAllManagedSounds(DAT_0044ddd8);
      DAT_00446ce8 = 0x2a;
      DAT_0051c2c8 = 1;
      DAT_0051c32c = 1;
      (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,uVar2,0,0);
    }
    if ((DAT_004fbfbc & 2) != 0) {
      NoOpLegacyHook();
      return;
    }
    break;
  case 0x30:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 8;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491970,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491970);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + -1;
    DAT_0051c27c = 0xe;
    OpenGlobalBinkMovie(&DAT_00494070);
    OpenWalkthroughMovie(9);
    DAT_0051c2f8 = 0;
    DAT_0051c318 = 1;
    return;
  case 0x31:
    if (DAT_0051c318 == 1) {
      DAT_00446fd8 = 0x1f1;
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1f1,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x471) * 4 + 0xd98) = 1;
    }
    UpdateHelpButtonController();
    DrawGenericUIScreen();
    UpdateWalkthroughMovie(param_1);
    DAT_0051c27c = 8;
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x31;
        return;
      case -5:
        DAT_0044de14 = 0x32;
        return;
      case -4:
        DAT_0051c284 = 2;
        DAT_0044de14 = 0x31;
        return;
      case -3:
        DAT_0044de14 = 0x31;
        DAT_0051c284 = 1;
        return;
      case -2:
        DAT_0044de14 = 0x32;
        DAT_0051c284 = 0;
        return;
      case -1:
        goto switchD_0042c8c6_caseD_ffffffff;
      default:
        return;
      }
    }
    return;
  case 0x32:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    CloseWalkthroughMovie();
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeParkDesignerActivity();
    EndLoadingCursorAnimation();
    DAT_0051c27c = 0xc;
    DAT_00446f38 = 0xffffffff;
    DAT_0044de14 = DAT_0044de14 + 1;
    return;
  case 0x33:
    DAT_0051b418 = DAT_0044de14;
    iVar4 = UpdateParkDesignerActivity(param_1);
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    UpdateGenericHelpHoverVoice();
    UpdateHelpButtonController();
    goto LAB_0042b557;
  case 0x34:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 2;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491470 + DAT_0051c2e4 * 0x100,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491470 + DAT_0051c2e4 * 0x100);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    OpenWalkthroughMovie(DAT_0051c2e4 + 4);
    DAT_0051c2f8 = 1;
    DAT_0051c318 = 1;
    DAT_0051c284 = DAT_0051c354;
    DAT_00446f38 = 0xffffffff;
    return;
  case 0x35:
    if (DAT_0051c318 == 1) {
      DAT_00446fd8 = 0x1e9;
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1e9,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x469) * 4 + 0xd98) = 1;
    }
    UpdateHelpButtonController();
    DrawGenericUIScreen();
    UpdateWalkthroughMovie(param_1);
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x35;
        return;
      case -5:
        DAT_0044de14 = 0x36;
        return;
      case -4:
        DAT_0044de14 = 0x35;
        DAT_0051c284 = 2;
        DAT_0051c354 = 2;
        return;
      case -3:
        DAT_0051c284 = 1;
        DAT_0051c354 = 1;
        DAT_0044de14 = 0x35;
        return;
      case -2:
        DAT_0044de14 = 0x35;
        DAT_0051c284 = 0;
        DAT_0051c354 = 0;
        return;
      case -1:
        goto switchD_0042ba7c_caseD_ffffffff;
      }
    }
    break;
  case 0x36:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeGolfActivity();
LAB_0042bb32:
    EndLoadingCursorAnimation();
    DAT_0051c27c = 0xc;
    DAT_0044de14 = DAT_0044de14 + 1;
    return;
  case 0x37:
    DAT_0051b418 = DAT_0044de14;
    iVar4 = UpdateGolfActivity();
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    UpdateGenericHelpHoverVoice();
    UpdateHelpButtonController();
    if ((DAT_004fbfbc & 1) != 0) {
      DAT_0051c2c8 = 1;
      DAT_0051c32c = 1;
      (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,uVar2,0,0);
    }
    if ((DAT_004fbfbc & 2) != 0) {
      NoOpLegacyHook();
      return;
    }
    break;
  case 0x38:
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    DAT_0051c27c = 2;
    DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar5,&DAT_00491170 + DAT_0051c2e4 * 0x100,0,0);
    RegisterBitmapSurface(&DAT_0051c268,&DAT_00491170 + DAT_0051c2e4 * 0x100);
    _DAT_0051c264 = 1;
    LoadGenericUIScreenResources();
    OpenWalkthroughMovie(DAT_0051c2e4 + 2);
    DAT_0051c2f8 = 1;
    DAT_0051c318 = 1;
    DAT_0051c284 = DAT_0051c350;
    DAT_00446f38 = 0xffffffff;
    return;
  case 0x39:
    if (DAT_0051c318 == 1) {
      DAT_00446fd8 = 0x1f9;
      DAT_0051c318 = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x1f9,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x479) * 4 + 0xd98) = 1;
    }
    UpdateHelpButtonController();
    DrawGenericUIScreen();
    UpdateWalkthroughMovie(param_1);
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < 0) {
      switch(DAT_0044de14) {
      case -6:
        DAT_0044de14 = 0x39;
        return;
      case -5:
        DAT_0044de14 = 0x3a;
        return;
      case -4:
        DAT_0044de14 = 0x39;
        DAT_0051c284 = 2;
        DAT_0051c350 = 2;
        return;
      case -3:
        DAT_0051c284 = 1;
        DAT_0051c350 = 1;
        DAT_0044de14 = 0x39;
        return;
      case -2:
        DAT_0044de14 = 0x39;
        DAT_0051c284 = 0;
        DAT_0051c350 = 0;
        return;
      case -1:
        goto switchD_0042b435_caseD_ffffffff;
      }
    }
    break;
  case 0x3a:
    DAT_0051c300 = 0;
    DAT_0051c2f8 = 0;
    CloseWalkthroughMovie();
    DAT_0051b418 = DAT_0044de14;
    UnregisterBitmapSurface(0x51c268);
    if (DAT_0051c268 != (int *)0x0) {
      (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
      DAT_0051c268 = (int *)0x0;
    }
    UnloadGenericUIScreenResources();
    DAT_0051c27c = 0xc;
    DAT_0051c33c = 1;
    BeginLoadingCursorAnimation();
    InitializeSpudMazeActivity();
LAB_0042b0a7:
    EndLoadingCursorAnimation();
    DAT_0044de14 = DAT_0044de14 + 1;
    return;
  case 0x3b:
    DAT_0051c334 = 1;
    iVar4 = UpdateSpudMazeActivity();
    if (iVar4 == -1) {
      DAT_0044de14 = 4;
    }
    UpdateGenericHelpHoverVoice();
    UpdateHelpButtonController();
LAB_0042b557:
    if ((DAT_004fbfbc & 1) != 0) {
      DAT_0051c2c8 = 1;
      (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,uVar2,0,0);
      return;
    }
    break;
  case 0x3c:
    DAT_0051c268 = LoadBitmapToDirectDrawSurface
                             (piVar5,s_Data_ui_playagain_playagainnodif_00447094,0,0);
    RegisterBitmapSurface(&DAT_0051c268,s_Data_ui_playagain_playagainnodif_00447094);
    MarkRegisteredSurfaceColorKeyed(0x51c268);
    SetSurfaceTransparencyColorKey(DAT_0051c268,0xff00ff);
    DAT_0051c27c = 9;
    goto LAB_0042cb39;
  case 0x3d:
    DrawGenericUIScreen();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 == -0x15) {
      UnloadGenericUIScreenResources();
      UnregisterBitmapSurface(0x51c268);
      if (DAT_0051c268 != (int *)0x0) {
        (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
        DAT_0051c268 = (int *)0x0;
      }
      DAT_0044de14 = 0x40;
      DAT_0051c300 = 0;
      return;
    }
    if (DAT_0044de14 == -0x14) {
      UnloadGenericUIScreenResources();
      UnregisterBitmapSurface(0x51c268);
      if (DAT_0051c268 != (int *)0x0) {
        (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
        DAT_0051c268 = (int *)0x0;
      }
      if (DAT_0051c2fc == 1) {
        DAT_0044de14 = 0x3e;
        return;
      }
      if (DAT_0051c2fc == 2) {
        DAT_0044de14 = 0x42;
        return;
      }
      DAT_0051c300 = 0;
      DAT_0044de14 = DAT_0051b418;
      return;
    }
    break;
  case 0x3e:
    DAT_0051c268 = LoadBitmapToDirectDrawSurface
                             (piVar5,s_Data_ui_playagain_playagain3_bmp_00447070,0,0);
    RegisterBitmapSurface(&DAT_0051c268,s_Data_ui_playagain_playagain3_bmp_00447070);
    MarkRegisteredSurfaceColorKeyed(0x51c268);
    SetSurfaceTransparencyColorKey(DAT_0051c268,0xff00ff);
    DAT_0051c27c = 10;
LAB_0042cb39:
    LoadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    NoOpLegacyHook();
    return;
  case 0x3f:
    DrawGenericUIScreen();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 < -0x1d) {
      StopActivityMusic();
      DAT_0051c300 = 0;
      UnloadGenericUIScreenResources();
      UnregisterBitmapSurface(0x51c268);
      if (DAT_0051c268 != (int *)0x0) {
        (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
        DAT_0051c268 = (int *)0x0;
      }
      DAT_0051c284 = -0x1e - DAT_0044de14;
      DAT_0044de14 = DAT_0051b418;
      DAT_0051c2fc = 0;
      return;
    }
    break;
  case 0x40:
    if (DAT_00446f38 == -1) {
      if (DAT_00446ce8 == -1) {
        DAT_00446ce8 = 4;
      }
      DAT_0044de14 = DAT_00446ce8;
      DAT_00446ce8 = 0xffffffff;
      return;
    }
    if ((-1 < DAT_00446f38) && (DAT_00446f38 != -2)) {
      OpenGlobalBinkMovie(&DAT_0048daf0 + DAT_00446f38 * 0x100);
      DAT_00446f38 = -2;
    }
    iVar4 = UpdateGlobalBinkMovie();
    if (iVar4 != 0) {
      DAT_0044de14 = DAT_00446ce8;
      if (DAT_00446ce8 == -1) {
        DAT_0044de14 = 4;
      }
      DAT_00446ce8 = -1;
      DAT_00446f38 = -1;
    }
    break;
  case 0x41:
    DAT_0051c310 = 0;
    iVar4 = UpdateSceneSetMovie();
    if (iVar4 != 0) {
      DAT_0044de14 = 4;
      return;
    }
    break;
  case 0x42:
    DAT_0051c268 = LoadBitmapToDirectDrawSurface
                             (piVar5,s_Data_ui_editplay_Fireworkplayaga_004470bc,0,0);
    RegisterBitmapSurface(&DAT_0051c268,s_Data_ui_editplay_Fireworkplayaga_004470bc);
    MarkRegisteredSurfaceColorKeyed(0x51c268);
    SetSurfaceTransparencyColorKey(DAT_0051c268,0xff00ff);
    DAT_0051c27c = 0xb;
    LoadGenericUIScreenResources();
    DAT_0044de14 = DAT_0044de14 + 1;
    return;
  case 0x43:
    DrawGenericUIScreen();
    UpdateGenericUIScreenInteraction();
    if (DAT_0044de14 == -0x15) {
      UnloadGenericUIScreenResources();
      UnregisterBitmapSurface(0x51c268);
      if (DAT_0051c268 != (int *)0x0) {
        (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
        DAT_0051c268 = (int *)0x0;
      }
      DAT_0044de14 = 0x25;
      DAT_0051c300 = 0;
      DAT_0050a5bc = 8;
      return;
    }
    if (DAT_0044de14 == -0x14) {
      UnloadGenericUIScreenResources();
      UnregisterBitmapSurface(0x51c268);
      if (DAT_0051c268 != (int *)0x0) {
        (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
        DAT_0051c268 = (int *)0x0;
      }
      DAT_0044de14 = 0x25;
      DAT_0051c300 = 0;
      DAT_0050a5bc = 0;
      PlayActivityMusicByIndex(4);
      UnloadFireworkMovieBank();
      LoadFireworksEditorResources();
      return;
    }
  }
  return;
}

