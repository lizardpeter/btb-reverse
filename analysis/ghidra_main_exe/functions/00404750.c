/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404750; function: CWaveFile_ReadMMIO; body bytes: 400
 * callers: 1; callees: 5; success: True
 */


undefined4 __fastcall CWaveFile_ReadMMIO(int *param_1)

{
  LPMMCKINFO pmmcki;
  MMRESULT MVar1;
  LONG LVar2;
  undefined4 *puVar3;
  uint uVar4;
  uint local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  _MMCKINFO local_14;
  
  pmmcki = (LPMMCKINFO)(param_1 + 7);
  *param_1 = 0;
  MVar1 = mmioDescend((HMMIO)param_1[1],pmmcki,(MMCKINFO *)0x0,0);
  if (MVar1 != 0) {
    return 0x80004005;
  }
  if (pmmcki->ckid != 0x46464952) {
    return 0x80004005;
  }
  if (param_1[9] != 0x45564157) {
    return 0x80004005;
  }
  local_14.ckid = 0x20746d66;
  MVar1 = mmioDescend((HMMIO)param_1[1],&local_14,pmmcki,0x10);
  if (MVar1 != 0) {
    return 0x80004005;
  }
  if (local_14.cksize < 0x10) {
    return 0x80004005;
  }
  LVar2 = mmioRead((HMMIO)param_1[1],(HPSTR)&local_24,0x10);
  if (LVar2 != 0x10) {
    return 0x80004005;
  }
  if ((short)local_24 == 1) {
    puVar3 = (undefined4 *)operator_new(0x12);
    *param_1 = (int)puVar3;
    if (puVar3 == (undefined4 *)0x0) {
      return 0x80004005;
    }
    *puVar3 = local_24;
    puVar3[1] = local_20;
    puVar3[2] = local_1c;
    puVar3[3] = local_18;
    *(undefined2 *)(*param_1 + 0x10) = 0;
  }
  else {
    local_28 = 0;
    LVar2 = mmioRead((HMMIO)param_1[1],(HPSTR)&local_28,2);
    if (LVar2 != 2) {
      return 0x80004005;
    }
    puVar3 = (undefined4 *)operator_new((local_28 & 0xffff) + 0x12);
    *param_1 = (int)puVar3;
    if (puVar3 == (undefined4 *)0x0) {
      return 0x80004005;
    }
    *puVar3 = local_24;
    puVar3[1] = local_20;
    puVar3[2] = local_1c;
    puVar3[3] = local_18;
    *(undefined2 *)(*param_1 + 0x10) = (undefined2)local_28;
    uVar4 = mmioRead((HMMIO)param_1[1],(HPSTR)(*param_1 + 0x12),local_28 & 0xffff);
    if (uVar4 != (local_28 & 0xffff)) goto LAB_004048b6;
  }
  MVar1 = mmioAscend((HMMIO)param_1[1],&local_14,0);
  if (MVar1 == 0) {
    return 0;
  }
LAB_004048b6:
  if ((undefined *)*param_1 != (undefined *)0x0) {
    FUN_0042fbdc((undefined *)*param_1);
    *param_1 = 0;
  }
  return 0x80004005;
}

