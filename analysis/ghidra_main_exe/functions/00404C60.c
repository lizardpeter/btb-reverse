/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404c60; function: FUN_00404c60; body bytes: 359
 * callers: 1; callees: 3; success: True
 */


uint __thiscall FUN_00404c60(void *this,short *param_1)

{
  LPMMCKINFO pmmcki;
  HMMIO hmmio;
  MMRESULT MVar1;
  LONG LVar2;
  char local_18 [4];
  _MMCKINFO local_14;
  
  local_18[0] = -1;
  local_18[1] = -1;
  local_18[2] = -1;
  local_18[3] = -1;
  *(undefined4 *)((int)this + 0x24) = 0x45564157;
  *(undefined4 *)((int)this + 0x20) = 0;
  MVar1 = mmioCreateChunk(*(HMMIO *)((int)this + 4),(LPMMCKINFO)((int)this + 0x1c),0x20);
  if (MVar1 != 0) {
    return 0x80004005;
  }
  hmmio = *(HMMIO *)((int)this + 4);
  pmmcki = (LPMMCKINFO)((int)this + 8);
  pmmcki->ckid = 0x20746d66;
  *(undefined4 *)((int)this + 0xc) = 0x10;
  MVar1 = mmioCreateChunk(hmmio,pmmcki,0);
  if (MVar1 != 0) {
    return 0x80004005;
  }
  if (*param_1 == 1) {
    LVar2 = mmioWrite(*(HMMIO *)((int)this + 4),(char *)param_1,0x10);
    if (LVar2 != 0x10) {
      return 0x80004005;
    }
  }
  else {
    LVar2 = mmioWrite(*(HMMIO *)((int)this + 4),(char *)param_1,(ushort)param_1[8] + 0x12);
    if (LVar2 != (ushort)param_1[8] + 0x12) {
      return 0x80004005;
    }
  }
  MVar1 = mmioAscend(*(HMMIO *)((int)this + 4),pmmcki,0);
  if (MVar1 != 0) {
    return 0x80004005;
  }
  local_14.ckid = 0x74636166;
  local_14.cksize = 0;
  MVar1 = mmioCreateChunk(*(HMMIO *)((int)this + 4),&local_14,0);
  if (MVar1 != 0) {
    return 0x80004005;
  }
  LVar2 = mmioWrite(*(HMMIO *)((int)this + 4),local_18,4);
  if (LVar2 != 4) {
    return 0x80004005;
  }
  MVar1 = mmioAscend(*(HMMIO *)((int)this + 4),(LPMMCKINFO)&stack0xffffffe0,0);
  return -(uint)(MVar1 != 0) & 0x80004005;
}

