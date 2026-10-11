/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041f6e0; function: HandleBobsBandEditorAction; body bytes: 318
 * callers: 1; callees: 4; success: True
 */


void __cdecl HandleBobsBandEditorAction(uint param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int local_4;
  
  uVar1 = param_1;
  if (((int)param_1 < 0) || (9 < (int)param_1)) {
    if (param_1 == 0xb) {
      DecodeBobsBandGridCell((int *)&param_1,&local_4);
      iVar2 = (&DAT_005123b8)[local_4 + param_1 * 0x18];
      while (iVar2 == 10) {
        DAT_0050a5c0 = DAT_0050a5c0 + -1;
        DecodeBobsBandGridCell((int *)&param_1,&local_4);
        iVar2 = (&DAT_005123b8)[local_4 + param_1 * 0x18];
      }
      if ((-1 < iVar2) && (iVar2 < 10)) {
        iVar2 = RemoveGrandOpeningEventAtCell(param_1,local_4);
        if (DAT_00513f24 == 0) {
          SetCursorSurface((int *)(&DAT_00512390)[iVar2]);
          DAT_005093d8 = iVar2;
          DAT_00512188 = 1;
          return;
        }
        DAT_00513f28 = 1;
      }
    }
  }
  else {
    if (DAT_00513f24 == 1) {
      DAT_00513f24 = 0;
    }
    SetCursorSurface((int *)(&DAT_00512390)[param_1]);
    DAT_00512188 = 1;
    DAT_005093d8 = uVar1;
    CSound_Play(*(void **)(&DAT_004fc0d4 + uVar1 * 4),0,0);
    if ((&DAT_00512378)[(int)uVar1 / 2] == 0) {
      uVar3 = uVar1 & 0x80000001;
      if ((int)uVar3 < 0) {
        uVar3 = (uVar3 - 1 | 0xfffffffe) + 1;
      }
      (&DAT_00512378)[(int)uVar1 / 2] = uVar3 + 1;
      return;
    }
  }
  return;
}

