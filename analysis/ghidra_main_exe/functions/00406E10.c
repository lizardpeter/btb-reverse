/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406e10; function: OpenGameDataFileWithCDFallback; body bytes: 145
 * callers: 28; callees: 2; success: True
 */


int __cdecl OpenGameDataFileWithCDFallback(LPCSTR param_1,char *param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  undefined4 *puVar6;
  char *pcVar7;
  char *pcVar8;
  undefined4 local_64;
  undefined4 local_60 [24];
  
  local_64 = DAT_0043e14c;
  puVar6 = local_60;
  for (iVar2 = 0x18; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  iVar2 = crt_fopen(param_1,param_2);
  if (DAT_0043e874 != -1) {
    if (iVar2 != 0) {
      return iVar2;
    }
    uVar3 = 0xffffffff;
    local_64 = CONCAT31(local_64._1_3_,(char)DAT_0043e874 + 'a');
    do {
      pcVar5 = param_1;
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      pcVar5 = param_1 + 1;
      cVar1 = *param_1;
      param_1 = pcVar5;
    } while (cVar1 != '\0');
    uVar3 = ~uVar3;
    iVar2 = -1;
    pcVar8 = (char *)&local_64;
    do {
      pcVar7 = pcVar8;
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      pcVar7 = pcVar8 + 1;
      cVar1 = *pcVar8;
      pcVar8 = pcVar7;
    } while (cVar1 != '\0');
    pcVar5 = pcVar5 + -uVar3;
    pcVar8 = pcVar7 + -1;
    for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(undefined4 *)pcVar8 = *(undefined4 *)pcVar5;
      pcVar5 = pcVar5 + 4;
      pcVar8 = pcVar8 + 4;
    }
    for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
      *pcVar8 = *pcVar5;
      pcVar5 = pcVar5 + 1;
      pcVar8 = pcVar8 + 1;
    }
    iVar2 = crt_fopen((LPCSTR)&local_64,param_2);
  }
  if (iVar2 == 0) {
    AbortForRemovedRetailCD();
  }
  return iVar2;
}

