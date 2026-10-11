/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404b30; function: CWaveFile_Close; body bytes: 290
 * callers: 1; callees: 6; success: True
 */


undefined4 __fastcall CWaveFile_Close(MMRESULT param_1)

{
  LPMMCKINFO pmmcki;
  LPMMCKINFO pmmcki_00;
  MMRESULT MVar1;
  HMMIO pHVar2;
  MMRESULT MStack_4;
  
  MStack_4 = param_1;
  if (*(int *)(param_1 + 0x7c) == 1) {
    pHVar2 = *(HMMIO *)(param_1 + 4);
  }
  else {
    pHVar2 = *(HMMIO *)(param_1 + 4);
    ((LPCMMIOINFO)(param_1 + 0x34))->dwFlags = *(uint *)(param_1 + 0x34) | 0x10000000;
    if (pHVar2 == (HMMIO)0x0) {
      return 0x800401f0;
    }
    MVar1 = mmioSetInfo(pHVar2,(LPCMMIOINFO)(param_1 + 0x34),0);
    if (MVar1 != 0) {
      return 0x80004005;
    }
    pmmcki = (LPMMCKINFO)(param_1 + 8);
    MVar1 = mmioAscend(*(HMMIO *)(param_1 + 4),pmmcki,0);
    if (MVar1 != 0) {
      return 0x80004005;
    }
    pmmcki_00 = (LPMMCKINFO)(param_1 + 0x1c);
    MVar1 = mmioAscend(*(HMMIO *)(param_1 + 4),pmmcki_00,0);
    if (MVar1 != 0) {
      return 0x80004005;
    }
    mmioSeek(*(HMMIO *)(param_1 + 4),0,0);
    MVar1 = mmioDescend(*(HMMIO *)(param_1 + 4),pmmcki_00,(MMCKINFO *)0x0,0);
    if (MVar1 != 0) {
      return 0x80004005;
    }
    pHVar2 = *(HMMIO *)(param_1 + 4);
    pmmcki->ckid = 0x74636166;
    MVar1 = mmioDescend(pHVar2,pmmcki,pmmcki_00,0x10);
    if (MVar1 == 0) {
      MStack_4 = MVar1;
      mmioWrite(*(HMMIO *)(param_1 + 4),(char *)&MStack_4,4);
      mmioAscend(*(HMMIO *)(param_1 + 4),pmmcki,0);
    }
    MVar1 = mmioAscend(*(HMMIO *)(param_1 + 4),pmmcki_00,0);
    if (MVar1 != 0) {
      return 0x80004005;
    }
    pHVar2 = *(HMMIO *)(param_1 + 4);
  }
  mmioClose(pHVar2,0);
  *(undefined4 *)(param_1 + 4) = 0;
  return 0;
}

