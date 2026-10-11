/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004048f0; function: CWaveFile_ResetFile; body bytes: 182
 * callers: 2; callees: 4; success: True
 */


undefined4 __fastcall CWaveFile_ResetFile(int param_1)

{
  HMMIO hmmio;
  LONG LVar1;
  MMRESULT MVar2;
  
  if (*(int *)(param_1 + 0x80) == 0) {
    hmmio = *(HMMIO *)(param_1 + 4);
    if (hmmio == (HMMIO)0x0) {
      return 0x800401f0;
    }
    if (*(int *)(param_1 + 0x7c) == 1) {
      LVar1 = mmioSeek(hmmio,*(int *)(param_1 + 0x28) + 4,0);
      if (LVar1 == -1) {
        return 0x80004005;
      }
      ((LPMMCKINFO)(param_1 + 8))->ckid = 0x61746164;
      MVar2 = mmioDescend(*(HMMIO *)(param_1 + 4),(LPMMCKINFO)(param_1 + 8),
                          (MMCKINFO *)(param_1 + 0x1c),0x10);
      if (MVar2 != 0) {
        return 0x80004005;
      }
    }
    else {
      ((LPMMCKINFO)(param_1 + 8))->ckid = 0x61746164;
      *(undefined4 *)(param_1 + 0xc) = 0;
      MVar2 = mmioCreateChunk(hmmio,(LPMMCKINFO)(param_1 + 8),0);
      if (MVar2 != 0) {
        return 0x80004005;
      }
      MVar2 = mmioGetInfo(*(HMMIO *)(param_1 + 4),(LPMMIOINFO)(param_1 + 0x34),0);
      if (MVar2 != 0) {
        return 0x80004005;
      }
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x88) = *(undefined4 *)(param_1 + 0x84);
  }
  return 0;
}

