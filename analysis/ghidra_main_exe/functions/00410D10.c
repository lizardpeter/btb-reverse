/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00410d10; function: UpdateParkDesignerActivity; body bytes: 603
 * callers: 1; callees: 12; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl UpdateParkDesignerActivity(HGLOBAL param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  
  EnableInputProcessing();
  if ((DAT_0044ddb0 == 0) && (DAT_0051c2e8 == 0)) {
    if (DAT_0051c2dc != 0) {
      DAT_0051c2dc = 0;
      puVar1 = &DAT_004fcacc;
      do {
        *puVar1 = 0xffffffff;
        puVar1 = puVar1 + 0x13;
      } while ((int)puVar1 < 0x50418c);
      DAT_00441dc8 = 300;
      DAT_00507a2c = 0xffffffff;
      DAT_00441dcc = 200;
      DAT_00507a30 = 0xffffffff;
      DAT_00509340 = 0;
      _DAT_00507a34 = 0xffffffff;
      DAT_00507b14 = 0;
      DAT_00507b5c = 0;
      _DAT_00507a3c = 0;
      DAT_00507ae4 = 0;
      DAT_00507ae8 = 0;
      DAT_00507aec = 0;
      _DAT_00507af0 = 0;
      _DAT_00507af4 = 0;
      DAT_00508bf4 = 0;
      _DAT_00508bf8 = 0;
      _DAT_00507a38 = 0xffffffff;
      DAT_00507a40 = 0;
      DAT_00507c54 = 0xffffffff;
      DAT_00507a28 = 0xffffffff;
      DAT_005079fc = 0xffffffff;
      DAT_00507e3c = 0xffffffff;
      DAT_0050418c = -1;
    }
    NoOpLegacyHook();
    DrawParkDesignerActivity();
    if (DAT_00509354 == 0) {
      UpdateParkDesignerEditorInteraction(param_1);
    }
    else {
      CSound_Stop(DAT_0051c2b8);
      PlayActivityMusicByIndex(2);
      if (DAT_0051c30c == 1) {
        PlayManagedSoundById(DAT_0044ddd8,0x123,0x32,1);
        *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x3a3) * 4 + 0xd98) = 1;
        DAT_0051c30c = 0;
      }
      else {
        iVar2 = AnyManagedSoundPlaying(DAT_0044ddd8);
        if (iVar2 == 0) {
          DAT_00509354 = 0;
        }
      }
    }
    CompactParkDesignerObjectRecordFamilies();
    if (-1 < DAT_0050418c) {
      iVar3 = AnyManagedSoundPlaying(DAT_0044ddd8);
      iVar2 = DAT_0050418c;
      if (iVar3 == 0) {
        DAT_0050418c = -1;
        PlayInputInterruptibleParkDesignerVoice(iVar2,DAT_00507b60);
      }
    }
    return 0;
  }
  if (DAT_00509368 == 0) {
    iVar2 = 1;
    uVar4 = FUN_0042ffc4();
    PlayInputInterruptibleParkDesignerVoice((int)uVar4 % 3 + 0xf6,iVar2);
    DAT_00509368 = 1;
    DrawParkDesignerActivity();
    return 0;
  }
  if (DAT_00509368 == 1) {
    DrawParkDesignerActivity();
    iVar2 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if (iVar2 == 0) {
      DAT_00509368 = DAT_00509368 + 1;
    }
    return 0;
  }
  UnloadParkDesignerActivityResources();
  iVar2 = DAT_00519934;
  piVar5 = &DAT_004fcacc;
  do {
    if (*piVar5 != -1) {
      DAT_00446f38 = 7;
      *(undefined4 *)(&DAT_0051b5ac + iVar2 * 400) = 1;
    }
    piVar5 = piVar5 + 0x13;
  } while ((int)piVar5 < 0x50418c);
  if (DAT_0051c2e8 != 0) {
    DAT_0051c2e8 = 0;
    DAT_0044de14 = 4;
  }
  return 1;
}

