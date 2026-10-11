/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00412d50; function: CompleteFireworksEditorAction; body bytes: 254
 * callers: 1; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl CompleteFireworksEditorAction(int param_1)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int local_4;
  
  if ((-1 < param_1) && (param_1 < 0xc)) {
    DAT_005093d8 = param_1;
    return;
  }
  if (param_1 == 0xc) {
    SetCursorSurface((int *)0x0);
    DecodeSelectedFireworkGridCell(&local_4,&param_1);
    if (((int)(&DAT_0050a678)[param_1 + local_4 * 6] < 0) ||
       (0xb < (int)(&DAT_0050a678)[param_1 + local_4 * 6])) {
      if (local_4 == 2) {
        uVar4 = 0x32;
        DAT_005093e0 = local_4;
        DAT_005093e8 = param_1;
        DAT_0050ab24 = 10;
        iVar5 = local_4;
        uVar2 = FUN_0042ffc4();
        uVar2 = uVar2 & 0x80000003;
        if ((int)uVar2 < 0) {
          uVar2 = (uVar2 - 1 | 0xfffffffc) + 1;
        }
        iVar3 = uVar2 + 0x388;
      }
      else {
        iVar5 = 2;
        uVar4 = 0x32;
        _DAT_005093dc = local_4;
        _DAT_005093e4 = param_1;
        DAT_0050ab20 = 10;
        uVar2 = FUN_0042ffc4();
        uVar2 = uVar2 & 0x80000003;
        if ((int)uVar2 < 0) {
          uVar2 = (uVar2 - 1 | 0xfffffffc) + 1;
        }
        iVar3 = uVar2 + 0x363;
      }
      PlayManagedSoundById(DAT_0044ddd8,iVar3,uVar4,iVar5);
      cVar1 = PlaceFireworkGridItem(local_4,param_1,DAT_005093d8,0);
      if (cVar1 == '\x01') {
        DAT_0050a5bc = 0;
        SetCursorSurface((int *)0x0);
      }
    }
  }
  return;
}

