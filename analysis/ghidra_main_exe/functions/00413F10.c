/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00413f10; function: UpdateFireworksActivity; body bytes: 1208
 * callers: 1; callees: 31; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl UpdateFireworksActivity(undefined4 param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  bool bVar5;
  
  if ((DAT_0044ddb0 != 0) || (DAT_0051c2e8 != 0)) {
    SaveFireworksLayoutAndUnloadResources();
    if (DAT_0051c2e8 != 0) {
      DAT_0051c2e8 = 0;
      DAT_0044de14 = 4;
    }
    return 1;
  }
  if (DAT_0051c2dc != 0) {
    puVar3 = &DAT_0050a678;
    for (iVar4 = 0x12; iVar4 != 0; iVar4 = iVar4 + -1) {
      *puVar3 = 0xffffffff;
      puVar3 = puVar3 + 1;
    }
    DAT_0051c2dc = 0;
    DAT_0050ab20 = 0;
    DAT_0050ab24 = 0;
    DAT_0050a5bc = 0;
    SetCursorSurface((int *)0x0);
  }
  NoOpLegacyHook();
  if (DAT_0050ab70 == 0) {
    if (DAT_0051c30c == 1) {
      CSound_Stop(DAT_0051c2b8);
      PlayActivityMusicByIndex(4);
      DAT_0051c30c = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x35d,0x32,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x5dd) * 4 + 0xd98) = 1;
    }
    else {
      iVar4 = AnyManagedSoundPlaying(DAT_0044ddd8);
      if (iVar4 == 0) {
        DAT_0050ab70 = 1;
        PlayManagedSoundById(DAT_0044ddd8,0x382,0x32,1);
        *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x602) * 4 + 0xd98) = 1;
      }
    }
  }
  if ((DAT_0050ab74 != -1) && (DAT_0050ab74 = DAT_0050ab74 + 1, 2999 < DAT_0050ab74)) {
    DAT_0050ab74 = 0;
    StopAllManagedSounds(DAT_0044ddd8);
    uVar1 = FUN_0042ffc4();
    uVar1 = uVar1 & 0x80000001;
    bVar5 = uVar1 == 0;
    if ((int)uVar1 < 0) {
      bVar5 = (uVar1 - 1 | 0xfffffffe) == 0xffffffff;
    }
    if (!bVar5) {
      PlayManagedSoundById(DAT_0044ddd8,0x357,0x32,1);
    }
  }
  NoOpLegacyHook();
  switch(DAT_0050a5bc) {
  case 0:
    DrawFireworksEditor();
    UpdateFireworksEditorControls();
    if ((DAT_004fbe54 != 0) && (iVar4 = HitTestFireworksEditorRegions(), iVar4 != -1)) {
      DAT_0050ab74 = 0xffffffff;
      BeginFireworksEditorAction(iVar4);
      CompleteFireworksEditorAction(iVar4);
      return 1;
    }
    break;
  case 1:
    DrawFireworksEditor();
    UpdateFireworksEditorControls();
    if ((DAT_004fbe54 != 0) && (iVar4 = HitTestFireworksEditorRegions(), iVar4 != -1)) {
      CompleteFireworksEditorAction(iVar4);
      return 1;
    }
    break;
  case 2:
    DrawFireworksEditor();
    DAT_005093f4 = 0;
    DAT_005093f0 = DAT_005093d8;
    DAT_005093f8 = 0;
    DAT_0050a5bc = DAT_0050a5bc + 1;
    return 1;
  case 3:
    DrawFireworksEditor();
    iVar4 = DAT_005093f0;
    iVar2 = DecodeAndBlitBinkFrame
                      (param_1,(int *)(&DAT_0050aaac)[DAT_005093f0],(&DAT_0050937c)[DAT_005093f0]);
    if (iVar2 != 0) {
      DAT_0050a5bc = DAT_0050a5bc + 1;
      RestartBinkMovie((&DAT_0050aaac)[iVar4]);
      return 1;
    }
    break;
  case 4:
    DrawFireworksEditor();
    DAT_0050a5bc = 0;
    return 1;
  case 8:
    StopAllManagedSounds(DAT_0044ddd8);
    UnloadFireworksEditorResources();
    LoadFireworkMovieBank();
    puVar3 = &DAT_0050a678;
    do {
      iVar4 = 6;
      do {
        puVar3 = puVar3 + 1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    } while ((int)puVar3 < 0x50a6c0);
    puVar3 = &DAT_005093f0;
    do {
      *puVar3 = 0xffffffff;
      puVar3 = puVar3 + 4;
    } while ((int)puVar3 < 0x509670);
    DAT_0050a5bc = DAT_0050a5bc + 1;
    _DAT_005093ec = 0;
    GetSystemTime((LPSYSTEMTIME)&DAT_00512178);
    GetSystemTime((LPSYSTEMTIME)&DAT_00512598);
    SystemTimeToFileTime((SYSTEMTIME *)&DAT_00512178,(LPFILETIME)&DAT_00510d10);
    SystemTimeToFileTime((SYSTEMTIME *)&DAT_00512598,(LPFILETIME)&DAT_005120f0);
    CSound_Reset((int)DAT_004fc0ac);
    CSound_Play(DAT_004fc0ac,0,1);
    DAT_0051218c = 0xfffffe6f;
    puVar3 = &DAT_0050aaac;
    do {
      RestartBinkMovie(*puVar3);
      puVar3 = puVar3 + 1;
    } while ((int)puVar3 < 0x50ab08);
    if (DAT_0051c2b4 != 0) {
      CSound_Stop(DAT_0051c2b4);
      return 1;
    }
    break;
  case 9:
    UpdateFireworksShowPlayback();
    return 1;
  case 0xd:
    DrawFireworksEditor();
    UpdateFireworksEditorControls();
    if ((DAT_004fbe54 != 0) && (iVar4 = HitTestFireworksEditorRegions(), iVar4 != -1)) {
      DeleteFireworkAtSelectedRegion(iVar4);
    }
    break;
  case 0xe:
    iVar4 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if (iVar4 == 1) {
      DrawFireworksEditor();
      return 1;
    }
    CSound_Stop(DAT_0051c2b4);
    ClearBackBuffer(DAT_0044de08,0);
    DisableInputProcessing();
    OpenGlobalBinkMovie(s_Data_movies_fireworkcomplete_bik_004435f4);
    DAT_0050a5bc = 0xf;
    return 1;
  case 0xf:
    iVar4 = UpdateGlobalBinkMovie();
    if (iVar4 != 0) {
      EnableInputProcessing();
      DAT_0050a5bc = 8;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x8e,0x5a,1);
      *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x30d) * 4 + 0xd98) = 1;
      return 1;
    }
    break;
  case 0x10:
    DrawFireworksCertificateScreen();
    return 1;
  case 0x11:
    PreparePlayAgainTransition();
    DAT_0044de14 = 0x3c;
    DAT_0051b418 = 0x24;
    SaveFireworksLayoutAndUnloadResources();
    DAT_0051c2fc = 0;
    return 1;
  }
  return 1;
}

