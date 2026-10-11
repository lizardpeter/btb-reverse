/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041f550; function: UpdateBobsBandBrickCursorAndDelete; body bytes: 385
 * callers: 1; callees: 5; success: True
 */


void __cdecl UpdateBobsBandBrickCursorAndDelete(uint param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  char cVar4;
  undefined4 *puVar5;
  uint uVar6;
  int local_4;
  
  uVar6 = param_1;
  if ((-1 < (int)param_1) && ((int)param_1 < 10)) {
    if ((DAT_00512188 == 1) && (param_1 == DAT_005093d8)) {
      SetCursorSurface((int *)0x0);
      DAT_00512188 = 0;
      return;
    }
    SetCursorSurface((int *)(&DAT_00512390)[param_1]);
    DAT_00512188 = 1;
    DAT_005093d8 = uVar6;
    CSound_Play(*(void **)(&DAT_004fc0d4 + uVar6 * 4),0,0);
    return;
  }
  if (param_1 == 0xb) {
    DecodeBobsBandGridCell((int *)&param_1,&local_4);
    iVar2 = local_4 + param_1 * 0x18;
    puVar5 = &DAT_005123b8 + iVar2;
    iVar2 = (&DAT_005123b8)[iVar2];
    if ((-1 < iVar2) && (iVar3 = local_4, iVar2 < 10)) {
      while (8 < iVar2) {
        piVar1 = puVar5 + -1;
        puVar5 = puVar5 + -1;
        iVar3 = iVar3 + -1;
        iVar2 = *piVar1;
      }
      uVar6 = RemoveGrandOpeningEventAtCell(param_1,local_4);
      SetCursorSurface((int *)(&DAT_00512390)[uVar6]);
      DAT_00512188 = 1;
      cVar4 = PlaceGrandOpeningEvent(param_1,local_4,DAT_005093d8);
      if (cVar4 != '\0') {
        DAT_005093d8 = uVar6;
        return;
      }
      PlaceGrandOpeningEvent(param_1,iVar3,uVar6);
      SetCursorSurface((int *)(&DAT_00512390)[DAT_005093d8]);
      return;
    }
    cVar4 = PlaceGrandOpeningEvent(param_1,local_4,DAT_005093d8);
    if (cVar4 == '\x01') {
      DAT_00512188 = 0;
      SetCursorSurface((int *)0x0);
      return;
    }
  }
  else if (((param_1 == 0x16) && (DAT_00512188 == 0)) && (DAT_00513f24 == 1)) {
    SetCursorSurface((int *)0x0);
  }
  return;
}

