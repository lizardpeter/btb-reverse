/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404560; function: CWaveFile_Open; body bytes: 484
 * callers: 1; callees: 11; success: True
 */


uint __thiscall CWaveFile_Open(void *this,LPSTR param_1,short *param_2,int param_3)

{
  HMMIO pHVar1;
  HRSRC hResInfo;
  HGLOBAL hResData;
  DWORD DVar2;
  char *pcVar3;
  HPSTR pcVar4;
  uint uVar5;
  int iVar6;
  HPSTR pcVar7;
  _MMIOINFO *p_Var8;
  _MMIOINFO _Stack_48;
  
  *(int *)((int)this + 0x7c) = param_3;
  *(undefined4 *)((int)this + 0x80) = 0;
  if (param_3 == 1) {
    if (param_1 == (LPSTR)0x0) {
      return 0x80070057;
    }
                    /* WARNING: Load size is inaccurate */
    if (*this != (undefined *)0x0) {
      FUN_0042fbdc(*this);
      *(undefined4 *)this = 0;
    }
    pHVar1 = mmioOpenA(param_1,(LPMMIOINFO)0x0,0x10000);
    *(HMMIO *)((int)this + 4) = pHVar1;
    if (pHVar1 == (HMMIO)0x0) {
      hResInfo = FindResourceA((HMODULE)0x0,param_1,&DAT_0043e154);
      if (hResInfo == (HRSRC)0x0) {
        hResInfo = FindResourceA((HMODULE)0x0,param_1,&DAT_0043e150);
        if (hResInfo == (HRSRC)0x0) {
          return 0x80004005;
        }
      }
      hResData = LoadResource((HMODULE)0x0,hResInfo);
      if (hResData == (HGLOBAL)0x0) {
        return 0x80004005;
      }
      DVar2 = SizeofResource((HMODULE)0x0,hResInfo);
      if (DVar2 == 0) {
        return 0x80004005;
      }
      pcVar3 = (char *)LockResource(hResData);
      if (pcVar3 == (char *)0x0) {
        return 0x80004005;
      }
      pcVar4 = (HPSTR)operator_new(DVar2);
      pcVar7 = pcVar4;
      for (uVar5 = DVar2 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar7 = *(undefined4 *)pcVar3;
        pcVar3 = pcVar3 + 4;
        pcVar7 = pcVar7 + 4;
      }
      for (uVar5 = DVar2 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar7 = *pcVar3;
        pcVar3 = pcVar3 + 1;
        pcVar7 = pcVar7 + 1;
      }
      p_Var8 = &_Stack_48;
      for (iVar6 = 0x12; iVar6 != 0; iVar6 = iVar6 + -1) {
        p_Var8->dwFlags = 0;
        p_Var8 = (_MMIOINFO *)&p_Var8->fccIOProc;
      }
      _Stack_48.fccIOProc = 0x204d454d;
      _Stack_48.cchBuffer = DVar2;
      _Stack_48.pchBuffer = pcVar4;
      pHVar1 = mmioOpenA((LPSTR)0x0,&_Stack_48,0x10000);
      *(HMMIO *)((int)this + 4) = pHVar1;
    }
    uVar5 = CWaveFile_ReadMMIO((int *)this);
    if ((int)uVar5 < 0) {
      mmioClose(*(HMMIO *)((int)this + 4),0);
      return uVar5;
    }
    uVar5 = CWaveFile_ResetFile((int)this);
    if (-1 < (int)uVar5) {
      *(undefined4 *)((int)this + 0x30) = *(undefined4 *)((int)this + 0xc);
      return uVar5;
    }
  }
  else {
    pHVar1 = mmioOpenA(param_1,(LPMMIOINFO)0x0,0x11002);
    *(HMMIO *)((int)this + 4) = pHVar1;
    if (pHVar1 == (HMMIO)0x0) {
      return 0x80004005;
    }
    uVar5 = FUN_00404c60(this,param_2);
    if ((int)uVar5 < 0) {
      mmioClose(*(HMMIO *)((int)this + 4),0);
      return uVar5;
    }
    uVar5 = CWaveFile_ResetFile((int)this);
  }
  return uVar5;
}

