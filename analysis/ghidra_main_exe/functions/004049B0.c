/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004049b0; function: CWaveFile_Read; body bytes: 381
 * callers: 1; callees: 3; success: True
 */


undefined4 __thiscall CWaveFile_Read(void *this,undefined4 *param_1,uint param_2,uint *param_3)

{
  char cVar1;
  MMRESULT MVar2;
  uint uVar3;
  HPSTR pcVar4;
  undefined4 *puVar5;
  uint uVar6;
  _MMIOINFO local_48;
  
  if (*(int *)((int)this + 0x80) != 0) {
    if (*(int *)((int)this + 0x88) == 0) {
      return 0x800401f0;
    }
    if (param_3 != (uint *)0x0) {
      *param_3 = 0;
    }
    puVar5 = *(undefined4 **)((int)this + 0x88);
    if ((undefined1 *)(*(int *)((int)this + 0x84) + *(int *)((int)this + 0x8c)) <
        (undefined1 *)((int)puVar5 + param_2)) {
      param_2 = (*(int *)((int)this + 0x84) - (int)puVar5) + *(int *)((int)this + 0x8c);
    }
    for (uVar3 = param_2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *param_1 = *puVar5;
      puVar5 = puVar5 + 1;
      param_1 = param_1 + 1;
    }
    for (uVar3 = param_2 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(undefined1 *)param_1 = *(undefined1 *)puVar5;
      puVar5 = (undefined4 *)((int)puVar5 + 1);
      param_1 = (undefined4 *)((int)param_1 + 1);
    }
    if (param_3 != (uint *)0x0) {
      *param_3 = param_2;
    }
    return 0;
  }
  if (*(int *)((int)this + 4) == 0) {
    return 0x800401f0;
  }
  if ((param_1 == (undefined4 *)0x0) || (param_3 == (uint *)0x0)) {
    return 0x80070057;
  }
  *param_3 = 0;
  MVar2 = mmioGetInfo(*(HMMIO *)((int)this + 4),&local_48,0);
  if (MVar2 != 0) {
    return 0x80004005;
  }
  uVar3 = *(uint *)((int)this + 0xc);
  if (uVar3 < param_2) {
    param_2 = uVar3;
  }
  uVar6 = 0;
  *(uint *)((int)this + 0xc) = uVar3 - param_2;
  pcVar4 = local_48.pchEndRead;
  if (param_2 != 0) {
    do {
      if (local_48.pchNext == pcVar4) {
        MVar2 = mmioAdvance(*(HMMIO *)((int)this + 4),&local_48,0);
        if (MVar2 != 0) {
          return 0x80004005;
        }
        pcVar4 = local_48.pchEndRead;
        if (local_48.pchNext == local_48.pchEndRead) {
          return 0x80004005;
        }
      }
      cVar1 = *local_48.pchNext;
      local_48.pchNext = local_48.pchNext + 1;
      *(char *)(uVar6 + (int)param_1) = cVar1;
      uVar6 = uVar6 + 1;
    } while (uVar6 < param_2);
  }
  MVar2 = mmioSetInfo(*(HMMIO *)((int)this + 4),&local_48,0);
  if (MVar2 != 0) {
    return 0x80004005;
  }
  *param_3 = param_2;
  return 0;
}

