/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00420030; function: UpdateBobsBandActivity; body bytes: 757
 * callers: 1; callees: 20; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 UpdateBobsBandActivity(void)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  _SYSTEMTIME local_10;
  
  EnableInputProcessing();
  if ((DAT_0044ddb0 == 0) && (DAT_0051c2e8 == 0)) {
    if (DAT_0051c2dc != 0) {
      DAT_0051c2dc = 0;
      puVar4 = &DAT_005123b8;
      for (iVar3 = 0x78; iVar3 != 0; iVar3 = iVar3 + -1) {
        *puVar4 = 0xffffffff;
        puVar4 = puVar4 + 1;
      }
    }
    NoOpLegacyHook();
    switch(DAT_00512188) {
    case 0:
      DrawGrandOpeningCompositionAndMachines();
      UpdateGrandOpeningToolbar();
      if (DAT_004fbe54 != 0) {
        uVar2 = HitTestBobsBandRegions();
        if (uVar2 != 0xffffffff) {
          HandleBobsBandEditorAction(uVar2);
        }
        if (((DAT_00513f24 == 1) && (DAT_00513f28 == 0)) &&
           ((DAT_004fbd30 < 0x122 || (400 < DAT_004fbd30)))) {
          DAT_00513f28 = 0;
          DAT_00513f24 = 0;
          SetCursorSurface((int *)0x0);
        }
        else {
          DAT_00513f28 = 0;
        }
      }
      if (DAT_00513f24 == 0) {
        UpdateGenericHelpHoverVoice();
        UpdateHelpButtonController();
      }
      break;
    case 1:
      DrawGrandOpeningCompositionAndMachines();
      UpdateGrandOpeningToolbar();
      if ((DAT_004fbe54 != 0) && (uVar2 = HitTestBobsBandRegions(), uVar2 != 0xffffffff)) {
        UpdateBobsBandBrickCursorAndDelete(uVar2);
      }
      break;
    case 2:
      DrawGrandOpeningCompositionAndMachines();
      break;
    case 8:
      DAT_00512188 = DAT_00512188 + 1;
      _DAT_00512190 = 0;
      DAT_00446f38 = 6;
      *(undefined4 *)(&DAT_0051b5a0 + (DAT_0051c284 + DAT_00519934 * 100) * 4) = 1;
      GetSystemTime((LPSYSTEMTIME)&DAT_00512178);
      GetSystemTime(&local_10);
      SystemTimeToFileTime((SYSTEMTIME *)&DAT_00512178,(LPFILETIME)&DAT_00510d10);
      SystemTimeToFileTime(&local_10,(LPFILETIME)&DAT_005120f0);
      CSound_Reset((int)DAT_004fc14c);
      CSound_Play(DAT_004fc14c,0,0);
      DAT_0051218c = 999999;
      break;
    case 9:
      PlayAndDrawGrandOpeningComposition();
      piVar1 = *(int **)(DAT_0044de08 + 0xc);
      UpdateGrandOpeningToolbar();
      iVar3 = HitTestBobsBandRegions();
      if (iVar3 == 0x16) {
        if (DAT_004fbfb8 == 0) {
          (**(code **)(*piVar1 + 0x1c))(piVar1,DAT_00444d64,DAT_00444d68,DAT_00512774,0,0);
        }
        else {
          (**(code **)(*piVar1 + 0x1c))(piVar1,DAT_00444d64,DAT_00444d68,DAT_00512774,0,0);
        }
        if (DAT_004fbfb4 != 0) {
          CSound_Stop((int)DAT_004fc14c);
          DAT_00512188 = 0;
        }
      }
      uVar2 = CSound_IsSoundPlaying((int)DAT_004fc14c);
      if (uVar2 == 0) {
        DAT_00512188 = 0;
      }
      break;
    case 10:
      PreparePlayAgainTransition();
      DAT_0044de14 = 0x3c;
      DAT_0051b418 = 0x2e;
      SaveAndUnloadBobsBandActivity();
      DAT_0051c2fc = 0;
      return 1;
    }
    if (DAT_0051c30c == 1) {
      CSound_Stop(DAT_0051c2b8);
      DAT_0051c30c = 0;
      PlayManagedSoundById(DAT_0044ddd8,DAT_00512740,0x32,1);
      *(undefined4 *)
       ((int)DAT_0044ddd8 + *(char *)(DAT_00512740 + 0x280 + (int)DAT_0044ddd8) * 4 + 0xd98) = 1;
    }
    return 1;
  }
  SaveAndUnloadBobsBandActivity();
  if (DAT_0051c2e8 != 0) {
    DAT_0051c2e8 = 0;
    DAT_00446ce8 = 0x2a;
    DAT_0044de14 = (-(uint)(DAT_00446f38 != -1) & 0xffffffda) + 0x2a;
  }
  if (DAT_00446f38 != -1) {
    DAT_0044de14 = 0x40;
  }
  return 1;
}

