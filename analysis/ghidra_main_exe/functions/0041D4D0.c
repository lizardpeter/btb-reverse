/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041d4d0; function: UpdateMazeActivity; body bytes: 737
 * callers: 1; callees: 18; success: True
 */


undefined4 UpdateMazeActivity(void)

{
  bool bVar1;
  uint uVar2;
  undefined3 extraout_var;
  int iVar3;
  _SYSTEMTIME local_20;
  _SYSTEMTIME local_10;
  
  EnableInputProcessing();
  DAT_0051c334 = 1;
  if ((DAT_0044ddb0 != 0) || (DAT_0051c2e8 != 0)) {
    UnloadMazeActivityResources();
    if (DAT_0051c2e8 != 0) {
      DAT_0051c2e8 = 0;
      DAT_0044de14 = 0x1c;
    }
    if (DAT_00446f38 != -1) {
      DAT_0044de14 = 0x40;
    }
    return 1;
  }
  if (DAT_005109c8 == 0xf) {
    StopAllManagedSounds(DAT_0044ddd8);
    PlayManagedSoundById(DAT_0044ddd8,0x53,0x5a,1);
  }
  if (((DAT_00510ba8 < 1) || (DAT_005109c8 < 0)) && (DAT_00512128 == 0)) {
    DAT_0051c32c = 1;
    DAT_00446ce8 = 0x1c;
    if (DAT_005107d0 == 0) {
      DAT_005107d0 = 1;
      StopAllManagedSounds(DAT_0044ddd8);
      if (DAT_00510ba8 < 1) {
        DAT_0051c32c = 1;
        if (DAT_005109c8 < 0x10) {
          iVar3 = 0x55;
        }
        else {
          iVar3 = 0x54;
        }
      }
      else {
        iVar3 = 0x56;
      }
      PlayManagedSoundById(DAT_0044ddd8,iVar3,0x5a,1);
      DrawMazeActivity();
      return 1;
    }
    if (DAT_005107d0 == 1) {
      DrawMazeActivity();
      iVar3 = AnyManagedSoundPlaying(DAT_0044ddd8);
      if (iVar3 != 0) {
        return 1;
      }
      DAT_005107d0 = DAT_005107d0 + 1;
      return 1;
    }
    if (DAT_00510ba8 < 1) {
      iVar3 = DAT_00519934 * 400;
      *(undefined4 *)(&DAT_0051b5c4 + iVar3) = 1;
      DAT_00446f38 = (-(uint)(*(int *)(&DAT_0051b5c8 + iVar3) != 1) & 0xfffffffc) + 3;
    }
    PreparePlayAgainTransition();
    DAT_005109c8 = 999;
    DAT_0044de14 = 0x3c;
    DAT_0051b418 = 0x20;
    UnloadMazeActivityResources();
    DAT_0051c2fc = 1;
    return 1;
  }
  uVar2 = CSound_IsSoundPlaying((int)DAT_004fc088);
  if (uVar2 == 0) {
    CSound_Play(DAT_004fc088,0,1);
  }
  if (DAT_00512128 == 0) {
    if ((DAT_0051212c == 0) && (UpdateMazePlayerMovement(), DAT_00512128 != 0)) goto LAB_0041d688;
  }
  else {
    DrawMazeScreenTransition();
  }
  DrawMazeActivity();
LAB_0041d688:
  if (DAT_0051212c == 0) {
    UpdateMazeSpudNPC();
  }
  if (DAT_0051212c == 1) {
    if (DAT_0051c30c == 1) {
      CSound_Stop(DAT_0051c2b8);
      PlayActivityMusicByIndex(5);
      DAT_0051c30c = 0;
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,DAT_0051c284 + 0x49,0x5a,1);
      *(undefined4 *)
       ((int)DAT_0044ddd8 + *(char *)(DAT_0051c284 + 0x2c9 + (int)DAT_0044ddd8) * 4 + 0xd98) = 1;
      return 0;
    }
    GetSystemTime(&local_20);
    GetSystemTime(&local_10);
    SystemTimeToFileTime(&local_20,(LPFILETIME)&DAT_00510d10);
    SystemTimeToFileTime(&local_10,(LPFILETIME)&DAT_005120f0);
    Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
    bVar1 = IsSoundIdPlaying(DAT_0044ddd8,DAT_0051c284 + 0x49);
    if (CONCAT31(extraout_var,bVar1) == 0) {
      DAT_0051212c = 0;
    }
  }
  return 0;
}

