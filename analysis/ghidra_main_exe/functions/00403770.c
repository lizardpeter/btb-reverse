/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403770; function: RegisterBitmapSurface; body bytes: 179
 * callers: 25; callees: 0; success: True
 */


void __cdecl RegisterBitmapSurface(undefined4 param_1,char *param_2)

{
  char cVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  char *pcVar6;
  char *pcVar7;
  
  if (DAT_00482420 == 0) {
    iVar5 = 0;
    if (0 < DAT_0048241c) {
      piVar2 = &DAT_0044eb1c;
      do {
        if (*piVar2 == 0) break;
        iVar5 = iVar5 + 1;
        piVar2 = piVar2 + 1;
      } while (iVar5 < DAT_0048241c);
    }
    uVar3 = 0xffffffff;
    (&DAT_0044de9c)[iVar5] = 0;
    do {
      pcVar6 = param_2;
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      pcVar6 = param_2 + 1;
      cVar1 = *param_2;
      param_2 = pcVar6;
    } while (cVar1 != '\0');
    uVar3 = ~uVar3;
    pcVar6 = pcVar6 + -uVar3;
    pcVar7 = &DAT_0044f79c + iVar5 * 0x104;
    for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(undefined4 *)pcVar7 = *(undefined4 *)pcVar6;
      pcVar6 = pcVar6 + 4;
      pcVar7 = pcVar7 + 4;
    }
    piVar2 = &DAT_0044eb1c;
    for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
      *pcVar7 = *pcVar6;
      pcVar6 = pcVar6 + 1;
      pcVar7 = pcVar7 + 1;
    }
    (&DAT_0044eb1c)[iVar5] = param_1;
    DAT_0048241c = 0;
    iVar5 = 0;
    do {
      if (*piVar2 != 0) {
        DAT_0048241c = iVar5 + 1;
      }
      piVar2 = piVar2 + 1;
      iVar5 = iVar5 + 1;
    } while ((int)piVar2 < 0x44f79c);
  }
  return;
}

